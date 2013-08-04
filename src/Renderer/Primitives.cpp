// ==================================================================================
// = NAS2D
// = Copyright © 2008 - 2013 New Age Software
// ==================================================================================
// = NAS2D is distributed under the terms of the zlib license. You are free to copy,
// = modify and distribute the software under the terms of the zlib license.
// = 
// = Acknowledgement of your use of NAS2D is appriciated but is not required.
// ==================================================================================

#include "NAS2D/Renderer/Primitives.h"
#include "NAS2D/Common.h"

// ==================================================================================
// = Color_4ub Implementation
// ==================================================================================

Color_4ub::Color_4ub():	mR(static_cast<unsigned char>(255)),
						mG(static_cast<unsigned char>(255)),
						mB(static_cast<unsigned char>(255)),
						mA(static_cast<unsigned char>(255))
{}

Color_4ub::Color_4ub(int r, int g, int b, int a):	mR(static_cast<unsigned char>(clamp(r, 0, 255))),
													mG(static_cast<unsigned char>(clamp(g, 0, 255))),
													mB(static_cast<unsigned char>(clamp(b, 0, 255))),
													mA(static_cast<unsigned char>(clamp(a, 0, 255)))
{}

void Color_4ub::operator()(int r, int g, int b, int a)
{
	red(clamp(r, 0, 255));
	green(clamp(g, 0, 255));
	blue(clamp(b, 0, 255));
	alpha(clamp(a, 0, 255));
}


int Color_4ub::red() const
{
	return static_cast<int>(mR);
}


int Color_4ub::green() const
{
	return static_cast<int>(mG);
}


int Color_4ub::blue() const
{
	return static_cast<int>(mB);
}


int Color_4ub::alpha() const
{
	return static_cast<int>(mA);
}


void Color_4ub::red(int red)
{
	mR = static_cast<unsigned char>(clamp(red, 0, 255));
}


void Color_4ub::green(int green)
{
	mG = static_cast<unsigned char>(clamp(green, 0, 255));
}


void Color_4ub::blue(int blue)
{
	mB = static_cast<unsigned char>(clamp(blue, 0, 255));
}


void Color_4ub::alpha(int alpha)
{
	mA = static_cast<unsigned char>(clamp(alpha, 0, 255));
}


// ==================================================================================
// = Rectangle_2d Implementation
// ==================================================================================

Rectangle_2d::Rectangle_2d():	mX(0),
								mY(0),
								mW(0),
								mH(0)
{}


Rectangle_2d::Rectangle_2d(int x, int y, int w, int h):	mX(x),
														mY(y),
														mW(w),
														mH(h)
{}


Rectangle_2d::Rectangle_2d(const Rectangle_2df& rect):	mX(static_cast<int>(rect.x())),
														mY(static_cast<int>(rect.y())),
														mW(static_cast<int>(rect.w())),
														mH(static_cast<int>(rect.h()))
{}


void Rectangle_2d::operator()(int _x, int _y, int _w, int _h)
{
	x(_x);
	y(_y);
	w(_w);
	h(_h);
}


bool Rectangle_2d::operator==(const Rectangle_2d& rect)
{
	return (x() == rect.x()) && (y() == rect.y()) && (w() == rect.w()) && (h() == rect.h());
}


bool Rectangle_2d::operator==(const Rectangle_2df& rect)
{
	return (x() == rect.x()) && (y() == rect.y()) && (w() == rect.w()) && (h() == rect.h());
}


bool Rectangle_2d::operator!=(const Rectangle_2d& rect)
{
	return (x() != rect.x()) || (y() != rect.y()) || (w() != rect.w()) || (h() != rect.h());
}


bool Rectangle_2d::operator!=(const Rectangle_2df& rect)
{
	return (x() != rect.x()) || (y() != rect.y()) || (w() != rect.w()) || (h() != rect.h());
}

