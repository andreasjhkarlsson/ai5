#pragma once

#ifdef _MSC_VER
#include <Windows.h>
typedef HANDLE HeapHandle;
#endif

class Heap
{
public:
	Heap(size_t initialSize);
	~Heap(void);
	void* alloc(size_t amount);
	void free(void* mem);
private:
	HeapHandle heapHandle;
};

