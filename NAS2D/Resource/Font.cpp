// ==================================================================================
// = NAS2D
// = Copyright © 2008 - 2020 New Age Software
// ==================================================================================
// = NAS2D is distributed under the terms of the zlib license. You are free to copy,
// = modify and distribute the software under the terms of the zlib license.
// =
// = Acknowledgment of your use of NAS2D is appreciated but is not required.
// ==================================================================================
#include "Font.h"

#include "../Filesystem.h"
#include "../Utility.h"
#include "../Math/MathUtils.h"
#include "../Math/PointInRectangleRange.h"

#if defined(__XCODE_BUILD__)
#include <GLEW/GLEW.h>
#include <SDL2_image/SDL_image.h>
#include <SDL2_ttf/SDL_ttf.h>
#else
#include <GL/glew.h>
#include <SDL2/SDL_image.h>
#include <SDL2/SDL_ttf.h>
#endif

#include <cmath>
#include <algorithm>
#include <cstddef>
#include <stdexcept>
#include <string>


extern unsigned int generateTexture(SDL_Surface* surface);


using namespace NAS2D;


namespace
{
	struct ColorMasks
	{
		unsigned int red;
		unsigned int green;
		unsigned int blue;
		unsigned int alpha;
	};

	constexpr ColorMasks MasksLittleEndian{0x000000ff, 0x0000ff00, 0x00ff0000, 0xff000000};
	constexpr ColorMasks MasksBigEndian{0xff000000, 0x00ff0000, 0x0000ff00, 0x000000ff};
	constexpr ColorMasks MasksDefault = (SDL_BYTEORDER == SDL_LIL_ENDIAN) ? MasksLittleEndian : MasksBigEndian;

	const int ASCII_TABLE_COUNT = 256;
	const int GLYPH_MATRIX_SIZE = 16;
	const int BITS_32 = 32;

	Font::FontInfo load(const std::string& path, unsigned int ptSize);
	Font::FontInfo loadBitmap(const std::string& path);
	unsigned int generateFontTexture(SDL_Surface* fontSurface, std::vector<Font::GlyphMetrics>& glyphMetricsList);
	SDL_Surface* generateFontSurface(TTF_Font* font, Vector<int> characterSize);
	Vector<int> maxCharacterDimensions(TTF_Font* font);
	Vector<int> roundedCharacterDimensions(Vector<int> maxSize);
	void fillInCharacterDimensions(TTF_Font* font, std::vector<Font::GlyphMetrics>& glyphMetricsList);
	void fillInTextureCoordinates(std::vector<Font::GlyphMetrics>& glyphMetricsList);
}


/**
 * Instantiate a Font using a TrueType or OpenType font.
 *
 * \param	filePath	Path to a font file.
 * \param	ptSize		Point size of the font. Defaults to 12pt.
 */
Font::Font(const std::string& filePath, unsigned int ptSize) :
	mFontInfo{load(filePath, ptSize)}
{
}


/**
 * Instantiate a Font as a bitmap font.
 *
 * \param	filePath	Path to a font file.
 */
Font::Font(const std::string& filePath) :
	mFontInfo{loadBitmap(filePath)}
{
}


Font::~Font()
{
	glDeleteTextures(1, &mFontInfo.textureId);
}


Vector<int> Font::glyphCellSize() const
{
	return mFontInfo.glyphSize;
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
	auto& gml = mFontInfo.metrics;
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
	return mFontInfo.height;
}


/**
 * The maximum pixel ascent of all glyphs in the Font.
 */
int Font::ascent() const
{
	return mFontInfo.ascent;
}


/**
 * Returns the point size of the Font.
 */
unsigned int Font::ptSize() const
{
	return mFontInfo.pointSize;
}


const std::vector<Font::GlyphMetrics>& Font::metrics() const
{
	return mFontInfo.metrics;
}


unsigned int Font::textureId() const
{
	return mFontInfo.textureId;
}


