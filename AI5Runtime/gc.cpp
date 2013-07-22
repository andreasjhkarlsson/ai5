#pragma once
#include <functional>
#include <chrono>
#include "gc.h"
#include "StackMachine.h"
#include "Variant.h"
#include "VariantReference.h"
#include "GlobalOptions.h"
#include "misc.h"


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
	instance->orphans.splice(&context->gcInfo->gen0);
	delete context->gcInfo;
}


void GC::shutdown()
{
	instance->messageQueue.push(MESSAGE_STOP);
	instance->markAndSweepThread.join();
}

GC::GC(StackMachine* machine):  killThread(false), machine(machine), cycleComplete(0)
{
	markAndSweepThread = std::thread(std::bind(&GC::run,this));
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
	ThreadContext* context = ThreadContext::current();
	if(context)
		context->gcInfo->gen0.push_back(object);
	else
		orphans.push_back(object);
}

void GC::addStaticObject(BlockHeader* object)
{
	object->generation = GENERATION_STATIC;
	staticList.push_front(object);
}

void GC::mark(ListVariant* list)
{
	for(size_t i=0;i<list->list->size();i++)
	{
		mark((*list->list)[i]);
	}
}

void GC::mark(HashMapVariant* hashMap)
{
	for(auto it = hashMap->map.begin();it != hashMap->map.end();it++)
	{
		mark(it->first);
		mark(it->second);
	}
}

void GC::mark(Scope* scope)
{
	for(auto it = scope->lookup.begin();it!=scope->lookup.end();it++)
		mark(it->second);

	if(!scope->enclosingScope.empty())
		mark(scope->enclosingScope);
}

void GC::mark(ThreadContext* threadContext)
{
	StackMachineThread* machineThread = threadContext->getVirtualThread();
	// Mark active scopes by examining call (block) stack.
	BlockStack* stack = &machineThread->blockStack;
	for(int i=0;i<=stack->position;i++)
	{
		if(stack->stack[i]->isCallBlock())
		{
			mark(static_cast<CallBlock*>(stack->stack[i])->getScope());
		}
	}
					
	// Mark current working set in datastack.
	DataStack* dataStack = &machineThread->dataStack;
	for(int i=0;i<=dataStack->position;i++)
	{
		mark(dataStack->stack[i]);
	}
}

void GC::mark(NameVariant* name)
{
	mark(name->value);
}

void GC::mark(NameReferenceVariant* nameReference)
{
	mark(nameReference->value);
}

void GC::mark(UserFunctionVariant* userFunc)
{
	if(!userFunc->enclosingScope.empty())
		mark(userFunc->enclosingScope);
}

void GC::mark(BlockHeader* header)
{
	if(header == nullptr || header->mark)
	{
		// This object should not be processed.
		return;
	}
	header->mark = true;
	switch(header->object->getType())
	{
	case Variant::NAME_REFERENCE:
		mark(header->object->cast<NameReferenceVariant>());
		break;
	case Variant::NAME:
		mark(header->object->cast<NameVariant>());
		break;
	case Variant::LIST:
		mark(header->object->cast<ListVariant>());
		break;
	case Variant::HASH_MAP:
		mark(header->object->cast<HashMapVariant>());
		break;
	case Variant::USER_FUNCTION:
		mark(header->object->cast<UserFunctionVariant>());
		break;
	case Variant::SCOPE:
		mark(header->object->cast<Scope>());
		break;
	case Variant::THREAD_VAR:
		mark(header->object->cast<ThreadContext>());
		break;
	}
}

void GC::mark(const VariantReference<>&ref)
{
	BlockHeader* head = VarRefToBlockHead(ref);
	mark(head);
}

void GC::sweep(DoubleLinkedList<BlockHeader>* objects)
{
	BlockHeader* current = objects->firstElement();
	while(!current->sentinel)
	{
		if(current->next == nullptr)
			break;
		if(current->mark)
		{
			current->mark = false; // Clear mark for next cycle.
			current = current->next;
		}
		else
		{
			DebugOut(L"GC") << "Found garbage of type " << current->object->typeAsString();

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

			stopTheWorld();

			// Find all roots and mark them.
			// Roots are:
			// * Global scope
			// * Local scopes
			// * Data stack
			// * CallFrames
			// * Static objects

			// Mark statics.
			BlockHeader* current = staticList.firstElement();
			while(!current->sentinel)
			{
				current->mark = false;
				mark(current);
				current = current->next;
			}

			// Mark global scope.
			VariantReference<Scope>& globalScope = machine->globalScope;
			VarRefToBlockHead(globalScope)->mark = false;
			mark(globalScope);
			

			ThreadContext* context = machine->getThreadManager()->threads.firstElement();
			while(!context->sentinel)
			{
				VarRefToBlockHead(context)->mark = false;
				mark(VarRefToBlockHead(context));
				context = context->next;
			}

			context = machine->getThreadManager()->threads.firstElement();
			while(!context->sentinel)
			{
				sweep(&context->gcInfo->gen0);
				context = context->next;
			}
			

			sweep(&orphans);

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
	return reinterpret_cast<BlockHeader*>(((char*)ref.ref.variant)-BLOCKHEADER_ALIGNED_SIZE);
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

void GC::freeAll()
{
	DebugOut(L"GC") << L"Freeing all remaining objects.";

	// Clear all static objects.
	BlockHeader* current = staticList.firstElement();
	while(!current->sentinel)
	{
		BlockHeader* next = current->next;
		freeObject(current);
		current = next;
	}

	// Clear all orphans (should be all remaining objects!)
	current = orphans.firstElement();
	while(!current->sentinel)
	{
		BlockHeader* next = current->next;
		freeObject(current);
		current = next;
	}
}

void GC::freeObject(BlockHeader* header)
{
	header->object->~Variant();
	free(header);
}