// ==================================================================================
// = NAS2D
// = Copyright © 2008 - 2019 New Age Software
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

#include <GL/glew.h>

#include <SDL_image.h>
#include <SDL_ttf.h>

#include <iostream>
#include <cmath>
#include <algorithm>
#include <cstddef>

using namespace NAS2D;
using namespace NAS2D::Exception;


std::string buildName(TTF_Font*);

const int	ASCII_TABLE_FIRST	= 0;
const int	ASCII_TABLE_LAST	= 255;
const int	ASCII_TABLE_COUNT	= ASCII_TABLE_LAST - ASCII_TABLE_FIRST + 1;

const int	GLYPH_MATRIX_SIZE	= 16;

const int	BITS_32				= 32;


std::map<std::string, FontInfo>	FONTMAP;


extern unsigned int generateTexture(void *buffer, int bytesPerPixel, int width, int height);

// ==================================================================================
// = UNEXPOSED FUNCTION PROTOTYPES
// ==================================================================================
bool load(const std::string& path, unsigned int ptSize);
bool loadBitmap(const std::string& path, int glyphWidth, int glyphHeight, int glyphSpace);
Point_2d generateGlyphMap(TTF_Font* ft, const std::string& name, unsigned int font_size);
bool fontAlreadyLoaded(const std::string& name);
void setupMasks(unsigned int& rmask, unsigned int& gmask, unsigned int& bmask, unsigned int& amask);
void updateFontReferenceCount(const std::string& name);


unsigned nextPowerOf2(unsigned n)
{
	return (unsigned)pow(2, ceil(log((float)n) / log(2.0f)));
}


/**
 * Instantiate a Font using a TrueType or OpenType font.
 *
 * \param	filePath	Path to a font file.
 * \param	ptSize		Point size of the font. Defaults to 12pt.
 *
 */
NAS2D::Font::Font(const std::string& filePath, int ptSize) :	Resource(filePath)
{
	loaded(::load(name(), ptSize));
	name(name() + string_format("_%ipt", ptSize));
}


/**
 * Instantiate a Font as a bitmap font.
 *
 * \param	filePath	Path to a font file.
 * \param	glyphWidth	Width of glyphs in the bitmap Font.
 * \param	glyphHeight	Height of glyphs in the bitmap Font.
 * \param	glyphSpace	Space between glyphs when rendering a bitmap font. This value can be negative.
 *
 */
NAS2D::Font::Font(const std::string& filePath, int glyphWidth, int glyphHeight, int glyphSpace) :	Resource(filePath)
{
	loaded(loadBitmap(filePath, glyphWidth, glyphHeight, glyphSpace));
}


/**
 * Default c'tor.
 *
 * Fonts instantiated with this constructor are not valid for use.
 */
NAS2D::Font::Font() :	Resource("Default Font")
{}


/**
 * Copy c'tor.
 *
 * \param	rhs	Font to copy.
 */
NAS2D::Font::Font(const Font& rhs) : Resource(rhs.name())
{
	auto it = FONTMAP.find(name());
	if (it != FONTMAP.end())
	{
		++it->second.ref_count;
		loaded(rhs.loaded());
	}
	else
	{
		loaded(false);
	}
}


/**
* D'tor
*/
NAS2D::Font::~Font()
{
	updateFontReferenceCount(name());
}


/**
 * Copy assignment operator.
 *
 * \param rhs Font to copy.
 */
NAS2D::Font& NAS2D::Font::operator=(const Font& rhs)
{
	if (this == &rhs) { return *this; }

	updateFontReferenceCount(name());

	name(rhs.name());

	auto it = FONTMAP.find(name());
	if (it == FONTMAP.end()) { throw font_bad_data(); }

	++it->second.ref_count;
	loaded(rhs.loaded());

	return *this;
}


/**
 * Gets the glyph cell width.
 */
const int NAS2D::Font::glyphCellWidth() const
{
	return FONTMAP[name()].glyph_size.x();
}


/**
 * Gets the glyph cell height.
 */
const int NAS2D::Font::glyphCellHeight() const
{
	return FONTMAP[name()].glyph_size.y();
}


/**
 * Gets the width in pixels of a string rendered using the Font.
 *
 * \param	str		Reference to a std::string to get the width of.
 */
int NAS2D::Font::width(const std::string& str) const
{
	if (str.empty()) { return 0; }

	int width = 0;
	GlyphMetricsList& gml = FONTMAP[name()].metrics;
	if (gml.empty()) { return 0; }

	for (size_t i = 0; i < str.size(); i++)
	{
		auto glyph = std::clamp<std::size_t>(str[i], 0, 255);
		width += gml[glyph].advance + gml[glyph].minX;
	}

	return width;
}


