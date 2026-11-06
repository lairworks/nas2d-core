#pragma once

#include <string>
#include <charconv>
#include <type_traits>
#include <system_error>
#include <stdexcept>


namespace NAS2D
{
	template <typename T>
	concept hasFromChars = requires(const std::string& value, T result)
	{
		std::from_chars(value.data(), value.data() + value.size(), result);
	};


	template <typename T>
	T stringTo(const std::string& value)
	{
		if constexpr (std::is_same_v<T, std::string>)
		{
			return value;
		}
		else if constexpr (std::is_same_v<T, bool>)
		{
			return value == "true" || value == "1" ? true : value == "false" || value == "0" ? false
				: throw std::invalid_argument("Value must be 'true' or 'false': " + value);
		}
		else if constexpr (std::is_same_v<T, long double> && !hasFromChars<long double>)
		{
			return std::stold(value);
		}
		else if constexpr (std::is_arithmetic_v<T>)
		{
			T result{};
			const auto [end, errorCode] = std::from_chars(value.data(), value.data() + value.size(), result);

			if (errorCode == std::errc::invalid_argument)
			{
				throw std::invalid_argument("Invalid arithmetic value: " + value);
			}
			if (errorCode == std::errc::result_out_of_range)
			{
				throw std::out_of_range("Value out of range: " + value);
			}
			if (end != value.data() + value.size())
			{
				throw std::invalid_argument("Unexpected characters trailing arithmetic value: '" + value + "' ('" + std::string(end, value.data() + value.size()) + "')");
			}

			return result;
		}
		else
		{
			static_assert(false, "Unsupported type");
		}
	}
}
