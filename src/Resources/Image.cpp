// ==================================================================================
// = NAS2D
// = Copyright © 2008 - 2014 New Age Software
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


#if defined(__APPLE__)
	#include <SDL2/SDL_opengl.h>
#elif defined(__linux__)
    #include <SDL2/SDL_opengl.h>
#elif defined(WINDOWS)
	#include "SDL_opengl.h"
#else
	#include "SDL/SDL_opengl.h"
	#include "SDL/SDL_gfxPrimitives.h"
#endif


#include <string>
#include <sstream>

using namespace std;
using namespace NAS2D;

const string DEFAULT_IMAGE_NAME		= "Default Image";
const string ARBITRARY_IMAGE_NAME	= "Arbitrary Image ";


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
	stringstream str;
	str << _Arbitrary++;

	name(ARBITRARY_IMAGE_NAME + str.str());
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
	if(buffer == nullptr)
		throw Exception(0, "Bad Data", "Attempted to construct an Image with a NULL pixel buffer.");

	if(bytesPerPixel != 3 && bytesPerPixel != 4)
	{
		#if defined(_DEBUG)
		cout << "(EXCEPTION) Image::Image(void*, int, int, int): " << "bytesPerPixel: " << bytesPerPixel << " W: " << width << " H: " << height << endl;
		#endif

		throw Exception(0, "Unsupported Image", "Attempted to construct an Image less than 24-bit. Images must be 24- or 32-bit.");
	}

	// Create an SDL_Surface so that this Image has something to look at if the user
	// wants to be peeking pixels out of it.
	// 
	// I believe this to be correct but I have to test it.
	SDL_Surface* pixels = SDL_CreateRGBSurfaceFrom(buffer, width, height, bytesPerPixel * 4, 0, 0, 0, 0, SDL_BYTEORDER == SDL_BIG_ENDIAN ? 0x000000FF : 0xFF000000);

	stringstream str;
	str << _Arbitrary++;

	name(ARBITRARY_IMAGE_NAME + str.str());

	mRect(0, 0, width, height);
	generateTexture(buffer, bytesPerPixel, width, height);

	// Update resource management.
	Image::_IdMap[name()] = ImageInfo(texture_id(), 0, mRect.w(), mRect.h());
	Image::_IdMap[name()].ref_count++;
	Image::_IdMap[name()].pixels_raw = pixels;
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

		if(Image::_IdMap[name()].pixels_raw != nullptr)
		{
			SDL_FreeSurface(Image::_IdMap[name()].pixels_raw);
			Image::_IdMap[name()].pixels_raw = nullptr;
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

	// File has not been previously loaded, so open it.
	File imageFile = Utility<Filesystem>::get().open(name());
	if(imageFile.size() == 0)
	{
		//cout << "(ERROR) Image::load(): " << Utility<Filesystem>::get().lastError() << endl;
		return;
	}

	// Create a temporary surface and ensure that the image loaded properly.
	//SDL_Surface *tmpSurface = IMG_Load_RW(SDL_RWFromConstMem(imageFile.raw_bytes(), imageFile.size()), 0);
	SDL_Surface* pixels = IMG_Load_RW(SDL_RWFromConstMem(imageFile.raw_bytes(), imageFile.size()), 0);
	//if(!tmpSurface)
	if(!pixels)
	{		
		// loading failed, log a message, set a default image and return.
		cout << "(ERROR) Image::load(): " << SDL_GetError() << endl;
		loadDefault();
		return;
	}

	mRect = Rectangle_2d(0, 0, pixels->w, pixels->h);

	// Generate OpenGL Texture from SDL_Surface
	generateTexture(pixels->pixels, pixels->format->BytesPerPixel, pixels->w, pixels->h);

	// Add generated texture id to texture ID map.
	Image::_IdMap[name()] = ImageInfo(texture_id(), 0, mRect.w(), mRect.h());
	Image::_IdMap[name()].ref_count++;
	Image::_IdMap[name()].pixels_raw = pixels;

	loaded(true);
}


/**
 * Used by the default c'tor and any time a default
 * image needs to be used.
 */
void Image::loadDefault()
{
	// Check if texture id was generated and back out if it was.
	/*
	if(checkTextureId())
		return;

	mPixels = IMG_Load_RW(SDL_RWFromMem(errorImg, ERRORIMG_LEN), 1);
	if(!mPixels)
	{
		cout << "Unable to generate default texture." << endl;
		return;
	}

	generateTexture(mPixels->pixels, mPixels->format->BytesPerPixel, mPixels->w, mPixels->h);
	*/
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
			cout << "EXCEPTION: Only 24- and 32-bit images are supported." << std::endl;
			throw Exception(0, "Unsupported Bit Depth", "Only 24- and 32-bit images are supported.");
			break;
	}

	glPixelStorei(GL_UNPACK_ALIGNMENT, 1);	// Does this need to be called every time
											// or can we set it once in the Renderer?

	glGenTextures(1, &mTextureId);
	glBindTexture(GL_TEXTURE_2D, mTextureId);

	glTexImage2D(GL_TEXTURE_2D, 0, textureFormat, width, height, 0, textureFormat, GL_UNSIGNED_BYTE, buffer);

	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
	glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
	glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
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
	// Ensure we're only ever reading within the bounds of the pixel data.
	if(x < 0 || x > width() || y < 0 || y > height())
		return Color_4ub(0, 0, 0, 255);

	SDL_Surface* pixels = Image::_IdMap[name()].pixels_raw;

	if(!pixels)
		throw Exception(0, "NULL Surface", "Image::pixelColor() called on an Image with no pixel data.");

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

		default:
			break;       // shouldn't happen, but avoids warnings
	}

	Uint8 r, g, b, a;

	SDL_GetRGBA(c, pixels->format, &r, &g, &b, &a);

	SDL_UnlockSurface(pixels);

	return Color_4ub(r, g, b, a);
}


/**
 * Permanently desaturates the Image.
 * 
 * \note	This is currently a stub function that has no effect.
 *			Expect functionality in NAS2D 1.2.
 */
void Image::desaturate()
{
	/*
	Color_4ub color;
	for(int y = 0; y < mPixels->h; y++)
	{
		for(int x = 0; x < mPixels->w; x++)
		{
			color = pixelColor(x, y);
			unsigned char grey = static_cast<unsigned char>((color.red() + color.green() + color.blue()) / 3);
			pixelRGBA(mPixels, x, y, grey, grey, grey, color.alpha());
		}
	}
	*/
}

