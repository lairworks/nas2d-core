// ==================================================================================
// = NAS2D
// = Copyright © 2008 - 2019 New Age Software
// ==================================================================================
// = NAS2D is distributed under the terms of the zlib license. You are free to copy,
// = modify and distribute the software under the terms of the zlib license.
// =
// = Acknowledgement of your use of NAS2D is appriciated but is not required.
// ==================================================================================

#include "NAS2D/Renderer/Rectangle.h"

using namespace NAS2D;


// ==================================================================================
// = Rectangle_2d Implementation
// ==================================================================================


/**
 * C'tor.
 *
 * Instantiates a Rectangle_2d to a given set of values.
 *
 * \param x	X-Coordinate.
 * \param y Y-Coordinate.
 * \param w	Width of the Rectangle_2d.
 * \param h Height of the Rectangle_2d.
 */
Rectangle_2d::Rectangle_2d(int x, int y, int w, int h):	mX(x),
														mY(y),
														mW(w),
														mH(h)
{}


/**
 * Copy c'tor.
 *
 * \param rect Rectangle_2d to copy.
 */
Rectangle_2d::Rectangle_2d(const Rectangle_2df& rect):	mX(static_cast<int>(rect.x())),
														mY(static_cast<int>(rect.y())),
														mW(static_cast<int>(rect.width())),
														mH(static_cast<int>(rect.height()))
{}


/**
 * Sets a Rectangle_2d with a given size.
 *
 * \param _x X-Coordinate.
 * \param _y Y-Coordinate.
 * \param _w Width of the Rectangle_2d.
 * \param _h Height of the Rectangle_2d.
 */
void Rectangle_2d::operator()(int _x, int _y, int _w, int _h)
{
	x(_x);
	y(_y);
	width(_w);
	height(_h);
}


/**
 * Compares two rectangles.
 *
 * \param rect	Rectangle_2d to compare against.
 *
 * \returns True if the two rectangles are equivalent.
 */
bool Rectangle_2d::operator==(const Rectangle_2d& rect)
{
	return (x() == rect.x()) && (y() == rect.y()) && (width() == rect.width()) && (height() == rect.height());
}


/**
 * Compares two rectangles, \c float version.
 *
 * \param rect	Rectangle_2df to compare against.
 *
 * \returns True if the two rectangles are equivalent.
 */
bool Rectangle_2d::operator==(const Rectangle_2df& rect)
{
	return (x() == rect.x()) && (y() == rect.y()) && (width() == rect.width()) && (height() == rect.height());
}


/**
 * Compares two rectangles.
 *
 * \param rect	Rectangle_2d to compare against.
 *
 * \returns True if the two rectangles are not equivalent.
 */
bool Rectangle_2d::operator!=(const Rectangle_2d& rect)
{
	return (x() != rect.x()) || (y() != rect.y()) || (width() != rect.width()) || (height() != rect.height());
}


/**
 * Compares two rectangles, \c float version.
 *
 * \param rect	Rectangle_2d to compare against.
 *
 * \returns True if the two rectangles are not equivalent.
 */
bool Rectangle_2d::operator!=(const Rectangle_2df& rect)
{
	return (x() != rect.x()) || (y() != rect.y()) || (width() != rect.width()) || (height() != rect.height());
}


/**
 * Indicates a rectangle has a size of 0.
 */
bool Rectangle_2d::null()
{
	return width() * height() == 0;
}


/**
 * Sets the X-Coordinate of the rectangle.
 */
void Rectangle_2d::x(int x)
{
	mX = x;
}


/**
 * Gets the X-Coordinate of the rectangle.
 */
int Rectangle_2d::x() const
{
	return mX;
}


/**
 * Gets the X-Coordinate of the rectangle.
 */
int& Rectangle_2d::x()
{
	return mX;
}


/**
 * Sets the Y-Coordinate of the rectangle.
 */
void Rectangle_2d::y(int y)
{
	mY = y;
}


/**
 * Gets the Y-Coordinate of the rectangle.
 */
int Rectangle_2d::y() const
{
	return mY;
}


/**
 * Gets the Y-Coordinate of the rectangle.
 */
int& Rectangle_2d::y()
{
	return mY;
}


/**
 * Sets the width of the rectangle.
 */
void Rectangle_2d::width(int w)
{
	mW = w;
}


/**
 * Gets the width of the rectangle.
 */
int Rectangle_2d::width() const
{
	return mW;
}


/**
 * Gets the width of the rectangle.
 */
int& Rectangle_2d::width()
{
	return mW;
}


/**
 * Sets the height of the rectangle.
 */
void Rectangle_2d::height(int h)
{
	mH = h;
}


/**
 * Gets the height of the rectangle.
 */
int Rectangle_2d::height() const
{
	return mH;
}


