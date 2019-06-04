#include "NAS2D/Math/AABB2.h"

#include <algorithm>

namespace NAS2D
{
	//Ignore VS2019 26426 i.22 false-positives
	//These have known initialization and it doesn't matter what order.
	const AABB2 AABB2::Zero_to_One(0.0f, 0.0f, 1.0f, 1.0f);
	const AABB2 AABB2::Neg_One_to_One(-1.0f, -1.0f, 1.0f, 1.0f);

	AABB2::AABB2(float x, float y) noexcept
	: mins(x, y)
	, maxs(x, y)
	{
		/* DO NOTHING */
	}

	AABB2::AABB2(float minX, float minY, float maxX, float maxY) noexcept
	: mins(minX, minY)
	, maxs(maxX, maxY)
	{
		/* DO NOTHING */
	}

	AABB2::AABB2(const Vector2& mins, const Vector2& maxs) noexcept
	: mins(mins)
	, maxs(maxs)
	{
		/* DO NOTHING */
	}

	AABB2::AABB2(const Vector2& center, float radiusX, float radiusY) noexcept
	: mins(center.x - radiusX, center.y - radiusY)
	, maxs(center.x + radiusX, center.y + radiusY)
	{
		/* DO NOTHING */
	}

	void AABB2::stretchToIncludePoint(const Vector2& point) noexcept
	{
		if (point.x < mins.x)
		{
			mins.x = point.x;
		}
		if (point.y < mins.y)
		{
			mins.y = point.y;
		}
		if (maxs.x < point.x)
		{
			maxs.x = point.x;
		}
		if (maxs.y < point.y)
		{
			maxs.y = point.y;
		}
	}

	void AABB2::addPaddingToSides(float paddingX, float paddingY) noexcept
	{
		mins.x -= paddingX;
		mins.y -= paddingY;

		maxs.x += paddingX;
		maxs.y += paddingY;
	}

	void AABB2::addPaddingToSidesClamped(float paddingX, float paddingY) noexcept
	{
		auto width = maxs.x - mins.x;
		auto height = maxs.y - mins.y;
		auto half_width = width * 0.5f;
		auto half_height = height * 0.5f;

		paddingX = (std::max)(-half_width, paddingX);
		paddingY = (std::max)(-half_height, paddingY);

		mins.x -= paddingX;
		mins.y -= paddingY;

		maxs.x += paddingX;
		maxs.y += paddingY;
	}

	void AABB2::translate(const Vector2& translation) noexcept
	{
		mins += translation;
		maxs += translation;
	}

	Vector2 AABB2::calcCenter() const noexcept
	{
		return {mins.x + (maxs.x - mins.x) * 0.5f, mins.y + (maxs.y - mins.y) * 0.5f};
	}

	Vector2 AABB2::calcDimensions() const noexcept
	{
		return maxs - mins;
	}

	AABB2 AABB2::operator+(const Vector2& translation) const noexcept
	{
		return {mins + translation, maxs + translation};
	}

	AABB2 AABB2::operator-(const Vector2& antiTranslation) const noexcept
	{
		return {mins - antiTranslation, maxs - antiTranslation};
	}

	AABB2& AABB2::operator-=(const Vector2& antiTranslation) noexcept
	{
		mins -= antiTranslation;
		maxs -= antiTranslation;
		return *this;
	}

	AABB2& AABB2::operator+=(const Vector2& translation) noexcept
	{
		mins += translation;
		maxs += translation;
		return *this;
	}

} // namespace NAS2D