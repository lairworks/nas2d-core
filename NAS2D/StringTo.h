#pragma once

#include <string_view>
#include <string>
#include <charconv>
#include <utility>
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
	concept useLongDoubleFallback = std::is_same_v<T, long double> && !hasFromChars<T>;


	template <typename T>
	T stringTo(std::string_view value)
		requires (!useLongDoubleFallback<T>)
	{
		if constexpr (std::is_same_v<T, std::string_view>)
		{
			return value;
		}
		else if constexpr (std::is_same_v<T, std::string>)
		{
			return std::string{value};
		}
		else if constexpr (std::is_same_v<T, bool>)
		{
			return value == "true" || value == "1" ? true : value == "false" || value == "0" ? false
				: throw std::invalid_argument("Value must be 'true' or 'false': " + std::string{value});
		}
		else if constexpr (std::is_arithmetic_v<T>)
		{
			T result{};
			const auto [end, errorCode] = std::from_chars(value.data(), value.data() + value.size(), result);

			if (errorCode == std::errc::invalid_argument)
			{
				throw std::invalid_argument("Invalid arithmetic value: " + std::string{value});
			}
			if (errorCode == std::errc::result_out_of_range)
			{
				throw std::out_of_range("Value out of range: " + std::string{value});
			}
			if (end != value.data() + value.size())
			{
				throw std::invalid_argument("Unexpected characters trailing arithmetic value: '" + std::string{value} + "' ('" + std::string(end, value.data() + value.size()) + "')");
			}

			return result;
		}
		else
		{
			static_assert(false, "Unsupported type");
		}
	}


	template <typename T>
	T stringTo(std::string&& value)
		requires std::is_same_v<T, std::string_view>
	= delete;


	template <typename T>
	T stringTo(const std::string& value)
		requires useLongDoubleFallback<T>
	{
		return std::stold(value);
	}

	template <typename T>
	T stringTo(std::string_view value)
		requires useLongDoubleFallback<T>
	{
		return stringTo<T>(std::string{value});
	}


	template <typename T>
	T stringTo(std::string&& value)
		requires std::is_same_v<T, std::string>
	{
		return std::move(value);
	}


	template <typename T, std::size_t stringLiteralSize>
	T stringTo(const char (&value)[stringLiteralSize])
	{
		return stringTo<T>(std::string_view{value});
	}
}