/**
 * Gets the height in pixels of the Font.
 */
int NAS2D::Font::height() const
{
	return FONTMAP[name()].height;
}


/**
 * The maximum pixel ascent of all glyphs in the Font.
 */
int NAS2D::Font::ascent() const
{
	return FONTMAP[name()].ascent;
}


/**
 * Returns the point size of the Font.
 */
int NAS2D::Font::ptSize() const
{
	return FONTMAP[name()].pt_size;
}


// ==================================================================================
// = Unexposed module-level functions defined here that don't need to be part of the
// = API interface.
// ==================================================================================

/**
 * Loads a TrueType or OpenType font from a file.
 *
 * \param	path	Path to the TTF or OTF font file.
 * \param	ptSize	Point size to use when loading the font.
 */
bool load(const std::string& path, unsigned int ptSize)
{
	std::string fontname = path + string_format("_%ipt", ptSize);
	if (fontAlreadyLoaded(fontname))
	{
		++FONTMAP[fontname].ref_count;
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
	{
		return false;
	}

	TTF_Font *font = TTF_OpenFontRW(SDL_RWFromConstMem(fontBuffer.raw_bytes(), static_cast<int>(fontBuffer.size())), 0, ptSize);
	if (!font)
	{
		std::cout << "Font::load(): " << TTF_GetError() << std::endl;
		return false;
	}

	FONTMAP[fontname].height = TTF_FontHeight(font);
	FONTMAP[fontname].ascent = TTF_FontAscent(font);
	FONTMAP[fontname].glyph_size = generateGlyphMap(font, fontname, ptSize);
	TTF_CloseFont(font);

	return true;
}


/**
 * Internal function that loads a bitmap font from an file.
 *
 * \param	path		Path to the image file.
 * \param	glyphWidth	Width of glyphs in the bitmap font.
 * \param	glyphHeight	Height of the glyphs in the bitmap font.
 * \param	glyphSpace	Spacing to use when drawing glyphs.
 */
bool loadBitmap(const std::string& path, int glyphWidth, int glyphHeight, int glyphSpace)
{
	if (fontAlreadyLoaded(path))
	{
		++FONTMAP[path].ref_count;
		return true;
	}

	File fontBuffer = Utility<Filesystem>::get().open(path);
	if (fontBuffer.empty())
	{
		return false;
	}

	SDL_Surface* glyphMap = IMG_Load_RW(SDL_RWFromConstMem(fontBuffer.raw_bytes(), static_cast<int>(fontBuffer.size())), 0);
	if (!glyphMap)
	{
		std::cout << "Font::loadBitmap(): " << SDL_GetError() << std::endl;
		return false;
	}

	if (glyphMap->w / GLYPH_MATRIX_SIZE != glyphWidth)
	{
		throw font_invalid_glyph_map(string_format("image width is %i, expected %i.", glyphMap->w, glyphWidth * GLYPH_MATRIX_SIZE));
	}

	if (glyphMap->h / GLYPH_MATRIX_SIZE != glyphHeight)
	{
		throw font_invalid_glyph_map(string_format("image height is %i, expected %i.", glyphMap->h, glyphHeight * GLYPH_MATRIX_SIZE));
	}

	GlyphMetricsList& glm = FONTMAP[path].metrics;
	glm.resize(ASCII_TABLE_COUNT);
	for (size_t i = 0; i < glm.size(); ++i)
	{
		glm[i].minX = glyphWidth;
	}

	for (int row = 0; row < GLYPH_MATRIX_SIZE; row++)
	{
		for (int col = 0; col < GLYPH_MATRIX_SIZE; col++)
		{
			const std::size_t glyph = (row * GLYPH_MATRIX_SIZE) + col;

			glm[glyph].uvX = (float)(col * glyphWidth) / (float)glyphMap->w;
			glm[glyph].uvY = (float)(row * glyphHeight) / (float)glyphMap->h;
			glm[glyph].uvW = glm[glyph].uvX + (float)(glyphWidth) / (float)glyphMap->w;
			glm[glyph].uvH = glm[glyph].uvY + (float)(glyphHeight) / (float)glyphMap->h;
			glm[glyph].advance = glyphSpace;
		}
	}

	unsigned int texture_id = generateTexture(glyphMap->pixels, glyphMap->format->BytesPerPixel, glyphMap->w, glyphMap->h);

	// Add generated texture id to texture ID map.
	FONTMAP[path].texture_id = texture_id;
	FONTMAP[path].pt_size = glyphHeight;
	FONTMAP[path].height = glyphHeight;
	FONTMAP[path].ref_count++;
	FONTMAP[path].glyph_size = {glyphWidth, glyphHeight};
	SDL_FreeSurface(glyphMap);

	return true;
}


/**
 * Generates a glyph map of all ASCII standard characters from 0 - 255.
 *
 * Internal function used to generate a glyph texture map from an TTF_Font struct.
 */
Point_2d generateGlyphMap(TTF_Font* ft, const std::string& name, unsigned int font_size)
{
	int largest_width = 0;

	GlyphMetricsList& glm = FONTMAP[name].metrics;

	// Go through each glyph and determine how much space we need in the texture.
	for (Uint16 i = 0; i < ASCII_TABLE_COUNT; i++)
	{
		GlyphMetrics metrics;

		TTF_GlyphMetrics(ft, i, &metrics.minX, &metrics.maxX, &metrics.minY, &metrics.maxY, &metrics.advance);
		if (metrics.advance > largest_width)
		{
			largest_width = metrics.advance;
		}

		if (metrics.minX + metrics.maxX > largest_width)
		{
			largest_width = metrics.minX + metrics.maxX;
		}

		if (metrics.minY + metrics.maxY > largest_width)
		{
			largest_width = metrics.minY + metrics.maxY;
		}

		glm.push_back(metrics);
	}

	Point_2d size(nextPowerOf2(largest_width), nextPowerOf2(largest_width));
	int textureSize = size.x() * GLYPH_MATRIX_SIZE;

	unsigned int rmask = 0, gmask = 0, bmask = 0, amask = 0;
	setupMasks(rmask, gmask, bmask, amask);

	SDL_Surface* glyphMap = SDL_CreateRGBSurface(SDL_SWSURFACE, textureSize, textureSize, BITS_32, rmask, gmask, bmask, amask);

	SDL_Color white = { 255, 255, 255 };
	for (int row = 0; row < 16; row++)
	{
		for (int col = 0; col < GLYPH_MATRIX_SIZE; col++)
		{
			int glyph = (row * GLYPH_MATRIX_SIZE) + col;

			glm[glyph].uvX = (float)(col * size.x()) / (float)textureSize;
			glm[glyph].uvY = (float)(row * size.y()) / (float)textureSize;
			glm[glyph].uvW = glm[glyph].uvX + (float)(size.x()) / (float)textureSize;
			glm[glyph].uvH = glm[glyph].uvY + (float)(size.y()) / (float)textureSize;

			// HACK HACK HACK!
			// Apparently glyph zero has no size with some fonts and so SDL_TTF complains about it.
			// This is here only to prevent the message until I find the time to put in something
			// less bad.
			if (glyph == 0) { continue; }

			SDL_Surface* srf = TTF_RenderGlyph_Blended(ft, static_cast<uint16_t>(glyph), white);
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

	unsigned int texture_id = generateTexture(glyphMap->pixels, glyphMap->format->BytesPerPixel, glyphMap->w, glyphMap->h);

	// Add generated texture id to texture ID map.
	FONTMAP[name].texture_id = texture_id;
	FONTMAP[name].pt_size = font_size;
	FONTMAP[name].ref_count++;
	SDL_FreeSurface(glyphMap);

	return size;
}


/**
 * Internal utility function used to test if a given Font has already
 * been loaded.
 *
 * \param	name	Name of the Font to check against.
 */
bool fontAlreadyLoaded(const std::string& name)
{
	auto it = FONTMAP.find(name);
	if (it != FONTMAP.end())
	{
		return true;
	}

	return false;
}


/**
 * Sets up image masks for generating OpenGL textures based on machine endianness.
 */
void setupMasks(unsigned int& rmask, unsigned int& gmask, unsigned int& bmask, unsigned int& amask)
{
	if constexpr (SDL_BYTEORDER == SDL_LIL_ENDIAN)
	{
		rmask = 0x000000ff; gmask = 0x0000ff00; bmask = 0x00ff0000; amask = 0xff000000;
	}
	else
	{
		rmask = 0xff000000; gmask = 0x00ff0000; bmask = 0x0000ff00; amask = 0x000000ff;
	}
}


/**
 * Internal function used to clean up references to fonts when the Font
 * destructor or copy assignment operators are called.
 *
 * \param	name	Name of the Font to check against.
 */
void updateFontReferenceCount(const std::string& name)
{
	auto it = FONTMAP.find(name);
	if (it == FONTMAP.end())
	{
		std::cout << "Font '" << name << "' was not found in the resource management." << std::endl;
		return;
	}

	--it->second.ref_count;

	// if texture id reference count is 0, delete the texture.
	if (it->second.ref_count < 1)
	{
		glDeleteTextures(1, &it->second.texture_id);
		FONTMAP.erase(it);
	}

	if (FONTMAP.empty())
	{
		TTF_Quit();
	}
}
