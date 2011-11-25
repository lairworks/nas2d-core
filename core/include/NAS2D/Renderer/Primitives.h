// ==================================================================================
// = NAS2D
// = Copyright © 2008 - 2011 New Age Software
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
 * \brief	Represents a color in standard RGBA format.
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
 * \brief	Basic 2D rectangle.
 */
class Rectangle_2d
{
public:
	Rectangle_2d(): x(0), y(0), w(0), h(0) {}
	Rectangle_2d(int x, int y, int w, int h): x(x), y(y), w(w), h(h) {}

	int x, y, w, h;
};


/**
 * \struct	Point_2d
 * \brief	Basic 2D point.
 */
class Point_2d
{
public:
	Point_2d(): x(0), y(0) {}
	Point_2d(int x, int y): x(x), y(y) {}

	int x, y;
};


#endif