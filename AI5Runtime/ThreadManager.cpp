#include "ThreadManager.h"
#include "StackMachineThread.h"



ThreadManager::ThreadManager(void)
{
}


ThreadManager::~ThreadManager(void)
{
}

ThreadContext* ThreadManager::createThread(StackMachine* machine)
{
//	DebugOut(L"ThreadManager") << "Creating thread" << 
//		((name->length()!=0) ? (std::wstring(L" (")+name->getTerminatedBuffer()+L")"): L"");
	std::lock_guard<std::mutex> guard(threadsLock);
	ThreadContext* context = GC::alloc<ThreadContext,StackMachine*>(machine);
	threads.push_back(context);
	return context;
}


void ThreadManager::reportTermination(ThreadContext* threadContext)
{
	std::lock_guard<std::mutex> guard(threadsLock);
	threads.erase(threadContext);
}

void ThreadManager::killAll()
{
	DebugOut(L"ThreadManager") << "Killing all threads!";
	
	// During thread termination, threads will try to
	// take the threadLock. This will deadlock if we hold onto it.
	// We therefore make a copy of the list and then release the lock.
	std::vector<ThreadContext*> threadsCopy;
	{
		std::lock_guard<std::mutex> guard(threadsLock);
		ThreadContext* context = threads.firstElement();
		while(!context->sentinel)
		{
			threadsCopy.push_back(context);
			context = context->next;
		}
	}

	for(ThreadContext* context: threadsCopy)
	{
		DebugOut(L"ThreadManager") << "Signalling death...";
		context->kill();
	}

	for(ThreadContext* context: threadsCopy)
	{
		DebugOut(L"ThreadManager") << "Joining...";
		context->join();
	}
	DebugOut(L"ThreadManager") << "All threads killed!";
}

void ThreadManager::suspendAll()
{
	
	// Threads have 25 ms to stop!
	const __int64 TIME_LIMIT = 2500;
	std::lock_guard<std::mutex> guard(threadsLock);
	
	DebugOut(L"ThreadManager") << "Signaling all threads to stop!";

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
					DebugOut(L"ThreadManager") << "Thread did not respond in time!!";
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

	DebugOut(L"ThreadManager") << "All threads stopped!";
	
}
void ThreadManager::resumeAll()
{
	
	DebugOut(L"ThreadManager") << "Signaling all threads to resume!";
	ThreadContext* context = threads.firstElement();
	while(!context->sentinel)
	{
		context->safePoint.release();
		context = context->next;
	}
}