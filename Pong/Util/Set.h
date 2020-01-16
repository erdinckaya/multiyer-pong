#pragma once
#include <mutex>
#include <unordered_set>
#include <atomic>

/// Thread safe basic set
template <typename T>
class Set
{
public:
	Set() : empty_(true)
	{
	}

	void insert(const T& t)
	{
		std::lock_guard<std::mutex> guard(locker);
		if (set_.find(t) != set_.end())
		{
			return;
		}
		set_.insert(t);
		empty_ = set_.empty();
	}

	bool remove(const T& t)
	{
		std::lock_guard<std::mutex> guard(locker);
		if (set_.empty() || set_.find(t) == set_.end())
		{
			return false;
		}
		set_.erase(t);
		empty_ = set_.empty();
		return true;
	}

	T& operator[](const T& t)
	{
		std::lock_guard<std::mutex> guard(locker);
		return set_[t];
	}

	void fetch(T& t)
	{
		std::lock_guard<std::mutex> guard(locker);
		for (auto& pop : set_)
		{
			t = pop;
			break;
		}
	}

	bool empty() const
	{
		return empty_;
	}


private:
	std::unordered_set<T> set_;
	std::mutex locker;
	std::atomic<bool> empty_;
};
