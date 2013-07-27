#pragma once
#include "types.h"
#include "DoubleLinkedList.h"
#include <thread>
#include <mutex>
#include "Variant.h"
#include "gc.h"
#include "ThreadContext.h"

class ThreadManager
{
public:
	friend class GC;
	ThreadManager(void);
	~ThreadManager(void);
	ThreadContext* createThread(StackMachine* machine,shared_string name);
	void reportTermination(ThreadContext* threadContext);
	void killAll();
	void suspendAll();
	void resumeAll();
private:
	static void threadEntry(ThreadContext*);
	DoubleLinkedList<ThreadContext> threads;
	std::mutex threadsLock;

};