Rectangle_2d& Rectangle_2d::operator+=(const Rectangle_2d& rect)
{
	mX += rect.x();
	mY += rect.y();
	mW += rect.w();
	mH += rect.h();

	return *this;
}


Rectangle_2d& Rectangle_2d::operator+=(const Rectangle_2df& rect)
{
	mX += static_cast<int>(rect.x());
	mY += static_cast<int>(rect.y());
	mW += static_cast<int>(rect.w());
	mH += static_cast<int>(rect.h());

	return *this;
}


Rectangle_2d& Rectangle_2d::operator-=(const Rectangle_2d& rect)
{
	mX -= rect.x();
	mY -= rect.y();
	mW -= rect.w();
	mH -= rect.h();

	return *this;
}


Rectangle_2d& Rectangle_2d::operator-=(const Rectangle_2df& rect)
{
	mX -= static_cast<int>(rect.x());
	mY -= static_cast<int>(rect.y());
	mW -= static_cast<int>(rect.w());
	mH -= static_cast<int>(rect.h());

	return *this;
}


Rectangle_2d& Rectangle_2d::operator*=(const Rectangle_2d& rect)
{
	mX *= rect.x();
	mY *= rect.y();
	mW *= rect.w();
	mH *= rect.h();

	return *this;
}


Rectangle_2d& Rectangle_2d::operator*=(const Rectangle_2df& rect)
{
	mX *= static_cast<int>(rect.x());
	mY *= static_cast<int>(rect.y());
	mW *= static_cast<int>(rect.w());
	mH *= static_cast<int>(rect.h());

	return *this;
}


const Rectangle_2d Rectangle_2d::operator+(const Rectangle_2d& rect)
{
	Rectangle_2d r = *this;
	r += rect;
	return r;
}


const Rectangle_2d Rectangle_2d::operator+(const Rectangle_2df& rect)
{
	Rectangle_2d r = *this;
	r += rect;
	return r;
}


const Rectangle_2d Rectangle_2d::operator-(const Rectangle_2d& rect)
{
	Rectangle_2d r = *this;
	r -= rect;
	return r;
}


const Rectangle_2d Rectangle_2d::operator-(const Rectangle_2df& rect)
{
	Rectangle_2d r = *this;
	r -= rect;
	return r;
}


const Rectangle_2d Rectangle_2d::operator*(const Rectangle_2d& rect)
{
	Rectangle_2d r = *this;
	r *= rect;
	return r;
}


const Rectangle_2d Rectangle_2d::operator*(const Rectangle_2df& rect)
{
	Rectangle_2d r = *this;
	r *= rect;
	return r;
}


bool Rectangle_2d::null()
{
	return w() * h() == 0;
}

void Rectangle_2d::x(int x)
{
	mX = x;
}


int Rectangle_2d::x() const
{
	return mX;
}


void Rectangle_2d::y(int y)
{
	mY = y;
}


int Rectangle_2d::y() const
{
	return mY;
}


void Rectangle_2d::w(int w)
{
	mW = w;
}


int Rectangle_2d::w() const
{
	return mW;
}


void Rectangle_2d::h(int h)
{
	mH = h;
}


int Rectangle_2d::h() const
{
	return mH;
}


// ==================================================================================
// = Rectangle_2df Implementation
// ==================================================================================


Rectangle_2df::Rectangle_2df():	mX(0),
								mY(0),
								mW(0),
								mH(0)
{}


Rectangle_2df::Rectangle_2df(float x, float y, float w, float h):	mX(x),
																	mY(y),
																	mW(w),
																	mH(h)
{}


void Rectangle_2df::operator()(float _x, float _y, float _w, float _h)
{
	x(_x);
	y(_y);
	w(_w);
	h(_h);
}


bool Rectangle_2df::operator==(const Rectangle_2d& rect)
{
	return (x() == rect.x()) && (y() == rect.y()) && (w() == rect.w()) && (h() == rect.h());
}


