// ==================================================================================
// = NAS2D
// = Copyright © 2008 - 2017 New Age Software
// ==================================================================================
// = NAS2D is distributed under the terms of the zlib license. You are free to copy,
// = modify and distribute the software under the terms of the zlib license.
// = 
// = Acknowledgement of your use of NAS2D is appriciated but is not required.
// ==================================================================================

#ifndef _NAS_GRAPHICS_PRIMITIVES_
#define _NAS_GRAPHICS_PRIMITIVES_

namespace NAS2D {

/**
 * \class	Color_4ub
 * \brief	RGBA Color.
 * 
 * RGBA values can only be between 0 and 255.
 */
class Color_4ub
{
public:
	Color_4ub();

	Color_4ub(int r, int g, int b, int a);

	void operator()(int r, int g, int b, int a);

	int red() const;
	int green() const;
	int blue() const;
	int alpha() const;

	void red(int red);
	void green(int green);
	void blue(int blue);
	void alpha(int alpha);

private:
	unsigned char mR, mG, mB, mA;
};


class Rectangle_2d;
class Rectangle_2df;


/**
 * \class	Rectangle_2d
 * \brief	2D rectangle.
 */
class Rectangle_2d
{
public:
	Rectangle_2d();
	Rectangle_2d(int x, int y, int w, int h);
	Rectangle_2d(const Rectangle_2df& rect);

	void operator()(int x, int y, int w, int h);

	bool operator==(const Rectangle_2d& rect);
	bool operator==(const Rectangle_2df& rect);

	bool operator!=(const Rectangle_2d& rect);
	bool operator!=(const Rectangle_2df& rect);

	Rectangle_2d& operator+=(const Rectangle_2d& rect);
	Rectangle_2d& operator+=(const Rectangle_2df& rect);

	Rectangle_2d& operator-=(const Rectangle_2d& rect);
	Rectangle_2d& operator-=(const Rectangle_2df& rect);

	Rectangle_2d& operator*=(const Rectangle_2d& rect);
	Rectangle_2d& operator*=(const Rectangle_2df& rect);

	const Rectangle_2d operator+(const Rectangle_2d& rect);
	const Rectangle_2d operator+(const Rectangle_2df& rect);

	const Rectangle_2d operator-(const Rectangle_2d& rect);
	const Rectangle_2d operator-(const Rectangle_2df& rect);

	const Rectangle_2d operator*(const Rectangle_2d& rect);
	const Rectangle_2d operator*(const Rectangle_2df& rect);

	bool null();

	void x(int x);
	int x() const;
	int& x();

	void y(int y);
	int y() const;
	int& y();

	void w(int w);
	int w() const;
	int& w();

	void h(int h);
	int h() const;
	int& h();

private:
	int mX, mY, mW, mH;
};


/**
 * \class	Rectangle_2df
 * \brief	Floating point 2D Rectangle.
 */
class Rectangle_2df
{
public:
	Rectangle_2df();
	Rectangle_2df(float x, float y, float w, float h);

	void operator()(float x, float y, float w, float h);

	bool operator==(const Rectangle_2d& rect);
	bool operator==(const Rectangle_2df& rect);

	bool operator!=(const Rectangle_2d& rect);
	bool operator!=(const Rectangle_2df& rect);

	Rectangle_2df& operator+=(const Rectangle_2d& rect);
	Rectangle_2df& operator+=(const Rectangle_2df& rect);

	Rectangle_2df& operator-=(const Rectangle_2d& rect);
	Rectangle_2df& operator-=(const Rectangle_2df& rect);

	Rectangle_2df& operator*=(const Rectangle_2d& rect);
	Rectangle_2df& operator*=(const Rectangle_2df& rect);

	const Rectangle_2df operator+(const Rectangle_2d& rect);
	const Rectangle_2df operator+(const Rectangle_2df& rect);

	const Rectangle_2df operator-(const Rectangle_2d& rect);
	const Rectangle_2df operator-(const Rectangle_2df& rect);

	const Rectangle_2df operator*(const Rectangle_2d& rect);
	const Rectangle_2df operator*(const Rectangle_2df& rect);

	bool null();

	void x(float x);
	float x() const;
	float& x();

	void y(float y);
	float y() const;
	float& y();

	void w(float w);
	float w() const;
	float& w();

	void h(float h);
	float h() const;
	float& h();

private:
	float mX, mY, mW, mH;
};


class Point_2d;
class Point_2df;


/**
 * \class	Point_2d
 * \brief	2D point.
 */
class Point_2d
{
public:
	Point_2d();
	Point_2d(int x, int y);
	Point_2d(const Point_2df& _p);

	void operator()(int x, int y);

	bool operator==(const Point_2d& pt);
	bool operator==(const Point_2df& pt);

	bool operator!=(const Point_2d& pt);
	bool operator!=(const Point_2df& pt);

	Point_2d& operator+=(const Point_2d& pt);
	Point_2d& operator+=(const Point_2df& pt);

	Point_2d& operator-=(const Point_2d& pt);
	Point_2d& operator-=(const Point_2df& pt);

	Point_2d& operator*=(const Point_2d& pt);
	Point_2d& operator*=(const Point_2df& pt);

	const Point_2d operator+(const Point_2d& pt);
	const Point_2d operator+(const Point_2df& pt);

	const Point_2d operator-(const Point_2d& pt);
	const Point_2d operator-(const Point_2df& pt);

	const Point_2d operator*(const Point_2d& pt);
	const Point_2d operator*(const Point_2df& pt);

	void x(int x);
	int x() const;
	int& x();

	void y(int y);
	int y() const;
	int& y();

private:
	int mX, mY;
};


/**
 * \class	Point_2df
 * \brief	Floating point 2D Point.
 */
class Point_2df
{
public:
	Point_2df();
	Point_2df(float x, float y);
	Point_2df(const Point_2d& _p);

	void operator()(float _x, float _y);

	bool operator==(const Point_2d& pt);
	bool operator==(const Point_2df& pt);

	bool operator!=(const Point_2d& pt);
	bool operator!=(const Point_2df& pt);

	Point_2df& operator+=(const Point_2d& pt);
	Point_2df& operator+=(const Point_2df& pt);

	Point_2df& operator-=(const Point_2d& pt);
	Point_2df& operator-=(const Point_2df& pt);

	Point_2df& operator*=(const Point_2d& pt);
	Point_2df& operator*=(const Point_2df& pt);

	const Point_2df operator+(const Point_2d& pt);
	const Point_2df operator+(const Point_2df& pt);

	const Point_2df operator-(const Point_2d& pt);
	const Point_2df operator-(const Point_2df& pt);

	const Point_2df operator*(const Point_2d& pt);
	const Point_2df operator*(const Point_2df& pt);

	void x(float x);
	float x() const;
	float& x();

	void y(float y);
	float y() const;
	float& y();

private:
	float mX, mY;
};

} // namespace

#endif