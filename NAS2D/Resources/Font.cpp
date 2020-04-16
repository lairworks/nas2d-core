// ==================================================================================
// = NAS2D
// = Copyright © 2008 - 2019 New Age Software
// ==================================================================================
// = NAS2D is distributed under the terms of the zlib license. You are free to copy,
// = modify and distribute the software under the terms of the zlib license.
// =
// = Acknowledgement of your use of NAS2D is appriciated but is not required.
// ==================================================================================
#include "Font.h"
#include "FontInfo.h"

#include "../Exception.h"
#include "../Filesystem.h"
#include "../Utility.h"
#include "../MathUtils.h"

#include <GL/glew.h>

#include <SDL2/SDL_image.h>
#include <SDL2/SDL_ttf.h>

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


std::map<std::string, FontInfo>	fontMap;


extern unsigned int generateTexture(void *buffer, int bytesPerPixel, int width, int height);

// ==================================================================================
// = UNEXPOSED FUNCTION PROTOTYPES
// ==================================================================================
bool load(const std::string& path, unsigned int ptSize);
bool loadBitmap(const std::string& path, int glyphWidth, int glyphHeight, int glyphSpace);
Vector<int> generateGlyphMap(TTF_Font* ft, const std::string& name, unsigned int font_size);
bool fontAlreadyLoaded(const std::string& name);
void setupMasks(unsigned int& rmask, unsigned int& gmask, unsigned int& bmask, unsigned int& amask);
void updateFontReferenceCount(const std::string& name);


/**
 * Instantiate a Font using a TrueType or OpenType font.
 *
 * \param	filePath	Path to a font file.
 * \param	ptSize		Point size of the font. Defaults to 12pt.
 *
 */
NAS2D::Font::Font(const std::string& filePath, unsigned int ptSize) :	Resource(filePath)
{
	loaded(::load(name(), ptSize));
	name(name() + "_" + std::to_string(ptSize) + "pt");
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
	auto it = fontMap.find(name());
	if (it != fontMap.end())
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

	auto it = fontMap.find(name());
	if (it == fontMap.end()) { throw font_bad_data(); }

	++it->second.ref_count;
	loaded(rhs.loaded());

	return *this;
}


/**
 * Gets the glyph cell width.
 */
int NAS2D::Font::glyphCellWidth() const
{
	return fontMap[name()].glyph_size.x;
}


/**
 * Gets the glyph cell height.
 */
int NAS2D::Font::glyphCellHeight() const
{
	return fontMap[name()].glyph_size.y;
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
	GlyphMetricsList& gml = fontMap[name()].metrics;
	if (gml.empty()) { return 0; }

	for (auto character : str)
	{
		auto glyph = std::clamp<std::size_t>(static_cast<uint8_t>(character), 0, 255);
		width += gml[glyph].advance + gml[glyph].minX;
	}

	return width;
}


/**
 * Gets the height in pixels of the Font.
 */
int NAS2D::Font::height() const
{
	return fontMap[name()].height;
}


/**
 * The maximum pixel ascent of all glyphs in the Font.
 */
int NAS2D::Font::ascent() const
{
	return fontMap[name()].ascent;
}


/**
 * Returns the point size of the Font.
 */
