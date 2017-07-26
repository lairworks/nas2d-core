// ==================================================================================
// = NAS2D
// = Copyright © 2008 - 2017 New Age Software
// ==================================================================================
// = NAS2D is distributed under the terms of the zlib license. You are free to copy,
// = modify and distribute the software under the terms of the zlib license.
// = 
// = Acknowledgement of your use of NAS2D is appriciated but is not required.
// ==================================================================================

#include "NAS2D/Renderer/Primitives.h"
#include "NAS2D/Common.h"

using namespace NAS2D;

// ==================================================================================
// = Color_4ub Implementation
// ==================================================================================


/**
 * Default c'tor.
 */
Color_4ub::Color_4ub():	mR(static_cast<unsigned char>(255)),
						mG(static_cast<unsigned char>(255)),
						mB(static_cast<unsigned char>(255)),
						mA(static_cast<unsigned char>(255))
{}


/**
 * C'tor
 * 
 * Instantiates a Color_4ub with a given RGBA value set.
 * 
 * \param r	Red compontent. Valid values are 0 - 255.
 * \param g	Green compontent. Valid values are 0 - 255.
 * \param b	Blue compontent. Valid values are 0 - 255.
 * \param a	Alpha compontent. Valid values are 0 - 255.
 */
Color_4ub::Color_4ub(int r, int g, int b, int a):	mR(static_cast<unsigned char>(clamp(r, 0, 255))),
													mG(static_cast<unsigned char>(clamp(g, 0, 255))),
													mB(static_cast<unsigned char>(clamp(b, 0, 255))),
													mA(static_cast<unsigned char>(clamp(a, 0, 255)))
{}


/**
 * Sets a Color_4ub with a given RGBA value set.
 * 
 * \param r	Red compontent. Valid values are 0 - 255.
 * \param g	Green compontent. Valid values are 0 - 255.
 * \param b	Blue compontent. Valid values are 0 - 255.
 * \param a	Alpha compontent. Valid values are 0 - 255.
 */
void Color_4ub::operator()(int r, int g, int b, int a)
{
	red(clamp(r, 0, 255));
	green(clamp(g, 0, 255));
	blue(clamp(b, 0, 255));
	alpha(clamp(a, 0, 255));
}


/**
 * Gets the red component value of the color.
 */
int Color_4ub::red() const
{
	return static_cast<int>(mR);
}


/**
 * Gets the green component value of the color.
 */
int Color_4ub::green() const
{
	return static_cast<int>(mG);
}


/**
 * Gets the blue component value of the color.
 */
int Color_4ub::blue() const
{
	return static_cast<int>(mB);
}


/**
 * Gets the alpha component value of the color.
 */
int Color_4ub::alpha() const
{
	return static_cast<int>(mA);
}


/**
 * Sets the red component value of the color.
 * 
 * \param red Red value. Valid values are 0 - 255.
 */
void Color_4ub::red(int red)
{
	mR = static_cast<unsigned char>(clamp(red, 0, 255));
}


/**
 * Sets the green component value of the color.
 * 
 * \param green Green value. Valid values are 0 - 255.
 */
void Color_4ub::green(int green)
{
	mG = static_cast<unsigned char>(clamp(green, 0, 255));
}


/**
 * Sets the blue component value of the color.
 * 
 * \param blue Blue value. Valid values are 0 - 255.
 */
void Color_4ub::blue(int blue)
{
	mB = static_cast<unsigned char>(clamp(blue, 0, 255));
}


/**
 * Sets the alpha component value of the color.
 * 
 * \param alpha Alpha value. Valid values are 0 - 255.
 */
void Color_4ub::alpha(int alpha)
{
	mA = static_cast<unsigned char>(clamp(alpha, 0, 255));
}


// ==================================================================================
// = Rectangle_2d Implementation
// ==================================================================================

/**
 * Default c'tor.
 */
Rectangle_2d::Rectangle_2d():	mX(0),
								mY(0),
								mW(0),
								mH(0)
{}


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
 * Adds two rectangles together.
 *
 * \param rect	Rectangle to add.
 */
