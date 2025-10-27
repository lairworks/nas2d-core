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
		Vector<BaseType> size;


		constexpr static Rectangle<BaseType> Create(Point<BaseType> startPoint, Point<BaseType> endPoint)
		{
			return {startPoint, endPoint - startPoint};
		}


		constexpr bool operator==(const Rectangle& rect) const
		{
			return (position == rect.position) && (size == rect.size);
		}

		constexpr bool operator!=(const Rectangle& rect) const
		{
			return !(*this == rect);
		}

		constexpr Point<BaseType> startPoint() const
		{
			return position;
		}

		constexpr Point<BaseType> endPoint() const
		{
			return position + size;
		}

		constexpr Point<BaseType> crossXPoint() const
		{
			return {position.x + size.x, position.y};
		}

		constexpr Point<BaseType> crossYPoint() const
		{
			return {position.x, position.y + size.y};
		}

		constexpr bool null() const
		{
			return size == Vector<BaseType>{0, 0};
		}

		constexpr bool empty() const
		{
			return (size.x <= 0) || (size.y <= 0);
		}

		void startPoint(NAS2D::Point<BaseType> newStartPoint)
		{
			position = newStartPoint;
		}

		constexpr Rectangle translate(Vector<BaseType> offset) const
		{
			return {position + offset, size};
		}

		constexpr Rectangle inset(BaseType amount) const
		{
			return {{position.x + amount, position.y + amount}, {size.x - 2 * amount, size.y - 2 * amount}};
		}

		constexpr Rectangle inset(Vector<BaseType> amount) const
		{
			return {position + amount, size - (amount * 2)};
		}

		constexpr Rectangle inset(Vector<BaseType> amountStart, Vector<BaseType> amountEnd) const
		{
			return {position + amountStart, size - amountStart - amountEnd};
		}

		constexpr Rectangle skewBy(const Vector<BaseType>& scaleFactor) const
		{
			return {position.skewBy(scaleFactor), size.skewBy(scaleFactor)};
		}

		constexpr Rectangle skewInverseBy(const Vector<BaseType>& scaleFactor) const
		{
			return {position.skewInverseBy(scaleFactor), size.skewInverseBy(scaleFactor)};
		}

		template <typename NewBaseType>
		constexpr operator Rectangle<NewBaseType>() const
		{
			return {
				Point<NewBaseType>(position),
				Vector<NewBaseType>(size),
			};
		}

		template <typename NewBaseType>
		constexpr Rectangle<NewBaseType> to() const
		{
			return Rectangle<NewBaseType>(*this);
		}

		// Start point inclusive (x, y), endpoint exclusive (x + width, y + height)
		// Area in interval notation: [x .. x + width), [y .. y + height)
		constexpr bool contains(const Point<BaseType>& point) const
		{
			return position <= point && point < endPoint();
		}

		constexpr bool contains(const Rectangle<BaseType>& rect) const
		{
			return position <= rect.position && rect.endPoint() <= endPoint();
		}

		// Start point inclusive (x, y), endpoint exclusive (x + width, y + height)
		// Area in interval notation: [x .. x + width), [y .. y + height)
		constexpr bool overlaps(const Rectangle& rect) const
		{
			return position < rect.endPoint() && rect.position < endPoint();
		}

		constexpr Point<BaseType> center() const
		{
			return position + size / 2;
		}
	};


	template <typename BaseType>
	Rectangle(BaseType, BaseType, BaseType, BaseType) -> Rectangle<BaseType>;

	template <typename BaseType>
	Rectangle(Point<BaseType>, Vector<BaseType>) -> Rectangle<BaseType>;


	extern template struct Rectangle<int>;
	extern template struct Rectangle<float>;
}
