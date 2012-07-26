// ==================================================================================
// = NAS2D
// = Copyright © 2008 - 2012 New Age Software
// ==================================================================================
// = NAS2D is distributed under the terms of the zlib license. You are free to copy,
// = modify and distribute the software under the terms of the zlib license.
// = 
// = Acknowledgement of your use of NAS2D is appriciated but is not required.
// ==================================================================================

#include "NAS2D/Resources/Font.h"

using namespace std;

string buildName(FT_Face);
void toggleFontStyle(FT_Face, int);


FT_Library Font::mFontLib;
bool Font::mFontLibInited = false;


/**
 * Primary method of instantiating a font.
 *
 * \param	filePath	Path to a font file.
 * \param	ptSize		Point size of the font. Defaults to 12pt.
 *
 */
Font::Font(const std::string& filePath, int ptSize):	Resource(filePath),
														mFont(NULL),
														mHeight(0),
														mPtSize(ptSize)
{
	
	if(!Font::mFontLibInited)
	{
		int err = FT_Init_FreeType(&Font::mFontLib);
		if(err)
		{
			errorMessage("FreeType Font Library failed to initialize.");
		}

		mFontLibInited = true;
	}
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
				mPtSize(0),
				mFontName("Default Font")
{
}


/**
 * D'tor
 */
Font::~Font()
{
	FT_Done_Face(mFont);
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
	mFontBuffer = Utility<Filesystem>::get().open(name());

	if(mFontBuffer.size() == 0)
	{
		errorMessage(Utility<Filesystem>::get().lastError());
		return;
	}
	
	int err = FT_New_Memory_Face(mFontLib, reinterpret_cast<const FT_Byte*>(mFontBuffer.bytes().c_str()), mFontBuffer.size(), 0, &mFont);
											// ^^^ DANGEROUS CAST!!!!

	if(err == FT_Err_Unknown_File_Format)
	{
		errorMessage("Unknown font file format.");
	}
	else if(err)
	{
		errorMessage("Font file could not be read.");
	}
	
	//std::printf("%li", mFont->num_glyphs);  << ?????

	// Should check for NULL font and set to invalid state if face is NULL.
	cout << mFont->num_glyphs << endl;

	//mHeight = TTF_FontHeight(mFont);

	mFontName = buildName(mFont);

	loaded(true);
}


/**
 * Returns the width in pixels of the font.
 *
 * \param	str		Reference to a std::string to get the width of.
 */
int Font::width(const std::string& str) const
{
	if(mFont == NULL)
		return 0;

	int width = 0;

//	if(TTF_SizeText(mFont, str.c_str(), &width, NULL))
//		return 0;

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
 * \todo	This is mostly a design issue but I'd like to hide
 *			this functionality somehow or wrap the TTF_Font
 *			structure to something somewhat more generic.
 */
FT_Face Font::font() const
{
	return mFont;
}


/**
 * Returns the typeface name.
 */
const std::string& Font::typefaceName() const
{
	return mFontName;
}


/**
 * Toggles Bold style.
 */
void Font::bold()
{
	//toggleFontStyle(mFont, TTF_STYLE_BOLD);
}


/**
 * Toggles Italic style.
 */
void Font::italic()
{
	//toggleFontStyle(mFont, TTF_STYLE_ITALIC);
}


/**
 * Toggles Underline style.
 */
void Font::underline()
{
	//toggleFontStyle(mFont, TTF_STYLE_UNDERLINE);
}


/**
 * Resets all font styling.
 */
void Font::normal()
{
	//TTF_SetFontStyle(mFont, TTF_STYLE_NORMAL);
}



// ==================================================================================
// = Unexposed module-level functions defined here that don't need to be part of the
// = API interface.
// ==================================================================================

/*
 * Builds a typeface name given a TTF_Font.
 */
string buildName(FT_Face font)
{
	// Build Font Name with Family Name and Style Name.
//	string fontFamily = TTF_FontFaceFamilyName(font);
//	string fontStyle = TTF_FontFaceStyleName(font);
//
//	// If Font style is regular, just use the family name.
//	if(toLowercase(fontStyle) == "regular")
//		return fontFamily;
//	else
//		return fontFamily + " " + fontStyle;
	return "";
}


/* 
 * Toggles a font style for a given TTF_Font.
 */ 
void toggleFontStyle(FT_Face font, int fontStyle)
{
	// Defend against NULL pointers.
	if(!font)
		return;

	//TTF_SetFontStyle(font, TTF_GetFontStyle(font) ^ fontStyle);
}