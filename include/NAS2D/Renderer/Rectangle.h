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

#include "Point.h"
#include "Vector.h"


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

	// Factory method
	static Rectangle<BaseType> Create(Point<BaseType> startPoint, Vector<BaseType> size) {
		return {
			startPoint.x(),
			startPoint.y(),
			size.x,
			size.y
		};
	}

	// Factory method
	static Rectangle<BaseType> Create(Point<BaseType> startPoint, Point<BaseType> endPoint) {
		return {
			startPoint.x(),
			startPoint.y(),
			endPoint.x() - startPoint.x(),
			endPoint.y() - startPoint.y()
		};
	}

	bool operator==(const Rectangle& rect) const {
		return (mX == rect.mX) && (mY == rect.mY) && (mW == rect.mW) && (mH == rect.mH);
	}
	bool operator!=(const Rectangle& rect) const {
		return !(*this == rect);
	}

	Vector<BaseType> size() const {
		return {mW, mH};
	}

	Point<BaseType> startPoint() const {
		return {mX, mY};
	}

	Point<BaseType> endPoint() const {
		return Point<BaseType>{mX, mY} + Vector<BaseType>{mW, mH};
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

	template <typename NewBaseType>
	Rectangle<NewBaseType> to() const {
		return static_cast<Rectangle<NewBaseType>>(*this);
	}

	// Start point inclusive (x, y), endpoint exclusive (x + width, y + height)
	// Area in interval notation: [x .. x + width), [y .. y + height)
	bool contains(const Point<BaseType>& point) const {
		auto px = point.x();
		auto py = point.y();
		return ((mX <= px) && (px < mX + mW)) && ((mY <= py) && (py < mY + mH));
	}

	// Start point inclusive (x, y), endpoint exclusive (x + width, y + height)
	// Area in interval notation: [x .. x + width), [y .. y + height)
	bool overlaps(const Rectangle& rect) const {
		return ((mX < rect.mX + rect.mW) && (rect.mX < mX + mW)) && ((mY < rect.mY + rect.mH) && (rect.mY < mY + mH));
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

	Point<BaseType> center() const {
		return {mX + (mW / 2), mY + (mH / 2)};
	}

private:
	BaseType mX = 0;
	BaseType mY = 0;
	BaseType mW = 0;
	BaseType mH = 0;
};


/**
 * \class	Rectangle_2d
 * \brief	2D rectangle.
 */
using Rectangle_2d = Rectangle<int>;
extern template struct Rectangle<int>;


/**
 * \class	Rectangle_2df
 * \brief	Floating point 2D Rectangle.
 */
using Rectangle_2df = Rectangle<float>;
extern template struct Rectangle<float>;

} // namespace
