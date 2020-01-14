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

	Point& operator+=(const Point& point) {
		mX += point.mX;
		mY += point.mY;
		return *this;
	}
	Point& operator-=(const Point& point) {
		mX -= point.mX;
		mY -= point.mY;
		return *this;
	}

	Point operator+(const Point& point) const {
		return {
			mX + point.mX,
			mY + point.mY
		};
	}
	Point operator-(const Point& point) const {
		return {
			mX - point.mX,
			mY - point.mY
		};
	}

	template <typename NewBaseType>
	operator Point<NewBaseType>() const {
		return {
			static_cast<NewBaseType>(mX),
			static_cast<NewBaseType>(mY)
		};
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


struct Point_2d;
struct Point_2df;


/**
 * \class	Point_2d
 * \brief	2D point.
 */
struct Point_2d
{
public:
	Point_2d() = default;
	Point_2d(int x, int y);

public:
	bool operator==(const Point_2d& pt) const;
	bool operator!=(const Point_2d& pt) const;

	Point_2d& operator+=(const Point_2d& pt);
	Point_2d& operator-=(const Point_2d& pt);

	Point_2d operator+(const Point_2d& pt) const;
	Point_2d operator-(const Point_2d& pt) const;

	operator Point_2df() const;

public:
	void x(int x);
	int x() const;
	int& x();

	void y(int y);
	int y() const;
	int& y();

private:
	int mX = 0, mY = 0;
};


/**
 * \class	Point_2df
 * \brief	Floating point 2D Point.
 */
struct Point_2df
{
public:
	Point_2df() = default;
	Point_2df(float x, float y);

public:
	bool operator==(const Point_2df& pt) const;
	bool operator!=(const Point_2df& pt) const;

	Point_2df& operator+=(const Point_2df& pt);
	Point_2df& operator-=(const Point_2df& pt);

	Point_2df operator+(const Point_2df& pt) const;
	Point_2df operator-(const Point_2df& pt) const;

	operator Point_2d() const;

public:
	void x(float x);
	float x() const;
	float& x();

	void y(float y);
	float y() const;
	float& y();

private:
	float mX = 0.0f, mY = 0.0f;
};

} // namespace
