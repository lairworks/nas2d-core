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

class Point_2d;
class Point_2df;


/**
 * \class	Point_2d
 * \brief	2D point.
 */
class Point_2d
{
public:
	Point_2d() = default;
	Point_2d(int x, int y);
	Point_2d(const Point_2df& _p);

public:
	void operator()(int x, int y);

	bool operator==(const Point_2d& pt);
	bool operator==(const Point_2df& pt);

	bool operator!=(const Point_2d& pt);
	bool operator!=(const Point_2df& pt);

	Point_2d& operator+=(const Point_2d& pt);
	Point_2d& operator+=(const Point_2df& pt);

	Point_2d& operator-=(const Point_2d& pt);
	Point_2d& operator-=(const Point_2df& pt);

	const Point_2d operator+(const Point_2d& pt);
	const Point_2d operator+(const Point_2df& pt);

	const Point_2d operator-(const Point_2d& pt);
	const Point_2d operator-(const Point_2df& pt);

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
class Point_2df
{
public:
	Point_2df() = default;
	Point_2df(float x, float y);
	Point_2df(const Point_2d& _p);

public:
	void operator()(float _x, float _y);

	bool operator==(const Point_2d& pt);
	bool operator==(const Point_2df& pt);

	bool operator!=(const Point_2d& pt);
	bool operator!=(const Point_2df& pt);

	Point_2df& operator+=(const Point_2d& pt);
	Point_2df& operator+=(const Point_2df& pt);

	Point_2df& operator-=(const Point_2d& pt);
	Point_2df& operator-=(const Point_2df& pt);

	const Point_2df operator+(const Point_2d& pt);
	const Point_2df operator+(const Point_2df& pt);

	const Point_2df operator-(const Point_2d& pt);
	const Point_2df operator-(const Point_2df& pt);

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
