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
#include "ImageInfo.h"

#include "../Exception.h"
#include "../Filesystem.h"
#include "../Utility.h"

#include <GL/glew.h>
#include <SDL2/SDL_image.h>

#include <iostream>
#include <map>
#include <string>

using namespace NAS2D;
using namespace NAS2D::Exception;


std::map<std::string, ImageInfo> imageIdMap; /**< Lookup table for OpenGL Texture ID's. */


unsigned int generateTexture(SDL_Surface* surface);
unsigned int generateTexture(void *buffer, int bytesPerPixel, int width, int height);


namespace {
	const std::string DEFAULT_IMAGE_NAME = "Default Image";
	const std::string ARBITRARY_IMAGE_NAME = "arbitrary_image_";
	int IMAGE_ARBITRARY = 0; /**< Counter for arbitrary image ID's. */

	bool checkTextureId(const std::string& name);
	void updateImageReferenceCount(const std::string& name);
}


/**
 * Loads an Image from disk.
 *
 * \param filePath Path to an image file.
 */
Image::Image(const std::string& filePath) : Resource(filePath)
{
	load();
}


/**
 * Default C'tor.
 */
Image::Image() : Resource(DEFAULT_IMAGE_NAME)
{
	loaded(true);
}


/**
 * Create a blank Image of X, Y dimensions.
 *
 * \param	width	Width of the Image.
 * \param	height	Height of the Image.
 */
Image::Image(int width, int height) : Resource(ARBITRARY_IMAGE_NAME)
{
	name(ARBITRARY_IMAGE_NAME + std::to_string(++IMAGE_ARBITRARY));
	mSize = Vector{width, height};

	// Update resource management.
	auto& imageInfo = imageIdMap[name()];
	imageInfo.textureId = 0;
	imageInfo.size = {width, height};
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
Image::Image(void* buffer, int bytesPerPixel, int width, int height) : Resource(ARBITRARY_IMAGE_NAME)
{
	if (buffer == nullptr)
	{
		throw image_null_data();
	}

	if (bytesPerPixel != 3 && bytesPerPixel != 4)
	{
		throw image_unsupported_bit_depth();
	}

	name(ARBITRARY_IMAGE_NAME + std::to_string(++IMAGE_ARBITRARY));

	SDL_Surface* surface = SDL_CreateRGBSurfaceFrom(buffer, width, height, bytesPerPixel * 4, 0, 0, 0, 0, SDL_BYTEORDER == SDL_BIG_ENDIAN ? 0x000000FF : 0xFF000000);

	mSize = Vector{width, height};

	unsigned int textureId = generateTexture(surface);

	// Update resource management.
	auto& imageInfo = imageIdMap[name()];
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
Image::Image(const Image &src) : Resource(src.name()), mSize(src.mSize)
{
	if (!src.loaded())
	{
		throw image_bad_copy();
	}

	loaded(src.loaded());
	imageIdMap[name()].refCount++;
}


/**
 * D'tor
 */
Image::~Image()
{
	updateImageReferenceCount(name());
}


/**
 * Copy assignment operator.
 *
 * \param	rhs		Image to copy.
 */
Image& Image::operator=(const Image& rhs)
{
	if (this == &rhs) { return *this; }

	updateImageReferenceCount(name());

	name(rhs.name());
	mSize = rhs.mSize;

	auto it = imageIdMap.find(name());
	if (it == imageIdMap.end())
	{
		throw image_bad_data();
	}

	loaded(rhs.loaded());
	++it->second.refCount;

	return *this;
}


/**
 * Loads an image file from disk.
 *
 * \note	If loading fails, Image will be set to a valid internal state.
 */
void Image::load()
{
	if (checkTextureId(name()))
	{
		mSize = imageIdMap[name()].size;
		loaded(true);
		return;
	}

	#ifdef _DEBUG
	//std::cout << "Loading image '" << name() << "'" << std::endl;
	#endif

	File imageFile = Utility<Filesystem>::get().open(name());
	if (imageFile.size() == 0)
	{
		std::cout << "Image::load(): '" << name() << "' is empty." << std::endl;
		return;
	}

	SDL_Surface* surface = IMG_Load_RW(SDL_RWFromConstMem(imageFile.raw_bytes(), static_cast<int>(imageFile.size())), 0);
	if (!surface)
	{
		std::cout << "Image::load(): " << SDL_GetError() << std::endl;
		return;
	}

	mSize = Vector{surface->w, surface->h};

	unsigned int textureId = generateTexture(surface);

	// Add generated texture id to texture ID map.
	auto& imageInfo = imageIdMap[name()];
	imageInfo.surface = surface;
	imageInfo.textureId = textureId;
	imageInfo.size = mSize;
	imageInfo.refCount++;

	loaded(true);
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
	return pixelColor(point.x, point.y);
}


/**
 * Gets the color of a pixel at a given coordinate.
 *
 * \param	x	X-Coordinate of the pixel to check.
 * \param	y	Y-Coordinate of the pixel to check.
 */
Color Image::pixelColor(int x, int y) const
{
	if (x < 0 || x >= mSize.x || y < 0 || y >= mSize.y)
	{
		return Color::Black;
	}

	SDL_Surface* surface = imageIdMap[name()].surface;

	if (!surface) { throw image_null_data(); }

	SDL_LockSurface(surface);
	uint8_t bytesPerPixel = surface->format->BytesPerPixel;
	auto pixelPtr = reinterpret_cast<std::uintptr_t>(surface->pixels) + static_cast<std::size_t>(y) * static_cast<std::size_t>(surface->pitch) + static_cast<std::size_t>(x) * bytesPerPixel;

	unsigned int pixelBytes = 0;

	switch (bytesPerPixel)
	{
	case 1:
	{
		auto p = reinterpret_cast<uint8_t*>(pixelPtr);
		pixelBytes = *p;
		break;
	}
	case 2:
	{
		auto p = reinterpret_cast<uint16_t*>(pixelPtr);
		pixelBytes = *p;
		break;
	}
	case 3:
	{
		auto p = reinterpret_cast<uint8_t*>(pixelPtr);
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
		auto p = reinterpret_cast<uint32_t*>(pixelPtr);
		pixelBytes = *p;
		break;
	}
	default: // Should never be possible.
		throw image_bad_data();
	}

	uint8_t r, g, b, a;
	SDL_GetRGBA(pixelBytes, surface->format, &r, &g, &b, &a);
	SDL_UnlockSurface(surface);

	return Color{r, g, b, a};
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
			if (imageInfo.textureId == 0)
			{
				return;
			}
			glDeleteTextures(1, &imageInfo.textureId);

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
	* Checks to see if a texture has already been generated
	* and if it has, increases the reference count.
	*
	* \return	True if texture already exists. False otherwise.
	*/
	bool checkTextureId(const std::string& name)
	{
		auto it = imageIdMap.find(name);

		if (it != imageIdMap.end())
		{
			++imageIdMap[name].refCount;
			return true;
		}

		return false;
	}
}


/**
 * Generates a new OpenGL texture from an SDL_Surface.
 */
unsigned int generateTexture(SDL_Surface* surface)
{
	return generateTexture(surface->pixels, surface->format->BytesPerPixel, surface->w, surface->h);
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
