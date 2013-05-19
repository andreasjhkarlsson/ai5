#pragma once
#include <memory.h>

// Very simple list, designed to be as fast as possible.
// Only works with POD!!
template<typename T>
class FastList
{
public:
	FastList(int reserveSize,bool clearMemory): clearMemory(clearMemory),listSize(0)
	{
		bufferSize = reserveSize;
		buffer = new T[reserveSize];
		if(clearMemory)
			memset(buffer,0,sizeof(T)*bufferSize);

	}
	~FastList()
	{
		delete[] buffer;
	}
	__forceinline T get(size_t index)
	{
		return buffer[index];
	}

	__forceinline T& operator[](size_t index)
	{
		return buffer[index];
	}
	__forceinline void set(int index,T data)
	{
		buffer[index] = data;
	}
	__forceinline void push_back(T data)
	{
		if((listSize+1) > this->bufferSize)
		{
			realloc((listSize+1)*2);
		}

		buffer[listSize++] = data;

	}
	__forceinline size_t size()
	{
		return listSize;
	}
	__forceinline void resize(size_t newSize)
	{
		if(newSize > this->bufferSize)
		{
			realloc((newSize+1)*2);
		}

		this->listSize = newSize;

	}

	__forceinline void clear()
	{
		if(clearMemory)
			memset(buffer,0,sizeof(T)*listSize);
		listSize = 0;
	}

private:
	__forceinline void realloc(size_t newSize)
	{
		T* newBuffer = new T[newSize];
		memcpy(newBuffer,buffer,this->listSize*sizeof(T));
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
	size_t listSize;
	bool clearMemory;
};