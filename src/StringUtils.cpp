// ==================================================================================
// = NAS2D
// = Copyright © 2008 - 2019 New Age Software
// ==================================================================================
// = NAS2D is distributed under the terms of the zlib license. You are free to copy,
// = modify and distribute the software under the terms of the zlib license.
// =
// = Acknowledgment of your use of NAS2D is appreciated but is not required.
// ==================================================================================
#include "NAS2D/StringUtils.h"

#include <algorithm>
#include <cctype>
#include <numeric>
#include <sstream>

/**
 * Converts a string to lowercase.
 *
 * \param str	Source string.
 *
 * \return	Returns the converted string.
 */
std::string NAS2D::toLowercase(std::string str)
{
	std::transform(std::begin(str), std::end(str), std::begin(str), [](unsigned char c) noexcept->unsigned char { return static_cast<unsigned char>(::tolower(c)); });
	return str;
}

/**
 * Converts a string to uppercase.
 *
 * \param str	Source string.
 *
 * \return	Returns the converted string.
 */
std::string NAS2D::toUppercase(std::string str)
{
	std::transform(std::begin(str), std::end(str), std::begin(str), [](unsigned char c) noexcept->unsigned char { return static_cast<unsigned char>(::toupper(c)); });
	return str;
}

std::vector<std::string> NAS2D::split(std::string str, char delim /*= ','*/)
{
	const auto potential_count = 1 + std::count(std::begin(str), std::end(str), delim);
	NAS2D::StringList result{};
	result.reserve(potential_count);

	std::istringstream ss(str);

	std::string curString{};
	while (std::getline(ss, curString, delim))
	{
		result.push_back(curString);
	}
	if (ss.eof() && str.back() == delim)
	{
		result.push_back(std::string{});
	}
	result.shrink_to_fit();
	return result;
}
std::vector<std::string> NAS2D::splitSkipEmpty(std::string str, char delim /*= ','*/)
{
	const auto potential_count = 1 + std::count(std::begin(str), std::end(str), delim);
	NAS2D::StringList result{};
	result.reserve(potential_count);

	std::istringstream ss(str);

	std::string curString{};
	while (std::getline(ss, curString, delim))
	{
		if (curString.empty()) { continue; }
		result.push_back(curString);
	}
	result.shrink_to_fit();
	return result;
}

std::pair<std::string, std::string> NAS2D::splitOnFirst(const std::string& str, char delim)
{
	const auto delim_loc = str.find_first_of(delim);
	if (delim_loc == std::string::npos)
	{
		return std::make_pair(str, std::string{});
	}
	else
	{
		return std::make_pair(str.substr(0, delim_loc), str.substr(delim_loc + 1));
	}
}

std::pair<std::string, std::string> NAS2D::splitOnLast(const std::string& str, char delim)
{
	const auto delim_loc = str.find_last_of(delim);
	if (delim_loc == std::string::npos)
	{
		return std::make_pair(std::string{}, str);
	}
	else
	{
		return std::make_pair(str.substr(0, delim_loc), str.substr(delim_loc + 1));
	}
}

std::string NAS2D::join(std::vector<std::string> strs)
{
	const auto acc_op = [](const std::size_t& a, const std::string& b) noexcept->std::size_t { return a + b.size(); };
	auto total_size = std::accumulate(std::begin(strs), std::end(strs), std::size_t{0u}, acc_op);
	std::string result;
	result.reserve(total_size);
	for (const auto& s : strs)
	{
		result += s;
	}
	result.shrink_to_fit();
	return result;
}

std::string NAS2D::join(std::vector<std::string> strs, char delim)
{
	const auto acc_op = [](const std::size_t& a, const std::string& b) noexcept->std::size_t { return a + std::size_t{1u} + b.size(); };
	auto total_size = std::accumulate(std::begin(strs), std::end(strs), std::size_t{0u}, acc_op);
	std::string result;
	result.reserve(total_size);

	for (auto iter = std::begin(strs); iter != std::end(strs); ++iter)
	{
		result += (*iter);
		if (iter + 1 != std::end(strs))
		{
			result.push_back(delim);
		}
	}

	result.shrink_to_fit();
	return result;
}
std::string NAS2D::joinSkipEmpty(std::vector<std::string> strs)
{
	const auto acc_op = [](const std::size_t& a, const std::string& b) noexcept->std::size_t { return a + b.size(); };
	auto total_size = std::accumulate(std::begin(strs), std::end(strs), std::size_t{0u}, acc_op);
	std::string result;
	result.reserve(total_size);
	for (const auto& s : strs)
	{
		if (s.empty()) { continue; }
		result += s;
	}
	result.shrink_to_fit();
	return result;
}

std::string NAS2D::joinSkipEmpty(std::vector<std::string> strs, char delim)
{
	const auto acc_op = [](const std::size_t& a, const std::string& b) noexcept->std::size_t { return a + std::size_t{1u} + b.size(); };
	auto total_size = std::accumulate(std::begin(strs), std::end(strs), std::size_t{0u}, acc_op);
	std::string result;
	result.reserve(total_size);

	for (auto iter = std::begin(strs); iter != std::end(strs); ++iter)
	{
		if ((*iter).empty()) { continue; }
		result += (*iter);
		if (iter + 1 != std::end(strs))
		{
			result.push_back(delim);
		}
	}

	result.shrink_to_fit();
	return result;
}

std::string NAS2D::trimWhitespace(std::string string)
{
	const auto first_non_space = string.find_first_not_of(" \r\n\t\v\f");
	if (first_non_space == std::string::npos)
	{
		return std::string{};
	}
	const auto last_non_space = string.find_last_not_of(" \r\n\t\v\f");
	return string.substr(first_non_space, last_non_space - first_non_space + 1);
}

bool NAS2D::startsWith(std::string_view string, std::string_view start) noexcept
{
	return string.compare(0, start.size(), start) == 0;
}

bool NAS2D::endsWith(std::string_view string, std::string_view end) noexcept
{
	return string.compare(string.size() - end.size(), end.size(), end) == 0;
}

bool NAS2D::startsWith(std::string_view string, char start) noexcept
{
	return !string.empty() && string.front() == start;
}

bool NAS2D::endsWith(std::string_view string, char end) noexcept
{
	return !string.empty() && string.back() == end;
}
