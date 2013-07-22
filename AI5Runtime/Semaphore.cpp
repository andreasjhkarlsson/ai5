#include "Semaphore.h"

void Semaphore::signal() {
	std::lock_guard<std::mutex> lock(mutex_);
	value_++;
	if (value_ <= 0) {
		wakeups_++;
		cond_.notify_one();
	}
}

void Semaphore::wait() {
	std::unique_lock<std::mutex> lock(mutex_);
	value_--;
	if (value_ <0) {
		cond_.wait (lock,
			[this] { return this->wakeups_ > 0; });
		wakeups_--;
	}
}
