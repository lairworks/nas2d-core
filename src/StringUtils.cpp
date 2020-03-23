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

namespace
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

	template<>
	char rangeCheckHelper(const std::string& value)
	{
		if (value.size() != 1)
		{
			throw std::invalid_argument("rangeCheckHelper string length must be exactly one (1).");
		}
		return value[0];
	}

}

namespace NAS2D
{

	// template char rangeCheckHelper(const std::string& value);

	template<>
	std::string stringTo(const std::string& value)
	{
		return value;
	}

	template<>
	bool stringTo(const std::string& value)
	{
		auto v = toLowercase(value);
		if(v == "true") return true;
		if(v == "false") return false;
		throw std::invalid_argument("Value is not 'true' or 'false' value: " + value);
	}

	template<>
	char stringTo(const std::string& value)
	{
		return rangeCheckHelper<char>(value);
	}

	template<>
	unsigned char stringTo(const std::string& value)
	{
		return rangeCheckHelper<unsigned char>(value);
	}

	template<>
	signed char stringTo(const std::string& value)
	{
		return rangeCheckHelper<signed char>(value);
	}

	template<>
	short stringTo(const std::string& value)
	{
		return rangeCheckHelper<short>(value);
	}

	template<>
	unsigned short stringTo(const std::string& value)
	{
		return rangeCheckHelper<unsigned short>(value);
	}

	template<>
	int stringTo(const std::string& value)
	{
		return std::stoi(value);
	}

	template<>
	unsigned int stringTo(const std::string& value)
	{
		const auto valueAsInteger = std::stoul(value);
		if constexpr(std::numeric_limits<unsigned int>::max() < std::numeric_limits<unsigned long>::max())
		{
			if(std::numeric_limits<unsigned int>::max() < valueAsInteger)
			{
				throw std::out_of_range("from_string argument out of range");
			}
		}
		return static_cast<unsigned int>(valueAsInteger);
	}

	template<>
	long stringTo(const std::string& value)
	{
		return std::stol(value);
	}

	template<>
	unsigned long stringTo(const std::string& value)
	{
		return std::stoul(value);
	}

	template<>
	long long stringTo(const std::string& value)
	{
		return std::stoll(value);
	}

	template<>
	unsigned long long stringTo(const std::string& value)
	{
		return std::stoull(value);
	}

	template<>
	float stringTo(const std::string& value)
	{
		return std::stof(value);
	}

	template<>
	double stringTo(const std::string& value)
	{
		return std::stod(value);
	}

	template<>
	long double stringTo(const std::string& value)
	{
		return std::stold(value);
	}


	template<>
	std::string stringFrom(std::string value)
	{
		return value;
	}

	template<>
	std::string stringFrom(bool value)
	{
		return value ? "true" : "false";
	}

	template<>
	std::string stringFrom(char value)
	{
		return std::string{value};
	}

	template<>
	std::string stringFrom(signed char value)
	{
		return std::to_string(static_cast<int>(value));
	}

	template<>
	std::string stringFrom(unsigned char value)
	{
		return std::to_string(static_cast<unsigned int>(value));
	}

	template<>
	std::string stringFrom(short value)
	{
		return std::to_string(static_cast<int>(value));
	}

	template<>
	std::string stringFrom(unsigned short value)
	{
		return std::to_string(static_cast<unsigned int>(value));
	}


	/**
	 * \fn toLowercase(const std::string& str)
	 *
	 * Converts a string to lowercase.
	 *
	 * \param str	Source string.
	 *
	 * \return	Returns the converted string.
	 */
	std::string toLowercase(std::string str)
	{
		std::transform(std::begin(str), std::end(str), std::begin(str), [](unsigned char c) noexcept->unsigned char { return static_cast<unsigned char>(::tolower(c)); });
		return str;
	}

	/**
	 * \fn toUppercase(const std::string& str)
	 *
	 * Converts a string to uppercase.
	 *
	 * \param str	Source string.
	 *
	 * \return	Returns the converted string.
	 */
	std::string toUppercase(std::string str)
	{
		std::transform(std::begin(str), std::end(str), std::begin(str), [](unsigned char c) noexcept->unsigned char { return static_cast<unsigned char>(::toupper(c)); });
		return str;
	}

