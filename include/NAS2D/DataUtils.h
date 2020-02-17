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
			const auto found = mDictionary.find(key);
			if(found == std::end(mDictionary))
			{
				throw std::runtime_error(std::string{"Dictionary key with name \'"} + key + "\' does not exist");
			}
			if constexpr (std::is_fundamental_v<T>)
			{
				// is bool, arithmetic, void, or nullptr.
				// nullptr is undefined behavior
				// void type is undefined behavior.
				return value_helper(key, T{});
			}
			else
			{
				return T(found->second);
			}
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
			if constexpr(std::is_fundamental_v<T>)
			{
				// is bool, arithmetic, void, or nullptr.
				// nullptr is undefined behavior.
				// void type is undefined behavior.
				return value_or_helper(key, defaultValue);
			}
			else
			{
				return T(found->second);
			}
		}

	protected:
	private:
		std::unordered_map<std::string, std::string> mDictionary;


		// Using Template specializations as function overloads breaks overload sets.
		// Don't do it.
		// See https://www.youtube.com/watch?v=NIDEjY5ywqU&t=39m35s

		bool value_or_helper(const std::string& key, const bool& defaultValue) const;

		char value_or_helper(const std::string& key, const char& defaultValue) const noexcept;
		signed char value_or_helper(const std::string& key, const signed char& defaultValue) const;
		unsigned char value_or_helper(const std::string& key, const unsigned char& defaultValue) const;

		short value_or_helper(const std::string& key, const short& defaultValue) const;
		unsigned short value_or_helper(const std::string& key, const unsigned short& defaultValue) const;

		int value_or_helper(const std::string& key, const int& defaultValue) const;
		unsigned int value_or_helper(const std::string& key, const unsigned int& defaultValue) const;

		long value_or_helper(const std::string& key, const long& defaultValue) const;
		unsigned long value_or_helper(const std::string& key, const unsigned long& defaultValue) const;

		long long value_or_helper(const std::string& key, const long long& defaultValue) const;
		unsigned long long value_or_helper(const std::string& key, const unsigned long long& defaultValue) const;

		float value_or_helper(const std::string& key, const float& defaultValue) const;
		double value_or_helper(const std::string& key, const double& defaultValue) const;
		long double value_or_helper(const std::string& key, const long double& defaultValue) const;

		bool value_helper(const std::string& key, bool) const;
		char value_helper(const std::string& key, char) const;
		signed char value_helper(const std::string& key, signed char) const;
		unsigned char value_helper(const std::string& key, unsigned char) const;

		short value_helper(const std::string& key, short) const;
		unsigned short value_helper(const std::string& key, unsigned short) const;

		int value_helper(const std::string& key, int) const;
		unsigned int value_helper(const std::string& key, unsigned int) const;

		long value_helper(const std::string& key, long) const;
		unsigned long value_helper(const std::string& key, unsigned long) const;

		long long value_helper(const std::string& key, long long) const;
		unsigned long long value_helper(const std::string& key, unsigned long long) const;

		float value_helper(const std::string& key, float) const;
		double value_helper(const std::string& key, double) const;
		long double value_helper(const std::string& key, long double) const;

	};


}
