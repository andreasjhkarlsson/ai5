#pragma once
#include <functional>
#include <chrono>
#include "gc.h"
#include "StackMachine.h"
#include "Variant.h"
#include "VariantReference.h"
#include "GlobalOptions.h"
#include "misc.h"
#include "ThreadContext.h"

GC* GC::instance = nullptr;

void GC::init(StackMachine* machine)
{
	instance = new GC(machine);
	
}

void GC::initThread(ThreadContext* context)
{
	context->gcInfo = new ThreadInfo();
}

void GC::uninitThread(ThreadContext* context)
{
	delete context->gcInfo;
}


void GC::shutdown()
{
	instance->messageQueue.push(MESSAGE_STOP);
	instance->markAndSweepThread.join();
}

GC::GC(StackMachine* machine):  killThread(false), machine(machine), cycleComplete(0)
{
	InitializeLightWeightMutex(&heapLock);
	markAndSweepThread = std::thread(std::bind(&GC::run,this));
	renameNativeThread(markAndSweepThread,"GC");
}

GC::BlockHeader::BlockHeader(): mark(false),generation(0),referencedFrom(-1)
{
	
}

void GC::collect(bool wait)
{
	
	if(wait)
	{
		instance->messageQueue.push(MESSAGE_START_SYNCHRONOUS_CYCLE);
		instance->cycleComplete.wait();
	}
	else
	{
		instance->messageQueue.push(MESSAGE_START_ASYNCHRONOUS_CYCLE);
	}
}

void GC::trackObject(BlockHeader* object)
{
	LockLightWeightMutex(&heapLock);
	gens[0].push_back(object);
	UnlockLightWeightMutex(&heapLock);
}

void GC::trackStaticObject(BlockHeader* object)
{
	LockLightWeightMutex(&heapLock);
	object->generation = STATIC_GEN;
	statics.push_front(object);
	UnlockLightWeightMutex(&heapLock);
}

void GC::mark(ListVariant* list,int gen)
{
	for(size_t i=0;i<list->list->size();i++)
	{
		mark((*list->list)[i],gen);
	}
}

void GC::mark(HashMapVariant* hashMap, int gen)
{
	for(auto it = hashMap->map.begin();it != hashMap->map.end();it++)
	{
		mark(it->first,gen);
		mark(it->second,gen);
	}
}

void GC::mark(Scope* scope,int gen)
{
	for(auto it = scope->lookup.begin();it!=scope->lookup.end();it++)
		mark(it->second,gen);

	if(!scope->enclosingScope.empty())
		mark(scope->enclosingScope,gen);
}

void GC::mark(ThreadContext* threadContext, int gen)
{
	StackMachineThread* machineThread = threadContext->getVirtualThread();
	// Mark active scopes by examining call (block) stack.
	BlockStack* stack = &machineThread->blockStack;
	for(int i=0;i<=stack->position;i++)
	{
		if(stack->stack[i]->isCallBlock())
		{
			mark(static_cast<CallBlock*>(stack->stack[i])->scope,gen);
			mark(static_cast<CallBlock*>(stack->stack[i])->closureScope,gen);
			mark(static_cast<CallBlock*>(stack->stack[i])->owner,gen);
		}
		else if (stack->stack[0]->isFinallyBlock())
		{
			// Finally block may hold references to objects. Enumerate them.
			const FinallyBlock::ReturnInfo& info = static_cast<FinallyBlock*>(stack->stack[i])->getReturnInfo();
			
			if(info.action == FinallyBlock::ReturnInfo::THROW_EXCEPTION)
				mark(info.exceptionToThrow,gen);
			else if(info.action == FinallyBlock::ReturnInfo::RETURN_FROM_FUNC)
				mark(info.returnValue,gen);
		}
	}

	// Mark current exception.
	mark(machineThread->currentException,gen);
					
	// Mark current working set in datastack.
	DataStack* dataStack = &machineThread->dataStack;
	for(int i=0;i<=dataStack->position;i++)
	{
		mark(dataStack->stack[i],gen);
	}
}

void GC::mark(NameVariant* name,int gen)
{
	mark(name->value,gen);
}

void GC::mark(NameReferenceVariant* nameReference,int gen)
{
	mark(nameReference->value,gen);
}

void GC::mark(UserFunctionVariant* userFunc,int gen)
{
	if(!userFunc->enclosingScope.empty())
		mark(userFunc->enclosingScope,gen);
}

void GC::mark(BlockHeader* header,int gen,bool root)
{
	// Condition explained:
	// If the BlockHeader is a root object, always mark it.
	// Otherwise, skip it if it has already been marked, belongs to a generation
	// older than we're marking or if it's simply null. 
	if((header == nullptr || header->mark || header->generation > gen) && (!root))
	{
		// This object should not be processed.
		return;
	}
	header->mark = true;
	switch(header->object->getType())
	{
	case Variant::NAME_REFERENCE:
		mark(header->object->cast<NameReferenceVariant>(),gen);
		break;
	case Variant::NAME:
		mark(header->object->cast<NameVariant>(),gen);
		break;
	case Variant::LIST:
		mark(header->object->cast<ListVariant>(),gen);
		break;
	case Variant::HASH_MAP:
		mark(header->object->cast<HashMapVariant>(),gen);
		break;
	case Variant::USER_FUNCTION:
		mark(header->object->cast<UserFunctionVariant>(),gen);
		break;
	case Variant::SCOPE:
		mark(header->object->cast<Scope>(),gen);
		break;
	case Variant::THREAD_VAR:
		mark(header->object->cast<ThreadContext>(),gen);
		break;
	}
}

