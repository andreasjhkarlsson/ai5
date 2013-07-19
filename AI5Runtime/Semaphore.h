#pragma once
#include <mutex>
#include <condition_variable>

// Code copied from: http://p9as.blogspot.se/2012/06/c11-semaphores.html
class Semaphore {
	int value_;
	int wakeups_;
	std::mutex mutex_;
	std::condition_variable cond_;
 public:
	Semaphore(int value) : value_(value), wakeups_(0) {}
	void wait();
	void signal();
};

