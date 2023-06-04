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
#include <sstream>


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

	std::vector<std::string> split(const std::string& string, char delimiter /*= ','*/)
	{
		const auto potentialCount = static_cast<std::size_t>(1 + std::count(std::begin(string), std::end(string), delimiter));
		StringList result{};
		result.reserve(potentialCount);

		std::istringstream ss(string);

		std::string curString{};
		while (std::getline(ss, curString, delimiter))
		{
			result.push_back(curString);
		}
		if (ss.eof() && !string.empty() && string.back() == delimiter)
		{
			result.push_back(std::string{});
		}
		result.shrink_to_fit();
		return result;
	}

	std::pair<std::string, std::string> splitOnFirst(const std::string& string, char delimiter)
	{
		const auto delim_loc = string.find_first_of(delimiter);
		if (delim_loc == std::string::npos)
		{
			return std::make_pair(string, std::string{});
		}
		else
		{
			return std::make_pair(string.substr(0, delim_loc), string.substr(delim_loc + 1));
		}
	}

	std::pair<std::string, std::string> splitOnLast(const std::string& string, char delimiter)
	{
		const auto delim_loc = string.find_last_of(delimiter);
		if (delim_loc == std::string::npos)
		{
			return std::make_pair(std::string{}, string);
		}
		else
		{
			return std::make_pair(string.substr(0, delim_loc), string.substr(delim_loc + 1));
		}
	}

	std::string join(const std::vector<std::string>& strs, std::string_view delimiter)
	{
		std::string result;

		if (!strs.empty())
		{
			const auto totalStringSize = flattenSize(strs);
			const auto delimiterSize = (strs.size() - 1) * delimiter.size();
			result.reserve(totalStringSize + delimiterSize);

			result += strs.front();
			for (auto iter = std::begin(strs) + 1; iter != std::end(strs); ++iter)
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
