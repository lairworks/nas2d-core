// ==================================================================================
// = NAS2D
// = Copyright © 2008 - 2019 New Age Software
// ==================================================================================
// = NAS2D is distributed under the terms of the zlib license. You are free to copy,
// = modify and distribute the software under the terms of the zlib license.
// =
// = Acknowledgement of your use of NAS2D is appriciated but is not required.
// ==================================================================================
#include "NAS2D/Common.h"

#include <cctype>
#include <algorithm>
#include <sstream>


const int NAS2D_MAJOR_VERSION = 1;
const int NAS2D_MINOR_VERSION = 4;
const int NAS2D_PATCH_VERSION = 2;


/**
 * Gets a string containing the version of NAS2D being used.
 */
std::string NAS2D::versionString()
{
	std::stringstream ss;
	ss << versionMajor() << "." << versionMinor() << "." << versionPatch();
	return ss.str();
}


/**
 * Gets version major.
 */
int NAS2D::versionMajor()
{
	return NAS2D_MAJOR_VERSION;
}


/**
 * Gets version minor.
 */
int NAS2D::versionMinor()
{
	return NAS2D_MINOR_VERSION;
}


/**
 * Gets version patch.
 */
int NAS2D::versionPatch()
{
	return NAS2D_PATCH_VERSION;
}


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
 * \fn isRectInRect(int aX, int aY, int aX2, int aY2, int bX, int bY, int bX2, int bY2)
 *
 * Determines if two rectangles, A and B, intersect.
 *
 * \param aX	X-Coordinate of origin point of rectangle A.
 * \param aY	Y-Coordinate of origin point of rectangle A.
 * \param aX2	X-Coordinate of end point of rectangle A.
 * \param aY2	Y-Coordinate of end point of rectangle A.
 * \param bX	X-Coordinate of origin point of rectangle B.
 * \param bY	Y-Coordinate of origin point of rectangle B.
 * \param bX2	X-Coordinate of end point of rectangle B.
 * \param bY2	Y-Coordinate of end point of rectangle B.
 *
 * \return Returns true if rectangles intersect.
 */
bool NAS2D::isRectInRect(int aX, int aY, int aX2, int aY2, int bX, int bY, int bX2, int bY2)
{
	return (aX <= bX2 && aX2 >= bX && aY <= bY2 && aY2 >= bY);
}


/**
 * \fn isRectInRect(const Rectangle_2d& a, const Rectangle_2d& b)
 *
 * Determines if two rectangles, A and B, intersect.
 *
 * \param a	Rectangle to test.
 * \param b	Rectangle to test against.
 *
 * \return Returns true if rectangles intersect.
 */
bool NAS2D::isRectInRect(const Rectangle_2d& a, const Rectangle_2d& b)
{
	return (a.x() <= (b.x() + b.width()) && (a.x() + a.width()) >= b.x() && a.y() <= (b.y() + b.height()) && (a.y() + a.height()) >= b.y());
}


/**
 * \fn toLowercase(const std::string& str)
 *
 * Converts a string to lowercase.
 *
 * \param str	Source string.
 *
 * \return	Returns the converted string.
 */
std::string NAS2D::toLowercase(const std::string& str)
{
	std::string transformStr(str);
	std::transform(transformStr.begin(), transformStr.end(), transformStr.begin(), (int(*)(int))std::tolower);
	return transformStr;
}


/**
 * \fn toUppercase(const std::string& str)
 *
 * Converts a string to uppercase.
 *
 * \param str	Source string.
 *
 * \return	Returns the converted string.
 */
std::string NAS2D::toUppercase(const std::string& str)
{
	std::string transformStr(str);
	std::transform(transformStr.begin(), transformStr.end(), transformStr.begin(), (int(*)(int))std::toupper);
	return transformStr;
}


/**
 * \fn clamp(int x, int a, int b)
 *
 * Clamps an \c int value to a specified range.
 *
 * \param	x	Value to clamp.
 * \param	min	Minimum value to clamp to.
 * \param	max	Maximum value to clamp to.
 *
 * \return	Clamped value.
 */
int NAS2D::clamp(int x, int min, int max)
{
	return x < min ? min : (x > max ? max : x);
}


/**
 * \fn float clamp(float x, float a, float b)
 *
 * Clamps a \c float value to a specified range.
 *
 * \param	x	Value to clamp.
 * \param	min	Minimum value to clamp to.
 * \param	max	Maximum value to clamp to.
 *
 * \return	Clamped value.
 */
float NAS2D::clamp(float x, float min, float max)
{
	return x < min ? min : (x > max ? max : x);
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
	return (to_divide + (divisor - 1)) / divisor;
}
