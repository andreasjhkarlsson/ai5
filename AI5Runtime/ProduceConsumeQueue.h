#pragma once
#include <mutex>
#include <queue>
#include "Semaphore.h"

template <typename T>
class ProduceConsumeQueue
{
public:
	ProduceConsumeQueue(void): objectsAvailable(0)
	{

	}
	~ProduceConsumeQueue(void)
	{

	}
	void push(T object);
	T pop();
private:
	std::queue<T> objects;
	Semaphore objectsAvailable;
	std::mutex queueLock;
};

template <typename T>
void ProduceConsumeQueue<T>::push(T object)
{
	queueLock.lock();
	objects.push(object);
	queueLock.unlock();
	objectsAvailable.signal();
}

template <typename T>
T ProduceConsumeQueue<T>::pop()
{
	objectsAvailable.wait();
	queueLock.lock();

	const T& obj = objects.front();
	objects.pop();
	queueLock.unlock();
	return obj;
}