// ==================================================================================
// = NAS2D
// = Copyright © 2008 - 2019 New Age Software
// ==================================================================================
// = NAS2D is distributed under the terms of the zlib license. You are free to copy,
// = modify and distribute the software under the terms of the zlib license.
// =
// = Acknowledgment of your use of NAS2D is appreciated but is not required.
// ==================================================================================
#pragma once

#include "NAS2D/Math/Vector2.h"

namespace NAS2D
{

	class AABB2
	{
	  public:
		Vector2 mins{};
		Vector2 maxs{};

		static const AABB2 Zero_to_One;
		static const AABB2 Neg_One_to_One;

		AABB2() = default;
		AABB2(const AABB2& other) = default;
		AABB2(AABB2&& other) = default;
		AABB2& operator=(const AABB2& other) = default;
		AABB2& operator=(AABB2&& other) = default;
		~AABB2() = default;

		AABB2(float x, float y) noexcept;
		AABB2(float minX, float minY, float maxX, float maxY) noexcept;
		AABB2(const Vector2& mins, const Vector2& maxs) noexcept;
		AABB2(const Vector2& center, float radiusX, float radiusY) noexcept;

		void stretchToIncludePoint(const Vector2& point) noexcept;
		void addPaddingToSides(float paddingX, float paddingY) noexcept;
		void addPaddingToSidesClamped(float paddingX, float paddingY) noexcept;
		void translate(const Vector2& translation) noexcept;

		Vector2 calcCenter() const noexcept;
		Vector2 calcDimensions() const noexcept;

		AABB2 operator+(const Vector2& translation) const noexcept;
		AABB2 operator-(const Vector2& antiTranslation) const noexcept;
		AABB2& operator+=(const Vector2& translation) noexcept;
		AABB2& operator-=(const Vector2& antiTranslation) noexcept;

	  protected:
	  private:
	};

} // namespace NAS2D