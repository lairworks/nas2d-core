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
#include "../Renderer/PointInRectangleRange.h"

#include <GL/glew.h>

#include <SDL2/SDL_image.h>
#include <SDL2/SDL_ttf.h>

#include <iostream>
#include <cmath>
#include <algorithm>
#include <cstddef>
#include <stdexcept>


extern unsigned int generateTexture(SDL_Surface* surface);


using namespace NAS2D;
using namespace NAS2D::Exception;


std::map<std::string, FontInfo> fontMap;


namespace {
	const int ASCII_TABLE_COUNT = 256;
	const int GLYPH_MATRIX_SIZE = 16;
	const int BITS_32 = 32;

	bool load(const std::string& path, unsigned int ptSize);
	bool loadBitmap(const std::string& path, int glyphWidth, int glyphHeight, int glyphSpace);
	Vector<int> generateGlyphMap(TTF_Font* ft, const std::string& name, unsigned int fontSize);
	Vector<int> maxCharacterDimensions(const GlyphMetricsList& glyphMetricsList);
	void fillInTextureCoordinates(GlyphMetricsList& glyphMetricsList, Vector<int> characterSize, Vector<int> textureSize);
	bool fontAlreadyLoaded(const std::string& name);
	void setupMasks(unsigned int& rmask, unsigned int& gmask, unsigned int& bmask, unsigned int& amask);
	void updateFontReferenceCount(const std::string& name);
}


/**
 * Instantiate a Font using a TrueType or OpenType font.
 *
 * \param	filePath	Path to a font file.
 * \param	ptSize		Point size of the font. Defaults to 12pt.
 *
 */
