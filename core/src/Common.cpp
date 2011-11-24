// ==================================================================================
// = NAS2D
// = Copyright © 2008 New Age Software
// ==================================================================================
// = NAS2D is distributed under the terms of the zlib license. You are free to copy,
// = modify and distribute the software as long under the terms of the zlib license.
// = 
// = Acknowledgement of your use of NAS2D is appriciated but is not required.
// ==================================================================================

#include "NAS2D/Common.h"

#include <cctype>
#include <algorithm>
#include <sstream>

#if defined(WIN32)
#include <windows.h>
#endif

/**
 * \fn isPointInRect
 * Checks to see if the mouse is within a certain area.
 *
 * \param pointX	X-Coordinate of point to test.
 * \param pointY	Y-Coordinate of point to test.
 * \param rectX		X-Coordinate of origin point of the Rectangular area to test.
 * \param rectY		Y-Coordinate of origin point of the Rectangular area to test.
 * \param rectW		Width of the Rectangular area to test.
 * \param rectH		Height of the Rectangular area to test.
 */
bool isPointInRect(int pointX, int pointY, int rectX, int rectY, int rectW, int rectH)
{
	return (pointX > rectX && pointX < rectX + rectW && pointY > rectY && pointY < rectY + rectH);
}


bool isPointInRect(const Point_2d& point, const Rectangle_2d& rect)
{
	return (point.x > rect.x && point.x < rect.x + rect.w && point.y > rect.y && point.y < rect.y + rect.h);
}


bool isRectInRect(int aX, int aY, int aX2, int aY2, int bX, int bY, int bX2, int bY2)
{
	return (aX < bX2 && aX2 > bX && aY < bY2 && aY2 > bY);
}


bool isRectInRect(const Rectangle_2d& a, const Rectangle_2d& b)
{
	return (a.x < (b.x + b.w) && (a.x + a.w) > b.x && a.y < (b.y + b.h) && (a.y + a.h) > b.y);
}


/**
 * \fn toLowercase
 *
 * Converts a string to lowercase.
 */
std::string toLowercase(const std::string& str)
{
	std::string transformStr(str);
	std::transform(transformStr.begin(), transformStr.end(), transformStr.begin(), (int(*)(int))std::tolower);
	return transformStr;
}


/**
 * \fn toUppercase
 *
 * Converts a string to uppercase.
 */
std::string toUppercase(const std::string& str)
{
	std::string transformStr(str);
	std::transform(transformStr.begin(), transformStr.end(), transformStr.begin(), (int(*)(int))std::toupper);
	return transformStr;
}


/**
 * Clamps an \c int value to a specified range.
 *
 * \param	x	Value to clamp.
 * \param	a	Minimum value to clamp to.
 * \param	b	Maximum value to clamp to.
 */
int clamp(int x, int a, int b)
{
    return x < a ? a : (x > b ? b : x);
}


/**
 * Clamps a \c float value to a specified range.
 *
 * \param	x	Value to clamp.
 * \param	a	Minimum value to clamp to.
 * \param	b	Maximum value to clamp to.
 */
float clamp(float x, float a, float b)
{
    return x < a ? a : (x > b ? b : x);
}
