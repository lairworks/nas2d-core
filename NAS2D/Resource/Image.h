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

#include "../Math/Vector.h"

#include <string>


struct SDL_Surface;


namespace NAS2D
{
	struct Color;

	template <typename BaseType>
	struct Point;


	/**
	 * Image Class
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
	protected:
		static SDL_Surface* fileToSdlSurface(const std::string& filePath);
		static SDL_Surface* dataToSdlSurface(const std::string& data);
		static SDL_Surface* dataToSdlSurface(void* buffer, int bytesPerPixel, Vector<int> size);

	public:
		explicit Image(const std::string& filePath);
		Image(void* buffer, int bytesPerPixel, Vector<int> size);
		Image(SDL_Surface& surface);

		Image(const Image& rhs) = delete;
		Image& operator=(const Image& rhs) = delete;

		~Image();

		Vector<int> size() const;

		Color pixelColor(Point<int> point) const;

	protected:
		friend class RendererOpenGL;
		unsigned int textureId() const;
		unsigned int frameBufferObjectId() const;

	private:
		SDL_Surface* mSurface{nullptr};
		mutable unsigned int mTextureId{0u};
		mutable unsigned int mFrameBufferObjectId{0u};
		Vector<int> mSize{0, 0};
	};

} // namespace
