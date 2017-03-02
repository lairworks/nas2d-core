// ==================================================================================
// = NAS2D
// = Copyright © 2008 - 2017 New Age Software
// ==================================================================================
// = NAS2D is distributed under the terms of the zlib license. You are free to copy,
// = modify and distribute the software under the terms of the zlib license.
// = 
// = Acknowledgement of your use of NAS2D is appriciated but is not required.
// ==================================================================================

#include "NAS2D/Resources/Font.h"
#include "NAS2D/Exception.h"

#if defined(__APPLE__)
	#include "SDL2/SDL_opengl.h"
	#include "SDL2_ttf/SDL_ttf.h"
	#include "SDL2_image/SDL_image.h"
#elif defined(WINDOWS)
	#include "SDL_opengl.h"
	#include "SDL_ttf.h"
	#include "SDL_image.h"
#elif defined(__linux__)
#define NO_SDL_GLEXT
#include "GLee.h"
#include "SDL2/SDL_ttf.h"
#include "SDL2/SDL_image.h"
#else
	#include "SDL/SDL_opengl.h"
	#include "SDL/SDL_gfxPrimitives.h"
	#include "SDL/SDL_ttf.h"
#endif


#include <math.h>

using namespace std;
using namespace NAS2D;

string buildName(TTF_Font*);

const int	ASCII_TABLE_COUNT	= 256;
const int	ASCII_TABLE_FIRST	= 0;
const int	ASCII_TABLE_LAST	= 255;

const int	BITS_32				= 32;

NAS2D::Font::FontMap NAS2D::Font::_FontMap;


void setupMasks(unsigned int& rmask, unsigned int& gmask, unsigned int& bmask, unsigned int& amask);


unsigned nextPowerOf2(unsigned n)
{
	return (unsigned)pow(2, ceil(log((float)n) / log(2.0f)));
}


/**
 * Primary method of instantiating a font.
 *
 * \param	filePath	Path to a font file.
 * \param	ptSize		Point size of the font. Defaults to 12pt.
 *
 */
NAS2D::Font::Font(const std::string& filePath, int ptSize):	Resource(filePath),
														mHeight(0),
														mAscent(0),
														mPtSize(ptSize),
														mTextureId(0)
{

	load();
}


NAS2D::Font::Font(const string& filePath, int glyphWidth, int glyphHeight, int glyphSpace):	Resource(filePath),
																						mHeight(glyphHeight),
																						mAscent(0),
																						mPtSize(glyphHeight),
																						mGlyphCellSize(glyphWidth, glyphHeight),
																						mTextureId(0)
{
	loadBitmap(filePath, glyphWidth, glyphHeight, glyphSpace);
}


/**
 * Default c'tor.
 * 
 * Fonts instantiated with this constructor are not valid for use.
 */
NAS2D::Font::Font():	Resource("Default Font"),
				mHeight(0),
				mAscent(0),
				mPtSize(0),
				mTextureId(0),
				mFontName("Default Font")
{}


NAS2D::Font::Font(const Font& _f): Resource(_f.name())
{
    FontMap::iterator it = Font::_FontMap.find(name());
    if(it != Font::_FontMap.end())
    {
        it->second.ref_count++;
    }

    loaded(_f.loaded());
}

NAS2D::Font& NAS2D::Font::operator=(const Font& _f)
{
    FontMap::iterator it = Font::_FontMap.find(name());
    if(it != Font::_FontMap.end())
    {
        it->second.ref_count++;
    }

    name(_f.name());
    loaded(_f.loaded());

    return *this;
}


/**
 * D'tor
 */
NAS2D::Font::~Font()
{
	// Is this check necessary?
	FontMap::iterator it = Font::_FontMap.find(name());
	if(it == Font::_FontMap.end())
		return;

	it->second.ref_count--;

	// if texture id reference count is 0, delete the texture.
	if(it->second.ref_count < 1)
	{
		glDeleteTextures(1, &mTextureId);

		Font::_FontMap.erase(it);
	}
}


/**
 * Loads a Font file.
 *
 * This is the normal method of loading a Font Resource and is called internally
 * by the constructor. It is not publicly exposed and should never be called
 * anywhere except the constructor.
 */
void NAS2D::Font::load()
{
	if(TTF_WasInit() == 0)
	{
		if(TTF_Init() == -1)
		{
			cout << "(ERROR) Font::load(): " << TTF_GetError() << endl;
			return;
		}
	}

	File fontBuffer = Utility<Filesystem>::get().open(name());

	if(fontBuffer.empty())
	{
		cout << "(ERROR) Font::load(): " << Utility<Filesystem>::get().lastError() << endl;
		return;
	}

	// Attempt to load the font.
	TTF_Font *font = TTF_OpenFontRW(SDL_RWFromConstMem(fontBuffer.raw_bytes(), fontBuffer.size()), 0, mPtSize);
	if(!font)
	{
		cout << "(ERROR) Font::load(): " << TTF_GetError() << endl;
		return;
	}

	mHeight = TTF_FontHeight(font);
	mAscent = TTF_FontAscent(font);
	mFontName = buildName(font);

	generateGlyphMap(font);
	TTF_CloseFont(font);

	loaded(true);
}


