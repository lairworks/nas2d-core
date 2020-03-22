// ==================================================================================
// = NAS2D
// = Copyright © 2008 - 2019 New Age Software
// ==================================================================================
// = NAS2D is distributed under the terms of the zlib license. You are free to copy,
// = modify and distribute the software under the terms of the zlib license.
// =
// = Acknowledgment of your use of NAS2D is appreciated but is not required.
// ==================================================================================
#pragma once

#include <memory>
#include <string>
#include <string_view>
#include <utility>
#include <vector>

namespace NAS2D
{

	template<typename T>
	T stringTo(const std::string& value);

	// extern template std::string stringTo(const std::string& value);
	// extern template bool stringTo(const std::string& value);
	// extern template char stringTo(const std::string& value);
	// extern template signed char stringTo(const std::string& value);
	// extern template unsigned char stringTo(const std::string& value);
	// extern template short stringTo(const std::string& value);
	// extern template unsigned short stringTo(const std::string& value);
	// extern template int stringTo(const std::string& value);
	// extern template unsigned int stringTo(const std::string& value);
	// extern template long stringTo(const std::string& value);
	// extern template unsigned long stringTo(const std::string& value);
	// extern template long long stringTo(const std::string& value);
	// extern template unsigned long long stringTo(const std::string& value);
	// extern template float stringTo(const std::string& value);
	// extern template double stringTo(const std::string& value);
	// extern template long double stringTo(const std::string& value);


	template<typename T>
	std::string stringFrom(T value);

	// extern template std::string stringFrom(std::string value);
	// extern template std::string stringFrom(bool value);
	// extern template std::string stringFrom(char value);
	// extern template std::string stringFrom(signed char value);
	// extern template std::string stringFrom(unsigned char value);
	// extern template std::string stringFrom(short value);
	// extern template std::string stringFrom(unsigned short value);
	// extern template std::string stringFrom(int value);
	// extern template std::string stringFrom(unsigned int value);
	// extern template std::string stringFrom(long value);
	// extern template std::string stringFrom(unsigned long value);
	// extern template std::string stringFrom(long long value);
	// extern template std::string stringFrom(unsigned long long value);
	// extern template std::string stringFrom(float value);
	// extern template std::string stringFrom(double value);
	// extern template std::string stringFrom(long double value);



	std::string toLowercase(std::string str);
	std::string toUppercase(std::string str);
	std::vector<std::string> split(std::string str, char delim = ',');
	std::vector<std::string> splitSkipEmpty(std::string str, char delim = ',');
	std::pair<std::string, std::string> splitOnFirst(const std::string& str, char delim);
	std::pair<std::string, std::string> splitOnLast(const std::string& str, char delim);
	std::string join(std::vector<std::string> strs);
	std::string join(std::vector<std::string> strs, char delim);
	std::string joinSkipEmpty(std::vector<std::string> strs);
	std::string joinSkipEmpty(std::vector<std::string> strs, char delim);
	std::string trimWhitespace(std::string string);
	bool startsWith(std::string_view string, std::string_view start) noexcept;
	bool endsWith(std::string_view string, std::string_view end) noexcept;
	bool startsWith(std::string_view string, char start) noexcept;
	bool endsWith(std::string_view string, char end) noexcept;

	template<typename... Args>
	std::string string_format(const std::string& format, Args... args)
	{
		std::string buffer;
		std::size_t size = snprintf(buffer.data(), buffer.size(), format.c_str(), args...);
		buffer.resize(size + 1); // Including null (avoid clipping/undefined behavior)
		snprintf(buffer.data(), buffer.size(), format.c_str(), args...);
		buffer.resize(size); // Strip null temrinator
		return buffer;
	}

	/**
 * \typedef StringList
 * \brief	A list of std::string's.
 *
 * The StringList is provided primarily as a convenience typedef
 * but is also used by some of NAS2D's functions.
 */
	using StringList = std::vector<std::string>;

	namespace detail
	{
		template<typename T>
		T rangeCheckHelper(const std::string& value)
		{
			const auto valueAsInteger = std::stoi(value);
			if constexpr (std::numeric_limits<T>::max() < std::numeric_limits<int>::max())
			{
				if (std::numeric_limits<T>::max() < valueAsInteger)
				{
					throw std::out_of_range("rangeCheckHelper argument out of range");
				}
			}
			return static_cast<T>(valueAsInteger);
		}
	}

} // namespace NAS2D