Font::Font(const std::string& filePath, unsigned int ptSize) : Resource(filePath)
{
	loaded(::load(filePath, ptSize));
	mResourceName = filePath + "_" + std::to_string(ptSize) + "pt";
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
Font::Font(const std::string& filePath, int glyphWidth, int glyphHeight, int glyphSpace) : Resource(filePath)
{
	loaded(loadBitmap(filePath, glyphWidth, glyphHeight, glyphSpace));
}


/**
 * Copy c'tor.
 *
 * \param	rhs	Font to copy.
 */
Font::Font(const Font& rhs) : Resource(rhs.mResourceName)
{
	auto it = fontMap.find(mResourceName);
	if (it != fontMap.end())
	{
		++it->second.refCount;
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
Font::~Font()
{
	updateFontReferenceCount(mResourceName);
}


/**
 * Copy assignment operator.
 *
 * \param rhs Font to copy.
 */
Font& Font::operator=(const Font& rhs)
{
	if (this == &rhs) { return *this; }

	updateFontReferenceCount(mResourceName);

	mResourceName = rhs.mResourceName;

	auto it = fontMap.find(mResourceName);
	if (it == fontMap.end()) { throw font_bad_data(); }

	++it->second.refCount;
	loaded(rhs.loaded());

	return *this;
}


Vector<int> Font::glyphCellSize() const
{
	return fontMap[mResourceName].glyphSize;
}


Vector<int> Font::size(std::string_view string) const
{
	return {width(string), height()};
}


/**
 * Gets the width in pixels of a string rendered using the Font.
 *
 * \param	string		String to get the width of.
 */
int Font::width(std::string_view string) const
{
	if (string.empty()) { return 0; }

	int width = 0;
	GlyphMetricsList& gml = fontMap[mResourceName].metrics;
	if (gml.empty()) { return 0; }

	for (auto character : string)
	{
		auto glyph = std::clamp<std::size_t>(static_cast<uint8_t>(character), 0, 255);
		width += gml[glyph].advance + gml[glyph].minX;
	}

	return width;
}


/**
 * Gets the height in pixels of the Font.
 */
int Font::height() const
{
	return fontMap[mResourceName].height;
}


/**
 * The maximum pixel ascent of all glyphs in the Font.
 */
int Font::ascent() const
{
	return fontMap[mResourceName].ascent;
}


/**
 * Returns the point size of the Font.
 */
unsigned int Font::ptSize() const
{
	return fontMap[mResourceName].pointSize;
}


namespace {
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
			++fontMap[fontname].refCount;
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
		fontMap[fontname].glyphSize = generateGlyphMap(font, fontname, ptSize);
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
			++fontMap[path].refCount;
			return true;
		}

		File fontBuffer = Utility<Filesystem>::get().open(path);
		if (fontBuffer.empty())
		{
			return false;
		}

		SDL_Surface* fontSurface = IMG_Load_RW(SDL_RWFromConstMem(fontBuffer.raw_bytes(), static_cast<int>(fontBuffer.size())), 0);
		if (!fontSurface)
		{
			std::cout << "Font::loadBitmap(): " << SDL_GetError() << std::endl;
			return false;
		}

		const auto fontSurfaceSize = Vector{fontSurface->w, fontSurface->h};
		const auto glyphLayoutSize = Vector{GLYPH_MATRIX_SIZE, GLYPH_MATRIX_SIZE};
		const auto glyphSize = Vector{glyphWidth, glyphHeight};
		const auto expectedSize = glyphLayoutSize.skewBy(glyphSize);
		if (fontSurfaceSize != expectedSize)
		{
			const auto vectorToString = [](auto vector) { return "{" + std::to_string(vector.x) + ", " + std::to_string(vector.y) + "}"; };
			throw font_invalid_glyph_map("Unexpected font image size. Expected: " + vectorToString(expectedSize) + " Actual: " + vectorToString(fontSurfaceSize));
		}

		auto& fontInfo = fontMap[path];
		auto& glm = fontInfo.metrics;
		glm.resize(ASCII_TABLE_COUNT);
		for (auto& metrics : glm)
		{
			metrics.minX = glyphSize.x;
			metrics.advance = glyphSpace;
		}
		fillInTextureCoordinates(glm, glyphSize, fontSurfaceSize);

		unsigned int textureId = generateTexture(fontSurface);

		// Add generated texture id to texture ID map.
		fontInfo.textureId = textureId;
		fontInfo.pointSize = static_cast<unsigned int>(glyphSize.y);
		fontInfo.height = glyphSize.y;
		fontInfo.refCount++;
		fontInfo.glyphSize = glyphSize;
		SDL_FreeSurface(fontSurface);

		return true;
	}


	/**
	 * Generates a glyph map of all ASCII standard characters from 0 - 255.
	 *
	 * Internal function used to generate a glyph texture map from an TTF_Font struct.
	 */
	Vector<int> generateGlyphMap(TTF_Font* ft, const std::string& name, unsigned int fontSize)
	{
		GlyphMetricsList& glm = fontMap[name].metrics;

		// Build table of character sizes
		for (Uint16 i = 0; i < ASCII_TABLE_COUNT; i++)
		{
			GlyphMetrics metrics;
			TTF_GlyphMetrics(ft, i, &metrics.minX, &metrics.maxX, &metrics.minY, &metrics.maxY, &metrics.advance);
			glm.push_back(metrics);
		}

		const auto charBoundsSize = maxCharacterDimensions(glm);
		const auto longestEdge = std::max(charBoundsSize.x, charBoundsSize.y);
		const auto roundedLongestEdge = static_cast<int>(roundUpPowerOf2(static_cast<uint32_t>(longestEdge)));
		const auto size = Vector{roundedLongestEdge, roundedLongestEdge};
		const auto textureSize = size.x * GLYPH_MATRIX_SIZE;

		unsigned int rmask = 0, gmask = 0, bmask = 0, amask = 0;
		setupMasks(rmask, gmask, bmask, amask);

		SDL_Surface* fontSurface = SDL_CreateRGBSurface(SDL_SWSURFACE, textureSize, textureSize, BITS_32, rmask, gmask, bmask, amask);

		fillInTextureCoordinates(glm, size, {textureSize, textureSize});

		SDL_Color white = { 255, 255, 255, 255 };
		for (const auto glyphPosition : PointInRectangleRange(Rectangle{0, 0, GLYPH_MATRIX_SIZE, GLYPH_MATRIX_SIZE}))
		{
			const std::size_t glyph = static_cast<std::size_t>(glyphPosition.y) * GLYPH_MATRIX_SIZE + glyphPosition.x;

			// Avoid glyph 0, which has size 0 for some fonts
			// SDL_TTF will produce errors for a glyph of size 0
			if (glyph == 0) { continue; }

			SDL_Surface* characterSurface = TTF_RenderGlyph_Blended(ft, static_cast<uint16_t>(glyph), white);
			if (!characterSurface)
			{
				throw std::runtime_error("Font::generateGlyphMap(): " + std::string(TTF_GetError()));
			}

			SDL_SetSurfaceBlendMode(characterSurface, SDL_BLENDMODE_NONE);
			const auto pixelPosition = glyphPosition.skewBy(size);
			SDL_Rect rect = { pixelPosition.x, pixelPosition.y, 0, 0 };
			SDL_BlitSurface(characterSurface, nullptr, fontSurface, &rect);
			SDL_FreeSurface(characterSurface);
		}

		unsigned int textureId = generateTexture(fontSurface);

		// Add generated texture id to texture ID map.
		fontMap[name].textureId = textureId;
		fontMap[name].pointSize = fontSize;
		fontMap[name].refCount++;
		SDL_FreeSurface(fontSurface);

		return size;
	}


	Vector<int> maxCharacterDimensions(const GlyphMetricsList& glyphMetricsList)
	{
		Vector<int> size{0, 0};

		for (const auto metrics : glyphMetricsList)
		{
			size.x = std::max({size.x, metrics.minX + metrics.maxX, metrics.advance});
			size.y = std::max({size.y, metrics.minY + metrics.maxY});
		}
		return size;
	}


	void fillInTextureCoordinates(GlyphMetricsList& glyphMetricsList, Vector<int> characterSize, Vector<int> textureSize)
	{
		const auto floatTextureSize = textureSize.to<float>();
		const auto uvSize = characterSize.to<float>().skewInverseBy(floatTextureSize);
		for (const auto glyphPosition : PointInRectangleRange(Rectangle{0, 0, GLYPH_MATRIX_SIZE, GLYPH_MATRIX_SIZE}))
		{
			const std::size_t glyph = static_cast<std::size_t>(glyphPosition.y) * GLYPH_MATRIX_SIZE + glyphPosition.x;
			const auto uvStart = glyphPosition.skewBy(characterSize).to<float>().skewInverseBy(floatTextureSize);
			glyphMetricsList[glyph].uvRect = Rectangle<float>::Create(uvStart, uvSize);
		}
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

		auto& fontInfo = it->second;
		--fontInfo.refCount;
		if (fontInfo.refCount <= 0)
		{
			glDeleteTextures(1, &fontInfo.textureId);
			fontMap.erase(it);
		}

		if (fontMap.empty())
		{
			TTF_Quit();
		}
	}
}
