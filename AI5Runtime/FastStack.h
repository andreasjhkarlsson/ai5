#pragma once

#include "RuntimeError.h"
class GC;

// Very simple stack designed to be as fast as possible.
template<typename T>
class FastStack
{
public:
	friend class GC;
	FastStack(const size_t limit);
	~FastStack(void);
	__forceinline void push(T element);
	__forceinline T pop();
	__forceinline void popMany(size_t count);
	__forceinline T top();
	__forceinline T& get(size_t offset_from_top);
	__forceinline size_t size();
	__forceinline bool empty();
	__forceinline bool full();
private:
	T* stack;
	int position;
	const size_t limit;
};

template<typename T>
void FastStack<T>::push(T element)
{
	if(full())
		throw RuntimeError(L"Stack overflow!");
	stack[++position] = element;
}

template<typename T>
T FastStack<T>::pop()
{
	if(position == -1)
		throw new RuntimeError(L"Stack underflow!");

	return stack[position--];
}

template<typename T>
void FastStack<T>::popMany(size_t count)
{
	for(size_t i=0;i<count;i++)
		pop();
}

template<typename T>
T& FastStack<T>::get(size_t offset_from_top)
{
	return stack[position-offset_from_top];
}

template<typename T>
size_t FastStack<T>::size()
{
	return position + 1;
}

template<typename T>
T FastStack<T>::top()
{
	return stack[position];
}

template<typename T>
bool FastStack<T>::empty()
{
	return position == -1;
}


template<typename T>
bool FastStack<T>::full()
{
	return (position+1) == limit;
}


template<typename T>
FastStack<T>::FastStack(const size_t limit): position(-1), limit(limit)
{
	stack = new T[limit];
}

template<typename T>
FastStack<T>::~FastStack(void)
{
	delete []stack;
}