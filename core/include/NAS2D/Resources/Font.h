// ==================================================================================
// = NAS2D
// = Copyright © 2008 - 2012 New Age Software
// ==================================================================================
// = NAS2D is distributed under the terms of the zlib license. You are free to copy,
// = modify and distribute the software under the terms of the zlib license.
// = 
// = Acknowledgement of your use of NAS2D is appriciated but is not required.
// ==================================================================================

#ifndef _NAS_FONTRESOURCE_
#define _NAS_FONTRESOURCE_

#include "Resource.h"

#ifdef __APPLE__
#include "SDL/SDL_opengl.h"
#include "SDL_ttf/SDL_ttf.h"
#include <ft2build.h>
#include <FreeType/freetype.h>
#include <FreeType/ftglyph.h>
#include <FreeType/ftoutln.h>
#include <FreeType/fttrigon.h>
#else
#include "SDL/SDL_ttf.h"
#endif

#include <map>


/**
 *  \class Font
 *  \brief Font Resource.
 *
 *  Implements a Font resource.
 */
class Font: public Resource
{
public:
	Font();
	Font(const std::string& filePath, int ptSize = 12);
	~Font();

	int width(const std::string& str) const;
	int height() const;

	const std::string& typefaceName() const;

	void bold();
	void italic();
	void underline();
	void normal();

protected:
	friend class Renderer;
	friend class SDL_Renderer;
	friend class OGL_Renderer;

	FT_Face &font();
	GLuint texture(char ch) const;
	int getGlyphWidth(char ch);
	int getGlyphHeight(char ch);

private:
	// explicitly disallow copy construction/assignment operator
	Font(const Font &font);
	Font& operator=(const Font& font);

	void load();
	bool generateCharacterTexture(unsigned char ch, FT_Face fontInfo);

//	TTF_Font		*mFont;			/**< True Type Font. */
	FT_Face			mFont;

	int				mHeight;		/**< Font Height. */
	int				mPtSize;		/**< Point Size to load the Font in. */

	File			mFontBuffer;	/**< Persistent memory buffer for TTF_Font. */
	std::string		mFontName;		/**< Full typeface name. */
	
	std::vector<GLuint> mTextures; //Store room for the character textures
	std::map<char, std::pair<int, int> > mGlyphDimensions;
    std::map<char, std::pair<int, int> > mGlyphPositions;
    std::map<char, int> mGlyphAdvances;
};

#endif

