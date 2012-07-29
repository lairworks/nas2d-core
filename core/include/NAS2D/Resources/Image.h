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
#include <map>

class Rectangle_2d;

/**
 * \class Image
 * \brief Image Class
 *
 * Stores an image as a representation of pixels in memory.
 */
class Image: public Resource
{
public:
	Image(const std::string& filePath);
	Image(int x, int y);
	Image();

	Image(const Image &src);
	Image& operator=(const Image& rhs);

	~Image();

	int width() const;
	int height() const;

	const Rectangle_2d& rect() const;

	Color_4ub pixelColor(int x, int y) const;

	void desaturate();

protected:
	friend class Renderer;
	friend class OGL_Renderer;

	unsigned int texture_id() const;

private:

	struct ImageInfo
	{
		ImageInfo(): textureId(0), w(0), h(0) {}
		ImageInfo(unsigned int id, int w, int h): textureId(id), w(w), h(h) {}
		
		void operator()(unsigned int id, int w, int h) { textureId = id; w = w; h = h; }

		unsigned int textureId;
		int w;
		int h;
	};

	typedef std::map<std::string, ImageInfo> TextureIdMap;
	typedef std::map<unsigned int, int> ReferenceCountMap;

	void load();
	void loadDefault();
	void generateTexture(SDL_Surface *src);

	bool checkTextureId();

	Color_4ub pixelColor(int x, int y, SDL_Surface* src) const; // internal version

	Rectangle_2d	mRect;			/**< Used to store width/height information about the image. */

	unsigned int	mTextureId;		/**< OpenGL Texture ID. */


	static TextureIdMap			_IdMap;		/*< Lookup table for OpenGL Texture ID's. */
	static ReferenceCountMap	_RefMap;	/*< Lookup table for OpenGL Texture ID reference counts. */

	static int					_Arbitrary;	/*< Counter for arbitrary image ID's. */
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
