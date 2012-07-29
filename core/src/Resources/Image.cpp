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
#include "NAS2D/Filesystem.h"
#include "NAS2D/Utility.h"

#include "NAS2D/Renderer/Primitives.h"

#include "NAS2D/Resources/Image.h"
#include "NAS2D/Resources/errorImage.h"


#if defined(__APPLE__)
	#include <OpenGL/OpenGL.h>
	#include "SDL_gfx/SDL_gfxPrimitives.h"
#elif defined(WIN32)
	#include "GLee.h"
	#include "SDL/SDL_opengl.h"
	#include "SDL/SDL_gfxPrimitives.h"
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
Image::ReferenceCountMap Image::_RefMap;
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
 * \param	x	Width of the Image.
 * \param	y	Height of the Image.
 */
Image::Image(int w, int h):	Resource(ARBITRARY_IMAGE_NAME),
							mTextureId(0)
{
	stringstream str;
	str << _Arbitrary++;

	name(ARBITRARY_IMAGE_NAME + str.str());

	glPixelStorei(GL_UNPACK_ALIGNMENT, 1);	// Does this need to be called every time
											// or can we set it once in the Renderer?

	mRect(0, 0, w, h);

	glGenTextures(1, &mTextureId);
	glBindTexture(GL_TEXTURE_2D, mTextureId);

	unsigned char* buffer = new unsigned char[4 * (sizeof(unsigned char) * (w * h))] (); // 3 = R G B channels

	GLenum textureFormat = 0;
	SDL_BYTEORDER == SDL_BIG_ENDIAN ? textureFormat = GL_BGRA : textureFormat = GL_RGBA;

	glTexImage2D(GL_TEXTURE_2D, 0, 3, w, h, 0, textureFormat, GL_UNSIGNED_BYTE, buffer);

	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
	glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
	glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);

	delete [] buffer;
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
	Image::_RefMap[mTextureId] ++;
}


/**
 * D'tor
 */
Image::~Image()
{

	// decrement texture id reference count.
	Image::_RefMap[mTextureId] --;

	// if texture id reference count is 0, delete the texture.
	if(Image::_RefMap[mTextureId] < 1)
	{
		glDeleteTextures(1, &mTextureId);
		Image::_RefMap.erase(Image::_RefMap.find(mTextureId));

		TextureIdMap::iterator it = Image::_IdMap.find(name());

		if(it != Image::_IdMap.end())
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
	Image::_RefMap[mTextureId] ++;

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
		Image::_RefMap[mTextureId] ++;
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
		cout << Utility<Filesystem>::get().lastError();
		return;
	}

	// Create a temporary surface and ensure that the image loaded properly.
	SDL_Surface *tmpSurface = IMG_Load_RW(SDL_RWFromConstMem(imageFile.raw_bytes(), imageFile.size()), 0);
	if(!tmpSurface)
	{		
		// loading failed, log a message, set a default image and return.
		errorMessage(SDL_GetError());
		loadDefault();
		return;
	}

	mRect = Rectangle_2d(0, 0, tmpSurface->w, tmpSurface->h);

	// Generate OpenGL Texture from SDL_Surface
	generateTexture(tmpSurface);

	// Add generated texture id to texture ID map.
	Image::_IdMap[name()] = ImageInfo(texture_id(), mRect.w, mRect.h);
	// Increment texture id reference count.
	Image::_RefMap[texture_id()] ++;

	// Free SDL_Surface as we no longer need it.
	SDL_FreeSurface(tmpSurface);


	loaded(true);
	errorMessage("");
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

	SDL_Surface* s = IMG_Load_RW(SDL_RWFromMem(errorImg, ERRORIMG_LEN), 1);

	if(!s)
	{
		cout << "Unable to generate default texture." << endl;
		return;
	}

	generateTexture(s);

	SDL_FreeSurface(s);
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
	
	/**
	 * \todo	When compiling in nightmare mode with Visual Studio, it indicated that \c textureFormat could be potentially
	 *			uninitialized should nColors be anything other than '3' or '4' (24 bit and 32 bit color modes, respectively).
	 *			The only other expected values would be '2' (16 bit color mode) or '1' (8 bit color mode). While in practice
	 *			neither of these cases is likely to show up, it's extremely important that textureFormat is initialized to a
	 *			good default value that will work for generally all modes or one that will at least not cause a crash.
	 */
	GLenum textureFormat = 0;
	if(nColors == 4)
	{
		if(src->format->Rmask == 0x000000ff)
			textureFormat = GL_RGBA;
		else
			textureFormat = GL_BGRA;
	}
	else if(nColors == 3)     // no alpha channel
	{
		if(src->format->Rmask == 0x000000ff)
			textureFormat = GL_RGB;
		else
			textureFormat = GL_BGR;
	}
	else
	{
		cout << "Image must be 16-, 24- or 32-bit." << std::endl;
		return;
	}

	glPixelStorei(GL_UNPACK_ALIGNMENT, 1);	// Does this need to be called every time
											// or can we set it once in the Renderer?

	glGenTextures(1, &mTextureId);
	glBindTexture(GL_TEXTURE_2D, mTextureId);

	glTexImage2D(GL_TEXTURE_2D, 0, nColors, src->w, src->h, 0, textureFormat, GL_UNSIGNED_BYTE, src->pixels);

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


/**
 * Gets the color of a pixel at a given coordinate.
 * 
 * \param	x	X-Coordinate of the pixel to check.
 * \param	y	Y-Coordinate of the pixel to check.
 * 
 * \note	This function is generally slow. Avoid overuse
 *			as there will be performance penalties.
 */
Color_4ub Image::pixelColor(int x, int y) const
{
	//return pixelColor(x, y, mPixels);
	return Color_4ub();
}


Color_4ub Image::pixelColor(int x, int y, SDL_Surface* src) const
{
	/*
	SDL_LockSurface(src);
    int bpp = src->format->BytesPerPixel;
    // Here p is the address to the pixel we want to retrieve
    Uint8 *p = (Uint8*)src->pixels + y * src->pitch + x * bpp;

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

	SDL_GetRGBA(c, src->format, &r, &g, &b, &a);

	SDL_UnlockSurface(src);

	return Color_4ub(r, g, b, a);
	*/

	return Color_4ub(0, 0, 0, 255);
}


/**
 * Permanently desaturates the Image.
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

