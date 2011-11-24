// ==================================================================================
// = NAS2D
// = Copyright © 2008 New Age Software
// ==================================================================================
// = NAS2D is distributed under the terms of the zlib license. You are free to copy,
// = modify and distribute the software as long under the terms of the zlib license.
// = 
// = Acknowledgement of your use of NAS2D is appriciated but is not required.
// ==================================================================================

#include "NAS2D/Common.h"
#include "NAS2D/Filesystem.h"
#include "NAS2D/Singleton.h"

#include "NAS2D/Renderer/Primitives.h"

#include "NAS2D/Resources/Image.h"
#include "NAS2D/Resources/errorImage.h"


using namespace std;

/**
 * \brief Loads an Image from disk.
 *
 * \param filePath Path to an image file.
 *
 * If the load fails, a default image is stored indicating an error condition.
 */
Image::Image(const string& filePath):	Resource(filePath),
										mPixels(NULL),
										mTextureId(0)
{
	load();
}


/**
 * \brief Loads a portion of an existing Image object.
 *
 * \param src		Pointer to an existing Image object.
 * \param x			'X' Coordinate to grab from source Image object.
 * \param y			'Y' Coordinate to grab from source Image object.
 * \param width		Width of the rectangular area to grab from source Image object.
 * \param height	Height of the rectangular area to grab from source Image object.
 *
 * If the load fails, a default image is stored indicating an error condition.
 */
Image::Image(Image *src, int x, int y, int width, int height) :	Resource(src->name()),
																mPixels(NULL),
																mTextureId(0)
{
	loadFromSource(src, x, y, width, height);
}


/**
 * Creates an image from a byte array.
 *
 * \param	buffer	A pointer to a byte array.
 * \param	size	Size of the buffer.
 *
 * \note	The buffer array must contain the complete binary data for
 *			an image file supported by SDL_Image.
 */
Image::Image(void *buffer, int size):	Resource("Default Image"),
										mPixels(NULL),
										mTextureId(0)
{

	createSurface((char*)buffer, size);
}


/**
 * Default Constructor.
 *
 * This Constructor should almost never be invoked. It initializes the Image
 * Resource to a valid state with valid pixel data. Generally this method of
 * instantiating an Image should be for testing purposes or to indicate an
 * error condition.		
 */
Image::Image() :	mPixels(NULL),
					mTextureId(0)
{
	loadDefault();
}


/**
 * Copy C'tor.
 * 
 * \param	src		A reference to an Image Resource.
 */
Image::Image(const Image &src):	Resource(src.name()),
								mPixels(NULL),
								mTextureId(src.mTextureId)
{
	// Create a new SDL_Surface from the source Image surface.
	mPixels = SDL_CreateRGBSurface(src.mPixels->flags, src.mPixels->w, src.mPixels->h, src.mPixels->format->BitsPerPixel, src.mPixels->format->Rmask, src.mPixels->format->Gmask, src.mPixels->format->Bmask, src.mPixels->format->Amask);

	// Check that mPixels was properly created. If not, load the default image.
	if(!mPixels)
	{
		loadDefault();
	}
	else
	{
		// Turn off source alpha blending so that the alpha data is copied properly.
		SDL_SetAlpha(src.mPixels, 0, src.mPixels->format->Amask); 
		SDL_BlitSurface(src.mPixels, NULL, mPixels, NULL);
		SDL_SetAlpha(src.mPixels, SDL_SRCALPHA, src.mPixels->format->Amask);

		loaded(true);
		errorMessage("");
		mRect = Rectangle_2d(0, 0, mPixels->w, mPixels->h);
	}
}


/**
 * Copy operator.
 * 
 * \param	rhs		Reference to an Image Resource.
 */
Image& Image::operator=(const Image& rhs)
{
	// Free any allocated memory.
	if(mPixels)
		SDL_FreeSurface(mPixels);
	
	// Create a new SDL_Surface from the source Image surface.
	mPixels = SDL_CreateRGBSurface(rhs.mPixels->flags, rhs.mPixels->w, rhs.mPixels->h, rhs.mPixels->format->BitsPerPixel, rhs.mPixels->format->Rmask, rhs.mPixels->format->Gmask, rhs.mPixels->format->Bmask, rhs.mPixels->format->Amask);

	// Check that mPixels was properly created. If not, load the default image.
	if(!mPixels)
	{
		loadDefault();
	}
	else
	{
		// Turn off source alpha blending so that the alpha data is copied properly.
		SDL_SetAlpha(rhs.mPixels, 0, rhs.mPixels->format->Amask); 
		SDL_BlitSurface(rhs.mPixels, NULL, mPixels, NULL);
		SDL_SetAlpha(rhs.mPixels, SDL_SRCALPHA, rhs.mPixels->format->Amask);

		loaded(true);
		errorMessage("");
		mRect = Rectangle_2d(0, 0, mPixels->w, mPixels->h);
	}

	mTextureId = rhs.mTextureId;

	return *this;
}


