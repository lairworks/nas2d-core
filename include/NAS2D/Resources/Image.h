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

#include "NAS2D/Resources/Resource.h"
#include "NAS2D/Renderer/Primitives.h"

#include <vector>
#include <utility>


namespace NAS2D {


/**
 * \class Image
 * \brief Image Class
 *
 * Provides a high level interface for image files. Can load the following formats:
 * - BMP
 * - JPEG
 * - PCX
 * - PNG
 * - TGA
 * - TIFF
 * - WEBP
 *
 * \note	Image currently only supports 24-bit and 32-bit images (true-color with
 *			and without an alpha channel).
 *
 */
class Image: public Resource
{
public:
	Image(const std::string& filePath);
	Image(void* buffer, int bytesPerPixel, int width, int height);
	Image(int width, int height);
	Image();

	Image(const Image &rhs);
	Image& operator=(const Image& rhs);

	~Image();

	int width() const;
	int height() const;

	Color_4ub pixelColor(int x, int y) const;

private:
	void load();

private:
	std::pair<int, int>		_size;			/**< Width/Height information about the Image. */
};


/**
 * \typedef	ImagePtrList
 * \brief	A list of pointers to Image objects.
 */
typedef std::vector<Image*> ImagePtrList;


/**
 * \typedef	ImageList
 * \brief	A list of Image objects.
 */
typedef std::vector<Image> ImageList;

} // namespace
