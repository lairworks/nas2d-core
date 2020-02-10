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
	Point() = default;
	Point(const Point& point) = default;
	Point& operator=(const Point& other) = default;

	Point(BaseType x, BaseType y) :
		mX(x),
		mY(y)
	{}

	bool operator==(const Point& point) const {
		return (mX == point.mX) && (mY == point.mY);
	}
	bool operator!=(const Point& point) const {
		return !(*this == point);
	}

	Point& operator+=(const Vector<BaseType>& vector) {
		mX += vector.x;
		mY += vector.y;
		return *this;
	}

	Point operator+(const Vector<BaseType>& vector) const {
		return {mX + vector.x, mY + vector.y};
	}

	Vector<BaseType> operator-(const Point& point) const {
		return {mX - point.mX, mY - point.mY};
	}

	template <typename NewBaseType>
	operator Point<NewBaseType>() const {
		return {
			static_cast<NewBaseType>(mX),
			static_cast<NewBaseType>(mY)
		};
	}

	template <typename NewBaseType>
	Point<NewBaseType> to() const {
		return static_cast<Point<NewBaseType>>(*this);
	}

	void x(BaseType x) {
		mX = x;
	}
	BaseType x() const {
		return mX;
	}
	BaseType& x() {
		return mX;
	}

	void y(BaseType y) {
		mY = y;
	}
	BaseType y() const {
		return mY;
	}
	BaseType& y() {
		return mY;
	}

private:
	BaseType mX = 0;
	BaseType mY = 0;
};


/**
 * \class	Point_2d
 * \brief	2D point.
 */
using Point_2d = Point<int>;
extern template struct Point<int>;


/**
 * \class	Point_2df
 * \brief	Floating point 2D Point.
 */
using Point_2df = Point<float>;
extern template struct Point<float>;


} // namespace
