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
 */
class Image
{
public:
	explicit Image(const std::string& filePath);
	Image(void* buffer, int bytesPerPixel, Vector<int> size);

	Image(const Image &rhs);
	Image& operator=(const Image& rhs);

	~Image();

	const std::string& name() const { return mResourceName; }

	Vector<int> size() const;

	Color pixelColor(Point<int> point) const;

	// Temporary methods, that will be removed in a future refactor
	// Intended only to be used by RendererOpenGL
	// As they are so specific, they should not be part of the Image class, nor ImageInfo
	unsigned int textureId() const;
	unsigned int frameBufferObjectId() const;

private:
	std::string mResourceName{"Default Resource"}; /**< File path and internal identifier. */
	Vector<int> mSize; /**< Width/Height information about the Image. */
};


/**
 * \typedef	ImageList
 * \brief	A list of Image objects.
 */
using ImageList = std::vector<Image>;

} // namespace
