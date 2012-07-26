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

#if defined(__APPLE__)
#include "SDL/SDL_opengl.h"
#elif defined(WIN32)
#include "GLee.h"
#include "SDL/SDL_opengl.h"
#else
#include "SDL/SDL_opengl.h"
#endif

#include "ft2build.h"
#include FT_FREETYPE_H
#include FT_GLYPH_H


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
	GLuint texture(const std::string& str);

	void bold();
	void italic();
	void underline();
	void normal();

protected:
	friend class Renderer;
	friend class SDL_Renderer;
	friend class OGL_Renderer;

	FT_Face font() const;

private:
	// explicitly disallow copy construction/assignment operator
	Font(const Font &font);
	Font& operator=(const Font& font);

	void load();

	FT_Face				mFont;			/**< True Type Font. */
	FT_GlyphSlot		mFontGlyphSlot;
	FT_BBox				mFontBounds;
	FT_UInt				mFontGlyphIndex;
	FT_Glyph			*mFontGlyph;

	static FT_Library	mFontLib;
	static bool			mFontLibInited;

	int					mHeight;		/**< Font Height. */
	int					mPtSize;		/**< Point Size to load the Font in. */

	File				mFontBuffer;	/**< Persistent memory buffer for TTF_Font. */
	std::string			mFontName;		/**< Full typeface name. */
	
	GLuint				mFontTexture;
};

#endif

