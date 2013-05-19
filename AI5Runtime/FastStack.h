#pragma once

// Very simple stack designed to be as fast as possible.
template<typename T>
class FastStack
{
public:
	FastStack(const int limit);
	~FastStack(void);
	__forceinline void push(T element);
	__forceinline T pop();
	__forceinline T top();
	__forceinline T get(int offset_from_top);
	__forceinline size_t size();
	__forceinline bool empty();
private:
	T* stack;
	int position;
};

template<typename T>
void FastStack<T>::push(T element)
{
	stack[++position] = element;
}

template<typename T>
T FastStack<T>::pop()
{
	return stack[position--];
}

template<typename T>
T FastStack<T>::get(int offset_from_top)
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
FastStack<T>::FastStack(const int limit): position(-1)
{
	stack = new T[limit];
}

template<typename T>
FastStack<T>::~FastStack(void)
{
	delete []stack;
}