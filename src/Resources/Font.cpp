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
#include "NAS2D/Resources/FontInfo.h"

#include "NAS2D/Exception.h"
#include "NAS2D/Filesystem.h"
#include "NAS2D/Utility.h"

#ifdef __APPLE__
#include "SDL2_image/SDL_image.h"
#include "SDL2_ttf/SDL_ttf.h"
#elif _WIN32
#include "GL/glew.h"
#define NO_SDL_GLEXT
#include "SDL_image.h"
#include "SDL_ttf.h"
#else
#include "SDL2/SDL_image.h"
#include "SDL2/SDL_ttf.h"
#endif

#include <iostream>
#include <math.h>

using namespace NAS2D;
using namespace NAS2D::exception;


std::string buildName(TTF_Font*);

const int	ASCII_TABLE_COUNT	= 256;
const int	ASCII_TABLE_FIRST	= 0;
const int	ASCII_TABLE_LAST	= 255;

const int	BITS_32				= 32;


std::map<std::string, FontInfo>	FONTMAP;


extern unsigned int generateTexture(void *buffer, int bytesPerPixel, int width, int height, bool support_24bit = true);

// ==================================================================================
// = UNEXPOSED FUNCTION PROTOTYPES
// ==================================================================================
bool load(const std::string path, unsigned int ptSize, int& height, int& ascent, Point_2d& size);
bool loadBitmap(const std::string& path, int glyphWidth, int glyphHeight, int glyphSpace);
Point_2d generateGlyphMap(TTF_Font* ft, const std::string& name, unsigned int font_size);
bool fontAlreadyLoaded(const std::string& name, unsigned int ptSize);
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
NAS2D::Font::Font(const std::string& filePath, int ptSize) :	Resource(filePath),
																mHeight(0),
																mAscent(0),
																mPtSize(ptSize)
{
	loaded(::load(filePath, ptSize, mHeight, mAscent, mGlyphCellSize));
}


NAS2D::Font::Font(const std::string& filePath, int glyphWidth, int glyphHeight, int glyphSpace) :	Resource(filePath),
																									mHeight(glyphHeight),
																									mAscent(0),
																									mPtSize(glyphHeight),
																									mGlyphCellSize(glyphWidth, glyphHeight)
{
	loaded(loadBitmap(filePath, glyphWidth, glyphHeight, glyphSpace));
}


/**
 * Default c'tor.
 * 
 * Fonts instantiated with this constructor are not valid for use.
 */
NAS2D::Font::Font() :	Resource("Default Font"),
						mHeight(0),
						mAscent(0),
						mPtSize(0)
{}


NAS2D::Font::Font(const Font& _f) : Resource(_f.name())
{
	auto it = FONTMAP.find(name());
	if (it != FONTMAP.end())
	{
		it->second.ref_count++;
	}

	loaded(_f.loaded());
}


