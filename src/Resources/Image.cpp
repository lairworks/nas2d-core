// ==================================================================================
// = NAS2D
// = Copyright © 2008 - 2019 New Age Software
// ==================================================================================
// = NAS2D is distributed under the terms of the zlib license. You are free to copy,
// = modify and distribute the software under the terms of the zlib license.
// =
// = Acknowledgement of your use of NAS2D is appriciated but is not required.
// ==================================================================================
#include "NAS2D/Resources/Image.h"
#include "NAS2D/Resources/ImageInfo.h"

#include "NAS2D/Exception.h"
#include "NAS2D/Filesystem.h"
#include "NAS2D/Utility.h"

#include <GL/glew.h>
#include <SDL_image.h>

#include <iostream>
#include <map>
#include <string>

using namespace NAS2D;
using namespace NAS2D::Exception;

const std::string DEFAULT_IMAGE_NAME	= "Default Image";
const std::string ARBITRARY_IMAGE_NAME	= "arbitrary_image_";


typedef std::map<std::string, ImageInfo> TextureIdMap;

TextureIdMap	IMAGE_ID_MAP;			/*< Lookup table for OpenGL Texture ID's. */
int				IMAGE_ARBITRARY = 0;	/*< Counter for arbitrary image ID's. */

// ==================================================================================
// = UNEXPOSED FUNCTION PROTOTYPES
// ==================================================================================
bool checkTextureId(const std::string& name);
unsigned int generateTexture(void *buffer, int bytesPerPixel, int width, int height);
void updateImageReferenceCount(const std::string& name);


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
	name(string_format("%s%i", ARBITRARY_IMAGE_NAME.c_str(), ++IMAGE_ARBITRARY));
	_size = std::make_pair(width, height);
	_center = std::make_pair(width / 2, height / 2);

	// Update resource management.
	IMAGE_ID_MAP[name()].texture_id = 0;
	IMAGE_ID_MAP[name()].w = width;
	IMAGE_ID_MAP[name()].h = height;
	IMAGE_ID_MAP[name()].ref_count++;
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

	name(string_format("%s%i", ARBITRARY_IMAGE_NAME.c_str(), ++IMAGE_ARBITRARY));

	SDL_Surface* pixels = SDL_CreateRGBSurfaceFrom(buffer, width, height, bytesPerPixel * 4, 0, 0, 0, 0, SDL_BYTEORDER == SDL_BIG_ENDIAN ? 0x000000FF : 0xFF000000);

	_size = std::make_pair(width, height);
	_center = std::make_pair(pixels->w / 2, pixels->h / 2);

	unsigned int texture_id = generateTexture(buffer, bytesPerPixel, width, height);

	// Update resource management.
	IMAGE_ID_MAP[name()].texture_id = texture_id;
	IMAGE_ID_MAP[name()].w = width;
	IMAGE_ID_MAP[name()].h = height;
	IMAGE_ID_MAP[name()].ref_count++;
	IMAGE_ID_MAP[name()].pixels = pixels;
}


/**
 * Copy C'tor.
 *
 * \param	src		Image to copy.
 */
Image::Image(const Image &src) : Resource(src.name()), _size(src._size)
{
	if (!src.loaded())
	{
		throw image_bad_copy();
	}

	loaded(src.loaded());
	IMAGE_ID_MAP[name()].ref_count++;
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
	_size = rhs._size;
	_center = rhs._center;

	auto it = IMAGE_ID_MAP.find(name());
	if (it == IMAGE_ID_MAP.end())
	{
		throw image_bad_data();
	}

	loaded(rhs.loaded());
	++it->second.ref_count;

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
		_size = std::make_pair(IMAGE_ID_MAP[name()].w, IMAGE_ID_MAP[name()].h);
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

	SDL_Surface* pixels = IMG_Load_RW(SDL_RWFromConstMem(imageFile.raw_bytes(), static_cast<int>(imageFile.size())), 0);
	if (!pixels)
	{
		std::cout << "Image::load(): " << SDL_GetError() << std::endl;
		return;
	}

	_size = std::make_pair(pixels->w, pixels->h);
	_center = std::make_pair(pixels->w / 2, pixels->h / 2);

	unsigned int texture_id = generateTexture(pixels->pixels, pixels->format->BytesPerPixel, pixels->w, pixels->h);

	// Add generated texture id to texture ID map.
	IMAGE_ID_MAP[name()].texture_id = texture_id;
	IMAGE_ID_MAP[name()].w = width();
	IMAGE_ID_MAP[name()].h = height();
	IMAGE_ID_MAP[name()].ref_count++;

	IMAGE_ID_MAP[name()].pixels = pixels;

	loaded(true);
}


