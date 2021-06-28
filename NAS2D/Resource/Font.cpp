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


namespace {
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

	//TODO: Rename loadFromFont?
	Font::FontInfo load(const std::string& path, unsigned int ptSize);
	//TODO: Rename loadFromBitmap?
	Font::FontInfo loadBitmap(const std::string& path);
	unsigned int generateFontTexture(SDL_Surface* fontSurface, std::vector<Font::GlyphMetrics>& glyphMetricsList);
	SDL_Surface* generateFontSurface(TTF_Font* font, Vector<int> characterSize);
	Vector<int> maxCharacterDimensions(TTF_Font* font);
	Vector<int> roundedCharacterDimensions(Vector<int> maxSize);
	void fillInCharacterDimensions(TTF_Font* font, std::vector<Font::GlyphMetrics>& glyphMetricsList);
	void fillInTextureCoordinates(std::vector<Font::GlyphMetrics>& glyphMetricsList);
}

Font::Font(const std::string& filePath, unsigned int ptSize) :
	//TODO: Hijacking the filepath is probably a bad idea.
	mResourceName{filePath + "_" + std::to_string(ptSize) + "pt"},
	mFontInfo{load(filePath, ptSize)}
{
}

Font::Font(const std::string& filePath) :
	mResourceName{filePath},
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

int Font::height() const
{
	return mFontInfo.height;
}

int Font::ascent() const
{
	return mFontInfo.ascent;
}

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


namespace {
	Font::FontInfo load(const std::string& path, unsigned int ptSize)
	{
		std::string fontname = path + "_" + std::to_string(ptSize) + "pt";

		if (TTF_WasInit() == 0)
		{
			if (TTF_Init() != 0)
			{
				throw std::runtime_error("Font load function failed: " + std::string{TTF_GetError()});
			}
		}

		auto fontBuffer = Utility<Filesystem>::get().open(path);
		if (fontBuffer.empty())
		{
			throw std::runtime_error("Font file is empty: " + path);
		}

		TTF_Font *font = TTF_OpenFontRW(SDL_RWFromConstMem(fontBuffer.raw_bytes(), static_cast<int>(fontBuffer.size())), 1, static_cast<int>(ptSize));
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

	Font::FontInfo loadBitmap(const std::string& path)
	{
		auto fontBuffer = Utility<Filesystem>::get().open(path);
		if (fontBuffer.empty())
		{
			throw std::runtime_error("Font file is empty: " + path);
		}

		SDL_Surface* fontSurface = IMG_Load_RW(SDL_RWFromConstMem(fontBuffer.raw_bytes(), static_cast<int>(fontBuffer.size())), 1);
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
			throw font_invalid_glyph_map("Unexpected font image size. Image dimensions " + vectorToString(fontSurfaceSize) + " must both be evenly divisble by " + std::to_string(GLYPH_MATRIX_SIZE));
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

	unsigned int generateFontTexture(SDL_Surface* fontSurface, std::vector<Font::GlyphMetrics>& glyphMetricsList)
	{
		fillInTextureCoordinates(glyphMetricsList);
		return generateTexture(fontSurface);
	}


	SDL_Surface* generateFontSurface(TTF_Font* font, Vector<int> characterSize)
	{
		const auto matrixSize = characterSize * GLYPH_MATRIX_SIZE;
		SDL_Surface* fontSurface = SDL_CreateRGBSurface(SDL_SWSURFACE, matrixSize.x, matrixSize.y, BITS_32, MasksDefault.red, MasksDefault.green, MasksDefault.blue, MasksDefault.alpha);

		SDL_Color white = { 255, 255, 255, 255 };
		for (const auto glyphPosition : PointInRectangleRange(Rectangle{0, 0, GLYPH_MATRIX_SIZE, GLYPH_MATRIX_SIZE}))
		{
			const std::size_t glyph = static_cast<std::size_t>(glyphPosition.y) * GLYPH_MATRIX_SIZE + glyphPosition.x;

			// Avoid glyph 0, which has size 0 for some fonts
			// SDL_TTF will produce errors for a glyph of size 0
			if (glyph == 0) { continue; }

			SDL_Surface* characterSurface = TTF_RenderGlyph_Blended(font, static_cast<uint16_t>(glyph), white);
			if (!characterSurface)
			{
				SDL_FreeSurface(fontSurface);
				throw std::runtime_error("Font failed to generate surface for character : " + std::to_string(glyph) + " : " + std::string(TTF_GetError()));
			}

			SDL_SetSurfaceBlendMode(characterSurface, SDL_BLENDMODE_NONE);
			const auto pixelPosition = glyphPosition.skewBy(characterSize);
			SDL_Rect rect = { pixelPosition.x, pixelPosition.y, 0, 0 };
			SDL_BlitSurface(characterSurface, nullptr, fontSurface, &rect);
			SDL_FreeSurface(characterSurface);
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
		for (Uint16 i = 0; i < ASCII_TABLE_COUNT; i++)
		{
			auto& metrics = glyphMetricsList.emplace_back();
			TTF_GlyphMetrics(font, i, &metrics.minX, &metrics.maxX, &metrics.minY, &metrics.maxY, &metrics.advance);
		}
	}


	void fillInTextureCoordinates(std::vector<Font::GlyphMetrics>& glyphMetricsList)
	{
		const auto uvSize = Vector<float>{1, 1} / 16.0f;
		for (const auto glyphPosition : PointInRectangleRange(Rectangle{0, 0, GLYPH_MATRIX_SIZE, GLYPH_MATRIX_SIZE}))
		{
			const std::size_t glyph = static_cast<std::size_t>(glyphPosition.y) * GLYPH_MATRIX_SIZE + glyphPosition.x;
			const auto uvStart = glyphPosition.to<float>().skewBy(uvSize);
			glyphMetricsList[glyph].uvRect = Rectangle<float>::Create(uvStart, uvSize);
		}
	}
}
