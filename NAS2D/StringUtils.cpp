// ==================================================================================
// = NAS2D
// = Copyright © 2008 - 2020 New Age Software
// ==================================================================================
// = NAS2D is distributed under the terms of the zlib license. You are free to copy,
// = modify and distribute the software under the terms of the zlib license.
// =
// = Acknowledgment of your use of NAS2D is appreciated but is not required.
// ==================================================================================
#include "StringUtils.h"

#include "ContainerUtils.h"

#include <algorithm>
#include <cctype>
#include <numeric>
#include <utility>


namespace NAS2D
{

	/**
	 * Converts a string to lowercase.
	 *
	 * \param string	Source string.
	 *
	 * \return	Returns the converted string.
	 */
	std::string toLowercase(std::string string)
	{
		std::transform(std::begin(string), std::end(string), std::begin(string), [](unsigned char c) noexcept -> unsigned char { return static_cast<unsigned char>(::tolower(c)); });
		return string;
	}

	/**
	 * Converts a string to uppercase.
	 *
	 * \param string	Source string.
	 *
	 * \return	Returns the converted string.
	 */
	std::string toUppercase(std::string string)
	{
		std::transform(std::begin(string), std::end(string), std::begin(string), [](unsigned char c) noexcept -> unsigned char { return static_cast<unsigned char>(::toupper(c)); });
		return string;
	}

	std::size_t countDelimiters(const std::string& string, char delimiter)
	{
		return static_cast<std::size_t>(std::count(std::begin(string), std::end(string), delimiter));
	}

	std::vector<std::string> split(const std::string& string, char delimiter /*= ','*/)
	{
		std::vector<std::string> result{};
		if (string.empty()) { return result; }
		result.reserve(1 + countDelimiters(string, delimiter));

		const auto length = string.size();
		for (std::size_t begin = 0, current = 0; current <= length; begin = ++current)
		{
			while (current < length && string[current] != delimiter) { ++current; }
			result.push_back(std::string(string, begin, current - begin));
		}

		return result;
	}

	std::pair<std::string, std::string> splitOnFirst(const std::string& string, char delimiter)
	{
		const auto delimiterLocation = string.find_first_of(delimiter);
		if (delimiterLocation == std::string::npos)
		{
			return std::make_pair(string, std::string{});
		}
		else
		{
			return std::make_pair(string.substr(0, delimiterLocation), string.substr(delimiterLocation + 1));
		}
	}

	std::pair<std::string, std::string> splitOnLast(const std::string& string, char delimiter)
	{
		const auto delimiterLocation = string.find_last_of(delimiter);
		if (delimiterLocation == std::string::npos)
		{
			return std::make_pair(std::string{}, string);
		}
		else
		{
			return std::make_pair(string.substr(0, delimiterLocation), string.substr(delimiterLocation + 1));
		}
	}

	std::string join(const std::vector<std::string>& strings, std::string_view delimiter)
	{
		std::string result;

		if (!strings.empty())
		{
			const auto totalStringSize = flattenSize(strings);
			const auto delimiterSize = (strings.size() - 1) * delimiter.size();
			result.reserve(totalStringSize + delimiterSize);

			result += strings.front();
			for (auto iter = std::begin(strings) + 1; iter != std::end(strings); ++iter)
			{
				result += delimiter;
				result += (*iter);
			}
		}

		return result;
	}

	std::string trimWhitespace(std::string_view string)
	{
		const auto first_non_space = string.find_first_not_of(" \r\n\t\v\f");
		if (first_non_space == std::string::npos)
		{
			return std::string{};
		}
		const auto last_non_space = string.find_last_not_of(" \r\n\t\v\f");
		return std::string{string.substr(first_non_space, last_non_space - first_non_space + 1)};
	}

	bool startsWith(std::string_view string, std::string_view start) noexcept
	{
		return string.compare(0, start.size(), start) == 0;
	}

	bool endsWith(std::string_view string, std::string_view end) noexcept
	{
		return string.compare(string.size() - end.size(), end.size(), end) == 0;
	}

	bool startsWith(std::string_view string, char start) noexcept
	{
		return !string.empty() && string.front() == start;
	}

	bool endsWith(std::string_view string, char end) noexcept
	{
		return !string.empty() && string.back() == end;
	}

} // namespace