Rectangle_2d& Rectangle_2d::operator+=(const Rectangle_2d& rect)
{
	mX += rect.x();
	mY += rect.y();
	mW += rect.width();
	mH += rect.height();

	return *this;
}


/**
 * Adds two rectangles together, \c float version.
 *
 * \param rect	Rectangle to add.
 */
Rectangle_2d& Rectangle_2d::operator+=(const Rectangle_2df& rect)
{
	mX += static_cast<int>(rect.x());
	mY += static_cast<int>(rect.y());
	mW += static_cast<int>(rect.width());
	mH += static_cast<int>(rect.height());

	return *this;
}


/**
 * Subtracts two rectangles.
 *
 * \param rect	Rectangle to subtract.
 */
Rectangle_2d& Rectangle_2d::operator-=(const Rectangle_2d& rect)
{
	mX -= rect.x();
	mY -= rect.y();
	mW -= rect.width();
	mH -= rect.height();

	return *this;
}


/**
 * Subtracts two rectangles, \c float version.
 *
 * \param rect	Rectangle to subtract.
 */
Rectangle_2d& Rectangle_2d::operator-=(const Rectangle_2df& rect)
{
	mX -= static_cast<int>(rect.x());
	mY -= static_cast<int>(rect.y());
	mW -= static_cast<int>(rect.width());
	mH -= static_cast<int>(rect.height());

	return *this;
}


/**
 * Multiplies two rectangles together.
 *
 * \param rect	Rectangle to multiply.
 */
Rectangle_2d& Rectangle_2d::operator*=(const Rectangle_2d& rect)
{
	mX *= rect.x();
	mY *= rect.y();
	mW *= rect.width();
	mH *= rect.height();

	return *this;
}


/**
 * Multiplies two rectangles together, \c float version.
 *
 * \param rect	Rectangle to multiply.
 */
Rectangle_2d& Rectangle_2d::operator*=(const Rectangle_2df& rect)
{
	mX *= static_cast<int>(rect.x());
	mY *= static_cast<int>(rect.y());
	mW *= static_cast<int>(rect.width());
	mH *= static_cast<int>(rect.height());

	return *this;
}


/**
 * \see operator+=
 */
const Rectangle_2d Rectangle_2d::operator+(const Rectangle_2d& rect)
{
	Rectangle_2d r = *this;
	r += rect;
	return r;
}


/**
 * \see operator+=
 */
const Rectangle_2d Rectangle_2d::operator+(const Rectangle_2df& rect)
{
	Rectangle_2d r = *this;
	r += rect;
	return r;
}


/**
 * \see operator-=
 */
const Rectangle_2d Rectangle_2d::operator-(const Rectangle_2d& rect)
{
	Rectangle_2d r = *this;
	r -= rect;
	return r;
}


/**
 * \see operator-=
 */
const Rectangle_2d Rectangle_2d::operator-(const Rectangle_2df& rect)
{
	Rectangle_2d r = *this;
	r -= rect;
	return r;
}


/**
 * \see operator*=
 */
const Rectangle_2d Rectangle_2d::operator*(const Rectangle_2d& rect)
{
	Rectangle_2d r = *this;
	r *= rect;
	return r;
}


/**
 * \see operator*=
 */
