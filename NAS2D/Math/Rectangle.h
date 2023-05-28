// ==================================================================================
// = NAS2D
// = Copyright © 2008 - 2020 New Age Software
// ==================================================================================
// = NAS2D is distributed under the terms of the zlib license. You are free to copy,
// = modify and distribute the software under the terms of the zlib license.
// =
// = Acknowledgment of your use of NAS2D is appreciated but is not required.
// ==================================================================================

#pragma once

#include "Point.h"
#include "Vector.h"


namespace NAS2D
{
	template <typename BaseType>
	struct Rectangle
	{
		Point<BaseType> position;
		Vector<BaseType> rectSize;

		constexpr static Rectangle<BaseType> Create(Point<BaseType> startPoint, Vector<BaseType> size)
		{
			return {startPoint, size};
		}

		constexpr static Rectangle<BaseType> Create(Point<BaseType> startPoint, Point<BaseType> endPoint)
		{
			return Create(startPoint, endPoint - startPoint);
		}

		constexpr bool operator==(const Rectangle& rect) const
		{
			return (position == rect.position) && (rectSize == rect.rectSize);
		}

		constexpr bool operator!=(const Rectangle& rect) const
		{
			return !(*this == rect);
		}

		constexpr Vector<BaseType> size() const
		{
			return rectSize;
		}

		constexpr Point<BaseType> startPoint() const
		{
			return position;
		}

		constexpr Point<BaseType> endPoint() const
		{
			return position + rectSize;
		}

		constexpr Point<BaseType> crossXPoint() const
		{
			return {position.x + rectSize.x, position.y};
		}

		constexpr Point<BaseType> crossYPoint() const
		{
			return {position.x, position.y + rectSize.y};
		}

		constexpr bool null() const
		{
			return (rectSize.x == 0) || (rectSize.y == 0);
		}

		constexpr bool empty() const
		{
			return (rectSize.x <= 0) || (rectSize.y <= 0);
		}

		void size(NAS2D::Vector<BaseType> newSize)
		{
			rectSize = newSize;
		}

		void startPoint(NAS2D::Point<BaseType> newStartPoint)
		{
			position = newStartPoint;
		}

		constexpr Rectangle translate(Vector<BaseType> offset) const
		{
			return Create(startPoint() + offset, size());
		}

		constexpr Rectangle inset(BaseType amount) const
		{
			return {{position.x + amount, position.y + amount}, {rectSize.x - 2 * amount, rectSize.y - 2 * amount}};
		}

		constexpr Rectangle inset(Vector<BaseType> amount) const
		{
			return {position + amount, rectSize - (amount * 2)};
		}

		constexpr Rectangle inset(Vector<BaseType> amountStart, Vector<BaseType> amountEnd) const
		{
			return {position + amountStart, rectSize - amountStart - amountEnd};
		}

		constexpr Rectangle skewBy(const Vector<BaseType>& scaleFactor) const
		{
			return Create(startPoint().skewBy(scaleFactor), size().skewBy(scaleFactor));
		}

		constexpr Rectangle skewInverseBy(const Vector<BaseType>& scaleFactor) const
		{
			return Create(startPoint().skewInverseBy(scaleFactor), size().skewInverseBy(scaleFactor));
		}

		template <typename NewBaseType>
		constexpr operator Rectangle<NewBaseType>() const
		{
			return {
				static_cast<Point<NewBaseType>>(position),
				static_cast<Vector<NewBaseType>>(rectSize),
			};
		}

		template <typename NewBaseType>
		constexpr Rectangle<NewBaseType> to() const
		{
			return static_cast<Rectangle<NewBaseType>>(*this);
		}

		// Start point inclusive (x, y), endpoint exclusive (x + width, y + height)
		// Area in interval notation: [x .. x + width), [y .. y + height)
		constexpr bool contains(const Point<BaseType>& point) const
		{
			return startPoint() <= point && point < endPoint();
		}

		constexpr bool contains(const Rectangle<BaseType>& rect) const
		{
			return startPoint() <= rect.startPoint() && rect.endPoint() <= endPoint();
		}

		// Start point inclusive (x, y), endpoint exclusive (x + width, y + height)
		// Area in interval notation: [x .. x + width), [y .. y + height)
		constexpr bool overlaps(const Rectangle& rect) const
		{
			return startPoint() < rect.endPoint() && rect.startPoint() < endPoint();
		}

		constexpr Point<BaseType> center() const
		{
			return {position.x + (rectSize.x / 2), position.y + (rectSize.y / 2)};
		}
	};


	template <typename BaseType>
	Rectangle(BaseType, BaseType, BaseType, BaseType) -> Rectangle<BaseType>;

	template <typename BaseType>
	Rectangle(Point<BaseType>, Vector<BaseType>) -> Rectangle<BaseType>;
} // namespace NAS2D
