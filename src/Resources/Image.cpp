// ==================================================================================
// = NAS2D
// = Copyright © 2008 - 2017 New Age Software
// ==================================================================================
// = NAS2D is distributed under the terms of the zlib license. You are free to copy,
// = modify and distribute the software under the terms of the zlib license.
// = 
// = Acknowledgement of your use of NAS2D is appriciated but is not required.
// ==================================================================================

#include "NAS2D/Common.h"
#include "NAS2D/Exception.h"
#include "NAS2D/Filesystem.h"
#include "NAS2D/Utility.h"

#include "NAS2D/Renderer/Primitives.h"

#include "NAS2D/Resources/Image.h"
#include "NAS2D/Resources/errorImage.h"

#ifdef __APPLE__
#include "SDL2_image/SDL_image.h"
#elif __linux__
#include "SDL2/SDL_image.h"
#elif _WIN32
#include "GL/glew.h"
#define NO_SDL_GLEXT
#include "SDL_image.h"
#endif

#include <iostream>
#include <string>

using namespace NAS2D;

const std::string DEFAULT_IMAGE_NAME	= "Default Image";
const std::string ARBITRARY_IMAGE_NAME	= "Arbitrary Image ";


Image::TextureIdMap Image::_IdMap;
int Image::_Arbitrary = 0;



/**
 * Loads an Image from disk.
 *
 * \param filePath Path to an image file.
 *
 * If the load fails, a default image is stored indicating an error condition.
 */
Image::Image(const std::string& filePath):	Resource(filePath),
											mTextureId(0)
{
	load();
}


/**
 * Default C'tor.
 *
 * This Constructor should almost never be invoked. It initializes the Image
 * Resource to a valid state with valid pixel data. Generally this method of
 * instantiating an Image should be for testing purposes or to indicate an
 * error condition.		
 */
Image::Image():	Resource(DEFAULT_IMAGE_NAME),
				mTextureId(0)
{}


/**
 * Create a blank Image of X, Y dimensions.
 * 
 * \param	width	Width of the Image.
 * \param	height	Height of the Image.
 */
Image::Image(int width, int height):	Resource(ARBITRARY_IMAGE_NAME),
										mTextureId(0)
{
	name(string_format("%s%i", ARBITRARY_IMAGE_NAME, ++_Arbitrary));
	mRect(0, 0, width, height);

	// MAGIC NUMBER: 4 == 4 1-byte color channels (RGBA)
	unsigned char* buffer = new unsigned char[4 * (sizeof(unsigned char) * (width * height))] ();
	generateTexture(buffer, 4, width, height);

	// Update resource management.
	Image::_IdMap[name()] = ImageInfo(texture_id(), 0, mRect.w(), mRect.h());
	Image::_IdMap[name()].ref_count++;

	delete [] buffer;
}


/**
 * Create an Image from a raw data buffer.
 * 
 * \param	buffer			Pointer to a data buffer.
 * \param	bytesPerPixel	Number of bytes per pixel. Valid values are 3 and 4 (images < 24-bit are not supported).
 * \param	width			Width of the Image.
 * \param	height			Height of the Image.
 */
Image::Image(void* buffer, int bytesPerPixel, int width, int height):	Resource(ARBITRARY_IMAGE_NAME),
																		mTextureId(0)
{
	if (buffer == nullptr)
		throw image_null_data();

	if (bytesPerPixel != 3 && bytesPerPixel != 4)
		throw image_unsupported_bit_depth();

	// Create an SDL_Surface so that this Image has something to look at if the user
	// wants to be peeking pixels out of it.
	SDL_Surface* pixels = SDL_CreateRGBSurfaceFrom(buffer, width, height, bytesPerPixel * 4, 0, 0, 0, 0, SDL_BYTEORDER == SDL_BIG_ENDIAN ? 0x000000FF : 0xFF000000);

	name(string_format("%s%i", ARBITRARY_IMAGE_NAME, ++_Arbitrary));

	mRect(0, 0, width, height);
	generateTexture(buffer, bytesPerPixel, width, height);

	// Update resource management.
	Image::_IdMap[name()] = ImageInfo(texture_id(), 0, mRect.w(), mRect.h());
	Image::_IdMap[name()].ref_count++;
	Image::_IdMap[name()].pixels = pixels;
}


