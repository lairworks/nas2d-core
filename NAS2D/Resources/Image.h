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

#include "../Renderer/Color.h"
#include "../Renderer/Point.h"
#include "../Renderer/Vector.h"

#include <string>
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
class Image
{
public:
	Image();
	explicit Image(const std::string& filePath);
	Image(void* buffer, int bytesPerPixel, int width, int height);

	Image(const Image &rhs);
	Image& operator=(const Image& rhs);

	~Image();

	const std::string& name() const { return mResourceName; }

	Vector<int> size() const;

	Color pixelColor(Point<int> point) const;
	Color pixelColor(int x, int y) const;

	// Temporary method, that will be removed in a future refactor
	// Intended only to be used by RendererOpenGL
	// As it is so specific, it should not be part of the Image class, nor ImageInfo
	unsigned int textureId() const;

private:
	void load();

private:
	std::string mResourceName{"Default Resource"}; /**< File path and internal identifier. */
	Vector<int> mSize; /**< Width/Height information about the Image. */
};


/**
 * \typedef	ImagePtrList
 * \brief	A list of pointers to Image objects.
 */
using ImagePtrList = std::vector<Image*>;


/**
 * \typedef	ImageList
 * \brief	A list of Image objects.
 */
using ImageList = std::vector<Image>;

} // namespace
