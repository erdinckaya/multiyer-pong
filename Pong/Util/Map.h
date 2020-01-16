#pragma once
#include <mutex>
#include <unordered_map>
#include <atomic>

/// Thread safe basic map.
template <typename K, typename V>
class Map
{
public:
	Map() : empty_(true)
	{
	}

	void insert(const K& k, const V& v)
	{
		std::lock_guard<std::mutex> guard(locker);
		if (map_.find(k) != map_.end())
		{
			return;
		}
		map_[k] = v;
		empty_ = map_.empty();
	}

	bool remove(const K& k)
	{
		std::lock_guard<std::mutex> guard(locker);
		if (map_.empty() || map_.find(k) == map_.end())
		{
			return false;
		}
		map_.erase(k);
		empty_ = map_.empty();
		return true;
	}

	V& operator[](const K& k)
	{
		std::lock_guard<std::mutex> guard(locker);
		return map_[k];
	}

	void fetch(K& k, V& v)
	{
		std::lock_guard<std::mutex> guard(locker);
		for (auto& pop : map_)
		{
			k = pop.first;
			v = pop.second;
			break;
		}
	}

	bool empty() const
	{
		return empty_;
	}

	int size()
	{
		std::lock_guard<std::mutex> guard(locker);
		return map_.size();
	}


private:
	std::unordered_map<K, V> map_;
	std::mutex locker;
	std::atomic<bool> empty_;
};
