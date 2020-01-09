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


struct Rectangle_2d;
struct Rectangle_2df;


/**
 * \class	Rectangle_2d
 * \brief	2D rectangle.
 */
struct Rectangle_2d
{
public:
	Rectangle_2d() = default;
	Rectangle_2d(int x, int y, int w, int h);

public:
	void operator()(int x, int y, int w, int h);

	bool operator==(const Rectangle_2d& rect) const;
	bool operator!=(const Rectangle_2d& rect) const;

	operator Rectangle_2df() const;

public:
	bool null();

	void x(int x);
	int x() const;
	int& x();

	void y(int y);
	int y() const;
	int& y();

	void width(int w);
	int width() const;
	int& width();

	void height(int h);
	int height() const;
	int& height();

	int center_x() const;
	int center_y() const;

private:
	int mX = 0, mY = 0, mW = 0, mH = 0;
};


/**
 * \class	Rectangle_2df
 * \brief	Floating point 2D Rectangle.
 */
struct Rectangle_2df
{
public:
	Rectangle_2df() = default;
	Rectangle_2df(float x, float y, float w, float h);

public:
	void operator()(float x, float y, float w, float h);

	bool operator==(const Rectangle_2df& rect) const;
	bool operator!=(const Rectangle_2df& rect) const;

	operator Rectangle_2d() const;

public:
	bool null();

	void x(float x);
	float x() const;
	float& x();

	void y(float y);
	float y() const;
	float& y();

	void width(float w);
	float width() const;
	float& width();

	void height(float h);
	float height() const;
	float& height();

	float center_x() const;
	float center_y() const;

private:
	float mX = 0.0f, mY = 0.0f, mW = 0.0f, mH = 0.0f;
};

} // namespace
