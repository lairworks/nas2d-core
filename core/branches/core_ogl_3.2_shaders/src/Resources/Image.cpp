// ==================================================================================
// = NAS2D
// = Copyright © 2008 - 2012 New Age Software
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
	#include <OpenGL/OpenGL.h>
	#include <OpenGL/gl3.h>
#elif defined(WIN32)
	#include "SDL/SDL_opengl.h"
#else
	#include "SDL/SDL_opengl.h"
	#include "SDL/SDL_gfxPrimitives.h"
#endif


#include <string>
#include <sstream>

using namespace std;


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
											mTextureId(0),
											mPixels(NULL)
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
				mTextureId(0),
				mPixels(NULL)
{}


/**
 * Create a blank Image of X, Y dimensions.
 * 
 * \param	w	Width of the Image.
 * \param	h	Height of the Image.
 */
Image::Image(int w, int h):	Resource(ARBITRARY_IMAGE_NAME),
							mTextureId(0),
							mPixels(NULL)
{
	stringstream str;
	str << _Arbitrary++;

	name(ARBITRARY_IMAGE_NAME + str.str());

	glPixelStorei(GL_UNPACK_ALIGNMENT, 1);	// Does this need to be called every time
											// or can we set it once in the Renderer?

	mRect(0, 0, w, h);

	glGenTextures(1, &mTextureId);
	glActiveTexture(GL_TEXTURE0);
	glBindTexture(GL_TEXTURE_2D, mTextureId);

	unsigned char* buffer = new unsigned char[4 * (sizeof(unsigned char) * (w * h))] (); // 4 = R G B A channels

	GLenum textureFormat = 0;
	SDL_BYTEORDER == SDL_BIG_ENDIAN ? textureFormat = GL_BGRA : textureFormat = GL_RGBA;

	glTexImage2D(GL_TEXTURE_2D, 0, textureFormat, w, h, 0, textureFormat, GL_UNSIGNED_BYTE, buffer);

	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
	glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
	glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);

	// Update resource management.
	Image::_IdMap[name()] = ImageInfo(texture_id(), 0, mRect.w, mRect.h);
	Image::_IdMap[name()].ref_count++;

	delete [] buffer;
}


/**
 * Copy C'tor.
 * 
 * \param	src		A reference to an Image Resource.
 */
Image::Image(const Image &src):	Resource(src.name()),
								mRect(src.mRect),
								mTextureId(src.mTextureId),
								mPixels(src.mPixels)
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
		glDeleteTextures(1, &mTextureId);

		if(it->second.fboId != 0)
		{
			unsigned int fbo = it->second.fboId;
			glDeleteBuffers(1, &fbo);
		}

		Image::_IdMap.erase(it);

		if(mPixels)
		{
			SDL_FreeSurface(mPixels); // be sure to free memory after all references are gone.
			mPixels = NULL;
		}
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
	mPixels = rhs.mPixels;
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
		cout << "(ERROR) Image::load(): " << Utility<Filesystem>::get().lastError() << endl;
		return;
	}

	// Create a temporary surface and ensure that the image loaded properly.
	//SDL_Surface *tmpSurface = IMG_Load_RW(SDL_RWFromConstMem(imageFile.raw_bytes(), imageFile.size()), 0);
	mPixels = IMG_Load_RW(SDL_RWFromConstMem(imageFile.raw_bytes(), imageFile.size()), 0);
	//if(!tmpSurface)
	if(!mPixels)
	{		
		// loading failed, log a message, set a default image and return.
		cout << "(ERROR) Image::load(): " << SDL_GetError() << endl;
		loadDefault();
		return;
	}

	mRect = Rectangle_2d(0, 0, mPixels->w, mPixels->h);

	// Generate OpenGL Texture from SDL_Surface
	generateTexture(mPixels);

	// Add generated texture id to texture ID map.
	Image::_IdMap[name()] = ImageInfo(texture_id(), 0, mRect.w, mRect.h);
	Image::_IdMap[name()].ref_count++;

	loaded(true);
}


/**
 * Used by the default c'tor and any time a default
 * image needs to be used.
 */
void Image::loadDefault()
{
	// Check if texture id was generated and back out if it was.
	if(checkTextureId())
		return;

	//SDL_Surface* s = IMG_Load_RW(SDL_RWFromMem(errorImg, ERRORIMG_LEN), 1);

	mPixels = IMG_Load_RW(SDL_RWFromMem(errorImg, ERRORIMG_LEN), 1);
	if(!mPixels)
	{
		cout << "Unable to generate default texture." << endl;
		return;
	}

	generateTexture(mPixels);

	//SDL_FreeSurface(s);
}


/**
 * Generates a new OpenGL texture from an SDL_Surface.
 * 
 * \param	src	Pointer to an SDL_Surface.
 * 
 * \note	This function assumes that the image is unique and
 *			has not been loaded. Does no resource management.
 */
void Image::generateTexture(SDL_Surface *src)
{
	// Detect which order the pixel data is in to properly feed OGL.
	GLint nColors = src->format->BytesPerPixel;
	
	GLenum textureFormat = 0;
	switch(src->format->BytesPerPixel)
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

	glTexImage2D(GL_TEXTURE_2D, 0, textureFormat, src->w, src->h, 0, textureFormat, GL_UNSIGNED_BYTE, src->pixels);

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
	return mRect.w;
}


/**
 * Gets the height in pixels of the image.
 */
int Image::height() const
{
	return mRect.h;
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

	SDL_LockSurface(mPixels);
    int bpp = mPixels->format->BytesPerPixel;
    // Here p is the address to the pixel we want to retrieve
    Uint8 *p = (Uint8*)mPixels->pixels + y * mPixels->pitch + x * bpp;

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

	SDL_GetRGBA(c, mPixels->format, &r, &g, &b, &a);

	SDL_UnlockSurface(mPixels);

	return Color_4ub(r, g, b, a);

	//return Color_4ub(0, 0, 0, 255);
}


/**
 * Permanently desaturates the Image.
 * 
 * \note	This is currently a stub function that has no effect.
 *			Expect functionality in NAS2D 1.1.
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

