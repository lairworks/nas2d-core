// ==================================================================================
// = NAS2D
// = Copyright © 2008 New Age Software
// ==================================================================================
// = NAS2D is distributed under the terms of the zlib license. You are free to copy,
// = modify and distribute the software as long under the terms of the zlib license.
// = 
// = Acknowledgement of your use of NAS2D is appriciated but is not required.
// ==================================================================================

#ifndef _NAS_FONTRESOURCE_
#define _NAS_FONTRESOURCE_

#include "Resource.h"

#ifdef __APPLE__
#include "SDL_ttf/SDL_ttf.h"
#else
#include "SDL/SDL_ttf.h"
#endif


/**
 * \enum	FontStyle
 *
 * Used to set the style of the font.
 */
enum FontStyle
{
	STYLE_NORMAL,
	STYLE_BOLD,
	STYLE_ITALIC,
	STYLE_UNDERLINE
};


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
	Font(const std::string& filePath, unsigned int ptSize = 12);
	~Font() { TTF_CloseFont(mFont); mFont = NULL; }


	int getWidth(const std::string& str) const;

	int getHeight() const;

	const std::string& getFontName() const;

	void setStyle(FontStyle style = STYLE_NORMAL);

	TTF_Font *getFont() const;	/**< Should this be private and just friend the Renderer class? */

protected:

private:
	// explicitly disallow copy construction/assignment operator
	Font(const Font &font);
	Font& operator=(const Font& font);

	void load();
	std::string buildName();

	TTF_Font		*mFont;			/**< True Type Font. */

	int				mHeight;		/**< Font Height. */
	int				mPtSize;		/**< Point Size to load the Font in. */

	File			mFontBuffer;	/**< Persistent memory buffer for TTF_Font. */
	std::string		mFontName;
};

#endif