/**
 * Copy C'tor.
 * 
 * \param	src		A reference to an Image Resource.
 */
Image::Image(const Image &src):	Resource(src.name()),
								mRect(src.mRect),
								mTextureId(src.mTextureId)
{
	loaded(src.loaded());
	Image::_IdMap[name()].ref_count++;
}


/**
 * D'tor
 */
Image::~Image()
{
	// Is this check necessary?
	TextureIdMap::iterator it = Image::_IdMap.find(name());
	if(it == Image::_IdMap.end())
		return;

	it->second.ref_count--;
	
	// if texture id reference count is 0, delete the texture.
	if(it->second.ref_count < 1)
	{
		if(mTextureId == 0)
			return;

		//cout << "Deleting Texture '" << name() << "' (" << it->second.textureId << ")" << endl;

		glDeleteTextures(1, &mTextureId);

		if(it->second.fboId != 0)
		{
            unsigned int fbo = it->second.fboId;
            glDeleteBuffers(1, &fbo);
		}

		if(Image::_IdMap[name()].pixels != nullptr)
		{
			SDL_FreeSurface(static_cast<SDL_Surface*>(Image::_IdMap[name()].pixels));
			Image::_IdMap[name()].pixels = nullptr;
		}

		Image::_IdMap.erase(it);
	}
}


/**
 * Copy operator.
 * 
 * \param	rhs		Reference to an Image Resource.
 */
Image& Image::operator=(const Image& rhs)
{
	name(rhs.name());
	mRect = rhs.rect();
	mTextureId = rhs.mTextureId;
	loaded(rhs.loaded());
	Image::_IdMap[name()].ref_count++;

	return *this;
}


/**
 * Checks to see if a texture has already been generated
 * and if it has, increases the reference count.
 * 
 * \note	This is an internal only function.
 * 
 * \return	True if texture already exists. False otherwise.
 */
bool Image::checkTextureId()
{
	// Check texture id map to see if this has already been loaded
	TextureIdMap::iterator it = Image::_IdMap.find(name());
	
	// if loaded, set texture ID to what's in the map and exit function.
	if(it != Image::_IdMap.end())
	{
		mTextureId = it->second.textureId;
		mRect(0, 0, it->second.w, it->second.h);
		Image::_IdMap[name()].ref_count++;

		loaded(true);
		return true;
	}

	// Texture ID was not found, texture needs to be generated.
	return false;
}


/**
 * Loads an image file from disk.
 * 
 * \note	If loading fails, Image will be set to a valid internal
 *			state with a default image indicating an error.
 */
void Image::load()
{
	// Check if texture id was generated and back out if it was.
	if(checkTextureId())
		return;

	File imageFile = Utility<Filesystem>::get().open(name());
	if(imageFile.size() == 0)
	{
		std::cout << "Image::load(): '" << name() << "' is empty." << std::endl;
		return;
	}

	SDL_Surface* pixels = IMG_Load_RW(SDL_RWFromConstMem(imageFile.raw_bytes(), static_cast<int>(imageFile.size())), 0);
	if(!pixels)
	{		
		std::cout << "Image::load(): " << SDL_GetError() << std::endl;
		return;
	}

	mRect = Rectangle_2d(0, 0, pixels->w, pixels->h);

	generateTexture(pixels->pixels, pixels->format->BytesPerPixel, pixels->w, pixels->h);

	// Add generated texture id to texture ID map.
	Image::_IdMap[name()] = ImageInfo(texture_id(), 0, mRect.w(), mRect.h());
	Image::_IdMap[name()].ref_count++;
	Image::_IdMap[name()].pixels = pixels;

	loaded(true);
}


