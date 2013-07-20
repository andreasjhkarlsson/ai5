#pragma once
#include <mutex>
#include <queue>
#include "Semaphore.h"

// Simple queue that allows to threads to post data to eachother in a thread safe manner.
template <typename T>
class ProduceConsumeQueue
{
public:
	ProduceConsumeQueue(void): objectsAvailable(0) { }
	~ProduceConsumeQueue(void) { }
	void push(T object);
	T pop();
private:
	std::queue<T> objects;
	Semaphore objectsAvailable;
	std::mutex queueLock;
};

// Pushes an object to queue and notifies a waiting thread that an object is available!
template <typename T>
void ProduceConsumeQueue<T>::push(T object)
{
	std::lock_guard<std::mutex> guard(queueLock);
	objects.push(object);
	
	objectsAvailable.signal();
}

// Blocks until an object is available.
template <typename T>
T ProduceConsumeQueue<T>::pop()
{
	// Wait for object to be ready.
	objectsAvailable.wait();
	std::lock_guard<std::mutex> guard(queueLock);
	// I hate that this is two lines with std::queue...
	const T& obj = objects.front();
	objects.pop();
	return obj;
}