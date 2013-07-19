#pragma once

#include "types.h"

class GC
{
private:
	static GC* instance;


	static const char GENERATION_STATIC = -1;

	struct BlockHeader
	{
		BlockHeader(BlockHeader*,BlockHeader*);
		BlockHeader();
		BlockHeader* previous;
		BlockHeader* next;
		Variant* object;
		bool sentinel;
		bool mark;
		char generation;
		char referencedFrom; 
		
	};

	static const size_t BLOCKHEADER_ALIGNED_SIZE = sizeof(BlockHeader) + (sizeof(BlockHeader)%sizeof(BlockHeader*));

	template <class T>
	static BlockHeader* allocBlockHeader();

public:
	static void init();
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
	static void collect(StackMachine*);
	static void cleanup();
private:


	class DoubleLinkedList
	{
	public:
		DoubleLinkedList();
		void push_back(BlockHeader*);
		void push_front(BlockHeader*);
		BlockHeader* erase(BlockHeader*);
		BlockHeader* firstElement();
	private:
		BlockHeader* start;
		BlockHeader* end;
	};
	DoubleLinkedList objectList;
	DoubleLinkedList staticList;
	void trackObject(BlockHeader*);
	void addStaticObject(BlockHeader*);
	void run(StackMachine*);
	void mark(BlockHeader*);
	void mark(const VariantReference<>&ref);
	void sweep();
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
	header->generation = GENERATION_STATIC;
	instance->addStaticObject(header);
	return static_cast<T*>(header->object);
}

template <class T,class U>
T* GC::staticAlloc(U arg)
{
	BlockHeader* header = allocBlockHeader<T>();
	new (header->object) T(arg);
	header->generation = GENERATION_STATIC;
	instance->addStaticObject(header);
	return static_cast<T*>(header->object);
}

template <class T,class U,class V>
T* GC::staticAlloc(U arg,V arg2)
{
	BlockHeader* header = allocBlockHeader<T>();
	new (header->object) T(arg,arg2);
	header->generation = GENERATION_STATIC;
	instance->addStaticObject(header);
	return static_cast<T*>(header->object);
}

// </BAD DRY>