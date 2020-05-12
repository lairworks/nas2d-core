#pragma once

#include "StringValue.h"
#include <map>


namespace NAS2D {
	class Dictionary
	{
	public:
		Dictionary() = default;
		Dictionary(std::map<std::string, StringValue> initialEntries) : mDictionary{initialEntries} {}

		bool operator==(const Dictionary& other) const;
		bool operator!=(const Dictionary& other) const;

		Dictionary& operator+=(const Dictionary& other);


		template <typename T = std::string>
		T get(const std::string& key) const
		{
			return mDictionary.at(key).to<T>();
		}

		template <typename T = std::string>
		void set(const std::string& key, T value)
		{
			mDictionary[key].from(value);
		}

		void erase(const std::string& key);
		bool has(const std::string& key) const;

		std::vector<std::string> keys() const;

	private:
		std::map<std::string, StringValue> mDictionary;
	};


	Dictionary operator+(Dictionary lhs, const Dictionary& rhs);
}
