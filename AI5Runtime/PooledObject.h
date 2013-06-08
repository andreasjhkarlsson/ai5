#pragma once
#include "FastStack.h"


template<class T>
class PooledObject
{
public:
	PooledObject()
	{

	}
	static T* getInstance()
	{
		if(pool == nullptr)
		{
			pool = new FastStack<T*>(T::POOL_SIZE);
			for(int i=0;i<T::POOL_SIZE;i++)
			{
				pool->push(new T());
			}
		}
		if(!pool->empty())
			return pool->pop();
		return new T();
	}

	static void returnInstance(T* instance)
	{
		if(pool->full())
			delete instance;
		else
			pool->push(instance);
	}

private:
	static FastStack<T*>* pool;
};

template<class T>
FastStack<T*>* PooledObject<T>::pool = nullptr;