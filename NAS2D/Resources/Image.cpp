// ==================================================================================
// = NAS2D
// = Copyright © 2008 - 2019 New Age Software
// ==================================================================================
// = NAS2D is distributed under the terms of the zlib license. You are free to copy,
// = modify and distribute the software under the terms of the zlib license.
// =
// = Acknowledgement of your use of NAS2D is appriciated but is not required.
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
	struct ImageInfo
	{
		SDL_Surface* surface{nullptr};
		unsigned int textureId{0u};
		unsigned int frameBufferObjectId{0u};
		Vector<int> size{0, 0};
		int refCount{0};
	};

	std::map<std::string, ImageInfo> imageIdMap; /**< Lookup table for OpenGL Texture ID's. */

	const std::string ARBITRARY_IMAGE_NAME = "arbitrary_image_";
	int IMAGE_ARBITRARY = 0; /**< Counter for arbitrary image ID's. */

	void updateImageReferenceCount(const std::string& name);

	GLuint generateFbo(const Image& image);
}


/**
 * Loads an Image from disk.
 *
 * \param filePath Path to an image file.
 */
Image::Image(const std::string& filePath) :
	mResourceName{filePath}
{
	if (imageIdMap.find(mResourceName) != imageIdMap.end())
	{
		++imageIdMap[mResourceName].refCount;
		mSize = imageIdMap[mResourceName].size;
		return;
	}

	File imageFile = Utility<Filesystem>::get().open(mResourceName);
	if (imageFile.size() == 0)
	{
		throw std::runtime_error("Image file is empty: " + mResourceName);
	}

	SDL_Surface* surface = IMG_Load_RW(SDL_RWFromConstMem(imageFile.raw_bytes(), static_cast<int>(imageFile.size())), 0);
	if (!surface)
	{
		throw std::runtime_error("Image failed to load: " + std::string{SDL_GetError()});
	}

	mSize = Vector{surface->w, surface->h};

	unsigned int textureId = generateTexture(surface);

	// Add generated texture id to texture ID map.
	auto& imageInfo = imageIdMap[mResourceName];
	imageInfo.surface = surface;
	imageInfo.textureId = textureId;
	imageInfo.size = mSize;
	imageInfo.refCount++;
}


/**
 * Create an Image from a raw data buffer.
 *
 * \param	buffer			Pointer to a data buffer.
 * \param	bytesPerPixel	Number of bytes per pixel. Valid values are 3 and 4 (images < 24-bit are not supported).
 * \param	width			Width of the Image.
 * \param	height			Height of the Image.
 */
Image::Image(void* buffer, int bytesPerPixel, int width, int height) :
	mResourceName{ARBITRARY_IMAGE_NAME + std::to_string(IMAGE_ARBITRARY)}
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

	SDL_Surface* surface = SDL_CreateRGBSurfaceFrom(buffer, width, height, bytesPerPixel * 8, 0, 0, 0, 0, SDL_BYTEORDER == SDL_BIG_ENDIAN ? 0x000000FF : 0xFF000000);

	mSize = Vector{width, height};

	unsigned int textureId = generateTexture(surface);

	// Update resource management.
	auto& imageInfo = imageIdMap[mResourceName];
	imageInfo.textureId = textureId;
	imageInfo.size = {width, height};
	imageInfo.refCount++;
	imageInfo.surface = surface;
}


/**
 * Copy C'tor.
 *
 * \param	src		Image to copy.
 */
Image::Image(const Image &src) :
	mResourceName{src.mResourceName},
	mSize{src.mSize}
{
	imageIdMap[mResourceName].refCount++;
}


/**
 * D'tor
 */
Image::~Image()
{
	updateImageReferenceCount(mResourceName);
}


/**
 * Copy assignment operator.
 *
 * \param	rhs		Image to copy.
 */
