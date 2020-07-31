// ==================================================================================
// = NAS2D
// = Copyright © 2008 - 2019 New Age Software
// ==================================================================================
// = NAS2D is distributed under the terms of the zlib license. You are free to copy,
// = modify and distribute the software under the terms of the zlib license.
// =
// = Acknowledgement of your use of NAS2D is appriciated but is not required.
// ==================================================================================
#pragma once

#include "Resource.h"
#include "../Renderer/Vector.h"

#include <map>
#include <vector>
#include <string_view>


namespace NAS2D {

/**
 * \class Font
 * \brief Font Resource.
 *
 * The Font class can be used to render TrueType, OpenType and Bitmap fonts. Two
 * contructors are provided for these types.
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
	explicit Font(const std::string& filePath, unsigned int ptSize = 12);
	Font(const std::string& filePath, int glyphWidth, int glyphHeight, int glyphSpace);
	Font(const Font& font);
	Font& operator=(const Font& font);
	~Font();

	const std::string& name() const { return mResourceName; }

	Vector<int> size(std::string_view string) const;
	int width(std::string_view string) const;
	int height() const;
	int ascent() const;

	unsigned int ptSize() const;

	Vector<int> glyphCellSize() const;

private:
	std::string mResourceName{"Default Resource"}; /**< File path and internal identifier. */
};

} // namespace
