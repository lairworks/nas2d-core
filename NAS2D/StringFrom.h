#pragma once

#include <string>
#include <type_traits>


namespace NAS2D
{
	template <typename T>
	std::string stringFrom(T value)
	{
		if constexpr (std::is_convertible<T, std::string>::value)
		{
			return value;
		}
		else if constexpr (std::is_same_v<T, bool>)
		{
			return value ? "true" : "false";
		}
		else
		{
			return std::to_string(value);
		}
	}
}