namespace
{
	/**
	 * Loads a TrueType or OpenType font from a file.
	 *
	 * \param	path	Path to the TTF or OTF font file.
	 * \param	ptSize	Point size to use when loading the font.
	 */
	Font::FontInfo load(const std::string& path, unsigned int ptSize)
	{
		if (TTF_WasInit() == 0)
		{
			if (TTF_Init() != 0)
			{
				throw std::runtime_error("Font load function failed: " + std::string{TTF_GetError()});
			}
		}

		auto fontBuffer = Utility<Filesystem>::get().readFile(path);
		if (fontBuffer.empty())
		{
			throw std::runtime_error("Font file is empty: " + path);
		}

		auto* font = TTF_OpenFontRW(SDL_RWFromConstMem(fontBuffer.c_str(), static_cast<int>(fontBuffer.size())), 1, static_cast<int>(ptSize));
		if (!font)
		{
			throw std::runtime_error("Font load function failed: " + std::string{TTF_GetError()});
		}

		Font::FontInfo fontInfo;
		auto& glm = fontInfo.metrics;
		fillInCharacterDimensions(font, glm);
		const auto charBoundsSize = maxCharacterDimensions(font);
		const auto roundedCharSize = roundedCharacterDimensions(charBoundsSize);
		SDL_Surface* fontSurface = generateFontSurface(font, roundedCharSize);

		fontInfo.pointSize = ptSize;
		fontInfo.height = TTF_FontHeight(font);
		fontInfo.ascent = TTF_FontAscent(font);
		fontInfo.glyphSize = roundedCharSize;
		fontInfo.textureId = generateFontTexture(fontSurface, glm);
		SDL_FreeSurface(fontSurface);
		TTF_CloseFont(font);

		return fontInfo;
	}


	/**
	 * Internal function that loads a bitmap font from an file.
	 *
	 * \param	path		Path to the image file.
	 */
	Font::FontInfo loadBitmap(const std::string& path)
	{
		auto fontBuffer = Utility<Filesystem>::get().readFile(path);
		if (fontBuffer.empty())
		{
			throw std::runtime_error("Font file is empty: " + path);
		}

		SDL_Surface* fontSurface = IMG_Load_RW(SDL_RWFromConstMem(fontBuffer.c_str(), static_cast<int>(fontBuffer.size())), 1);
		if (!fontSurface)
		{
			throw std::runtime_error("Font loadBitmap function failed: " + std::string{SDL_GetError()});
		}

		// Assume image is square array of equal sized character cells
		const auto fontSurfaceSize = Vector{fontSurface->w, fontSurface->h};
		const auto glyphSize = fontSurfaceSize / GLYPH_MATRIX_SIZE;
		if (fontSurfaceSize != glyphSize * GLYPH_MATRIX_SIZE)
		{
			SDL_FreeSurface(fontSurface);
			const auto vectorToString = [](auto vector) { return "{" + std::to_string(vector.x) + ", " + std::to_string(vector.y) + "}"; };
			throw std::runtime_error("Unexpected font image size. Image dimensions " + vectorToString(fontSurfaceSize) + " must both be evenly divisible by " + std::to_string(GLYPH_MATRIX_SIZE));
		}

		Font::FontInfo fontInfo;
		auto& glm = fontInfo.metrics;
		glm.resize(ASCII_TABLE_COUNT);
		for (auto& metrics : glm)
		{
			metrics.minX = 0;
			metrics.minY = 0;
			metrics.maxX = glyphSize.x;
			metrics.maxY = glyphSize.y;
			metrics.advance = glyphSize.x;
		}

		fontInfo.pointSize = static_cast<unsigned int>(glyphSize.y);
		fontInfo.height = glyphSize.y;
		fontInfo.ascent = glyphSize.y;
		fontInfo.glyphSize = glyphSize;
		fontInfo.textureId = generateFontTexture(fontSurface, glm);
		SDL_FreeSurface(fontSurface);

		return fontInfo;
	}


