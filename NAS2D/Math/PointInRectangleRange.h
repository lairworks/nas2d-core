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


#include "VectorSizeRange.h"
#include "Rectangle.h"
#include "Vector.h"
#include "Point.h"


namespace NAS2D
{
	template <typename BaseType>
	class PointInRectangleRange
	{
	public:
		class Iterator
		{
		public:
			Iterator(const Rectangle<BaseType>& rect, Vector<BaseType> initial = Vector<BaseType>{0, 0}) :
				mIterator(rect.size, initial),
				mStartPoint(rect.position)
			{}
			Iterator(const Iterator& other) = default;
			Iterator& operator=(const Iterator& other) = default;

			Iterator& operator++()
			{
				++mIterator;
				return *this;
			}

			Iterator& operator--()
			{
				--mIterator;
				return *this;
			}

			bool operator==(const Iterator& other) const
			{
				return **this == *other;
			}

			bool operator!=(const Iterator& other) const
			{
				return !(*this == other);
			}

			Point<BaseType> operator*() const
			{
				return mStartPoint + *mIterator;
			}

		private:
			typename VectorSizeRange<BaseType>::Iterator mIterator;
			const Point<BaseType> mStartPoint;
		};


		PointInRectangleRange(Rectangle<BaseType> rect) :
			mRect(rect)
		{}

		Iterator begin() const
		{
			return Iterator{mRect};
		}

		Iterator end() const
		{
			return Iterator{mRect, Vector<BaseType>{0, mRect.size.y}};
		}

	private:
		const Rectangle<BaseType> mRect;
	};


	template <typename BaseType>
	PointInRectangleRange(Rectangle<BaseType>) -> PointInRectangleRange<BaseType>;

}
