#pragma once

#include "types.h"

class GC
{
private:
	static GC* instance;

	struct BlockHeader
	{
		BlockHeader(BlockHeader*,BlockHeader*);
		BlockHeader();
		BlockHeader* previous;
		BlockHeader* next;
		Variant* object;
		bool mark;
		char generation;
		char referencedFrom; 
		
	};

	static const size_t BLOCKHEADER_ALIGNED_SIZE = sizeof(BlockHeader) + (sizeof(BlockHeader)%sizeof(BlockHeader*));

public:

	static void init()
	{
		instance = new GC();
	}

	template <class T>
	static T* alloc()
	{
		void* memory = malloc(BLOCKHEADER_ALIGNED_SIZE + sizeof(T));
		BlockHeader* header = static_cast<BlockHeader*>(memory);
		new (header) BlockHeader();
		Variant* variant = reinterpret_cast<Variant*>(((char*)memory)+BLOCKHEADER_ALIGNED_SIZE);
		new (variant) T();

		header->object = variant;

		instance->trackObject(header);

		return static_cast<T*>(variant);
	}
	template <class T,class U>
	static T* alloc(U arg)
	{
		void* memory = malloc(BLOCKHEADER_ALIGNED_SIZE + sizeof(T));
		BlockHeader* header = static_cast<BlockHeader*>(memory);
		new (header) BlockHeader();
		Variant* variant = reinterpret_cast<Variant*>(((char*)memory)+BLOCKHEADER_ALIGNED_SIZE);
		new (variant) T(arg);

		header->object = variant;

		instance->trackObject(header);

		return static_cast<T*>(variant);
	}

	static void collect(StackMachine*);
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
	void trackObject(BlockHeader*);
	void run(StackMachine*);
	void mark(BlockHeader*);
	void mark(const VariantReference<>&ref);
	void sweep();
	BlockHeader* VarRefToBlockHead(const VariantReference<>&ref);
};
