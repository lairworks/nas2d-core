// ==================================================================================
// = NAS2D
// = Copyright © 2008 - 2020 New Age Software
// ==================================================================================
// = NAS2D is distributed under the terms of the zlib license. You are free to copy,
// = modify and distribute the software under the terms of the zlib license.
// =
// = Acknowledgment of your use of NAS2D is appreciated but is not required.
// ==================================================================================
#pragma once

#include "../Math/Vector.h"
#include "../Math/Rectangle.h"

#include <cstddef>
#include <string_view>
#include <string>
#include <vector>


namespace NAS2D
{
	/**
	 * Font resource.
	 *
	 * The Font class can be used to render TrueType, OpenType and Bitmap fonts. Two
	 * constructors are provided for these types.
	 *
	 * TrueType and OpenType fonts generate their own glyph map internally. Only
	 * the ASCII values 0 - 255 are used. Unicode/UTF is not supported.
	 *
	 * Bitmap fonts are expected to be in a 16x16 glyph matrix with the top left
	 * glyph cell equating to ASCII value '0'. Glyph values increase from left to
	 * right up to ASCII value 255.
	 */
	class Font
	{
	public:
		struct GlyphMetrics
		{
			Rectangle<float> uvRect{};
			int minX{0};
			int minY{0};
			int maxX{0};
			int maxY{0};
			int advance{0};
		};

		/**
		 * Struct containing basic information related to Fonts. Not part of the public
		 * interface.
		 */
		struct FontInfo
		{
			unsigned int textureId{0u};
			unsigned int pointSize{0u};
			int height{0};
			int ascent{0};
			Vector<int> glyphSize{};
			std::vector<GlyphMetrics> metrics{};
		};

		static Font null();

		Font(const std::string& filePath, unsigned int ptSize);
		explicit Font(const std::string& filePath);
		Font(const Font& font) = delete;
		Font& operator=(const Font& font) = delete;
		~Font();

		Vector<int> glyphCellSize() const;
		Vector<int> size(std::string_view string) const;
		int width(std::string_view string) const;
		int width(char character) const;
		std::size_t widthBoundedSubstringLength(std::string_view string, int widthBound) const;
		int height() const;
		int ascent() const;
		unsigned int ptSize() const;
		const std::vector<GlyphMetrics>& metrics() const;

		// Temporary method, that will be removed in a future refactor
		// Intended only to be used by RendererOpenGL
		// As it is so specific, it should not be part of the Font class, nor FontInfo
		unsigned int textureId() const;

	protected:
		Font();

	private:
		FontInfo mFontInfo;
	};
} // namespace
