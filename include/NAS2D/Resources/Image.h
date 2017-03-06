// ==================================================================================
// = NAS2D
// = Copyright © 2008 - 2017 New Age Software
// ==================================================================================
// = NAS2D is distributed under the terms of the zlib license. You are free to copy,
// = modify and distribute the software under the terms of the zlib license.
// = 
// = Acknowledgement of your use of NAS2D is appriciated but is not required.
// ==================================================================================

#pragma once

#include "Resource.h"

#include <vector>
#include <map>

namespace NAS2D {

class Rectangle_2d;


/**
 * \class Image
 * \brief Image Class
 *
 * Stores an image as a representation of pixels in memory.
 */
class Image: public Resource
{
public:
	Image(const std::string& filePath);
	Image(void* buffer, int bytesPerPixel, int width, int height);
	Image(int width, int height);
	Image();

	Image(const Image &src);
	Image& operator=(const Image& rhs);

	~Image();

	int width() const;
	int height() const;

	const Rectangle_2d& rect() const;

	Color_4ub pixelColor(int x, int y) const;

private:
	void load();

private:
	Rectangle_2d	mRect;			/**< Width/Height information about the Image. */
};


/**
 * \typedef	ImageListPtr
 * \brief	A list of pointers to Image objects.
 */
typedef std::vector<Image*> ImagePtrList;


/**
 * \typedef	ImageList
 * \brief	A list of Image objects.
 */
typedef std::vector<Image> ImageList;

} // namespace