/**
 * Gets the width in pixels of the image.
 */
int Image::width() const
{
	return _size.first;
}


/**
 * Gets the height in pixels of the image.
 */
int Image::height() const
{
	return _size.second;
}


int Image::center_x() const
{
	return _center.first;
}


int Image::center_y() const
{
	return _center.second;
}


/**
 * Gets the color of a pixel at a given coordinate.
 *
 * \param	x	X-Coordinate of the pixel to check.
 * \param	y	Y-Coordinate of the pixel to check.
 */
Color_4ub Image::pixelColor(int x, int y) const
{
	if (x < 0 || x > width() || y < 0 || y > height())
	{
		return Color_4ub(0, 0, 0, 255);
	}

	SDL_Surface* pixels = static_cast<SDL_Surface*>(IMAGE_ID_MAP[name()].pixels);

	if (!pixels) { throw image_null_data(); }

	SDL_LockSurface(pixels);
	uint8_t bpp = pixels->format->BytesPerPixel;
	uint8_t* p = (uint8_t*)pixels->pixels + static_cast<std::size_t>(y) * pixels->pitch + static_cast<std::size_t>(x) * bpp;

	unsigned int c = 0;

	switch (bpp)
	{
	case 1:
		c = *p;
		break;

	case 2:
		c = *(uint16_t*)p;
		break;

	case 3:
		if constexpr (SDL_BYTEORDER == SDL_BIG_ENDIAN)
		{
			c = p[0] << 16 | p[1] << 8 | p[2];
		}
		else
		{
			c = p[0] | p[1] << 8 | p[2] << 16;
		}
		break;

	case 4:
		c = *(uint32_t*)p;
		break;

	default:	// Should never be possible.
		throw image_bad_data();
		break;
	}

	uint8_t r, g, b, a;
	SDL_GetRGBA(c, pixels->format, &r, &g, &b, &a);
	SDL_UnlockSurface(pixels);

	return Color_4ub(r, g, b, a);
}


// ==================================================================================
// = Unexposed module-level functions defined here that don't need to be part of the
// = API interface.
// ==================================================================================

/**
* Internal function used to clean up references to fonts when the Image
* destructor or copy assignment operators are called.
*
* \param	name	Name of the Image to check against.
*/
void updateImageReferenceCount(const std::string& name)
{
	auto it = IMAGE_ID_MAP.find(name);
	if (it == IMAGE_ID_MAP.end())
	{
		return;
	}

	--it->second.ref_count;

	// if texture id reference count is 0, delete the texture.
	if (it->second.ref_count < 1)
	{
		if (it->second.texture_id == 0)
		{
			return;
		}

		glDeleteTextures(1, &it->second.texture_id);

		if (it->second.fbo_id != 0)
		{
			glDeleteFramebuffers(1, &it->second.fbo_id);
		}

		if (it->second.pixels != nullptr)
		{
			SDL_FreeSurface(static_cast<SDL_Surface*>(it->second.pixels));
			it->second.pixels = nullptr;
		}

		IMAGE_ID_MAP.erase(it);
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
	auto it = IMAGE_ID_MAP.find(name);

	if (it != IMAGE_ID_MAP.end())
	{
		++IMAGE_ID_MAP[name].ref_count;
		return true;
	}

	return false;
}


/**
 * Generates a new OpenGL texture from an SDL_Surface.
 */
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
		break;
	}

	GLuint texture_id;
	glGenTextures(1, &texture_id);
	glBindTexture(GL_TEXTURE_2D, texture_id);

	// Set texture and pixel handling states.
	glPixelStorei(GL_UNPACK_ALIGNMENT, 1);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
	glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
	glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);

	glTexImage2D(GL_TEXTURE_2D, 0, textureFormat, width, height, 0, textureFormat, GL_UNSIGNED_BYTE, buffer);

	return texture_id;
}
