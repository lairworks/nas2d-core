#pragma once

#include <string>
#include <unordered_map>

namespace NAS2D
{
	class Dictionary
	{
	public:
		/**
		* /fn
		* template<typename T = std::string>
		* T value(const std::string& key)
		* 
		* Gets the value of a given key.
		*
		* /param key: The key string to search for.
		*
		* /returns The value of 'key' as type T.
		* /remarks
		* Throws std::runtime_error if the key is not found.
		* Throws any exceptions that T(std::string) would throw if T is User-defined.
		* 
		* User-defined types must support construction by `std::string`.
		* 
		*/
		template<typename T = std::string>
		T value(const std::string& key) const
		{
			return NAS2D::from_string<T>(mDictionary.at(key));
		}

		/**
		* /fn
		* template<typename T = std::string>
		* T value_or(const std::string& key, const T& defaultValue)
		*
		* Gets the value of a given key or returns `defaultValue` if it does not exist.
		*
		* /param key: The key string to search for.
		* /param defaultValue: The defaultValue to return if `key` does not exist.
		* /returns The value of 'key' as type T or defaultValue.
		* /remarks
		* User-defined types must support construction by `std::string`.
		* 
		* Throws any exceptions the constructor T(std::string) would throw if T is User-defined.
		*/
		template<typename T = std::string>
		T value_or(const std::string& key, const T& defaultValue) const
		{
			const auto found = mDictionary.find(key);
			if (found == std::end(mDictionary))
			{
				return defaultValue;
			}
			return NAS2D::from_string<T>(found->second);
		}

	protected:
	private:
		std::unordered_map<std::string, std::string> mDictionary;
	};

}
