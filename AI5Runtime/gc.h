#pragma once

#include "types.h"
#include "Semaphore.h"
#include <queue>
#include <thread>
#include "ProduceConsumeQueue.h"
#include "DoubleLinkedList.h"
#include <mutex>

class GC
{
public:
	struct BlockHeader: public DoubleLinkedList<BlockHeader>::Node
	{
		BlockHeader();
		Variant* object;
		bool mark;
		char generation;
		char referencedFrom; 
	};

	struct ThreadContext: public DoubleLinkedList<ThreadContext>::Node
	{
		ThreadContext(){}
		StackMachineThread* machineThread;
		DoubleLinkedList<BlockHeader>* gen0;

	};
private:
	static GC* instance;

	static const char GENERATION_STATIC = -1;

	static const int MESSAGE_START_CYCLE = 0;
	static const int MESSAGE_STOP = 1;

	static const size_t BLOCKHEADER_ALIGNED_SIZE = sizeof(BlockHeader) + (sizeof(BlockHeader)%sizeof(BlockHeader*));

	template <class T>
	static BlockHeader* allocBlockHeader();

public:
	static void init(StackMachine*);
	static void initThread(StackMachineThread*);
	static void uninitThread();
	static void shutdown();
	template <class T>
	static T* alloc();
	template <class T,class U>
	static T* alloc(U arg);
	template <class T,class U,class V>
	static T* alloc(U arg,V arg2);
	template <class T>
	static T* staticAlloc();
	template <class T,class U>
	static T* staticAlloc(U arg);
	template <class T,class U,class V>
	static T* staticAlloc(U arg,V arg2);
	static void collect(bool wait);
	static void cleanup();
public:



private:

	DoubleLinkedList<BlockHeader> staticList;
	std::mutex threadsLock;
	DoubleLinkedList<ThreadContext> threads;

	GC(StackMachine*);

	std::thread markAndSweepThread;
	ProduceConsumeQueue<int> messageQueue;
	Semaphore cycleComplete;

	volatile bool killThread;

	StackMachine* machine;

	void trackObject(BlockHeader*);
	void addStaticObject(BlockHeader*);
	void run();
	void mark(BlockHeader*);
	void mark(const VariantReference<>&ref);
	void sweep(DoubleLinkedList<BlockHeader>* objects);
	void freeAll();
	void freeObject(BlockHeader*);
	BlockHeader* VarRefToBlockHead(const VariantReference<>&ref);
};


template <class T>
GC::BlockHeader* GC::allocBlockHeader()
{
	void* memory = malloc(BLOCKHEADER_ALIGNED_SIZE + sizeof(T));
	BlockHeader* header = static_cast<BlockHeader*>(memory);
	new (header) BlockHeader();
	header->object = reinterpret_cast<Variant*>(((char*)memory)+BLOCKHEADER_ALIGNED_SIZE);
	return header;
}

// VS2012 doesn't support variadic templates, so really bad dry here :(
// <BAD DRY>

template <class T>
T* GC::alloc()
{
	BlockHeader* header = allocBlockHeader<T>();
	new (header->object) T();
	instance->trackObject(header);
	return static_cast<T*>(header->object);
}

template <class T,class U>
T* GC::alloc(U arg)
{
	BlockHeader* header = allocBlockHeader<T>();
	new (header->object) T(arg);
	instance->trackObject(header);
	return static_cast<T*>(header->object);
}

template <class T,class U,class V>
T* GC::alloc(U arg,V arg2)
{
	BlockHeader* header = allocBlockHeader<T>();
	new (header->object) T(arg,arg2);
	instance->trackObject(header);
	return static_cast<T*>(header->object);
}


template <class T>
T* GC::staticAlloc()
{
	BlockHeader* header = allocBlockHeader<T>();
	new (header->object) T();
	instance->addStaticObject(header);
	return static_cast<T*>(header->object);
}

template <class T,class U>
T* GC::staticAlloc(U arg)
{
	BlockHeader* header = allocBlockHeader<T>();
	new (header->object) T(arg);
	instance->addStaticObject(header);
	return static_cast<T*>(header->object);
}

template <class T,class U,class V>
T* GC::staticAlloc(U arg,V arg2)
{
	BlockHeader* header = allocBlockHeader<T>();
	new (header->object) T(arg,arg2);
	instance->addStaticObject(header);
	return static_cast<T*>(header->object);
}

// </BAD DRY>