Image& Image::operator=(const Image& rhs)
{
	if (this == &rhs) { return *this; }

	updateImageReferenceCount(mResourceName);

	mResourceName = rhs.mResourceName;
	mSize = rhs.mSize;

	auto it = imageIdMap.find(mResourceName);
	if (it == imageIdMap.end())
	{
		throw image_bad_data();
	}

	++it->second.refCount;

	return *this;
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
	if (!Rectangle<int>::Create({0, 0}, mSize).contains(point))
	{
		throw std::runtime_error("Pixel coordinates out of bounds: {" + std::to_string(point.x) + ", " + std::to_string(point.y) + "}");
	}

	SDL_Surface* surface = imageIdMap[mResourceName].surface;

	if (!surface) { throw image_null_data(); }

	SDL_LockSurface(surface);
	uint8_t bytesPerPixel = surface->format->BytesPerPixel;
	const auto unsignedPoint = point.to<std::size_t>();
	const auto pixelOffset = unsignedPoint.y * static_cast<std::size_t>(surface->pitch) + unsignedPoint.x * bytesPerPixel;
	auto pixelPtr = reinterpret_cast<std::uintptr_t>(surface->pixels) + pixelOffset;

	unsigned int pixelBytes = 0;

	switch (bytesPerPixel)
	{
		case 1:
		{
			auto p = reinterpret_cast<const uint8_t*>(pixelPtr);
			pixelBytes = *p;
			break;
		}
		case 2:
		{
			auto p = reinterpret_cast<const uint16_t*>(pixelPtr);
			pixelBytes = *p;
			break;
		}
		case 3:
		{
			auto p = reinterpret_cast<const uint8_t*>(pixelPtr);
			if constexpr (SDL_BYTEORDER == SDL_BIG_ENDIAN)
			{
				pixelBytes = p[0] << 16 | p[1] << 8 | p[2];
			}
			else
			{
				pixelBytes = p[0] | p[1] << 8 | p[2] << 16;
			}
			break;
		}
		case 4:
		{
			auto p = reinterpret_cast<const uint32_t*>(pixelPtr);
			pixelBytes = *p;
			break;
		}
		default: // Should never be possible.
		{
			throw image_bad_data();
		}
	}

	Color color;
	SDL_GetRGBA(pixelBytes, surface->format, &color.red, &color.green, &color.blue, &color.alpha);
	SDL_UnlockSurface(surface);

	return color;
}


/**
 * Gets the color of a pixel at a given coordinate.
 *
 * \param	x	X-Coordinate of the pixel to check.
 * \param	y	Y-Coordinate of the pixel to check.
 */
Color Image::pixelColor(int x, int y) const
{
	return pixelColor({x, y});
}


unsigned int Image::textureId() const
{
	return imageIdMap[mResourceName].textureId;
}


unsigned int Image::frameBufferObjectId() const
{
	auto& imageInfo = imageIdMap[mResourceName];
	if (imageInfo.frameBufferObjectId == 0)
	{
		imageInfo.frameBufferObjectId = generateFbo(*this);
	}
	return imageInfo.frameBufferObjectId;
}


namespace {
	/**
	* Internal function used to clean up references to fonts when the Image
	* destructor or copy assignment operators are called.
	*
	* \param	name	Name of the Image to check against.
	*/
	void updateImageReferenceCount(const std::string& name)
	{
		auto it = imageIdMap.find(name);
		if (it == imageIdMap.end())
		{
			return;
		}

		auto& imageInfo = it->second;
		--imageInfo.refCount;

		if (imageInfo.refCount <= 0)
		{
			if (imageInfo.textureId != 0)
			{
				glDeleteTextures(1, &imageInfo.textureId);
			}

			if (imageInfo.frameBufferObjectId != 0)
			{
				glDeleteFramebuffers(1, &imageInfo.frameBufferObjectId);
			}

			if (imageInfo.surface != nullptr)
			{
				SDL_FreeSurface(imageInfo.surface);
				imageInfo.surface = nullptr;
			}

			imageIdMap.erase(it);
		}
	}


	/**
	 * Generates an OpenGL Frame Buffer Object.
	 */
	GLuint generateFbo(const Image& image)
	{
		unsigned int framebuffer;
		glGenFramebuffers(1, &framebuffer);
		glBindFramebuffer(GL_FRAMEBUFFER, framebuffer);

		if (image.textureId() == 0)
		{
			unsigned int textureColorbuffer;
			glGenTextures(1, &textureColorbuffer);
			glBindTexture(GL_TEXTURE_2D, textureColorbuffer);
			const auto textureFormat = (SDL_BYTEORDER == SDL_BIG_ENDIAN) ? GL_BGRA : GL_RGBA;

			const auto imageSize = image.size();
			glTexImage2D(GL_TEXTURE_2D, 0, textureFormat, imageSize.x, imageSize.y, 0, textureFormat, GL_UNSIGNED_BYTE, nullptr);
			glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
			glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
		}

		glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, GL_TEXTURE_2D, image.textureId(), 0);
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

	// Set texture and pixel handling states.
	glPixelStorei(GL_UNPACK_ALIGNMENT, 1);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
	glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
	glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);

	glTexImage2D(GL_TEXTURE_2D, 0, textureFormat, width, height, 0, textureFormat, GL_UNSIGNED_BYTE, buffer);

	return textureId;
}
