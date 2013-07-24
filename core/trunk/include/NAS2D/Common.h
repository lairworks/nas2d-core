// ==================================================================================
// = NAS2D
// = Copyright © 2008 - 2013 New Age Software
// ==================================================================================
// = NAS2D is distributed under the terms of the zlib license. You are free to copy,
// = modify and distribute the software under the terms of the zlib license.
// = 
// = Acknowledgement of your use of NAS2D is appriciated but is not required.
// ==================================================================================

#ifndef _NAS_COMMON_
#define _NAS_COMMON_

#include <iostream>
#include <sstream>
#include <string>
#include <vector>

#include "NAS2D/Renderer/Primitives.h"

// Utility Functions
bool isPointInRect(int pointX, int pointY, int rectX, int rectY, int rectW, int rectH);
bool isPointInRect(const Point_2d& point, const Rectangle_2d& rect);
bool isRectInRect(int aX, int aY, int aX2, int aY2, int bX, int bY, int bX2, int bY2);
bool isRectInRect(const Rectangle_2d& a, const Rectangle_2d& b);

int clamp(int x, int a, int b);
float clamp(float x, float a, float b);

int divideUp(int a, int b);


// String functions & types
std::string toLowercase(const std::string& str);
std::string toUppercase(const std::string& str);

template <typename T>
bool from_string(T& t, const std::string& s, std::ios_base& (*f)(std::ios_base&))
{
	std::istringstream iss(s);
	return !(iss >> f >> t).fail();
}

/**
 * \typedef StringList
 * \brief	A list of strings.
 */
typedef std::vector<std::string> StringList;

#endif
