// ==================================================================================
// = NAS2D
// = Copyright © 2008 - 2013 New Age Software
// ==================================================================================
// = NAS2D is distributed under the terms of the zlib license. You are free to copy,
// = modify and distribute the software under the terms of the zlib license.
// = 
// = Acknowledgement of your use of NAS2D is appriciated but is not required.
// ==================================================================================

#include "NAS2D/Renderer/Primitives.h"

Point_2d::Point_2d():	x(0),
			y(0)
{}


Point_2d::Point_2d(int x, int y):	x(x),
						y(y)
{}

Point_2d::Point_2d(const Point_2df& _p):	x(static_cast<int>(_p.x)),
											y(static_cast<int>(_p.y))
{}


bool Point_2d::operator==(const Point_2d& pt)
{
	return (x == pt.x) && (y == pt.y);
}


bool Point_2d::operator!=(const Point_2d& pt)
{
	return (x != pt.x) || (y != pt.y);
}


void Point_2d::operator()(int _x, int _y)
{
	x = _x;
	y = _y;
}


Point_2d Point_2d::operator+(const Point_2d& pt)
{
	return Point_2d(x + pt.x, y + pt.y);
}


Point_2d Point_2d::operator-(const Point_2d& pt)
{
	return Point_2d(x - pt.x, y - pt.y);
}