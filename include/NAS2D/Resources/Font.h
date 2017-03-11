// ==================================================================================
// = NAS2D
// = Copyright © 2008 - 2017 New Age Software
// ==================================================================================
// = NAS2D is distributed under the terms of the zlib license. You are free to copy,
// = modify and distribute the software under the terms of the zlib license.
// = 
// = Acknowledgement of your use of NAS2D is appriciated but is not required.
// ==================================================================================

#pragma once

#include "NAS2D/Resources/Resource.h"
#include "NAS2D/Renderer/Primitives.h"

#include <map>
#include <vector>

namespace NAS2D {

/**
 *  \class Font
 *  \brief Font Resource.
 *
 *  Implements a Font resource.
 */
class Font : public Resource
{
public:
	Font();
	Font(const std::string& filePath, int ptSize = 12);
	Font(const std::string& filePath, int glyphWidth, int glyphHeight, int glyphSpace);
	Font(const Font& font);
	Font& operator=(const Font& font);
	~Font();

	int width(const std::string& str) const;
	int height() const;
	int ascent() const;

	int ptSize() const;

	const int glyphCellWidth() const;
	const int glyphCellHeight() const;

private:
	void load() {}

private:
	int					mHeight;			/**< Font Height. */
	int					mAscent;			/**< Height of each glyph relative to the baseline. */
	int					mPtSize;			/**< Point Size to load the Font in. */

	Point_2d			mGlyphCellSize;		/**< Size in pixels of each cell that glyphs occupy. */
};

} // namespace
