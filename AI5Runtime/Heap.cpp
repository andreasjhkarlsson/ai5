#include "Heap.h"

#ifdef _MSC_VER
Heap::Heap(size_t initialSize)
{
	heapHandle = HeapCreate(HEAP_NO_SERIALIZE,initialSize,0);
}


Heap::~Heap(void)
{
	HeapDestroy(heapHandle);
}


void* Heap::alloc(size_t amount)
{
	return HeapAlloc(heapHandle,HEAP_NO_SERIALIZE,amount);
}

void Heap::free(void* mem)
{
	HeapFree(heapHandle,HEAP_NO_SERIALIZE,mem);
}



#endif