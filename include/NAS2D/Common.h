// ==================================================================================
// = NAS2D
// = Copyright © 2008 - 2019 New Age Software
// ==================================================================================
// = NAS2D is distributed under the terms of the zlib license. You are free to copy,
// = modify and distribute the software under the terms of the zlib license.
// =
// = Acknowledgement of your use of NAS2D is appriciated but is not required.
// ==================================================================================
#pragma once

#include "NAS2D/Renderer/Primitives.h"

#include <memory>
#include <string>
#include <vector>

namespace NAS2D {

std::string versionString();

int versionMajor();
int versionMinor();
int versionPatch();

// Utility Functions
bool isPointInRect(int pointX, int pointY, int rectX, int rectY, int rectW, int rectH);
bool isPointInRect(const Point_2d& point, const Rectangle_2d& rect);
bool isRectInRect(int aX, int aY, int aX2, int aY2, int bX, int bY, int bX2, int bY2);
bool isRectInRect(const Rectangle_2d& a, const Rectangle_2d& b);

int clamp(int x, int min, int max);
float clamp(float x, float min, float max);

int divideUp(int to_divide, int divisor);


// String functions & types
std::string toLowercase(const std::string& str);
std::string toUppercase(const std::string& str);

/**
 * Simple helper function to provide a printf like function.
 */
template<typename ... Args>
std::string string_format(const std::string& format, Args ... args)
{
	size_t size = snprintf(nullptr, 0, format.c_str(), args ...) + 1;
	std::unique_ptr<char[]> buffer(new char[size]);
	snprintf(buffer.get(), size, format.c_str(), args ...);
	return std::string(buffer.get(), buffer.get() + size - 1);
}


/**
 * \typedef StringList
 * \brief	A list of std::string's.
 *
 * The StringList is provided primarily as a convenience typedef
 * but is also used by some of NAS2D's functions.
 */
typedef std::vector<std::string> StringList;

}
