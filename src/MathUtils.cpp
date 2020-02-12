// ==================================================================================
// = NAS2D
// = Copyright © 2008 - 2019 New Age Software
// ==================================================================================
// = NAS2D is distributed under the terms of the zlib license. You are free to copy,
// = modify and distribute the software under the terms of the zlib license.
// =
// = Acknowledgment of your use of NAS2D is appreciated but is not required.
// ==================================================================================
#include "NAS2D/MathUtils.h"
#include <stdexcept>

/**
 * \fn isPointInRect(int pointX, int pointY, int rectX, int rectY, int rectW, int rectH)
 *
 * Determines if a 2D coordinate is within a rectangular area.
 *
 * \param pointX	X-Coordinate of point to test.
 * \param pointY	Y-Coordinate of point to test.
 * \param rectX		X-Coordinate of origin point of the Rectangular area to test.
 * \param rectY		Y-Coordinate of origin point of the Rectangular area to test.
 * \param rectW		Width of the Rectangular area to test.
 * \param rectH		Height of the Rectangular area to test.
 *
 * \return Returns true if point is within rectangular area.
 */
bool NAS2D::isPointInRect(int pointX, int pointY, int rectX, int rectY, int rectW, int rectH)
{
	return (pointX >= rectX && pointX <= rectX + rectW && pointY >= rectY && pointY <= rectY + rectH);
}

/**
 * \fn isPointInRect(const Point_2d& point, const Rectangle_2d& rect)
 *
 * Determines if a 2D point is within a rectangular area.
 *
 * \param point	2D point to test.
 * \param rect	Rectangular area to test point against.
 *
 * \return Returns true if point is within rectangular area.
 */
bool NAS2D::isPointInRect(const Point_2d& point, const Rectangle_2d& rect)
{
	return (point.x() >= rect.x() && point.x() <= rect.x() + rect.width() && point.y() >= rect.y() && point.y() <= rect.y() + rect.height());
}

/**
 * \fn int divideUp(int a, int b)
 *
 * Basic integer division that rounds up to the nearest whole number.
 *
 * \param	to_divide	Number to be divided.
 * \param	divisor		Divisor.
 *
 * \return	Returns the divided number rounded up to the nearest whole number.
 */
int NAS2D::divideUp(int to_divide, int divisor)
{
	if (divisor == 0) {
		throw std::domain_error("Division by zero: divideUp(to_divide, 0)");
	}
	return (to_divide + (divisor - 1)) / divisor;
}
