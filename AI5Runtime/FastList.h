#pragma once
#include <memory.h>

// Very simple list, designed to be as fast as possible.
// Only works with POD!!
template<typename T>
class FastList
{
public:
	FastList(size_t reserveSize): FastList(reserveSize,false)
	{
		
	}
	FastList(int reserveSize,bool clearMemory): clearMemory(clearMemory)
	{
		bufferSize = reserveSize;
		buffer = new T[reserveSize];
		if(clearMemory)
			memset(bufer,0,sizeof(T)*bufferSize);

	}
	~FastList()
	{
		delete[] buffer;
	}
	__forceinline T get(int index)
	{
		return buffer[index];
	}
	__forceinline void set(int index,T data)
	{
		buffer[index] = data;
	}
	__forceinline void push_back(T data)
	{
		if((size+1) > this->bufferSize)
		{
			realloc((size+1)*2);
		}

		buffer[size++] = data;

	}
	__forceinline size_t size()
	{
		return size;
	}
	__forceinline void resize(size_t size)
	{
		if(size > this->bufferSize)
		{
			realloc((size+1)*2);
		}

		this->size = size;

	}
private:
	__forceinline void realloc(size_t newSize)
	{
		T* newBuffer = new T[newSize];
		memcpy(newBuffer,buffer,this->size*sizeof(T));
		delete[] buffer;
		this->buffer = newBuffer;
		bufferSize = newSize;
		if(clearMemory)
		{
			memset(buffer,0,sizeof(T)*bufferSize);
		}

	}
	T* buffer;
	size_t bufferSize;
	size_t size;
	bool clearMemory;
};