NAS2D::Font& NAS2D::Font::operator=(const Font& _f)
{
	auto it = FONTMAP.find(name());
	if (it != FONTMAP.end())
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
	auto it = FONTMAP.find(name());
	if (it == FONTMAP.end())
		return;

	it->second.ref_count--;

	// if texture id reference count is 0, delete the texture.
	if (it->second.ref_count < 1)
	{
		glDeleteTextures(1, &it->second.textureId);
		FONTMAP.erase(it);
	}

	if (FONTMAP.empty())
		TTF_Quit();
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
 * Gets the width in pixels of the Font.
 *
 * \param	str		Reference to a std::string to get the width of.
 */
int NAS2D::Font::width(const std::string& str) const
{
	if (str.empty())
		return 0;

	int width = 0;
	GlyphMetricsList& gml = FONTMAP[name()].metrics;
	for (size_t i = 0; i < str.size(); i++)
	{
		int glyph = clamp(str[i], 0, 255);
		width += gml[glyph].advance + gml[glyph].minX;
	}

	return width;
}


/**
 * Gets the height in pixels of the Font.
 */
int NAS2D::Font::height() const
{
	return mHeight;
}


/**
 * 
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



// ==================================================================================
// = Unexposed module-level functions defined here that don't need to be part of the
// = API interface.
// ==================================================================================

/**
* Loads a Font file.
*
* This is the normal method of loading a Font Resource and is called internally
* by the constructor. It is not publicly exposed and should never be called
* anywhere except the constructor.
*/
bool load(const std::string path, unsigned int ptSize, int& height, int& ascent, Point_2d& size)
{
	if (fontAlreadyLoaded(path, ptSize))
	{
		++FONTMAP[path].ref_count;
		return true;
	}

	if (TTF_WasInit() == 0)
	{
		if (TTF_Init() != 0)
		{
			std::cout << "Font::load(): " << TTF_GetError() << std::endl;
			return false;
		}
	}

	File fontBuffer = Utility<Filesystem>::get().open(path);
	if (fontBuffer.empty())
		return false;

	TTF_Font *font = TTF_OpenFontRW(SDL_RWFromConstMem(fontBuffer.raw_bytes(), static_cast<int>(fontBuffer.size())), 0, ptSize);
	if (!font)
	{
		std::cout << "Font::load(): " << TTF_GetError() << std::endl;
		return false;
	}

	height = TTF_FontHeight(font);
	ascent = TTF_FontAscent(font);

	size = generateGlyphMap(font, path, ptSize);
	TTF_CloseFont(font);

	return true;
}


bool loadBitmap(const std::string& path, int glyphWidth, int glyphHeight, int glyphSpace)
{
	if (fontAlreadyLoaded(path, glyphHeight))
	{
		++FONTMAP[path].ref_count;
		return true;
	}

	// Load specified file and check that it divides to a 16x16 grid.
	File fontBuffer = Utility<Filesystem>::get().open(path);
	if (fontBuffer.empty())
		return false;

	SDL_Surface* glyphMap = IMG_Load_RW(SDL_RWFromConstMem(fontBuffer.raw_bytes(), static_cast<int>(fontBuffer.size())), 0);
	if (!glyphMap)
	{
		std::cout << "Font::loadBitmap(): " << SDL_GetError() << std::endl;
		return false;
	}

	if ((glyphMap->w / 16 != glyphWidth) || (glyphMap->h / 16 != glyphHeight))
	{
		std::cout << "Font::loadBitmap(): Glyph texture does not have 16 columns or 16 rows." << std::endl;
		return false;
	}

	GlyphMetricsList& glm = FONTMAP[path].metrics;
	glm.resize(ASCII_TABLE_COUNT);
	for (size_t i = 0; i < ASCII_TABLE_COUNT; i++)
		glm[i].minX = glyphWidth;

	for (int row = 0; row < 16; row++)
	{
		for (int col = 0; col < 16; col++)
		{
			int glyph = (row * 16) + col;

			glm[glyph].uvX = (float)(col * glyphWidth) / (float)glyphMap->w;
			glm[glyph].uvY = (float)(row * glyphHeight) / (float)glyphMap->h;
			glm[glyph].uvW = glm[glyph].uvX + (float)(glyphWidth) / (float)glyphMap->w;
			glm[glyph].uvH = glm[glyph].uvY + (float)(glyphHeight) / (float)glyphMap->h;
			glm[glyph].advance = glyphSpace;
		}
	}

	unsigned int texture_id = generateTexture(glyphMap->pixels, glyphMap->format->BytesPerPixel, glyphMap->w, glyphMap->h, false);

	// Add generated texture id to texture ID map.
	FONTMAP[path] = FontInfo(texture_id, glyphHeight);
	FONTMAP[path].ref_count++;
	SDL_FreeSurface(glyphMap);

	return true;
}


/**
 * Generates a glyph map of all ASCII standard characters from 0 - 255.
 */
Point_2d generateGlyphMap(TTF_Font* ft, const std::string& name, unsigned int font_size)
{
	int largest_width = 0;

	GlyphMetricsList& glm = FONTMAP[name].metrics;

	// Go through each glyph and determine how much space we need in the texture.
	for (int i = 0; i < ASCII_TABLE_COUNT; i++)
	{
		GlyphMetrics metrics;

		TTF_GlyphMetrics(ft, i, &metrics.minX, &metrics.maxX, &metrics.minY, &metrics.maxY, &metrics.advance);
		if (metrics.advance > largest_width)
			largest_width = metrics.advance;

		if (metrics.minX + metrics.maxX > largest_width)
			largest_width = metrics.minX + metrics.maxX;

		if (metrics.minY + metrics.maxY > largest_width)
			largest_width = metrics.minY + metrics.maxY;

		glm.push_back(metrics);
	}

	Point_2d size(nextPowerOf2(largest_width), nextPowerOf2(largest_width));
	int textureSize = size.x() * 16;

	unsigned int rmask = 0, gmask = 0, bmask = 0, amask = 0;
	setupMasks(rmask, gmask, bmask, amask);

	SDL_Surface* glyphMap = SDL_CreateRGBSurface(SDL_SWSURFACE, textureSize, textureSize, BITS_32, rmask, gmask, bmask, amask);

	SDL_Color white = { 255, 255, 255 };
	for (int row = 0; row < 16; row++)
	{
		for (int col = 0; col < 16; col++)
		{
			int glyph = (row * 16) + col;

			glm[glyph].uvX = (float)(col * size.x()) / (float)textureSize;
			glm[glyph].uvY = (float)(row * size.y()) / (float)textureSize;
			glm[glyph].uvW = glm[glyph].uvX + (float)(size.x()) / (float)textureSize;
			glm[glyph].uvH = glm[glyph].uvY + (float)(size.y()) / (float)textureSize;

			// HACK HACK HACK!
			// Apparently glyph zero has no size with some fonts and so SDL_TTF complains about it.
			// This is here only to prevent the message until I find the time to put in something
			// less bad.
			if (glyph == 0)
				continue;

			SDL_Surface* srf = TTF_RenderGlyph_Blended(ft, glyph, white);
			if (!srf)
			{
				std::cout << "Font::generateGlyphMap(): " << TTF_GetError() << std::endl;
			}
			else
			{
				SDL_SetSurfaceBlendMode(srf, SDL_BLENDMODE_NONE);
				SDL_Rect rect = { col * size.x(), row * size.y(), 0, 0 };
				SDL_BlitSurface(srf, nullptr, glyphMap, &rect);
				SDL_FreeSurface(srf);
			}
		}
	}

	unsigned int texture_id = generateTexture(glyphMap->pixels, glyphMap->format->BytesPerPixel, glyphMap->w, glyphMap->h, false);

	// Add generated texture id to texture ID map.
	FONTMAP[name] = FontInfo(texture_id, font_size);
	FONTMAP[name].ref_count++;
	SDL_FreeSurface(glyphMap);

	return size;
}


bool fontAlreadyLoaded(const std::string& name, unsigned int ptSize)
{
	auto it = FONTMAP.find(name);
	if (it != FONTMAP.end() && it->second.fontSize == ptSize)
	{
		++it->second.ref_count;
		return true;
	}

	return false;
}


void setupMasks(unsigned int& rmask, unsigned int& gmask, unsigned int& bmask, unsigned int& amask)
{
	if (SDL_BYTEORDER == SDL_LIL_ENDIAN)
	{
		rmask = 0x000000ff; gmask = 0x0000ff00; bmask = 0x00ff0000; amask = 0xff000000;
	}
	else
	{
		rmask = 0xff000000; gmask = 0x00ff0000; bmask = 0x0000ff00; amask = 0x000000ff;
	}
}