	std::vector<std::string> split(std::string str, char delim /*= ','*/)
	{
		const auto potential_count = 1 + std::count(std::begin(str), std::end(str), delim);
		StringList result{};
		result.reserve(potential_count);

		std::istringstream ss(str);

		std::string curString{};
		while(std::getline(ss, curString, delim))
		{
			result.push_back(curString);
		}
		if(ss.eof() && str.back() == delim)
		{
			result.push_back(std::string{});
		}
		result.shrink_to_fit();
		return result;
	}
	std::vector<std::string> splitSkipEmpty(std::string str, char delim /*= ','*/)
	{
		const auto potential_count = 1 + std::count(std::begin(str), std::end(str), delim);
		StringList result{};
		result.reserve(potential_count);

		std::istringstream ss(str);

		std::string curString{};
		while(std::getline(ss, curString, delim))
		{
			if(curString.empty()) { continue; }
			result.push_back(curString);
		}
		result.shrink_to_fit();
		return result;
	}

	std::pair<std::string, std::string> splitOnFirst(const std::string& str, char delim)
	{
		const auto delim_loc = str.find_first_of(delim);
		if(delim_loc == std::string::npos)
		{
			return std::make_pair(str, std::string{});
		} else
		{
			return std::make_pair(str.substr(0, delim_loc), str.substr(delim_loc + 1));
		}
	}

	std::pair<std::string, std::string> splitOnLast(const std::string& str, char delim)
	{
		const auto delim_loc = str.find_last_of(delim);
		if(delim_loc == std::string::npos)
		{
			return std::make_pair(std::string{}, str);
		} else
		{
			return std::make_pair(str.substr(0, delim_loc), str.substr(delim_loc + 1));
		}
	}

	std::string join(std::vector<std::string> strs)
	{
		const auto acc_op = [](const std::size_t& a, const std::string& b) noexcept->std::size_t { return a + b.size(); };
		auto total_size = std::accumulate(std::begin(strs), std::end(strs), std::size_t{0u}, acc_op);
		std::string result;
		result.reserve(total_size);
		for(const auto& s : strs)
		{
			result += s;
		}
		result.shrink_to_fit();
		return result;
	}

	std::string join(std::vector<std::string> strs, char delim)
	{
		const auto acc_op = [](const std::size_t& a, const std::string& b) noexcept->std::size_t { return a + std::size_t{1u} +b.size(); };
		auto total_size = std::accumulate(std::begin(strs), std::end(strs), std::size_t{0u}, acc_op);
		std::string result;
		result.reserve(total_size);

		for(auto iter = std::begin(strs); iter != std::end(strs); ++iter)
		{
			result += (*iter);
			if(iter + 1 != std::end(strs))
			{
				result.push_back(delim);
			}
		}

		result.shrink_to_fit();
		return result;
	}
	std::string joinSkipEmpty(std::vector<std::string> strs)
	{
		const auto acc_op = [](const std::size_t& a, const std::string& b) noexcept->std::size_t { return a + b.size(); };
		auto total_size = std::accumulate(std::begin(strs), std::end(strs), std::size_t{0u}, acc_op);
		std::string result;
		result.reserve(total_size);
		for(const auto& s : strs)
		{
			if(s.empty()) { continue; }
			result += s;
		}
		result.shrink_to_fit();
		return result;
	}

	std::string joinSkipEmpty(std::vector<std::string> strs, char delim)
	{
		const auto acc_op = [](const std::size_t& a, const std::string& b) noexcept->std::size_t { return a + std::size_t{1u} +b.size(); };
		auto total_size = std::accumulate(std::begin(strs), std::end(strs), std::size_t{0u}, acc_op);
		std::string result;
		result.reserve(total_size);

		for(auto iter = std::begin(strs); iter != std::end(strs); ++iter)
		{
			if((*iter).empty()) { continue; }
			result += (*iter);
			if(iter + 1 != std::end(strs))
			{
				result.push_back(delim);
			}
		}

		result.shrink_to_fit();
		return result;
	}

	std::string trimWhitespace(std::string string)
	{
		const auto first_non_space = string.find_first_not_of(" \r\n\t\v\f");
		if(first_non_space == std::string::npos)
		{
			return std::string{};
		}
		const auto last_non_space = string.find_last_not_of(" \r\n\t\v\f");
		return string.substr(first_non_space, last_non_space - first_non_space + 1);
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

} // namespace NAS2D
