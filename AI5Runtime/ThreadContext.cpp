#include <functional>
#include "ThreadContext.h"
#include "ThreadManager.h"
#include "StackMachineThread.h"
#include "StackMachine.h"
#include "platform.h"

__declspec(thread) ThreadContext* currentThread = nullptr;


ThreadContext* ThreadContext::current()
{
	return currentThread;
}

SafePoint::SafePoint(): stop(false), stopped(false) {}
void SafePoint::check()
{
	if(stop)
	{
		stopped = true;
		std::lock_guard<std::mutex> guard(lock);
		stopped = false;
	}
}
void SafePoint::signalStop()
{
	lock.lock();
	stop = true;
}
void SafePoint::release()
{
	stop = false;
	lock.unlock();
}

SafeRegion::SafeRegion(ThreadContext* context): exited(false), sp(&context->safePoint)
{
	sp->stopped = true;
}
SafeRegion::~SafeRegion()
{
	leave();
}
void SafeRegion::leave()
{
	if(!exited)
	{
		DebugOut(L"Thread") << "SafeRegion exited";
		sp->check();
		exited = true;
	}
}

ThreadContext::ThreadContext(StackMachine* machine): Variant(TYPE), virtualThread(new StackMachineThread(machine)), machine(machine), threadHandle(nullptr)
{
	virtualThread->setThreadContext(this);	
}

ThreadContext::ThreadContext(): Variant(TYPE), virtualThread(nullptr), threadHandle(nullptr)
{

}

ThreadContext::~ThreadContext()
{
	if(virtualThread)
		delete virtualThread;
	if(threadHandle)
		delete threadHandle;
}

void ThreadContext::setThreadName(shared_string name)
{
	this->name = name;
}

void ThreadContext::threadFunction()
{
	currentThread = this;
	GC::initThread(this);
	virtualThread->run();
	GC::uninitThread(this);
	machine->getThreadManager()->reportTermination(this);
}


void ThreadContext::start()
{
	threadHandle = new std::thread(std::bind(&ThreadContext::threadFunction,this));
}

void ThreadContext::kill()
{
	virtualThread->terminate(-1);
	std::this_thread::sleep_for(std::chrono::milliseconds(25));
	if(!virtualThread->isTerminated())
	{
		KillNativeThread(*threadHandle);
	}
}

shared_string ThreadContext::getThreadName()
{
	return this->name;
}

int ThreadContext::join()
{
	this->threadHandle->join();
	return virtualThread->getReturnCode();
}

StackMachineThread* ThreadContext::getVirtualThread()
{
	return virtualThread;
}

