// ==================================================================================
// = NAS2D
// = Copyright © 2008 - 2019 New Age Software
// ==================================================================================
// = NAS2D is distributed under the terms of the zlib license. You are free to copy,
// = modify and distribute the software under the terms of the zlib license.
// =
// = Acknowledgement of your use of NAS2D is appriciated but is not required.
// ==================================================================================

#include "NAS2D/Renderer/Point.h"

using namespace NAS2D;


// ==================================================================================
// = Point_2d Implementation
// ==================================================================================


/**
 * C'tor
 *
 * Instantiates a Point_2d with a given value set.
 *
 * \param x	X-Coordinate.
 * \param y	Y-Coordinate.
 */
Point_2d::Point_2d(int x, int y):	mX(x),
									mY(y)
{}


bool Point_2d::operator==(const Point_2d& pt) const
{
	return (x() == pt.x()) && (y() == pt.y());
}


bool Point_2d::operator!=(const Point_2d& pt) const
{
	return (x() != pt.x()) || (y() != pt.y());
}


Point_2d& Point_2d::operator+=(const Point_2d& pt)
{
	mX += pt.x();
	mY += pt.y();

	return *this;
}


Point_2d& Point_2d::operator-=(const Point_2d& pt)
{
	mX -= pt.x();
	mY -= pt.y();

	return *this;
}


Point_2d Point_2d::operator+(const Point_2d& pt) const
{
	Point_2d r = *this;
	r += pt;
	return r;
}


Point_2d Point_2d::operator-(const Point_2d& pt) const
{
	Point_2d r = *this;
	r -= pt;
	return r;
}


Point_2d::operator Point_2df() const
{
	return Point_2df{
		static_cast<float>(mX),
		static_cast<float>(mY)
	};
}


void Point_2d::x(int x)
{
	mX = x;
}


int Point_2d::x() const
{
	return mX;
}


int& Point_2d::x()
{
	return mX;
}


void Point_2d::y(int y)
{
	mY = y;
}


int Point_2d::y() const
{
	return mY;
}


int& Point_2d::y()
{
	return mY;
}


// ==================================================================================
// = Point_2df Implementation
// ==================================================================================

Point_2df::Point_2df(float x, float y):	mX(x),
										mY(y)
{}


bool Point_2df::operator==(const Point_2df& pt) const
{
	return (mX == pt.x()) && (mY == pt.y());
}


bool Point_2df::operator!=(const Point_2df& pt) const
{
	return (x() != pt.x()) || (y() != pt.y());
}


Point_2df& Point_2df::operator+=(const Point_2df& pt)
{
	mX += pt.x();
	mY += pt.y();

	return *this;
}


Point_2df& Point_2df::operator-=(const Point_2df& pt)
{
	mX -= pt.x();
	mY -= pt.y();

	return *this;
}


Point_2df Point_2df::operator+(const Point_2df& pt) const
{
	Point_2df r = *this;
	r += pt;
	return r;
}


Point_2df Point_2df::operator-(const Point_2df& pt) const
{
	Point_2df r = *this;
	r -= pt;
	return r;
}


Point_2df::operator Point_2d() const
{
	return Point_2d{
		static_cast<int>(mX),
		static_cast<int>(mY)
	};
}


void Point_2df::x(float x)
{
	mX = x;
}


float Point_2df::x() const
{
	return mX;
}


float& Point_2df::x()
{
	return mX;
}


void Point_2df::y(float y)
{
	mY = y;
}


float Point_2df::y() const
{
	return mY;
}


float& Point_2df::y()
{
	return mY;
}
