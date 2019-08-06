#ifndef __BLOCKING_QUEUE_H__
#define __BLOCKING_QUEUE_H__

#include <deque>
#include "Mutex.h"
#include "Condition.h"

template<typename T>
class BlockingQueue
{
public:
	BlockingQueue()
		: mutex_(),
		  notEmpty(),
		  buffer_()
	{
	}

	void Put(const T& val)
	{
		MutexLockGuard lock(mutex_);
		buffer_.emplace_back(val);
		notEmpty_.notify();
	}

	void Put(T&& val)
	{
		MutexLockGuard lock(mutex_);
		buffer_.emplace_back(std::move(val));
		notEmpty_.notify();
	}

	T Take()
	{
		MutexLockGuard lock(&mutex);
		while (buffer_.empty())
		{
			notEmpty.wait();
		}
		T front(std::move(buffer_.front())); //reference front();
		buffer_.pop_front();
		return std::move(front);
	}

	int size() const
	{
		MutexLockGuard lock(mutex);
		return buffer_.size();
	}

private:
	mutable MutexLock mutex_;
	Condition notEmpty_ GUARDED_BY(mutex_);
	std::deque<T> buffer_ GUARDED_BY(mutex_);
};

#endif //__BLOCKING_QUEUE_H__
