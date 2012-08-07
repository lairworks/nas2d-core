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


const int ASCII_CHARACTER_COUNT			= 128;
const int ASCII_START_CODE				= 32;
const int ASCII_LAST_CHAR				= 127; // code 128 is DELETE, so ignore it
const int CHAR_COUNT					= ASCII_LAST_CHAR - ASCII_START_CODE;

const int ASCII_SPACE					= 32; // Scan code for ASCII space is 32

const int DEFAULT_SPACING				= 16;


bool Font::isSpace(char c)
{
	return static_cast<int>(c) == ASCII_SPACE;
}


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
	FT_Library library; //Create a freetype library instance
	
    if(FT_Init_FreeType(&library))
        std::cerr << "Could not initialize the freetype library" << std::endl;

	File fontFile = Utility<Filesystem>::get().open(name());

	const FT_Byte* stream = (const unsigned char*)fontFile.bytes().c_str(); // hate having to cast like this. Yuck!

	FT_New_Memory_Face(library, stream, fontFile.size(), 0, &mFont);
	
	mHeight = mFont->height / 64;
	
    //FreeType uses heights which are one 64th of the size in pixels so
    //we set our font height by multiplying by 64. The 96x96 is the dots per inch
    FT_Set_Char_Size(mFont, mPtSize * 64, mPtSize * 64, 96, 96);
	
	// Generate 128 textures (each character gets its own texture)
	GLuint *_textureID = new GLuint[CHAR_COUNT];
	glGenTextures(CHAR_COUNT, _textureID);
	
	mTextures.resize(CHAR_COUNT);
	mGlyphMetrics.resize(CHAR_COUNT);
	

	for(int i = ASCII_START_CODE; i < ASCII_LAST_CHAR; ++i)
	{
		mTextures[i - ASCII_START_CODE] = _textureID[i - ASCII_START_CODE];

		if(!generateCharacterTexture(i))
			std::cerr << "Could not generate the texture for character: " << (char)i << std::endl;
	}

	delete [] _textureID;

	loaded(true);
}


bool Font::generateCharacterTexture(int ch)
{
	if(FT_Get_Char_Index(mFont, ch) != 0 /*&& !isSpace(ch)*/)
	{
		if(FT_Load_Glyph(mFont, FT_Get_Char_Index(mFont, ch), FT_LOAD_DEFAULT))
		{
			std::cout << "Failed to load glyph for '" << (char)ch << "'." << endl;
			return false;
		}
	
		FT_Glyph glyph;
		if(FT_Get_Glyph(mFont->glyph, &glyph))
		{
			std::cout << "Failed to get glyph for char: " << (char)ch << "." << endl;
			return false;
		}
	
		if(FT_Glyph_To_Bitmap(&glyph, ft_render_mode_normal, 0, 1))
		{
			std::cout << "Failed to load bitmap for char: " << (char)ch << "." << endl;
			return false;
		}
	
		FT_BitmapGlyph bitmapGlyph = (FT_BitmapGlyph)glyph; // This is akin to reinterpret_cast, not safe and possibly not correct.
	
		int width = (bitmapGlyph->bitmap.width) ? bitmapGlyph->bitmap.width : DEFAULT_SPACING;
		int rows = (bitmapGlyph->bitmap.rows) ? bitmapGlyph->bitmap.rows : DEFAULT_SPACING;
	
		glBindTexture(GL_TEXTURE_2D, mTextures[ch - ASCII_START_CODE]);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
		glTexImage2D(GL_TEXTURE_2D, 0, GL_ALPHA, width, rows, 0, GL_ALPHA, GL_UNSIGNED_BYTE, bitmapGlyph->bitmap.buffer);
	
		mGlyphMetrics[ch - ASCII_START_CODE](bitmapGlyph->left, bitmapGlyph->top, width, rows);
		mGlyphAdvances[ch] = mFont->glyph->advance.x / 64;
	}
	else
		cout << "Char:\t" << (char)ch << "\tIndex:\t" << FT_Get_Char_Index(mFont, ch) << endl;

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

	for(size_t i = 0; i < str.size(); i++)
	{
		#if defined(_DEBUG)
		int c = static_cast<int>(str[i]);
		width += mGlyphMetrics[c - ASCII_START_CODE].w;
		#else
		width += mGlyphMetrics[static_cast<int>(str[i])].w;
		#endif
		
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
	return mTextures[static_cast<int>(ch) - ASCII_START_CODE];
}

/**
 * Returns an OpenGL Texture.
 *
 */
int Font::getGlyphWidth(char ch)
{
	return mGlyphMetrics[static_cast<int>(ch) - ASCII_START_CODE].w;
}

/**
 * Returns an OpenGL Texture.
 *
 */
int Font::getGlyphHeight(char ch)
{
	return mGlyphMetrics[static_cast<int>(ch) - ASCII_START_CODE].h;
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
