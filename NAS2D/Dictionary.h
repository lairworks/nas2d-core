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

#include "StringValue.h"

#include <string>
#include <vector>
#include <map>


namespace NAS2D
{
	class Dictionary
	{
	public:
		Dictionary() = default;
		Dictionary(std::map<std::string, StringValue> initialEntries);

		bool operator==(const Dictionary& other) const;
		bool operator!=(const Dictionary& other) const;

		const StringValue& operator[](const std::string& key) const;
		StringValue& operator[](const std::string& key);

		Dictionary& operator+=(const Dictionary& other);


		template <typename T>
		T get(const std::string& key, T defaultValue) const
		{
			const auto iterator = mDictionary.find(key);
			return (iterator != mDictionary.end()) ? iterator->second.to<T>() : defaultValue;
		}

		template <typename T = std::string>
		T get(const std::string& key) const
		{
			const auto iterator = mDictionary.find(key);
			if (iterator == mDictionary.end())
			{
				throw std::out_of_range("Dictionary lookup failed for key : " + key);
			}
			return iterator->second.to<T>();
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
		std::map<std::string, StringValue> mDictionary{};
	};


	Dictionary operator+(Dictionary lhs, const Dictionary& rhs);
}
