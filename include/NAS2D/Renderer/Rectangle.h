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
struct Rectangle
{
	Rectangle() = default;
	Rectangle(const Rectangle& other) = default;
	Rectangle& operator=(const Rectangle& other) = default;

	Rectangle(BaseType x, BaseType y, BaseType width, BaseType height) :
		mX(x),
		mY(y),
		mW(width),
		mH(height)
	{}

	bool operator==(const Rectangle& rect) const {
		return (mX == rect.mX) && (mY == rect.mY) && (mW == rect.mW) && (mH == rect.mH);
	}
	bool operator!=(const Rectangle& rect) const {
		return !(*this == rect);
	}

	bool null() const {
		return (mW == 0) || (mH == 0);
	}

	template <typename NewBaseType>
	operator Rectangle<NewBaseType>() const {
		return {
			static_cast<NewBaseType>(mX),
			static_cast<NewBaseType>(mY),
			static_cast<NewBaseType>(mW),
			static_cast<NewBaseType>(mH)
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

	void width(BaseType w) {
		mW = w;
	}
	BaseType width() const {
		return mW;
	}
	BaseType& width() {
		return mW;
	}

	void height(BaseType h) {
		mH = h;
	}
	BaseType height() const {
		return mH;
	}
	BaseType& height() {
		return mH;
	}

	BaseType center_x() const {
		return mX + (mW / 2);
	}
	BaseType center_y() const {
		return mY + (mH / 2);
	}

private:
	BaseType mX = 0;
	BaseType mY = 0;
	BaseType mW = 0;
	BaseType mH = 0;
};


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
