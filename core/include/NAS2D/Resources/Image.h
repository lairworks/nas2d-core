// ==================================================================================
// = NAS2D
// = Copyright © 2008 - 2012 New Age Software
// ==================================================================================
// = NAS2D is distributed under the terms of the zlib license. You are free to copy,
// = modify and distribute the software under the terms of the zlib license.
// = 
// = Acknowledgement of your use of NAS2D is appriciated but is not required.
// ==================================================================================

#ifndef _NAS_IMAGERESOURCE_
#define _NAS_IMAGERESOURCE_

#include "Resource.h"
#include "SDL/SDL.h"

#ifdef __APPLE__
#include "SDL_image/SDL_image.h"
#else
#include "SDL/SDL_image.h"
#endif

#include <vector>

class Rectangle_2d;

/**
 * \class Image
 * \brief Image Class
 *
 * \param char Path to an image file.
 *
 * Stores an image as a representation of pixels in memory.
 */
class Image : public Resource
{
public:
	Image(const std::string& filePath);
	Image(Image *src, int x, int y, int width, int height);
	Image(void *buffer, int size);
	Image(const Image &src);
	Image(int x, int y);
	Image();

	Image& operator=(const Image& rhs);

	/// Frees pixel data and cleans up after itself.
	~Image() { SDL_FreeSurface(mPixels); }

	int width() const;
	int height() const;

	unsigned int texutreId() { return mTextureId; }

protected:
	friend class Renderer;
	friend class SDL_Renderer;
	friend class OGL_Renderer;

	SDL_Surface *pixels();

	// Used for OpenGL modes -- pollutes the interface but is almost a necessary evil.
	void textureId(unsigned int id) { mTextureId = id; }

private:
	void load();
	void loadDefault();
	void loadFromSource(Image *src, int x, int y, int width, int height);

	void createSurface(const char *data, int dataLength);

	/** \todo	This may be better off as a straight-up char* buffer which can be converted by the renderers
	 *			as necessary. This will likely require a modification to the SDL Renderer that stores 'SDL_Surfaces'
	 *			in a similar manner to the way the OpenGL Renderer stores references to OGL Textures.
	 */
	SDL_Surface		*mPixels;	/**< SDL_Surface containing the Pixel Data. */

	Rectangle_2d	mRect;		/**< Used to store width/height information about the image. */

	unsigned int	mTextureId;	/**< Internal TextureID when in OpenGL mode. */
};


/**
 * \typedef	ImageListPtr
 * \brief	A vector if pointers to Image.
 */
typedef std::vector<Image*> ImageListPtr;


/**
 * \typedef	ImageList
 * \brief	A list of Image objects.
 */
typedef std::vector<Image> ImageList;

#endif
