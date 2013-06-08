#pragma once
#include "FastStack.h"


// Very simple pool! Only works wiht objects that have zero arguments in their constructor
template<typename T>
class SimplePool
{
public:
	SimplePool(int size,int limit): pool(size),limit(limit)
	{
		for(int i=0;i<size;i++)
		{
			pool.push(new T());
		}

	}

	~SimplePool()
	{
		while(!pool.empty())
			delete pool.pop();
	}

	T* getInstance()
	{
		if(!pool.empty())
			return pool.pop();
		return new T();
	}

	void returnInstance(T* instance)
	{
		if(pool.size() < limit)
			pool.push(instance);
		else
			delete instance;
	}

private:
	int limit;
	FastStack<T*> pool;
};