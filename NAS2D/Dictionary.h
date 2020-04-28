#pragma once

#include "StringUtils.h"
#include "ContainerUtils.h"
#include <map>


namespace NAS2D {
	class Dictionary
	{
	public:
		template <typename T = std::string>
		T get(const std::string& key) const
		{
			return stringTo<T>(mDictionary.at(key));
		}

		template <typename T = std::string>
		void set(const std::string& key, T value)
		{
			mDictionary[key] = stringFrom<T>(value);
		}

		std::vector<std::string> keys() const
		{
			return getKeys(mDictionary);
		}

	private:
		std::map<std::string, std::string> mDictionary;
	};
}
