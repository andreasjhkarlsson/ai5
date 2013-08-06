#pragma once

#include <queue>
#include <mutex>
#include <thread>
#include "types.h"
#include "Semaphore.h"
#include "ProduceConsumeQueue.h"
#include "DoubleLinkedList.h"
#include "platform.h"

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
		bool mark;
		char generation;
		char referencedFrom; 
	};


	struct ThreadInfo
	{
		DoubleLinkedList<BlockHeader> gen0;
	};

	// Public interface for users.
	static void init(StackMachine*);
	static void initThread(ThreadContext*);
	static void uninitThread(ThreadContext*);
	static void shutdown();
	static void collect(bool wait);
	static void cleanup();
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
	static const char GENERATION_STATIC = -1;
	static const int MESSAGE_START_SYNCHRONOUS_CYCLE = 0;
	static const int MESSAGE_START_ASYNCHRONOUS_CYCLE = 1;
	static const int MESSAGE_STOP = 2;
	static GC* instance;
	static const size_t BLOCKHEADER_ALIGNED_SIZE = sizeof(BlockHeader) + (sizeof(BlockHeader)%sizeof(BlockHeader*));


	template <class T>
	static BlockHeader* allocBlockHeader();
	static BlockHeader* VarRefToBlockHead(const VariantReference<>&ref);

	GC(StackMachine*);
	void trackObject(BlockHeader*);
	void addStaticObject(BlockHeader*);
	void run();
	void mark(BlockHeader*);
	void mark(const VariantReference<>&ref);
	void mark(ListVariant* list);
	void mark(HashMapVariant* hashMap);
	void mark(Scope* scope);
	void mark(HandleVariant* handle);
	void mark(NameVariant* name);
	void mark(NameReferenceVariant* nameReference);
	void mark(UserFunctionVariant* userFunc);
	void mark(ThreadContext* tContext);
	void sweep(DoubleLinkedList<BlockHeader>* objects);
	void freeAll();
	void freeObject(BlockHeader*);
	void stopTheWorld();
	void resumeTheWorld();

	DoubleLinkedList<BlockHeader> staticList;
	LightWeightMutex staticsLock;
	DoubleLinkedList<BlockHeader> orphans;
	LightWeightMutex orphansLock;
	
	std::thread markAndSweepThread;
	Semaphore cycleComplete;
	ProduceConsumeQueue<int> messageQueue;
	volatile bool killThread;

	StackMachine* machine;
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