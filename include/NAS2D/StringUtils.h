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

	template<> std::string stringTo<std::string>(const std::string& value);
	template<> bool stringTo<bool>(const std::string& value);
	template<> char stringTo<char>(const std::string& value);
	template<> signed char stringTo<signed char>(const std::string& value);
	template<> unsigned char stringTo<unsigned char>(const std::string& value);
	template<> short stringTo<short>(const std::string& value);
	template<> unsigned short stringTo<unsigned short>(const std::string& value);
	template<> int stringTo<int>(const std::string& value);
	template<> unsigned int stringTo<unsigned int>(const std::string& value);
	template<> long stringTo<long>(const std::string& value);
	template<> unsigned long stringTo<unsigned long>(const std::string& value);
	template<> long long stringTo<long long>(const std::string& value);
	template<> unsigned long long stringTo<unsigned long long>(const std::string& value);
	template<> float stringTo<float>(const std::string& value);
	template<> double stringTo<double>(const std::string& value);
	template<> long double stringTo<long double>(const std::string& value);


	template<typename T>
	std::string stringFrom(T value)
	{
		return std::to_string(value);
	}

	template<> std::string stringFrom(std::string value);
	template<> std::string stringFrom(bool value);
	template<> std::string stringFrom(char value);
	template<> std::string stringFrom(signed char value);
	template<> std::string stringFrom(unsigned char value);
	template<> std::string stringFrom(short value);
	template<> std::string stringFrom(unsigned short value);


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

} // namespace NAS2D