/**
 * Generates a new OpenGL texture from an SDL_Surface.
 * 
 * \param	src	Pointer to an SDL_Surface.
 * 
 * \note	This function assumes that the image is unique and
 *			has not been loaded. Does no resource management.
 */
void Image::generateTexture(void *buffer, int bytesPerPixel, int width, int height)
{
	GLenum textureFormat = 0;
	switch(bytesPerPixel)
	{
		case 4:
			SDL_BYTEORDER == SDL_BIG_ENDIAN ? textureFormat = GL_BGRA : textureFormat = GL_RGBA;
			break;
		case 3:
			SDL_BYTEORDER == SDL_BIG_ENDIAN ? textureFormat = GL_BGR : textureFormat = GL_RGB;
			break;

		default:
			throw image_unsupported_bit_depth();
			break;
	}

	glGenTextures(1, &mTextureId);
	glBindTexture(GL_TEXTURE_2D, mTextureId);

	// Set texture and pixel handling states.
	glPixelStorei(GL_UNPACK_ALIGNMENT, 1);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
	glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
	glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);

	glTexImage2D(GL_TEXTURE_2D, 0, textureFormat, width, height, 0, textureFormat, GL_UNSIGNED_BYTE, buffer);
}


/**
 * Gets the width in pixels of the image.
 */
int Image::width() const
{
	return mRect.w();
}


/**
 * Gets the height in pixels of the image.
 */
int Image::height() const
{
	return mRect.h();
}


/**
 * Gets a reference to a Rectangle_2d that represents the area of the Image.
 */
const Rectangle_2d& Image::rect() const
{
	return mRect;
}


unsigned int Image::texture_id() const
{
	return mTextureId;
}


unsigned int Image::fbo_id()
{
	// Create a framebuffer object
	TextureIdMap::iterator it = Image::_IdMap.find(name());

	// This texture was never generated, return a safe value.
	if(it == Image::_IdMap.end())
		return 0;

	// Check that there isn't already an FBO attached to this Image.
	if(it->second.fboId != 0)
		return it->second.fboId;

	// Image doesn't have an FBO attached to it, generate one.
	GLuint fbo = 0;
	glGenBuffers(1, &fbo);

	it->second.fboId = fbo;

	return fbo;
}


/**
 * Gets the color of a pixel at a given coordinate.
 * 
 * \param	x	X-Coordinate of the pixel to check.
 * \param	y	Y-Coordinate of the pixel to check.
 * 
 * \note	This function works by storing a local copy
 *			of the generated SDL_Surface instead of
 *			freeing it after creating an OpenGL texture.
 *			This is a brute force and memory inefficient
 *			method and will change in the future.
 */
Color_4ub Image::pixelColor(int x, int y) const
{
	if(x < 0 || x > width() || y < 0 || y > height())
		return Color_4ub(0, 0, 0, 255);

	SDL_Surface* pixels = static_cast<SDL_Surface*>(Image::_IdMap[name()].pixels);

	if (!pixels)
		throw image_null_data();

	SDL_LockSurface(pixels);
    int bpp = pixels->format->BytesPerPixel;
    Uint8 *p = (Uint8*)pixels->pixels + y * pixels->pitch + x * bpp;

	unsigned int c = 0;

	switch(bpp)
	{
		case 1:
			c = *p;
			break;

		case 2:
			c = *(Uint16 *)p;
			break;

		case 3:
			if(SDL_BYTEORDER == SDL_BIG_ENDIAN)
				c = p[0] << 16 | p[1] << 8 | p[2];
			else
				c = p[0] | p[1] << 8 | p[2] << 16;
			break;

		case 4:
			c = *(Uint32*)p;
			break;

		default:	// Should never be possible.
			throw image_bad_data();
			break;
	}

	Uint8 r, g, b, a;
	SDL_GetRGBA(c, pixels->format, &r, &g, &b, &a);
	SDL_UnlockSurface(pixels);

	return Color_4ub(r, g, b, a);
}