void GC::mark(const VariantReference<>&ref,int gen)
{
	BlockHeader* head = VarRefToBlockHead(ref);
	mark(head,gen);
}

void GC::sweep(DoubleLinkedList<BlockHeader>* objects,CycleStats& stats,int gen)
{
	BlockHeader* current = objects->firstElement();
	while(!current->sentinel)
	{
		if(current->next == nullptr)
			break;

		// If the object was marked OR is referenced from an older generation
		// then the object gets to live for another day and is promoted to older generation.
		if(current->mark || gen < current->referencedFrom)
		{
			current->mark = false; // Clear mark for next cycle.
			// Objects that survive collection AND isn't already in max gen gets promoted.
			if((current->generation+1) < GEN_COUNT && current->generation != STATIC_GEN)
				current->generation++;
			current = current->next;
		}
		else
		{
			//DebugOut(L"GC") << "Found garbage of type " << Variant::typeAsString(current->object->getType());
			stats.instancesFreed++;
			stats.bytesFreed += current->totalSize;
			BlockHeader* temp = objects->erase(current);
			freeObject(current);
			current = temp;
		}
	}
}

void GC::run()
{
	while(true)
	{
		int msg = messageQueue.pop();

		if(msg == MESSAGE_STOP)
			return;

		if(msg == MESSAGE_START_ASYNCHRONOUS_CYCLE || msg == MESSAGE_START_SYNCHRONOUS_CYCLE)
		{

			DebugOut(L"GC") << "Running cycle.";

			static int targetGen = -1;// GEN_COUNT-1;
			targetGen = (targetGen+1)%3;
			DebugOut(L"GC") << "Generation: " << targetGen;

			stopTheWorld();

			// Find all roots and mark them.
			// Roots are:
			// * Global scope
			// * Local scopes
			// * Data stack
			// * Static objects
			// * Blocks (Call, Finally) and exceptions

			
			DebugOut(L"GC") << "Marking root objects";

			// Mark statics.
			BlockHeader* current = statics.firstElement();
			while(!current->sentinel)
			{
				current->mark = false;
				mark(current,targetGen);
				current = current->next;
			}

			// Mark global scope.
			VariantReference<Scope>& globalScope = machine->globalScope;
			mark(VarRefToBlockHead(globalScope),targetGen,true);
			
			// Mark virtual threads.
			ThreadContext* context = machine->getThreadManager()->threads.firstElement();
			while(!context->sentinel)
			{
				BlockHeader* block = VarRefToBlockHead(context);
				block->mark = true;
				mark(block,targetGen,true);
				context = context->next;
			}

			DebugOut(L"GC") << "Sweeping.";
			CycleStats stats;
	


			for(int i=targetGen;i>=0;i--)
			{
				sweep(&gens[i],stats,targetGen);
				if((i+1) < GEN_COUNT)
				{
					gens[i+1].splice(&gens[i]);
				}
			}

			DebugOut(L"GC") << "Cycle complete";
			DebugOut(L"GC") << "Freed " << stats.instancesFreed << ", total size: " << stats.bytesFreed/1024.0 << " kB.";

			//DebugOut(L"GC") << "Mark complete, resuming mutator.";
			resumeTheWorld();

			if(msg == MESSAGE_START_SYNCHRONOUS_CYCLE)
				cycleComplete.signal();
		}
	}
}

GC::BlockHeader* GC::VarRefToBlockHead(const VariantReference<>&ref)
{
	if(!ref.isComplexType())
		return nullptr;
	return VarPtrToBlockHead(ref.ref.variant);
}

GC::BlockHeader* GC::VarPtrToBlockHead(Variant* var)
{
	return reinterpret_cast<BlockHeader*>(((char*)var)-BLOCKHEADER_ALIGNED_SIZE);
}

void GC::stopTheWorld()
{
	machine->getThreadManager()->suspendAll();

}

void GC::resumeTheWorld()
{
	machine->getThreadManager()->resumeAll();
}

void GC::cleanup()
{
	instance->freeAll();
}


void GC::freeBlockList(DoubleLinkedList<BlockHeader>* list)
{
	// Clear all static objects.
	BlockHeader* current = list->firstElement();
	while(!current->sentinel)
	{
		BlockHeader* next = current->next;
		freeObject(current);
		current = next;
	}

}

void GC::freeAll()
{

	DebugOut(L"GC") << L"Freeing all remaining objects.";

	// Clear all static objects.
	freeBlockList(&statics);

	for(int i=0;i<GEN_COUNT;i++) 
	{
		freeBlockList(&gens[i]);
	}

}

void GC::freeObject(BlockHeader* header)
{
	header->object->~Variant();
	free(header);
}