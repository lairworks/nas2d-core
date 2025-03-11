// ==================================================================================
// = NAS2D
// = Copyright © 2008 - 2020 New Age Software
// ==================================================================================
// = NAS2D is distributed under the terms of the zlib license. You are free to copy,
// = modify and distribute the software under the terms of the zlib license.
// =
// = Acknowledgment of your use of NAS2D is appreciated but is not required.
// ==================================================================================
#include "Image.h"

#include "../Renderer/Color.h"
#include "../Math/Rectangle.h"
#include "../Filesystem.h"
#include "../Utility.h"
#include "../StringFrom.h"

#if defined(__XCODE_BUILD__)
#include <GLEW/GLEW.h>
#include <SDL2_image/SDL_image.h>
#else
#include <GL/glew.h>
#include <SDL2/SDL_image.h>
#endif

#include <cstdint>
#include <utility>
#include <string>
#include <stdexcept>


using namespace NAS2D;


unsigned int generateTexture(SDL_Surface* surface);
unsigned int generateTexture(void* buffer, int bytesPerPixel, int width, int height);


namespace
{
	constexpr bool isBigEndian = SDL_BYTEORDER == SDL_BIG_ENDIAN;

	unsigned int generateFbo(unsigned int textureId, Vector<int> imageSize);
	unsigned int readPixelValue(std::uintptr_t pixelAddress, unsigned int bytesPerPixel);
}


SDL_Surface* Image::fileToSdlSurface(const std::string& filePath)
{
	const auto& data = Utility<Filesystem>::get().readFile(filePath);

	if (data.size() == 0)
	{
		throw std::runtime_error("Image file is empty: " + filePath);
	}

	return dataToSdlSurface(data);
}


SDL_Surface* Image::dataToSdlSurface(const std::string& data)
{
	auto surface = IMG_Load_RW(SDL_RWFromConstMem(data.c_str(), static_cast<int>(data.size())), 1);
	if (!surface)
	{
		throw std::runtime_error("Image failed to load: " + std::string{SDL_GetError()});
	}
	return surface;
}


SDL_Surface* Image::dataToSdlSurface(void* buffer, int bytesPerPixel, Vector<int> size)
{
	if (buffer == nullptr)
	{
		throw std::runtime_error("Image construction requires non-nullptr buffer");
	}

	if (bytesPerPixel != 3 && bytesPerPixel != 4)
	{
		throw std::runtime_error("Image bit-depth unsupported with bytesPerPixel: " + std::to_string(bytesPerPixel));
	}

	return SDL_CreateRGBSurfaceFrom(buffer, size.x, size.y, bytesPerPixel * 8, 0, 0, 0, 0, isBigEndian ? 0x000000FF : 0xFF000000);
}


/**
 * Loads an Image from disk.
 *
 * \param filePath Path to an image file.
 */
Image::Image(const std::string& filePath) :
	Image{*fileToSdlSurface(filePath)}
{
}


/**
 * Create an Image from a raw data buffer.
 *
 * \param	buffer			Pointer to a data buffer.
 * \param	bytesPerPixel	Number of bytes per pixel. Valid values are 3 and 4 (images < 24-bit are not supported).
 * \param	size			Size of the Image in pixels.
 */
Image::Image(void* buffer, int bytesPerPixel, Vector<int> size) :
	Image{*dataToSdlSurface(buffer, bytesPerPixel, size)}
{
}


Image::Image(SDL_Surface& surface) :
	mSurface{&surface},
	mSize{mSurface->w, mSurface->h}
{
}


Image::~Image()
{
	if (mFrameBufferObjectId != 0)
	{
		glDeleteFramebuffers(1, &mFrameBufferObjectId);
	}
	if (mTextureId != 0)
	{
		glDeleteTextures(1, &mTextureId);
	}

	SDL_FreeSurface(mSurface);
}


/**
 * Gets the dimensions in pixels of the image.
 */
Vector<int> Image::size() const
{
	return mSize;
}


/**
 * Gets the color of a pixel at a given coordinate.
 *
 * \param	point	Coordinates of the pixel to check.
 */
Color Image::pixelColor(Point<int> point) const
{
	if (!Rectangle{{0, 0}, mSize}.contains(point))
	{
		throw std::runtime_error("Pixel coordinates out of bounds: " + stringFrom(point));
	}

	if (!mSurface) { throw std::runtime_error("Image has no allocated surface"); }

	uint8_t bytesPerPixel = mSurface->format->BytesPerPixel;
	const auto unsignedPoint = point.to<std::size_t>();
	const auto pixelOffset = unsignedPoint.y * static_cast<std::size_t>(mSurface->pitch) + unsignedPoint.x * bytesPerPixel;

	SDL_LockSurface(mSurface);
	const auto pixelPtr = reinterpret_cast<std::uintptr_t>(mSurface->pixels) + pixelOffset;
	const auto pixelBytes = readPixelValue(pixelPtr, bytesPerPixel);
	SDL_UnlockSurface(mSurface);

	Color color;
	SDL_GetRGBA(pixelBytes, mSurface->format, &color.red, &color.green, &color.blue, &color.alpha);

	return color;
}


