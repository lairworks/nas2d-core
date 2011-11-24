// ==================================================================================
// = NAS2D
// = Copyright © 2008 New Age Software
// ==================================================================================
// = NAS2D is distributed under the terms of the zlib license. You are free to copy,
// = modify and distribute the software as long under the terms of the zlib license.
// = 
// = Acknowledgement of your use of NAS2D is appriciated but is not required.
// ==================================================================================

#include "NAS2D/Resources/Font.h"

using namespace std;

/**
 * Primary method of instantiating a font.
 *
 * \param	filePath	Path to a font file.
 * \param	ptSize		Point size of the font. Defaults to 12pt.
 *
 */
Font::Font(const std::string& filePath, unsigned int ptSize):	Resource(filePath),
																mFont(NULL),
																mHeight(0),
																mPtSize(ptSize)
{
	load();
}


/**
 * Default c'tor.
 * 
 * Fonts instantiated with this constructor are not valid for use.
 */
Font::Font():	Resource("Default Font"),
				mFont(NULL),
				mHeight(0),
				mPtSize(0)
{
}


/**
 * Loads a Font file.
 *
 * This is the normal method of loading a Font Resource and is called internally
 * by the constructor. It is not publicly exposed and should never be called
 * anywhere except the constructor.
 */
void Font::load()
{
	mFontBuffer = Singleton<Filesystem>::get().open(name());

	if(mFontBuffer.size() == 0)
	{
		errorMessage(Singleton<Filesystem>::get().lastError());
		//mFont = NULL;
		return;
	}

	mFont = TTF_OpenFontRW(SDL_RWFromConstMem(mFontBuffer.raw_bytes(), mFontBuffer.size()), 0, mPtSize);
	if(!mFont)
	{		
		// Get the error message and return false. 
		errorMessage(TTF_GetError());
		//mFont = NULL;
		return;
	}

	mHeight = TTF_FontHeight(mFont);

	//TTF_SetFontStyle(mFont, TTF_STYLE_BOLD);
	
	mFontName = buildName();

	loaded(true);
}


/**
 * Returns the width in pixels of the font.
 *
 * \param	str		Reference to a std::string to get the width of.
 */
int Font::width(const string& str) const
{
	if(mFont == NULL)
		return 0;

	int width = 0;

	if(TTF_SizeText(mFont, str.c_str(), &width, NULL))
		return 0;

	return width;
}


/**
 * Returns the height in pixels of the font.
 */
int Font::height() const
{
	return mHeight;
}


/**
 * Returns an SDL TTF_Font object.
 *
 * \todo	This is mostly a design issue but I'd like to somehow
 *			hide this functionality somehow or wrap the TTF_Font
 *			structure to something somewhat more generic.
 */
TTF_Font *Font::font() const
{
	return mFont;
}


/**
 * Returns the typeface name.
 */
const std::string& Font::fontName() const
{
	return mFontName;
}


/**
 * Build Font Name with Family Name and Style Name.
 *
 * This function is called interally to create a Font name based on
 * Font family and Font style.
 */
std::string Font::buildName()
{
	// Build Font Name with Family Name and Style Name.
	string fontFamily = TTF_FontFaceFamilyName(mFont);
	string fontStyle = TTF_FontFaceStyleName(mFont);

	// If Font style is regular, just use the family name.
	if(fontStyle == "Regular")
		return fontFamily;
	else
		return fontFamily + " " + fontStyle;
}


/**
 * Sets the style of the font to use for rendering.
 *
 * \param	style	Sets the style of the font. Can be STYLE_NORMAL (default),
					STYLE_BOLD, STYLE_ITALIC or STYLE_UNDERLINE.
 */
void Font::style(FontStyle style)
{
	switch(style)
	{
		case STYLE_NORMAL:
			if(mFont)
				TTF_SetFontStyle(mFont, TTF_STYLE_NORMAL);
			break;

		case STYLE_BOLD:
			if(mFont)
				TTF_SetFontStyle(mFont, TTF_STYLE_BOLD);
			break;

		case STYLE_ITALIC:
			if(mFont)
				TTF_SetFontStyle(mFont, TTF_STYLE_ITALIC);
			break;

		case STYLE_UNDERLINE:
			if(mFont)
				TTF_SetFontStyle(mFont, TTF_STYLE_UNDERLINE);
			break;

		default:
			cout << "Attempted to set an unknown font style." << endl;
			break;
	}
}