/**
 * Internal function.
 */
void Image::loadDefault()
{
	if(mPixels)
		delete mPixels;

	mPixels = IMG_Load_RW(SDL_RWFromMem(errorImg, ERRORIMG_LEN), 1);

	if(!mPixels)
	{
		errorMessage(SDL_GetError());
		mPixels = NULL;
		mRect = Rectangle_2d(0, 0, 0, 0);
		loaded(false);

		return;
	}

	mRect = Rectangle_2d(0, 0, mPixels->w, mPixels->h);
	loaded(true);
	errorMessage("");
}

/**
 * Loads an image file from disk.
 * 
 * \note	If loading fails, Image will be set to a valid internal
 *			state with a default image indicating an error.
 */
void Image::load()
{
	File imageFile = Singleton<Filesystem>::get().open(name());
	if(imageFile.size() == 0)
	{
		errorMessage(Singleton<Filesystem>::get().lastError());
		loadDefault();
		return;
	}

	createSurface(imageFile.raw_bytes(), imageFile.size());
}


/**
 * Loads a portion of an image from an existing Image object.
 *
 * \note	If loading fails, Image will be set to a valid internal
 *			state with a default image indicating an error.
 */
void Image::loadFromSource(Image *src, int x, int y, int width, int height)
{
	if(width == 0 || height == 0)
	{
		loadDefault();
		return;
	}

	// Make sure our source Image has actually loaded itself.
	if(!src->loaded())
	{
		cout << "Image resource used a source Image that hasn't properly loaded: " << src->errorMessage() << endl;
		loadDefault();
		return;
	}

	// Free existing pixel data and create a new surface of width/height.
	if(mPixels)
		SDL_FreeSurface(mPixels);
	
	mPixels = SDL_CreateRGBSurface(src->pixels()->flags, width, height, src->pixels()->format->BitsPerPixel, src->pixels()->format->Rmask, src->pixels()->format->Gmask, src->pixels()->format->Bmask, src->pixels()->format->Amask);
	if(!mPixels)
	{
		cout << "Failed to create Image from source Image '" << src->name() << "': " << SDL_GetError() << endl;
		loadDefault();
		return;
	}

	// Turn off source alpha blending so that the alpha data is copied properly.
	SDL_SetAlpha(src->pixels(), 0, src->pixels()->format->Amask); 

	SDL_Rect grabRect = {x, y, width, height};
	SDL_BlitSurface(src->pixels(), &grabRect, mPixels, NULL);
	SDL_SetAlpha(src->pixels(), SDL_SRCALPHA, src->pixels()->format->Amask); 
	
	mRect = Rectangle_2d(0, 0, mPixels->w, mPixels->h);
	
	loaded(true);
	errorMessage("");
}


/**
 * Converts the pixel formats internally to prevent unnecessary on-the-fly conversions.
 *
 * \note	If creation fails, Image will be set to a valid internal
 *			state with a default image indicating an error.
 */
void Image::createSurface(const char* data, int dataLength)
{
	// Create a temporary surface and make sure that all loading was smooth.
	SDL_Surface *tmpSurface = IMG_Load_RW(SDL_RWFromConstMem(data, dataLength), 0);
	if(!tmpSurface)
	{		
		// Get the error message, set mPixels with a visual error image and return.
		errorMessage(SDL_GetError());
		loadDefault();
		return;
	}

	if(mPixels)
		SDL_FreeSurface(mPixels);

	// Convert the surface to the display format so that the conversion doesn't have
	// to be done on the fly by the Renderer.
	mPixels = SDL_DisplayFormatAlpha(tmpSurface);
	SDL_FreeSurface(tmpSurface);
	if(!mPixels)
	{		
		// Get the error message, set mPixels with a visual error image and return.
		errorMessage(SDL_GetError());
		loadDefault();
		return;
	}

	mRect = Rectangle_2d(0, 0, mPixels->w, mPixels->h);

	loaded(true);
	errorMessage("");
}


/**
 * Returns the pixel data used in the Image Resource.
 */
SDL_Surface *Image::pixels()
{
	return mPixels;
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
 * Gets a reference to a const Rectangle_2d.

const Rectangle_2d& Image::rect() const
{
	return mRect;
}
*/
