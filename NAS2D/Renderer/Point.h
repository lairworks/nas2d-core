// ==================================================================================
// = NAS2D
// = Copyright © 2008 - 2019 New Age Software
// ==================================================================================
// = NAS2D is distributed under the terms of the zlib license. You are free to copy,
// = modify and distribute the software under the terms of the zlib license.
// =
// = Acknowledgement of your use of NAS2D is appriciated but is not required.
// ==================================================================================

#pragma once

#include "Vector.h"


namespace NAS2D {


template <typename BaseType>
struct Point {
	BaseType x = 0;
	BaseType y = 0;

	bool operator==(const Point& point) const {
		return (x == point.x) && (y == point.y);
	}
	bool operator!=(const Point& point) const {
		return !(*this == point);
	}

	Point& operator+=(const Vector<BaseType>& vector) {
		x += vector.x;
		y += vector.y;
		return *this;
	}

	Point& operator-=(const Vector<BaseType>& vector) {
		x -= vector.x;
		y -= vector.y;
		return *this;
	}

	Point operator+(const Vector<BaseType>& vector) const {
		return {x + vector.x, y + vector.y};
	}

	Point operator-(const Vector<BaseType>& vector) const {
		return {x - vector.x, y - vector.y};
	}

	Vector<BaseType> operator-(const Point& point) const {
		return {x - point.x, y - point.y};
	}

	template <typename NewBaseType>
	operator Point<NewBaseType>() const {
		return {
			static_cast<NewBaseType>(x),
			static_cast<NewBaseType>(y)
		};
	}

	template <typename NewBaseType>
	Point<NewBaseType> to() const {
		return static_cast<Point<NewBaseType>>(*this);
	}
};


template <typename BaseType>
Point(BaseType, BaseType) -> Point<BaseType>;

} // namespace
