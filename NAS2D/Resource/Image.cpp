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

#include "../Renderer/Rectangle.h"
#include "../Exception.h"
#include "../Filesystem.h"
#include "../Utility.h"

#include <GL/glew.h>
#include <SDL2/SDL_image.h>

#include <iostream>
#include <map>
#include <string>
#include <stdexcept>


using namespace NAS2D;
using namespace NAS2D::Exception;


unsigned int generateTexture(SDL_Surface* surface);
unsigned int generateTexture(void *buffer, int bytesPerPixel, int width, int height);


namespace {
	const std::string ARBITRARY_IMAGE_NAME = "arbitrary_image_";
	int IMAGE_ARBITRARY = 0; /**< Counter for arbitrary image ID's. */

	unsigned int generateFbo(unsigned int textureId, Vector<int> imageSize);
	unsigned int readPixelValue(std::uintptr_t pixelAddress, unsigned int bytesPerPixel);
}

Image::Image(const std::string& filePath) :
	mResourceName{filePath}
{
	auto imageFile = Utility<Filesystem>::get().open(mResourceName);
	if (imageFile.size() == 0)
	{
		throw std::runtime_error("Image file is empty: " + mResourceName);
	}

	mSurface = IMG_Load_RW(SDL_RWFromConstMem(imageFile.raw_bytes(), static_cast<int>(imageFile.size())), 1);
	if (!mSurface)
	{
		throw std::runtime_error("Image failed to load: " + std::string{SDL_GetError()});
	}
	mTextureId = generateTexture(mSurface);
	mSize = Vector{mSurface->w, mSurface->h};
}

Image::Image(void* buffer, int bytesPerPixel, Vector<int> size) :
	mResourceName{ARBITRARY_IMAGE_NAME + std::to_string(IMAGE_ARBITRARY)},
	mSize{size}
{
	if (buffer == nullptr)
	{
		throw image_null_data();
	}

	if (bytesPerPixel != 3 && bytesPerPixel != 4)
	{
		throw image_unsupported_bit_depth();
	}

	++IMAGE_ARBITRARY;

	mSurface = SDL_CreateRGBSurfaceFrom(buffer, size.x, size.y, bytesPerPixel * 8, 0, 0, 0, 0, SDL_BYTEORDER == SDL_BIG_ENDIAN ? 0x000000FF : 0xFF000000);
	mTextureId = generateTexture(mSurface);
}


Image::~Image()
{
	glDeleteTextures(1, &mTextureId);
	glDeleteFramebuffers(1, &mFrameBufferObjectId);
	SDL_FreeSurface(mSurface);
}

Vector<int> Image::size() const
{
	return mSize;
}

Color Image::pixelColor(Point<int> point) const
{
	if (!Rectangle<int>::Create({0, 0}, mSize).contains(point))
	{
		throw std::runtime_error("Pixel coordinates out of bounds: {" + std::to_string(point.x) + ", " + std::to_string(point.y) + "}");
	}

	if (!mSurface) { throw image_null_data(); }

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


namespace {
	unsigned int readPixelValue(std::uintptr_t pixelAddress, unsigned int bytesPerPixel)
	{
		switch (bytesPerPixel)
		{
			case 1:
			{
				return *reinterpret_cast<const uint8_t*>(pixelAddress);
			}
			case 2:
			{
				return *reinterpret_cast<const uint16_t*>(pixelAddress);
			}
			case 3:
			{
				auto p = reinterpret_cast<const uint8_t*>(pixelAddress);
				return (SDL_BYTEORDER == SDL_BIG_ENDIAN) ?
					(p[0] << 16 | p[1] << 8 | p[2]) :
					(p[0] | p[1] << 8 | p[2] << 16);
			}
			case 4:
			{
				return *reinterpret_cast<const uint32_t*>(pixelAddress);
			}
			default:
			{
				throw image_bad_data();
			}
		}
	}

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
			const auto textureFormat = (SDL_BYTEORDER == SDL_BIG_ENDIAN) ? GL_BGRA : GL_RGBA;

			glTexImage2D(GL_TEXTURE_2D, 0, textureFormat, imageSize.x, imageSize.y, 0, textureFormat, GL_UNSIGNED_BYTE, nullptr);
			glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
			glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
		}

		glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, GL_TEXTURE_2D, textureId, 0);
		glBindFramebuffer(GL_FRAMEBUFFER, 0);

		return framebuffer;
	}
}

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


unsigned int generateTexture(void *buffer, int bytesPerPixel, int width, int height)
{
	GLenum textureFormat = 0;
	switch (bytesPerPixel)
	{
	case 4:
		textureFormat = SDL_BYTEORDER == SDL_BIG_ENDIAN ? GL_BGRA : GL_RGBA;
		break;
	case 3:
		textureFormat = SDL_BYTEORDER == SDL_BIG_ENDIAN ? GL_BGR : GL_RGB;
		break;

	default:
		throw image_unsupported_bit_depth();
	}

	GLuint textureId;
	glGenTextures(1, &textureId);
	glBindTexture(GL_TEXTURE_2D, textureId);

	glPixelStorei(GL_UNPACK_ALIGNMENT, 1);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
	glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
	glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);

	glTexImage2D(GL_TEXTURE_2D, 0, textureFormat, width, height, 0, textureFormat, GL_UNSIGNED_BYTE, buffer);

	return textureId;
}