bool Rectangle_2df::operator==(const Rectangle_2df& rect)
{
	return (x() == rect.x()) && (y() == rect.y()) && (w() == rect.w()) && (h() == rect.h());
}


bool Rectangle_2df::operator!=(const Rectangle_2d& rect)
{
	return (x() != rect.x()) || (y() != rect.y()) || (w() != rect.w()) || (h() != rect.h());
}


bool Rectangle_2df::operator!=(const Rectangle_2df& rect)
{
	return (x() != rect.x()) || (y() != rect.y()) || (w() != rect.w()) || (h() != rect.h());
}

Rectangle_2df& Rectangle_2df::operator+=(const Rectangle_2d& rect)
{
	mX += rect.x();
	mY += rect.y();
	mW += rect.w();
	mH += rect.h();

	return *this;
}


Rectangle_2df& Rectangle_2df::operator+=(const Rectangle_2df& rect)
{
	mX += rect.x();
	mY += rect.y();
	mW += rect.w();
	mH += rect.h();

	return *this;
}


Rectangle_2df& Rectangle_2df::operator-=(const Rectangle_2d& rect)
{
	mX -= rect.x();
	mY -= rect.y();
	mW -= rect.w();
	mH -= rect.h();

	return *this;
}


Rectangle_2df& Rectangle_2df::operator-=(const Rectangle_2df& rect)
{
	mX -= rect.x();
	mY -= rect.y();
	mW -= rect.w();
	mH -= rect.h();

	return *this;
}


Rectangle_2df& Rectangle_2df::operator*=(const Rectangle_2d& rect)
{
	mX *= rect.x();
	mY *= rect.y();
	mW *= rect.w();
	mH *= rect.h();

	return *this;
}


Rectangle_2df& Rectangle_2df::operator*=(const Rectangle_2df& rect)
{
	mX *= rect.x();
	mY *= rect.y();
	mW *= rect.w();
	mH *= rect.h();

	return *this;
}


const Rectangle_2df Rectangle_2df::operator+(const Rectangle_2d& rect)
{
	Rectangle_2df r = *this;
	r += rect;
	return r;
}


const Rectangle_2df Rectangle_2df::operator+(const Rectangle_2df& rect)
{
	Rectangle_2df r = *this;
	r += rect;
	return r;
}


const Rectangle_2df Rectangle_2df::operator-(const Rectangle_2d& rect)
{
	Rectangle_2df r = *this;
	r -= rect;
	return r;
}


const Rectangle_2df Rectangle_2df::operator-(const Rectangle_2df& rect)
{
	Rectangle_2df r = *this;
	r -= rect;
	return r;
}


const Rectangle_2df Rectangle_2df::operator*(const Rectangle_2d& rect)
{
	Rectangle_2df r = *this;
	r *= rect;
	return r;
}


const Rectangle_2df Rectangle_2df::operator*(const Rectangle_2df& rect)
{
	Rectangle_2df r = *this;
	r *= rect;
	return r;
}


bool Rectangle_2df::null()
{
	return w() * h() == 0;
}

void Rectangle_2df::x(float x)
{
	mX = x;
}


float Rectangle_2df::x() const
{
	return mX;
}


void Rectangle_2df::y(float y)
{
	mY = y;
}


float Rectangle_2df::y() const
{
	return mY;
}


void Rectangle_2df::w(float w)
{
	mW = w;
}


float Rectangle_2df::w() const
{
	return mW;
}


void Rectangle_2df::h(float h)
{
	mH = h;
}


float Rectangle_2df::h() const
{
	return mH;
}


// ==================================================================================
// = Point_2d Implementation
// ==================================================================================

Point_2d::Point_2d():	mX(0),
						mY(0)
{}


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


void Point_2d::y(int y)
{
	mY = y;
}


int Point_2d::y() const
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


void Point_2df::y(float y)
{
	mY = y;
}


float Point_2df::y() const
{
	return mY;
}
