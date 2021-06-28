// ==================================================================================
// = NAS2D
// = Copyright © 2008 - 2020 New Age Software
// ==================================================================================
// = NAS2D is distributed under the terms of the zlib license. You are free to copy,
// = modify and distribute the software under the terms of the zlib license.
// =
// = Acknowledgment of your use of NAS2D is appreciated but is not required.
// ==================================================================================
#pragma once

#include "../Renderer/Color.h"
#include "../Renderer/Point.h"
#include "../Renderer/Vector.h"

#include <string>
#include <vector>
#include <utility>


struct SDL_Surface;


namespace NAS2D {

class Image
{
public:
	explicit Image(const std::string& filePath);
	Image(void* buffer, int bytesPerPixel, Vector<int> size);

	Image(const Image &rhs) = delete;
	Image& operator=(const Image& rhs) = delete;

	~Image();

	const std::string& name() const { return mResourceName; }

	Vector<int> size() const;

	Color pixelColor(Point<int> point) const;

protected:
	friend class RendererOpenGL;
	unsigned int textureId() const;
	unsigned int frameBufferObjectId() const;

private:
	//TODO: Rename filePath, change type to std::filesystem::path
	std::string mResourceName;
	SDL_Surface* mSurface{nullptr};
	unsigned int mTextureId{0u};
	mutable unsigned int mFrameBufferObjectId{0u};
	Vector<int> mSize{0, 0};
};

} // namespace
