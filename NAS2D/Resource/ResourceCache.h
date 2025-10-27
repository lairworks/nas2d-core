// ==================================================================================
// = NAS2D
// = Copyright © 2008 - 2020 New Age Software
// ==================================================================================
// = NAS2D is distributed under the terms of the zlib license. You are free to copy,
// = modify and distribute the software under the terms of the zlib license.
// =
// = Acknowledgment of your use of NAS2D is appreciated but is not required.
// ==================================================================================

#pragma once

#include <map>
#include <tuple>


namespace NAS2D
{
	template <typename Resource, typename... Params>
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
			if (iter != cache.end())
			{
				// Return reference to found cached object
				return iter->second;
			}

			// Resource wasn't found, so create new one using constructor parameters
			const auto pairIterBool = cache.try_emplace(key, params...);
			iter = pairIterBool.first;
			return iter->second;
		}


		void unload(Params... params)
		{
			cache.erase(Key{params...});
		}


		void clear()
		{
			cache.clear();
		}


		auto size() const
		{
			return cache.size();
		}

	private:
		std::map<Key, Resource> cache{};
	};
}
