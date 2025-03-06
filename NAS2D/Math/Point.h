// ==================================================================================
// = NAS2D
// = Copyright © 2008 - 2020 New Age Software
// ==================================================================================
// = NAS2D is distributed under the terms of the zlib license. You are free to copy,
// = modify and distribute the software under the terms of the zlib license.
// =
// = Acknowledgment of your use of NAS2D is appreciated but is not required.
// ==================================================================================

#pragma once

#include "Vector.h"

#include <string>


namespace NAS2D
{
	template <typename BaseType>
	struct Point
	{
		BaseType x = 0;
		BaseType y = 0;

		constexpr bool operator==(const Point& point) const
		{
			return (x == point.x) && (y == point.y);
		}

		constexpr bool operator!=(const Point& point) const
		{
			return !(*this == point);
		}

		Point& operator+=(const Vector<BaseType>& vector)
		{
			x += vector.x;
			y += vector.y;
			return *this;
		}

		Point& operator-=(const Vector<BaseType>& vector)
		{
			x -= vector.x;
			y -= vector.y;
			return *this;
		}

		constexpr Point operator+(const Vector<BaseType>& vector) const
		{
			return {x + vector.x, y + vector.y};
		}

		constexpr Point operator-(const Vector<BaseType>& vector) const
		{
			return {x - vector.x, y - vector.y};
		}

		constexpr Vector<BaseType> operator-(const Point& point) const
		{
			return {x - point.x, y - point.y};
		}

		constexpr Point skewBy(const Vector<BaseType>& other) const
		{
			return {x * other.x, y * other.y};
		}

		constexpr Point skewInverseBy(const Vector<BaseType>& other) const
		{
			verifyNotZero(other.x, other.y, "Cannot skewInverseBy a vector with a zero component");
			return {x / other.x, y / other.y};
		}

		template <typename NewBaseType>
		constexpr operator Point<NewBaseType>() const
		{
			return {NewBaseType(x), NewBaseType(y)};
		}

		template <typename NewBaseType>
		constexpr Point<NewBaseType> to() const
		{
			return Point<NewBaseType>(*this);
		}

		explicit operator std::string() const
		{
			return "(" + std::to_string(x) + ", " + std::to_string(y) + ")";
		}
	};


	template <typename BaseType>
	Point(BaseType, BaseType) -> Point<BaseType>;


	// Commutative Vector + Point
	template <typename BaseType>
	constexpr Point<BaseType> operator+(Vector<BaseType> vector, Point<BaseType> point)
	{
		return point + vector;
	}


	// Partial order comparisons

	template <typename BaseType>
	bool operator<=(Point<BaseType> p1, Point<BaseType> p2)
	{
		return (p1.x <= p2.x) && (p1.y <= p2.y);
	}

	template <typename BaseType>
	bool operator>=(Point<BaseType> p1, Point<BaseType> p2)
	{
		return p2 <= p1;
	}

	template <typename BaseType>
	bool operator<(Point<BaseType> p1, Point<BaseType> p2)
	{
		return (p1.x < p2.x) && (p1.y < p2.y);
	}

	template <typename BaseType>
	bool operator>(Point<BaseType> p1, Point<BaseType> p2)
	{
		return p2 < p1;
	}


	extern template struct Point<int>;
	extern template struct Point<float>;
} // namespace NAS2D
