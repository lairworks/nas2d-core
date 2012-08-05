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

#if defined(__APPLE__)
	#include <SDL/SDL_opengl.h>
#elif defined(WIN32)
	#include "GLee.h"
	#include "SDL/SDL_opengl.h"
#else
	#include "SDL/SDL_opengl.h"
#endif

using namespace std;

//string buildName(TTF_Font*);
//void toggleFontStyle(TTF_Font*, int);

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
	//TTF_CloseFont(mFont);
	mFont = NULL;
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
//	mFontBuffer = Utility<Filesystem>::get().open(name());
//
//	if(mFontBuffer.size() == 0)
//	{
//		errorMessage(Utility<Filesystem>::get().lastError());
//		//mFont = NULL;
//		return;
//	}
//
//	mFont = TTF_OpenFontRW(SDL_RWFromConstMem(mFontBuffer.raw_bytes(), mFontBuffer.size()), 0, mPtSize);
//	if(!mFont)
//	{		
//		// Get the error message and return false. 
//		errorMessage(TTF_GetError());
//		return;
//	}
//
//	mHeight = TTF_FontHeight(mFont);
//
//	mFontName = buildName(mFont);
	
	FT_Library library; //Create a freetype library instance
	
    if(FT_Init_FreeType(&library))
        std::cerr << "Could not initialize the freetype library" << std::endl;


    /*	std::string tmp = "data/" + name();
	
	
    //Now we attempt to load the font information

	Stop using function that directly access the filesystem, this violates
	filesystem security and breaks the virtual fileystems paradigm, e.g.,
	on Win32, it can't find the file because it can't read properly from
	relative paths.

	if(FT_New_Face(library, tmp.c_str(), 0, &mFont)) {
        std::cerr << "Could not load the specified font:\t" << name() << std::endl;
    }
	*/

	File fontFile = Utility<Filesystem>::get().open(name());

	const FT_Byte* stream = (const unsigned char*)fontFile.bytes().c_str(); // hate having to cast like this. Yuck!

	FT_New_Memory_Face(library, stream, fontFile.size(), 0, &mFont);
	
	mHeight = mFont->height/64;
	
    //FreeType uses heights which are one 64th of the size in pixels so
    //we set our font height by multiplying by 64. The 96x96 is the dots per inch
    FT_Set_Char_Size(mFont, (int)mPtSize * 64, (int)mPtSize * 64, 96, 96);
	
// Generate 128 textures (each character gets its own texture)
	GLuint m_textureID;
	for (int i = 0; i < 127; i++)
	{
		glGenTextures(1, &m_textureID);
		mTextures.push_back(m_textureID);
	}
	
    for (int ch = 32; ch < 128; ++ch)
    {
        if (!generateCharacterTexture(ch, mFont))
        {
            std::cerr << "Could not generate the texture for character: " << ch << std::endl;
        }
    }

	loaded(true);
}


bool Font::generateCharacterTexture(unsigned char ch, FT_Face fontInfo)
{
	if(FT_Get_Char_Index(mFont, ch) != 0 || !isspace(ch))
	{
		if(FT_Load_Glyph(fontInfo, FT_Get_Char_Index(fontInfo, ch), FT_LOAD_DEFAULT))
		{
			std::cout << "Failed to load glyph for char: " << ch << "." << endl;
			return false;
		}
	
		FT_Glyph glyph;
		if(FT_Get_Glyph(fontInfo->glyph, &glyph))
		{
			std::cout << "Failed to get glyph for char: " << ch << "." << endl;
			return false;
		}
	
		if (FT_Glyph_To_Bitmap(&glyph, ft_render_mode_normal, 0, 1))
		{
			std::cout << "Failed to load bitmap for char: " << ch << "." << endl;
			return false;
		}
	
		FT_BitmapGlyph bitmapGlyph = (FT_BitmapGlyph) glyph;
	
		int width = (bitmapGlyph->bitmap.width) ? bitmapGlyph->bitmap.width : 16;
		int rows = (bitmapGlyph->bitmap.rows) ? bitmapGlyph->bitmap.rows : 16;
	
		glBindTexture(GL_TEXTURE_2D, mTextures[ch]);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
		glTexImage2D(GL_TEXTURE_2D, 0, GL_ALPHA, width, rows, 0,
                 GL_ALPHA, GL_UNSIGNED_BYTE, bitmapGlyph->bitmap.buffer);
	
		mGlyphDimensions[ch] = std::make_pair(width, rows);
		mGlyphPositions[ch] = std::make_pair(bitmapGlyph->left, bitmapGlyph->top);
		mGlyphAdvances[ch] = fontInfo->glyph->advance.x / 64;
	}
	else
		printf("Char:\t%c\tIndex:\t%i\n", ch, FT_Get_Char_Index(mFont, ch));
    return true;
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

	for(int i = 0; i < static_cast<int>(str.size()); i++)
	{
		if(FT_Load_Glyph(mFont, FT_Get_Char_Index(mFont, str[i]), FT_LOAD_DEFAULT))
		{
			return false;
		}
		
		FT_Glyph glyph;
		if(FT_Get_Glyph(mFont->glyph, &glyph))
		{
			return false;
		}
		
		if (FT_Glyph_To_Bitmap(&glyph, ft_render_mode_normal, 0, 1))
		{
			return false;
		}
		
		FT_BitmapGlyph bitmapGlyph = (FT_BitmapGlyph) glyph;
		
		width += bitmapGlyph->bitmap.width;
	}

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
FT_Face &Font::font()
{
	return mFont;
}

/**
 * Returns an OpenGL Texture.
 *
 */
GLuint Font::texture(char ch) const
{
	return mTextures[ch];
}

/**
 * Returns an OpenGL Texture.
 *
 */
int Font::getGlyphWidth(char ch)
{
	return mGlyphDimensions[ch].first;
}

/**
 * Returns an OpenGL Texture.
 *
 */
int Font::getGlyphHeight(char ch)
{
	return mGlyphDimensions[ch].second;
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
//	toggleFontStyle(mFont, TTF_STYLE_BOLD);
}


/**
 * Toggles Italic style.
 */
void Font::italic()
{
//	toggleFontStyle(mFont, TTF_STYLE_ITALIC);
}


/**
 * Toggles Underline style.
 */
void Font::underline()
{
//	toggleFontStyle(mFont, TTF_STYLE_UNDERLINE);
}


/**
 * Resets all font styling.
 */
void Font::normal()
{
//	TTF_SetFontStyle(mFont, TTF_STYLE_NORMAL);
}



// ==================================================================================
// = Unexposed module-level functions defined here that don't need to be part of the
// = API interface.
// ==================================================================================

/*
 * Builds a typeface name given a TTF_Font.
 */
string buildName(TTF_Font* font)
{
	// Build Font Name with Family Name and Style Name.
	//string fontFamily = TTF_FontFaceFamilyName(font);
	//string fontStyle = TTF_FontFaceStyleName(font);

	// If Font style is regular, just use the family name.
	//if(toLowercase(fontStyle) == "regular")
	//	return fontFamily;
	//else
	//	return fontFamily + " " + fontStyle;

	return "";
}


/* 
 * Toggles a font style for a given TTF_Font.
 */ 
void toggleFontStyle(TTF_Font* font, int fontStyle)
{
	// Defend against NULL pointers.
	if(!font)
		return;

	//TTF_SetFontStyle(font, TTF_GetFontStyle(font) ^ fontStyle);
}