unsigned int Image::textureId() const
{
	if (mTextureId == 0)
	{
		mTextureId = generateTexture(mSurface);
	}
	return mTextureId;
}


unsigned int Image::frameBufferObjectId() const
{
	if (mFrameBufferObjectId == 0)
	{
		mFrameBufferObjectId = generateFbo(mTextureId, mSize);
	}
	return mFrameBufferObjectId;
}


namespace
{
	unsigned int readPixelValue(std::uintptr_t pixelAddress, unsigned int bytesPerPixel)
	{
		switch (bytesPerPixel)
		{
		case 1:
			return *reinterpret_cast<const uint8_t*>(pixelAddress);
		case 2:
			return *reinterpret_cast<const uint16_t*>(pixelAddress);
		case 3: {
			auto p = reinterpret_cast<const uint8_t*>(pixelAddress);
			uint32_t p0 = p[0], p1 = p[1], p2 = p[2];
			return (isBigEndian) ? (p0 << 16 | p1 << 8 | p2) : (p0 | p1 << 8 | p2 << 16);
		}
		case 4:
			return *reinterpret_cast<const uint32_t*>(pixelAddress);
		default:
			throw std::runtime_error("Unknown pixel format with bytesPerPixel: " + std::to_string(bytesPerPixel));
		}
	}


	/**
	 * Generates an OpenGL Frame Buffer Object.
	 */
	unsigned int generateFbo(unsigned int textureId, Vector<int> imageSize)
	{
		unsigned int framebuffer;
		glGenFramebuffers(1, &framebuffer);
		glBindFramebuffer(GL_FRAMEBUFFER, framebuffer);

		if (textureId == 0)
		{
			unsigned int textureColorbuffer;
			glGenTextures(1, &textureColorbuffer);
			glBindTexture(GL_TEXTURE_2D, textureColorbuffer);
			const auto textureFormat = (isBigEndian) ? GL_BGRA : GL_RGBA;

			glTexImage2D(GL_TEXTURE_2D, 0, textureFormat, imageSize.x, imageSize.y, 0, textureFormat, GL_UNSIGNED_BYTE, nullptr);
			glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
			glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
		}

		glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, GL_TEXTURE_2D, textureId, 0);
		glBindFramebuffer(GL_FRAMEBUFFER, 0);

		return framebuffer;
	}
}


/**
 * Generates a new OpenGL texture from an SDL_Surface.
 */
unsigned int generateTexture(SDL_Surface* surface)
{
	const auto bytesPerPixel = surface->format->BytesPerPixel;
	if (bytesPerPixel == 3 || bytesPerPixel == 4)
	{
		return generateTexture(surface->pixels, bytesPerPixel, surface->w, surface->h);
	}

	auto* newSurface = SDL_ConvertSurfaceFormat(surface, SDL_PIXELFORMAT_RGBA32, 0);
	const auto textureId = generateTexture(newSurface->pixels, newSurface->format->BytesPerPixel, newSurface->w, newSurface->h);
	SDL_FreeSurface(newSurface);
	return textureId;
}


unsigned int generateTexture(void* buffer, int bytesPerPixel, int width, int height)
{
	GLint internalFormat = 0;
	GLenum textureFormat = 0;
	switch (bytesPerPixel)
	{
	case 4:
		internalFormat = GL_RGBA;
		textureFormat = isBigEndian ? GL_BGRA : GL_RGBA;
		break;
	case 3:
		internalFormat = GL_RGB;
		textureFormat = isBigEndian ? GL_BGR : GL_RGB;
		break;

	default:
		throw std::runtime_error("Bit-depth unsupported with bytesPerPixel: " + std::to_string(bytesPerPixel));
	}

	GLuint textureId;
	glGenTextures(1, &textureId);
	glBindTexture(GL_TEXTURE_2D, textureId);

	// Set texture and pixel handling states.
	glPixelStorei(GL_UNPACK_ALIGNMENT, 1);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
	glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
	glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);

	glTexImage2D(GL_TEXTURE_2D, 0, internalFormat, width, height, 0, textureFormat, GL_UNSIGNED_BYTE, buffer);

	return textureId;
}
