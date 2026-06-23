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


#include "Vector.h"


namespace NAS2D
{
	template <typename BaseType>
	class VectorSizeRange
	{
	public:
		class Iterator
		{
		public:
			constexpr explicit Iterator(Vector<BaseType> size, Vector<BaseType> initial = Vector<BaseType>{0, 0}) :
				mCurrent(initial),
				mSize(size)
			{}
			constexpr Iterator(const Iterator& other) = default;
			constexpr Iterator& operator=(const Iterator& other) = default;

			constexpr Iterator& operator++()
			{
				++mCurrent.x;
				if (mCurrent.x >= mSize.x)
				{
					mCurrent.x = 0;
					++mCurrent.y;
				}
				return *this;
			}

			constexpr Iterator& operator--()
			{
				if (mCurrent.x <= 0)
				{
					mCurrent.x = mSize.x;
					--mCurrent.y;
				}
				--mCurrent.x;
				return *this;
			}

			constexpr bool operator==(const Iterator& other) const
			{
				return mCurrent == other.mCurrent;
			}

			constexpr bool operator!=(const Iterator& other) const
			{
				return !(*this == other);
			}

			constexpr Vector<BaseType> operator*() const
			{
				return mCurrent;
			}

		private:
			Vector<BaseType> mCurrent;
			Vector<BaseType> mSize;
		};


		constexpr explicit VectorSizeRange(Vector<BaseType> size) :
			mSize(size)
		{}

		constexpr Iterator begin() const
		{
			return Iterator{mSize};
		}

		constexpr Iterator end() const
		{
			return Iterator{mSize, Vector<BaseType>{0, mSize.y}};
		}

	private:
		const Vector<BaseType> mSize;
	};


	template <typename BaseType>
	VectorSizeRange(Vector<BaseType>) -> VectorSizeRange<BaseType>;

}