	/**
	 * Generates a glyph map of all ASCII standard characters from 0 - 255.
	 *
	 * Internal function used to generate a glyph texture map from an TTF_Font struct.
	 */
	unsigned int generateFontTexture(SDL_Surface* fontSurface, std::vector<Font::GlyphMetrics>& glyphMetricsList)
	{
		fillInTextureCoordinates(glyphMetricsList);
		return generateTexture(fontSurface);
	}


	SDL_Surface* generateFontSurface(TTF_Font* font, Vector<int> characterSize)
	{
		const auto matrixSize = characterSize * GLYPH_MATRIX_SIZE;
		SDL_Surface* fontSurface = SDL_CreateRGBSurface(SDL_SWSURFACE, matrixSize.x, matrixSize.y, BITS_32, MasksDefault.red, MasksDefault.green, MasksDefault.blue, MasksDefault.alpha);

		SDL_Color white = {255, 255, 255, 255};
		for (const auto glyphPosition : PointInRectangleRange(Rectangle<std::size_t>{{0, 0}, {GLYPH_MATRIX_SIZE, GLYPH_MATRIX_SIZE}}))
		{
			const std::size_t glyph = glyphPosition.y * GLYPH_MATRIX_SIZE + glyphPosition.x;

			SDL_Surface* characterSurface = TTF_RenderGlyph_Blended(font, static_cast<uint16_t>(glyph), white);
			// A character surface can fail to be created for glyphs of size 0
			if (characterSurface)
			{
				SDL_SetSurfaceBlendMode(characterSurface, SDL_BLENDMODE_NONE);
				const auto pixelPosition = glyphPosition.to<int>().skewBy(characterSize);
				SDL_Rect rect = {pixelPosition.x, pixelPosition.y, 0, 0};
				SDL_BlitSurface(characterSurface, nullptr, fontSurface, &rect);
				SDL_FreeSurface(characterSurface);
			}
		}

		return fontSurface;
	}


	Vector<int> maxCharacterDimensions(TTF_Font* font)
	{
		Vector<int> size{0, 0};

		for (int i = 0; i < 256; ++i)
		{
			Vector<int> sizeChar;
			char text[2] = {static_cast<char>(i), 0};
			TTF_SizeText(font, text, &sizeChar.x, &sizeChar.y);
			size = {std::max({size.x, sizeChar.x}), std::max({size.y, sizeChar.y})};
		}
		return size;
	}


	Vector<int> roundedCharacterDimensions(Vector<int> maxSize)
	{
		const auto maxSizeUint32 = maxSize.to<uint32_t>();
		const auto roundedUpSize = Vector{roundUpPowerOf2(maxSizeUint32.x), roundUpPowerOf2(maxSizeUint32.y)};
		return roundedUpSize.to<int>();
	}


	void fillInCharacterDimensions(TTF_Font* font, std::vector<Font::GlyphMetrics>& glyphMetricsList)
	{
		// Build table of character sizes
		for (Uint16 i = 0; i < ASCII_TABLE_COUNT; i++)
		{
			auto& metrics = glyphMetricsList.emplace_back();
			TTF_GlyphMetrics(font, i, &metrics.minX, &metrics.maxX, &metrics.minY, &metrics.maxY, &metrics.advance);
		}
	}


	void fillInTextureCoordinates(std::vector<Font::GlyphMetrics>& glyphMetricsList)
	{
		const auto uvSize = Vector<float>{1, 1} / 16.0f;
		for (const auto glyphPosition : PointInRectangleRange(Rectangle<std::size_t>{{0, 0}, {GLYPH_MATRIX_SIZE, GLYPH_MATRIX_SIZE}}))
		{
			const std::size_t glyph = glyphPosition.y * GLYPH_MATRIX_SIZE + glyphPosition.x;
			const auto uvStart = glyphPosition.to<float>().skewBy(uvSize);
			glyphMetricsList[glyph].uvRect = Rectangle{uvStart, uvSize};
		}
	}
}
