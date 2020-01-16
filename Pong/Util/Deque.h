#pragma once
#include <deque>
#include <mutex>
#include "UtilMacro.h"


// Thread safe deque
template<typename  T>
class Deque
{
public:
	Deque() : empty_(true) {}

	Deque(const Deque<T>& deque) : deque_(deque.deque_), empty_(deque.empty()) {}

	T& operator[](int index)
	{
		std::lock_guard<std::mutex> guard(locker);
		return deque_[index];
	}

	size_t size()
	{
		std::lock_guard<std::mutex> guard(locker);
		return deque_.size();
	}
	
	T front()
	{
		std::lock_guard<std::mutex> guard(locker);
		DEBUG_COND(empty_, "Deque is empty `front`");
		return deque_.front();
	}

	T back()
	{
		std::lock_guard<std::mutex> guard(locker);
		DEBUG_COND(empty_, "Deque is empty `back`");
		return deque_.back();
	}

	void push_back(const T& t)
	{
		std::lock_guard<std::mutex> guard(locker);
		deque_.push_back(t);
		empty_ = deque_.empty();
	}

	void push_front(const T& t)
	{
		std::lock_guard<std::mutex> guard(locker);
		deque_.push_front(t);
		empty_ = deque_.empty();
	}

	template<typename... Args>
	void emplace_back(Args... args)
	{
		std::lock_guard<std::mutex> guard(locker);
		deque_.emplace_back(args...);
		empty_ = deque_.empty();
	}

	template<typename... Args>
	void emplace_front(Args... args)
	{
		std::lock_guard<std::mutex> guard(locker);
		deque_.emplace_front(args...);
		empty_ = deque_.empty();
	}

	void pop_back()
	{
		std::lock_guard<std::mutex> guard(locker);
		DEBUG_COND(empty_, "Deque is empty `back`");
		deque_.pop_back();
		empty_ = deque_.empty();
	}

	void pop_front()
	{
		std::lock_guard<std::mutex> guard(locker);
		DEBUG_COND(empty_, "Deque is empty `back`");
		deque_.pop_front();
		empty_ = deque_.empty();
	}

	void clear()
	{
		std::lock_guard<std::mutex> guard(locker);
		deque_.clear();
		empty_ = deque_.empty();
	}
	
	std::deque<T> copy()
	{
		std::lock_guard<std::mutex> guard(locker);
		std::deque<T> temp = deque_;
		return temp;
	}

	bool empty() const
	{
		return empty_;
	}

private:
	std::deque<T> deque_;
	std::mutex locker;
	std::atomic<bool> empty_;
};
