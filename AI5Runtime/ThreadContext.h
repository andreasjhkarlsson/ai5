#pragma once
#include "gc.h"
#include "DoubleLinkedList.h"
#include "Variant.h"
#include "misc.h"
class ThreadManager;
class ThreadContext;
struct SafePoint
{
	SafePoint();
	void check();
	void signalStop();
	void release();
	volatile bool stop;
	volatile bool stopped;
	std::mutex lock;
};

class SafeRegion
{
public:
	SafeRegion(ThreadContext* context);
	~SafeRegion();
	void leave();
private:
	SafePoint* sp;
	bool exited;
};

class ThreadContext: public DoubleLinkedList<ThreadContext>::Node, public Variant
{
public:	
	static const VARIANT_TYPE TYPE = THREAD_VAR;
	static ThreadContext* current();
	friend class ThreadManager;
	friend class GC;
	friend class DoubleLinkedList<ThreadContext>;
	typedef unsigned int THREAD_ID;
	void start();
	void kill();
	void setThreadName(shared_string);
	shared_string getThreadName();
	int join();
	StackMachineThread* getVirtualThread();
	SafePoint safePoint;
private:
	ThreadContext();
	ThreadContext(StackMachine* machine);
	virtual ~ThreadContext();
	StackMachineThread* virtualThread;
	shared_string name;
	GC::ThreadInfo* gcInfo;
	THREAD_ID myId;
	void threadFunction();
	std::thread* threadHandle;
	StackMachine* machine;

};


#define SAFE_REGION(code) { SafeRegion region(ThreadContext::current()); code; }