unsigned int NAS2D::Font::ptSize() const
{
	return fontMap[name()].pt_size;
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
	std::string fontname = path + "_" + std::to_string(ptSize) + "pt";
	if (fontAlreadyLoaded(fontname))
	{
		++fontMap[fontname].ref_count;
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

	TTF_Font *font = TTF_OpenFontRW(SDL_RWFromConstMem(fontBuffer.raw_bytes(), static_cast<int>(fontBuffer.size())), 0, static_cast<int>(ptSize));
	if (!font)
	{
		std::cout << "Font::load(): " << TTF_GetError() << std::endl;
		return false;
	}

	fontMap[fontname].height = TTF_FontHeight(font);
	fontMap[fontname].ascent = TTF_FontAscent(font);
	fontMap[fontname].glyph_size = generateGlyphMap(font, fontname, ptSize);
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
		++fontMap[path].ref_count;
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
		throw font_invalid_glyph_map("image width is " + std::to_string(glyphMap->w) + ", expected " + std::to_string(glyphWidth * GLYPH_MATRIX_SIZE) + ".");
	}

	if (glyphMap->h / GLYPH_MATRIX_SIZE != glyphHeight)
	{
		throw font_invalid_glyph_map("image height is " + std::to_string(glyphMap->h) + ", expected " + std::to_string(glyphHeight * GLYPH_MATRIX_SIZE) + ".");
	}

	GlyphMetricsList& glm = fontMap[path].metrics;
	glm.resize(ASCII_TABLE_COUNT);
	for (std::size_t i = 0; i < glm.size(); ++i)
	{
		glm[i].minX = glyphWidth;
	}

	for (std::size_t row = 0; row < GLYPH_MATRIX_SIZE; row++)
	{
		for (std::size_t col = 0; col < GLYPH_MATRIX_SIZE; col++)
		{
			const std::size_t glyph = (row * GLYPH_MATRIX_SIZE) + col;

			glm[glyph].uvX = static_cast<float>(col * glyphWidth) / static_cast<float>(glyphMap->w);
			glm[glyph].uvY = static_cast<float>(row * glyphHeight) / static_cast<float>(glyphMap->h);
			glm[glyph].uvW = glm[glyph].uvX + static_cast<float>(glyphWidth) / static_cast<float>(glyphMap->w);
			glm[glyph].uvH = glm[glyph].uvY + static_cast<float>(glyphHeight) / static_cast<float>(glyphMap->h);
			glm[glyph].advance = glyphSpace;
		}
	}

	unsigned int texture_id = generateTexture(glyphMap->pixels, glyphMap->format->BytesPerPixel, glyphMap->w, glyphMap->h);

	// Add generated texture id to texture ID map.
	fontMap[path].texture_id = texture_id;
	fontMap[path].pt_size = glyphHeight;
	fontMap[path].height = glyphHeight;
	fontMap[path].ref_count++;
	fontMap[path].glyph_size = {glyphWidth, glyphHeight};
	SDL_FreeSurface(glyphMap);

	return true;
}


/**
 * Generates a glyph map of all ASCII standard characters from 0 - 255.
 *
 * Internal function used to generate a glyph texture map from an TTF_Font struct.
 */
Vector<int> generateGlyphMap(TTF_Font* ft, const std::string& name, unsigned int font_size)
{
	int largest_width = 0;

	GlyphMetricsList& glm = fontMap[name].metrics;

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

	const auto roundedLongestEdge = roundUpPowerOf2(static_cast<uint32_t>(largest_width));
	const auto size = Vector{roundedLongestEdge, roundedLongestEdge}.to<int>();
	int textureSize = size.x * GLYPH_MATRIX_SIZE;

	unsigned int rmask = 0, gmask = 0, bmask = 0, amask = 0;
	setupMasks(rmask, gmask, bmask, amask);

	SDL_Surface* glyphMap = SDL_CreateRGBSurface(SDL_SWSURFACE, textureSize, textureSize, BITS_32, rmask, gmask, bmask, amask);

	SDL_Color white = { 255, 255, 255, 255 };
	for (int row = 0; row < 16; row++)
	{
		for (int col = 0; col < GLYPH_MATRIX_SIZE; col++)
		{
			std::size_t glyph = static_cast<std::size_t>((row * GLYPH_MATRIX_SIZE) + col);

			glm[glyph].uvX = static_cast<float>(col * size.x) / static_cast<float>(textureSize);
			glm[glyph].uvY = static_cast<float>(row * size.y) / static_cast<float>(textureSize);
			glm[glyph].uvW = glm[glyph].uvX + static_cast<float>(size.x) / static_cast<float>(textureSize);
			glm[glyph].uvH = glm[glyph].uvY + static_cast<float>(size.y) / static_cast<float>(textureSize);

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
				SDL_Rect rect = { col * size.x, row * size.y, 0, 0 };
				SDL_BlitSurface(srf, nullptr, glyphMap, &rect);
				SDL_FreeSurface(srf);
			}
		}
	}

	unsigned int texture_id = generateTexture(glyphMap->pixels, glyphMap->format->BytesPerPixel, glyphMap->w, glyphMap->h);

	// Add generated texture id to texture ID map.
	fontMap[name].texture_id = texture_id;
	fontMap[name].pt_size = font_size;
	fontMap[name].ref_count++;
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
	auto it = fontMap.find(name);
	if (it != fontMap.end())
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
	auto it = fontMap.find(name);
	if (it == fontMap.end())
	{
		std::cout << "Font '" << name << "' was not found in the resource management." << std::endl;
		return;
	}

	--it->second.ref_count;

	// if texture id reference count is 0, delete the texture.
	if (it->second.ref_count < 1)
	{
		glDeleteTextures(1, &it->second.texture_id);
		fontMap.erase(it);
	}

	if (fontMap.empty())
	{
		TTF_Quit();
	}
}
