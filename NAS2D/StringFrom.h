#pragma once

#include <string>
#include <type_traits>


namespace NAS2D
{
	template <typename BaseType> struct Point;
	template <typename BaseType> struct Vector;


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


	template <typename BaseType>
	std::string stringFrom(Point<BaseType> point)
	{
		return "(" + std::to_string(point.x) + ", " + std::to_string(point.y) + ")";
	}


	template <typename BaseType>
	std::string stringFrom(Vector<BaseType> vector)
	{
		return "(" + std::to_string(vector.x) + ", " + std::to_string(vector.y) + ")";
	}
}
