#pragma once

#include "StringUtils.h"
#include <map>


namespace NAS2D {
	class Dictionary
	{
	public:
		bool operator==(const Dictionary& other) const;
		bool operator!=(const Dictionary& other) const;

		Dictionary& operator+=(const Dictionary& other);


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

		void erase(const std::string& key);
		bool has(const std::string& key) const;

		std::vector<std::string> keys() const;

	private:
		std::map<std::string, std::string> mDictionary;
	};


	Dictionary operator+(Dictionary lhs, const Dictionary& rhs);
}