void NAS2D::Font::loadBitmap(const std::string& path, int glyphWidth, int glyphHeight, int glyphSpace)
{
	// Load specified file and check that it divides to a 16x16 grid.
	File fontBuffer = Utility<Filesystem>::get().open(name());

	if(fontBuffer.empty())
	{
		cout << "(ERROR) Font::load(): " << Utility<Filesystem>::get().lastError() << endl;
		return;
	}

	SDL_Surface* glyphMap = IMG_Load_RW(SDL_RWFromConstMem(fontBuffer.raw_bytes(), fontBuffer.size()), 0);
	if(!glyphMap)
	{
		cout << "(ERROR) Font::loadBitmap(): " << SDL_GetError() << endl;
		return;
	}

	if( (glyphMap->w / 16 != glyphWidth) || (glyphMap->h / 16 != glyphHeight) )
	{
		cout << "(ERROR) Font::loadBitmap(): Glyph texture does not have 16 columns or 16 rows." << endl;
		return;
	}

	mGlyphMetrics.resize(ASCII_TABLE_COUNT);
	for(size_t i = 0; i < ASCII_TABLE_COUNT; i++)
		mGlyphMetrics[i].minX = glyphCellWidth();

	for(int row = 0; row < 16; row++)
	{
		for(int col = 0; col < 16; col++)
		{
			int glyph = (row * 16) + col;

			mGlyphMetrics[glyph].uvX = (float)(col * glyphCellWidth()) / (float)glyphMap->w;
			mGlyphMetrics[glyph].uvY = (float)(row * glyphCellHeight()) / (float)glyphMap->h;
			mGlyphMetrics[glyph].uvW = mGlyphMetrics[glyph].uvX + (float)(glyphCellWidth()) / (float)glyphMap->w;
			mGlyphMetrics[glyph].uvH = mGlyphMetrics[glyph].uvY + (float)(glyphCellHeight()) / (float)glyphMap->h;
			mGlyphMetrics[glyph].advance = glyphSpace;
		}
	}

	generateTexture(glyphMap);
	
	// Add generated texture id to texture ID map.
	Font::_FontMap[name()] = FontInfo(texture_id(), ptSize());
	Font::_FontMap[name()].ref_count++;
	SDL_FreeSurface(glyphMap);

	loaded(true);
}


/**
 * Generates a glyph map of all ASCII standard characters from 0 - 255.
 */
void NAS2D::Font::generateGlyphMap(TTF_Font* ft)
{	
	int largest_width = 0;

	// Go through each glyph and determine how much space we need in the texture.
	for(int i = 0; i < ASCII_TABLE_COUNT; i++)
	{
		GlyphMetrics metrics;

		TTF_GlyphMetrics(ft, i, &metrics.minX, &metrics.maxX, &metrics.minY, &metrics.maxY, &metrics.advance);
		if(metrics.advance > largest_width)
			largest_width = metrics.advance;
		
		if(metrics.minX + metrics.maxX > largest_width)
			largest_width = metrics.minX + metrics.maxX;

		if(metrics.minY + metrics.maxY > largest_width)
			largest_width = metrics.minY + metrics.maxY;

		mGlyphMetrics.push_back(metrics);
	}

	mGlyphCellSize(nextPowerOf2(largest_width), nextPowerOf2(largest_width));
	int textureSize = mGlyphCellSize.x() * 16; // glyph map contains 16 rows and 16 columns.

	//cout << "Largest Width: " << largest_width << " Nearest Po2: " << nextPowerOf2(largest_width) << endl;

	unsigned int rmask = 0, gmask = 0, bmask = 0, amask = 0;
	setupMasks(rmask, gmask, bmask, amask);

	SDL_Surface* glyphMap = SDL_CreateRGBSurface(SDL_SWSURFACE, textureSize, textureSize, BITS_32, rmask, gmask, bmask, amask);

	SDL_Color white = { 255, 255, 255 };
	for(int row = 0; row < 16; row++)
	{
		for(int col = 0; col < 16; col++)
		{
			int glyph = (row * 16) + col;

			mGlyphMetrics[glyph].uvX = (float)(col * glyphCellWidth()) / (float)textureSize;
			mGlyphMetrics[glyph].uvY = (float)(row * glyphCellHeight()) / (float)textureSize;
			mGlyphMetrics[glyph].uvW = mGlyphMetrics[glyph].uvX + (float)(glyphCellWidth()) / (float)textureSize;
			mGlyphMetrics[glyph].uvH = mGlyphMetrics[glyph].uvY + (float)(glyphCellHeight()) / (float)textureSize;

			// HACK HACK HACK!
			// Apparently glyph zero has no size with some fonts and so SDL_TTF complains about it.
			// This is here only to prevent the message until I find the time to put in something
			// less bad.
			if(glyph == 0)
				continue;

			SDL_Surface* srf = TTF_RenderGlyph_Blended(ft, glyph, white);
			if(!srf)
			{
				cout << "(ERROR) Font::generateGlyphMap(): " << TTF_GetError() << endl;
			}
			else
			{
				SDL_SetSurfaceBlendMode(srf, SDL_BLENDMODE_NONE);
				SDL_Rect rect = { col * mGlyphCellSize.x(), row * mGlyphCellSize.y(), 0, 0 };
				SDL_BlitSurface(srf, nullptr, glyphMap, &rect);
				SDL_FreeSurface(srf);
			}
		}
	}

	//SDL_SaveBMP(glyphMap, "glyphmap.bmp");

	generateTexture(glyphMap);
	
	// Add generated texture id to texture ID map.
	Font::_FontMap[name()] = FontInfo(texture_id(), ptSize());
	Font::_FontMap[name()].ref_count++;
	SDL_FreeSurface(glyphMap);
}