const Rectangle_2d Rectangle_2d::operator*(const Rectangle_2df& rect)
{
	Rectangle_2d r = *this;
	r *= rect;
	return r;
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
 * Default c'tor
 */
Rectangle_2df::Rectangle_2df():	mX(0),
								mY(0),
								mW(0),
								mH(0)
{}


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
 * Adds two rectangles together.
 *
 * \param rect	Rectangle to add.
 */
Rectangle_2df& Rectangle_2df::operator+=(const Rectangle_2d& rect)
{
	mX += rect.x();
	mY += rect.y();
	mW += rect.width();
	mH += rect.height();

	return *this;
}


/**
 * Adds two rectangles together, \c float version.
 *
 * \param rect	Rectangle to add.
 */
Rectangle_2df& Rectangle_2df::operator+=(const Rectangle_2df& rect)
{
	mX += rect.x();
	mY += rect.y();
	mW += rect.width();
	mH += rect.height();

	return *this;
}


/**
 * Subtracts two rectangles.
 *
 * \param rect	Rectangle to subtract.
 */
Rectangle_2df& Rectangle_2df::operator-=(const Rectangle_2d& rect)
{
	mX -= rect.x();
	mY -= rect.y();
	mW -= rect.width();
	mH -= rect.height();

	return *this;
}


/**
 * Subtracts two rectangles.
 *
 * \param rect	Rectangle to subtract.
 */
Rectangle_2df& Rectangle_2df::operator-=(const Rectangle_2df& rect)
{
	mX -= rect.x();
	mY -= rect.y();
	mW -= rect.width();
	mH -= rect.height();

	return *this;
}


/**
 * Multiplies two rectangles together.
 *
 * \param rect	Rectangle to multiply.
 */
Rectangle_2df& Rectangle_2df::operator*=(const Rectangle_2d& rect)
{
	mX *= rect.x();
	mY *= rect.y();
	mW *= rect.width();
	mH *= rect.height();

	return *this;
}


/**
 * Multiplies two rectangles together.
 *
 * \param rect	Rectangle to multiply.
 */
Rectangle_2df& Rectangle_2df::operator*=(const Rectangle_2df& rect)
{
	mX *= rect.x();
	mY *= rect.y();
	mW *= rect.width();
	mH *= rect.height();

	return *this;
}


/**
 * \see operator+=
 */
const Rectangle_2df Rectangle_2df::operator+(const Rectangle_2d& rect)
{
	Rectangle_2df r = *this;
	r += rect;
	return r;
}


/**
 * \see operator+=
 */
const Rectangle_2df Rectangle_2df::operator+(const Rectangle_2df& rect)
{
	Rectangle_2df r = *this;
	r += rect;
	return r;
}


/**
 * \see operator-=
 */
const Rectangle_2df Rectangle_2df::operator-(const Rectangle_2d& rect)
{
	Rectangle_2df r = *this;
	r -= rect;
	return r;
}


/**
 * \see operator-=
 */
const Rectangle_2df Rectangle_2df::operator-(const Rectangle_2df& rect)
{
	Rectangle_2df r = *this;
	r -= rect;
	return r;
}


/**
 * \see operator*=
 */
const Rectangle_2df Rectangle_2df::operator*(const Rectangle_2d& rect)
{
	Rectangle_2df r = *this;
	r *= rect;
	return r;
}


/**
 * \see operator*=
 */
const Rectangle_2df Rectangle_2df::operator*(const Rectangle_2df& rect)
{
	Rectangle_2df r = *this;
	r *= rect;
	return r;
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


// ==================================================================================
// = Point_2d Implementation
// ==================================================================================

/**
 * Default c'tor.
 */
Point_2d::Point_2d():	mX(0),
						mY(0)
{}


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

Point_2d::Point_2d(const Point_2df& _p):	mX(static_cast<int>(_p.x())),
											mY(static_cast<int>(_p.y()))
{}


void Point_2d::operator()(int _x, int _y)
{
	x(_x);
	y(_y);
}


bool Point_2d::operator==(const Point_2d& pt)
{
	return (x() == pt.x()) && (y() == pt.y());
}


bool Point_2d::operator==(const Point_2df& pt)
{
	return (static_cast<float>(x()) == pt.x()) && (static_cast<float>(y()) == pt.y());
}


bool Point_2d::operator!=(const Point_2d& pt)
{
	return (x() != pt.x()) || (y() != pt.y());
}


bool Point_2d::operator!=(const Point_2df& pt)
{
	return (static_cast<float>(x()) != pt.x()) || (static_cast<float>(y()) != pt.y());
}


Point_2d& Point_2d::operator+=(const Point_2d& pt)
{
	mX += pt.x();
	mY += pt.y();

	return *this;
}


Point_2d& Point_2d::operator+=(const Point_2df& pt)
{
	mX += static_cast<int>(pt.x());
	mY += static_cast<int>(pt.y());

	return *this;
}


Point_2d& Point_2d::operator-=(const Point_2d& pt)
{
	mX -= pt.x();
	mY -= pt.y();

	return *this;
}


Point_2d& Point_2d::operator-=(const Point_2df& pt)
{
	mX -= static_cast<int>(pt.x());
	mY -= static_cast<int>(pt.y());

	return *this;
}


Point_2d& Point_2d::operator*=(const Point_2d& pt)
{
	mX *= pt.x();
	mY *= pt.y();

	return *this;
}


Point_2d& Point_2d::operator*=(const Point_2df& pt)
{
	mX *= static_cast<int>(pt.x());
	mY *= static_cast<int>(pt.y());

	return *this;
}


const Point_2d Point_2d::operator+(const Point_2d& pt)
{
	Point_2d r = *this;
	r += pt;
	return r;
}


const Point_2d Point_2d::operator+(const Point_2df& pt)
{
	Point_2d r = *this;
	r += pt;
	return r;
}


const Point_2d Point_2d::operator-(const Point_2d& pt)
{
	Point_2d r = *this;
	r -= pt;
	return r;
}


const Point_2d Point_2d::operator-(const Point_2df& pt)
{
	Point_2d r = *this;
	r -= pt;
	return r;
}


const Point_2d Point_2d::operator*(const Point_2d& pt)
{
	Point_2d r = *this;
	r *= pt;
	return r;
}


const Point_2d Point_2d::operator*(const Point_2df& pt)
{
	Point_2d r = *this;
	r *= pt;
	return r;
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

Point_2df::Point_2df():	mX(0.0f),
						mY(0.0f)
{}


Point_2df::Point_2df(float x, float y):	mX(x),
										mY(y)
{}


Point_2df::Point_2df(const Point_2d& _p):	mX(static_cast<float>(_p.x())),
											mY(static_cast<float>(_p.y()))
{}


void Point_2df::operator()(float _x, float _y)
{
	x(_x);
	y(_y);
}


bool Point_2df::operator==(const Point_2d& pt)
{
	return (mX == pt.x()) && (mY == pt.y());
}


bool Point_2df::operator==(const Point_2df& pt)
{
	return (mX == pt.x()) && (mY == pt.y());
}


bool Point_2df::operator!=(const Point_2d& pt)
{
	return (x() != pt.x()) || (y() != pt.y());
}


bool Point_2df::operator!=(const Point_2df& pt)
{
	return (x() != pt.x()) || (y() != pt.y());
}


Point_2df& Point_2df::operator+=(const Point_2d& pt)
{
	mX += pt.x();
	mY += pt.y();

	return *this;
}


Point_2df& Point_2df::operator+=(const Point_2df& pt)
{
	mX += pt.x();
	mY += pt.y();

	return *this;
}


Point_2df& Point_2df::operator-=(const Point_2d& pt)
{
	mX -= pt.x();
	mY -= pt.y();

	return *this;
}


Point_2df& Point_2df::operator-=(const Point_2df& pt)
{
	mX -= pt.x();
	mY -= pt.y();

	return *this;
}


Point_2df& Point_2df::operator*=(const Point_2d& pt)
{
	mX *= pt.x();
	mY *= pt.y();

	return *this;
}


Point_2df& Point_2df::operator*=(const Point_2df& pt)
{
	mX *= pt.x();
	mY *= pt.y();

	return *this;
}


const Point_2df Point_2df::operator+(const Point_2d& pt)
{
	Point_2df r = *this;
	r += pt;
	return r;
}


const Point_2df Point_2df::operator+(const Point_2df& pt)
{
	Point_2df r = *this;
	r += pt;
	return r;
}


const Point_2df Point_2df::operator-(const Point_2d& pt)
{
	Point_2df r = *this;
	r -= pt;
	return r;
}


const Point_2df Point_2df::operator-(const Point_2df& pt)
{
	Point_2df r = *this;
	r -= pt;
	return r;
}


const Point_2df Point_2df::operator*(const Point_2d& pt)
{
	Point_2df r = *this;
	r *= pt;
	return r;
}


const Point_2df Point_2df::operator*(const Point_2df& pt)
{
	Point_2df r = *this;
	r *= pt;
	return r;
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
