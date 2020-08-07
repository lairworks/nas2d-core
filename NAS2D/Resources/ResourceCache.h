#pragma once

#include <map>
#include <tuple>


template <typename Resource, typename ... Params>
class ResourceCache
{
public:
	using Key = std::tuple<Params...>;

	const Resource& load(Params... params)
	{
		// Cache lookup key is a tuple of all Resource constructor parameters
		const auto key = Key{params...};

		// Try to find resource from the cache
		auto iter = cache.find(key);
		if (iter == cache.end())
		{
			// Resource wasn't found, so create new one using constructor parameters
			const auto pairIterBool = cache.try_emplace(key, params...);
			iter = pairIterBool.first;
		}

		// Return reference to found or created cached object
		return iter->second;
	}

private:
	std::map<Key, Resource> cache;
};
