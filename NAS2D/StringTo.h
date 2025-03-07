#pragma once

#include <string>
#include <type_traits>
#include <limits>
#include <stdexcept>


namespace NAS2D
{
	template <typename T>
	T stringTo(const std::string& value)
	{
		if constexpr (std::is_same_v<T, std::string> || std::is_same_v<T, const char*>)
		{
			return value;
		}
		else if constexpr (std::is_same_v<T, bool>)
		{
			return value == "true" || value == "1" ? true : value == "false" || value == "0" ? false
				: throw std::invalid_argument("Value must be 'true' or 'false': " + std::string{value});
		}
		else if constexpr (std::is_same_v<T, long double>)
		{
			return std::stold(value);
		}
		else if constexpr (std::is_same_v<T, double>)
		{
			return std::stod(value);
		}
		else if constexpr (std::is_same_v<T, float>)
		{
			return std::stof(value);
		}
		else if constexpr (std::is_same_v<T, long long>)
		{
			return std::stoll(value);
		}
		else if constexpr (std::is_same_v<T, long>)
		{
			return std::stol(value);
		}
		else if constexpr (std::is_same_v<T, int>)
		{
			return std::stoi(value);
		}
		else if constexpr (std::is_same_v<T, unsigned long long>)
		{
			return std::stoull(value);
		}
		else if constexpr (std::is_same_v<T, unsigned long>)
		{
			return std::stoul(value);
		}
		else if constexpr (std::is_integral_v<T> && std::is_signed_v<T>)
		{
			const auto integerValue = std::stoi(value);
			if constexpr (
				std::numeric_limits<int>::max() > std::numeric_limits<T>::max() || std::numeric_limits<int>::min() < std::numeric_limits<T>::min()
			)
			{
				if (integerValue > std::numeric_limits<T>::max() || integerValue < std::numeric_limits<T>::min())
				{
					throw std::out_of_range("Value out of range: " + value);
				}
			}
			return static_cast<T>(integerValue);
		}
		else if constexpr (std::is_integral_v<T> && std::is_unsigned_v<T>)
		{
			const auto integerValue = std::stoul(value);
			if constexpr (std::numeric_limits<unsigned long>::max() > std::numeric_limits<T>::max())
			{
				if (integerValue > std::numeric_limits<T>::max())
				{
					throw std::out_of_range("Value out of range: " + value);
				}
			}
			return static_cast<T>(integerValue);
		}
		else
		{
			static_assert(true, "Unsupported type");
		}
	}
}
