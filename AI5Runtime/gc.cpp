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

// MSVC2012 doesn't support thread_local :(
__declspec(thread) GC::ThreadContext* currentThread;

void GC::init(StackMachine* machine)
{
	instance = new GC(machine);
}

void GC::initThread(StackMachineThread* machineThread)
{
	currentThread = new ThreadContext();
	currentThread->machineThread = machineThread;
	currentThread->gen0 = new DoubleLinkedList<BlockHeader>();
	if(machineThread != nullptr)
		machineThread->threadContext = currentThread;
	std::lock_guard<std::mutex> guard(instance->threadsLock);
	instance->threads.push_back(currentThread);
}

void GC::uninitThread()
{
	std::lock_guard<std::mutex> guard(instance->threadsLock);
	instance->threads.erase(currentThread);

	instance->orphans.splice(currentThread->gen0);

	delete currentThread;
}


void GC::enterSafePoint()
{
	currentThread->safePoint.check();
}

void GC::enterSafePoint(StackMachineThread* machineThread)
{
	machineThread->threadContext->safePoint.check();
}

GC::ThreadContext::SafePoint::SafePoint(): stop(false), stopped(false)
{

}

void GC::ThreadContext::SafePoint::check()
{
	if(stop)
	{
		stopped = true;
		std::lock_guard<std::mutex> guard(lock);
		stopped = false;
	}
}

void GC::ThreadContext::SafePoint::signalStop()
{
	lock.lock();
	stop = true;
}
void GC::ThreadContext::SafePoint::release()
{
	stop = false;
	lock.unlock();
}

GC::SafeRegion::SafeRegion(StackMachineThread* machineThread): exited(false), sp(&machineThread->threadContext->safePoint)
{
	sp->stopped = true;
	DebugOut(L"Thread") << "SafeRegion entered";
}

GC::SafeRegion::SafeRegion(void): exited(false), sp(&currentThread->safePoint)
{
	sp->stopped = true;
	DebugOut(L"Thread") << "SafeRegion entered";
}

GC::SafeRegion::~SafeRegion()
{
	leave();
}

void GC::SafeRegion::leave()
{
	if(!exited)
	{
		DebugOut(L"Thread") << "SafeRegion exited";
		sp->check();
		exited = true;
	}
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
	instance->messageQueue.push(MESSAGE_START_CYCLE);
	if(wait)
		instance->cycleComplete.wait();
}

void GC::trackObject(BlockHeader* object)
{
	currentThread->gen0->push_back(object);
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

void GC::mark(ThreadHandle* threadHandle)
{
	StackMachineThread* machineThread = threadHandle->getMachineThread();
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

void GC::mark(HandleVariant* handle)
{
	if(handle->handleType == HandleVariant::THREAD_HANDLE)
	{
		mark(handle->castHandle<ThreadHandle>());
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
	case Variant::HANDLE_VAR:
		mark(header->object->cast<HandleVariant>());
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

		if(msg == MESSAGE_START_CYCLE)
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

			for(auto it = machine->threads.begin(); it != machine->threads.end(); it++)
			{
				VarRefToBlockHead(it->second)->mark = false;
				mark(it->second);
			}

			// Sweep all heaps.
			ThreadContext* thread = threads.firstElement();
			while(!thread->sentinel)
			{
				sweep(thread->gen0);
				thread = thread->next;
			}

			sweep(&orphans);

			resumeTheWorld();

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
	// Threads have 25 ms to stop!
	const __int64 TIME_LIMIT = 2500;
	std::lock_guard<std::mutex> guard(threadsLock);
	
	DebugOut(L"GC") << "Signaling all threads to stop!";

	// Tell all threads to stop!!
	ThreadContext* context = threads.firstElement();
	while(!context->sentinel)
	{
		context->safePoint.signalStop();
		context = context->next;
	}

	typedef std::chrono::high_resolution_clock Clock;
    typedef std::chrono::milliseconds milliseconds;
	// Give the threads some time to stop.
	auto startTime = Clock::now();

	while(true)
	{
		__int64 diff = std::chrono::duration_cast<milliseconds>(Clock::now() - startTime).count();
		bool allStopped = true; // A flag. Gross!
		context = threads.firstElement();
		while(!context->sentinel)
		{
			if(!context->safePoint.stopped)
			{
				if(diff > TIME_LIMIT)
				{
					// TODO: Suspend thread!
					DebugOut(L"GC") << "Thread did not respond in time!!";
				}
				else
				{
					allStopped = false;
				}
			}
			context = context->next;
		}
		if(allStopped)
			break;
	}

	DebugOut(L"GC") << "All threads stopped!";
}

void GC::resumeTheWorld()
{
	DebugOut(L"GC") << "Signaling all threads to resume!";
	ThreadContext* context = threads.firstElement();
	while(!context->sentinel)
	{
		context->safePoint.release();
		context = context->next;
	}
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
}

void GC::freeObject(BlockHeader* header)
{
	header->object->~Variant();
	free(header);
}