#pragma once

#include <queue>
#include <mutex>
#include <thread>
#include "types.h"
#include "Semaphore.h"
#include "ProduceConsumeQueue.h"
#include "DoubleLinkedList.h"
#include "platform.h"
#include "Heap.h"

class ListVariant;
class HashMapVariant;
class HandleVariant;
class Scope;
class NameVariant;
class NameReferenceVariant;
class UserFunctionVariant;
class ThreadContext;

class GC
{
public:

	// This represents info for a allocated variant. The memory layout looks like this:
	/*
		BlockHeader
		(padding to make sure variant is nicely aligned)
		Variant
	*/
	struct BlockHeader: public DoubleLinkedList<BlockHeader>::Node
	{
		BlockHeader();
		Variant* object;
		size_t totalSize;
		bool mark;
		char generation;
		char referencedFrom; 
	};

	static const size_t GEN0_LIMIT = 2*1024*1024; // 16 MB
	static const size_t HEAP_LIMIT = 901024*1024; // 90 MB
	static const int GEN_CYCLE_INTERVAL = 6;

	struct ThreadInfo
	{
		//DoubleLinkedList<BlockHeader> gen0;
	};

	// Public interface for users.
	static void init(StackMachine*);
	static void initThread(ThreadContext*);
	static void uninitThread(ThreadContext*);
	static void shutdown();
	static void collect(bool wait);
	static void cleanup();
	template <class T>
	static const VariantReference<T>& persistReference(Variant* owner,const VariantReference<T>& object);
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

private:

	static const int GEN_COUNT = 3;
	static const char STATIC_GEN = -1;

	static const int MESSAGE_START_SYNCHRONOUS_CYCLE = 0;
	static const int MESSAGE_START_ASYNCHRONOUS_CYCLE = 1;
	static const int MESSAGE_STOP = 2;
	static GC* instance;
	static const size_t BLOCKHEADER_ALIGNED_SIZE = sizeof(BlockHeader) + (sizeof(BlockHeader)%sizeof(BlockHeader*));

	struct CycleStats
	{
		CycleStats(): instancesFreed(0),bytesFreed(0)
		{ }
		size_t instancesFreed;
		size_t bytesFreed;
	};

	template <class T>
	static BlockHeader* allocBlockHeader();
	static BlockHeader* VarRefToBlockHead(const VariantReference<>&ref);
	static BlockHeader* VarPtrToBlockHead(Variant*);

	GC(StackMachine*);
	void trackObject(BlockHeader*);
	void trackStaticObject(BlockHeader*);
	void run();
	void mark(BlockHeader*,int gen,bool root=false);
	void mark(const VariantReference<>&ref,int gen);
	void mark(ListVariant* list,int gen);
	void mark(HashMapVariant* hashMap,int gen);
	void mark(Scope* scope,int gen);
	void mark(HandleVariant* handle,int gen);
	void mark(NameVariant* name,int gen);
	void mark(NameReferenceVariant* nameReference,int gen);
	void mark(UserFunctionVariant* userFunc,int gen);
	void mark(ThreadContext* tContext,int gen);
	void sweep(DoubleLinkedList<BlockHeader>* objects,CycleStats&,int gen);
	void freeAll();
	void freeBlockList(DoubleLinkedList<BlockHeader>*);
	void freeObject(BlockHeader*);
	void stopTheWorld();
	void resumeTheWorld();
	void checkHeapLimit();


	LightWeightMutex heapLock;
	
	std::thread markAndSweepThread;
	Semaphore cycleComplete;
	ProduceConsumeQueue<int> messageQueue;
	volatile bool killThread;

	DoubleLinkedList<BlockHeader> gens[GEN_COUNT];
	DoubleLinkedList<BlockHeader> statics;

	StackMachine* machine;

	Heap heap;

	size_t gen0Size;
	size_t totalSize;
	size_t cycle;

};


template <class T>
GC::BlockHeader* GC::allocBlockHeader()
{
	const size_t allocSize = BLOCKHEADER_ALIGNED_SIZE + sizeof(T);
	void* memory = instance->heap.alloc(allocSize);
	BlockHeader* header = static_cast<BlockHeader*>(memory);
	new (header) BlockHeader();
	header->object = reinterpret_cast<Variant*>(((char*)memory)+BLOCKHEADER_ALIGNED_SIZE);
	header->totalSize = allocSize;
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
	instance->trackStaticObject(header);
	return static_cast<T*>(header->object);
}

template <class T,class U>
T* GC::staticAlloc(U arg)
{
	BlockHeader* header = allocBlockHeader<T>();
	new (header->object) T(arg);
	instance->trackStaticObject(header);
	return static_cast<T*>(header->object);
}

template <class T,class U,class V>
T* GC::staticAlloc(U arg,V arg2)
{
	BlockHeader* header = allocBlockHeader<T>();
	new (header->object) T(arg,arg2);
	instance->trackStaticObject(header);
	return static_cast<T*>(header->object);
}

// </BAD DRY>

template<class T>
const VariantReference<T>& GC::persistReference(Variant* owner,const VariantReference<T>& object)
{
	LockLightWeightMutex(&instance->heapLock);
	int ownerGen = VarPtrToBlockHead(owner)->generation;
	BlockHeader* block = VarRefToBlockHead(object);

	if(block != nullptr && ownerGen > block->generation && ownerGen > block->referencedFrom)
	{
		// This means that an old object references a new object!! 
		block->referencedFrom = ownerGen;
	}

	UnlockLightWeightMutex(&instance->heapLock);

	return object;
}