/**
 * Gets the height of the rectangle.
 */
int& Rectangle_2d::height()
{
	return mH;
}


/**
 * Gets the center point X-coordinate of the rectangle.
 */
int Rectangle_2d::center_x() const
{
	return x() + (width() / 2);
}


/**
 * Gets the center point Y-coordinate of the rectangle.
 */
int Rectangle_2d::center_y() const
{
	return y() + (height() / 2);
}


// ==================================================================================
// = Rectangle_2df Implementation
// ==================================================================================


/**
 * C'tor.
 *
 * Instantiates a Rectangle_2df to a given set of values.
 *
 * \param x	X-Coordinate.
 * \param y Y-Coordinate.
 * \param w	Width of the Rectangle_2d.
 * \param h Height of the Rectangle_2d.
 */
Rectangle_2df::Rectangle_2df(float x, float y, float w, float h):	mX(x),
																	mY(y),
																	mW(w),
																	mH(h)
{}


/**
 * Sets a Rectangle_2df with a given size.
 *
 * \param _x X-Coordinate.
 * \param _y Y-Coordinate.
 * \param _w Width of the Rectangle_2d.
 * \param _h Height of the Rectangle_2d.
 */
void Rectangle_2df::operator()(float _x, float _y, float _w, float _h)
{
	x(_x);
	y(_y);
	width(_w);
	height(_h);
}


/**
 * Compares two rectangles.
 *
 * \param rect	Rectangle_2d to compare against.
 *
 * \returns True if the two rectangles are equivalent.
 */
bool Rectangle_2df::operator==(const Rectangle_2d& rect)
{
	return (x() == rect.x()) && (y() == rect.y()) && (width() == rect.width()) && (height() == rect.height());
}


/**
 * Compares two rectangles.
 *
 * \param rect	Rectangle_2df to compare against.
 *
 * \returns True if the two rectangles are equivalent.
 */
bool Rectangle_2df::operator==(const Rectangle_2df& rect)
{
	return (x() == rect.x()) && (y() == rect.y()) && (width() == rect.width()) && (height() == rect.height());
}


/**
 * Compares two rectangles.
 *
 * \param rect	Rectangle_2d to compare against.
 *
 * \returns True if the two rectangles are not equivalent.
 */
bool Rectangle_2df::operator!=(const Rectangle_2d& rect)
{
	return (x() != rect.x()) || (y() != rect.y()) || (width() != rect.width()) || (height() != rect.height());
}


/**
 * Compares two rectangles.
 *
 * \param rect	Rectangle_2df to compare against.
 *
 * \returns True if the two rectangles are not equivalent.
 */
bool Rectangle_2df::operator!=(const Rectangle_2df& rect)
{
	return (x() != rect.x()) || (y() != rect.y()) || (width() != rect.width()) || (height() != rect.height());
}


/**
 * Indicates a rectangle has a size of 0.
 */
bool Rectangle_2df::null()
{
	return width() * height() == 0;
}


/**
 * Sets the X-Coordinate of the rectangle.
 */
void Rectangle_2df::x(float x)
{
	mX = x;
}


/**
 * Gets the X-Coordinate of the rectangle.
 */
float Rectangle_2df::x() const
{
	return mX;
}


/**
 * Gets the X-Coordinate of the rectangle.
 */
float& Rectangle_2df::x()
{
	return mX;
}


/**
 * Sets the Y-Coordinate of the rectangle.
 */
void Rectangle_2df::y(float y)
{
	mY = y;
}


/**
 * Gets the Y-Coordinate of the rectangle.
 */
float Rectangle_2df::y() const
{
	return mY;
}


/**
 * Gets the Y-Coordinate of the rectangle.
 */
float& Rectangle_2df::y()
{
	return mY;
}


/**
 * Sets the width of the rectangle.
 */
void Rectangle_2df::width(float w)
{
	mW = w;
}


/**
 * Gets the width of the rectangle.
 */
float Rectangle_2df::width() const
{
	return mW;
}


/**
 * Gets the width of the rectangle.
 */
float& Rectangle_2df::width()
{
	return mW;
}


/**
 * Sets the height of the rectangle.
 */
void Rectangle_2df::height(float h)
{
	mH = h;
}


/**
 * Gets the height of the rectangle.
 */
float Rectangle_2df::height() const
{
	return mH;
}


/**
 * Gets the height of the rectangle.
 */
float& Rectangle_2df::height()
{
	return mH;
}


/**
 * Gets the center point X-coordinate of the rectangle.
 */
float Rectangle_2df::center_x() const
{
	return x() + (width() / 2);
}


/**
 * Gets the center point Y-coordinate of the rectangle.
 */
float Rectangle_2df::center_y() const
{
	return y() + (height() / 2);
}