/**
 * Generates a new OpenGL texture from an SDL_Surface.
 * 
 * \param	src	Pointer to an SDL_Surface.
 * 
 * \note	This function assumes that the image is unique and
 *			has not been loaded. Does no resource management.
 * 
 * \note	Code bloat.
 */
void NAS2D::Font::generateTexture(SDL_Surface *src)
{
	// Surface must always be 32-Bit, anything else should be treated as an error.
	if(src->format->BytesPerPixel < 4)
	{
		cout << "EXCEPTION: Could not create font, pixel format is not 32 bit." << endl;
		throw Exception(0, "Font Generation", "Font generation failed.");
	}

	glPixelStorei(GL_UNPACK_ALIGNMENT, 1);	// Does this need to be called every time
											// or can we set it once in the Renderer?

	glGenTextures(1, &mTextureId);
	glBindTexture(GL_TEXTURE_2D, mTextureId);

	GLenum textureFormat = 0;
	SDL_BYTEORDER == SDL_BIG_ENDIAN ? textureFormat = GL_BGRA : textureFormat = GL_RGBA;
	glTexImage2D(GL_TEXTURE_2D, 0, textureFormat, src->w, src->h, 0, textureFormat, GL_UNSIGNED_BYTE, src->pixels);

	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
	glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
	glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
}


/**
 * Gets the texture ID for the font.
 */
unsigned int NAS2D::Font::texture_id() const
{
	return mTextureId;
}


/**
 * Returns the width of a glyph's cell.
 */
const int NAS2D::Font::glyphCellWidth() const
{
	return mGlyphCellSize.x();
}


/**
 * Returns the height of a glyph's cell.
 */
const int NAS2D::Font::glyphCellHeight() const
{
	return mGlyphCellSize.y();
}


/**
 * Gets the metrics for a given glyph.
 * 
 * \param	glyph	Glyph index. Valid values are 0 - 255.
 */
const NAS2D::Font::GlyphMetrics& NAS2D::Font::glyphMetrics(int glyph) const
{
	int g = clamp(glyph, ASCII_TABLE_FIRST, ASCII_TABLE_LAST);
	return mGlyphMetrics[g];
}


/**
 * Returns the width in pixels of the font.
 *
 * \param	str		Reference to a std::string to get the width of.
 */
int NAS2D::Font::width(const std::string& str) const
{
	if(str.empty() || mGlyphMetrics.empty())
		return 0;

	int width = 0;
	for(size_t i = 0; i < str.size(); i++)
		width += mGlyphMetrics[str[i]].advance + mGlyphMetrics[i].minX;

	return width;
}


/**
 * Returns the height in pixels of the font.
 */
int NAS2D::Font::height() const
{
	return mHeight;
}


/**
 * Returns the height in pixels of the font.
 */
int NAS2D::Font::ascent() const
{
	return mAscent;
}


/**
 * Returns the font's pt size.
 */
int NAS2D::Font::ptSize() const
{
	return mPtSize;
}


/**
 * Returns the typeface name.
 */
const std::string& NAS2D::Font::typefaceName() const
{
	return mFontName;
}


/**
 * Toggles Bold style.
 */
void NAS2D::Font::bold()
{

}


/**
 * Toggles Italic style.
 */
void NAS2D::Font::italic()
{

}


/**
 * Toggles Underline style.
 */
void NAS2D::Font::underline()
{

}


/**
 * Resets all font styling.
 */
void NAS2D::Font::normal()
{

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
	string fontFamily = TTF_FontFaceFamilyName(font);
	string fontStyle = TTF_FontFaceStyleName(font);

	// If Font style is regular, just use the family name.
	if(toLowercase(fontStyle) == "regular")
		return fontFamily;
	else
		return fontFamily + " " + fontStyle;
}



void setupMasks(unsigned int& rmask, unsigned int& gmask, unsigned int& bmask, unsigned int& amask)
{
	if(SDL_BYTEORDER == SDL_LIL_ENDIAN)
	{
		rmask = 0x000000ff; gmask = 0x0000ff00; bmask = 0x00ff0000; amask = 0xff000000;
	}
	else
	{
		rmask = 0xff000000; gmask = 0x00ff0000; bmask = 0x0000ff00; amask = 0x000000ff;
	}
}
