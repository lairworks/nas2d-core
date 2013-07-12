// ==================================================================================
// = NAS2D
// = Copyright © 2008 - 2012 New Age Software
// ==================================================================================
// = NAS2D is distributed under the terms of the zlib license. You are free to copy,
// = modify and distribute the software under the terms of the zlib license.
// = 
// = Acknowledgement of your use of NAS2D is appriciated but is not required.
// ==================================================================================

#ifndef _NAS_GRAPHICS_PRIMITIVES_
#define _NAS_GRAPHICS_PRIMITIVES_


/**
 * \class	Color_4ub
 * \brief	RGBA Color.
 * 
 * RGBA values can only be between 0 and 255.
 */
class Color_4ub
{
public:
	Color_4ub():	mR(static_cast<unsigned char>(255)),
					mG(static_cast<unsigned char>(255)),
					mB(static_cast<unsigned char>(255)),
					mA(static_cast<unsigned char>(255))
	{}

	Color_4ub(int r, int g, int b, int a):	mR(static_cast<unsigned char>(r)),
											mG(static_cast<unsigned char>(g)),
											mB(static_cast<unsigned char>(b)),
											mA(static_cast<unsigned char>(a))
	{}

	void operator()(int r, int g, int b, int a) { red(r); green(g); blue(b); alpha(a); }

	int red() const { return static_cast<int>(mR); }
	int green() const { return static_cast<int>(mG); }
	int blue() const { return static_cast<int>(mB); }
	int alpha() const { return static_cast<int>(mA); }

	void red(int red) { mR = static_cast<unsigned char>(red); }
	void green(int green) { mG = static_cast<unsigned char>(green); }
	void blue(int blue) { mB = static_cast<unsigned char>(blue); }
	void alpha(int alpha) { mA = static_cast<unsigned char>(alpha); }

private:
	unsigned char mR, mG, mB, mA;
};


/**
 * \struct	Rectangle_2d
 * \brief	2D rectangle.
 */
class Rectangle_2d
{
public:
	Rectangle_2d(): x(0), y(0), w(0), h(0) {}
	Rectangle_2d(int x, int y, int w, int h): x(x), y(y), w(w), h(h) {}

	bool operator==(const Rectangle_2d& rect) { return (x == rect.x) && (y == rect.y) && (w == rect.w) && (h == rect.h); }

	void operator()(int _x, int _y, int _w, int _h) { x = _x; y = _y; w = _w; h = _h; }

	bool null() { return w * h == 0; }

	int x, y, w, h;
};


/**
 * \struct	Rectangle_2df
 * \brief	Floating point 2D Rectangle.
 */
class Rectangle_2df
{
public:
	Rectangle_2df(): x(0), y(0), w(0), h(0) {}
	Rectangle_2df(float x, float y, float w, float h): x(x), y(y), w(w), h(h) {}

	bool operator==(const Rectangle_2df& rect) { return (x == rect.x) && (y == rect.y) && (w == rect.w) && (h == rect.h); }

	void operator()(float _x, float _y, float _w, float _h) { x = _x; y = _y; w = _w; h = _h; }

	bool null() { return w * h == 0; }

	float x, y, w, h;
};


/**
 * \struct	Point_2d
 * \brief	2D point.
 */
class Point_2d
{
public:
	Point_2d(): x(0), y(0) {}
	Point_2d(int x, int y): x(x), y(y) {}

	bool operator==(const Point_2d& pt) { return (x == pt.x) && (y == pt.y); }
	bool operator!=(const Point_2d& pt) { return (x != pt.x) || (y != pt.y); }
	void operator()(int _x, int _y) { x = _x; y = _y; }

	Point_2d operator+(const Point_2d& pt) { return Point_2d(x + pt.x, y + pt.y); }
	Point_2d operator-(const Point_2d& pt) { return Point_2d(x - pt.x, y - pt.y); }

	int x, y;
};


/**
 * \struct	Point_2df
 * \brief	Floating point 2D Point.
 */
class Point_2df
{
public:
	Point_2df(): x(0.0f), y(0.0f) {}
	Point_2df(float x, float y): x(x), y(y) {}

	bool operator==(const Point_2df& pt) { return (x == pt.x) && (y == pt.y); }
	bool operator!=(const Point_2df& pt) { return (x != pt.x) || (y != pt.y); }
	void operator()(float _x, float _y) { x = _x; y = _y; }

	Point_2df operator+(const Point_2df& pt) { return Point_2df(x + pt.x, y + pt.y); }
	Point_2df operator-(const Point_2df& pt) { return Point_2df(x - pt.x, y - pt.y); }

	float x, y;
};


#endif