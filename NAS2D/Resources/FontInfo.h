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

#include "../Renderer/Vector.h"
#include "../Renderer/Rectangle.h"

#include <iostream>


namespace NAS2D {

struct GlyphMetrics
{
	Rectangle<float> uvRect{};
	int minX{0};
	int minY{0};
	int maxX{0};
	int maxY{0};
	int advance{0};
};

using GlyphMetricsList = std::vector<GlyphMetrics>;

/**
 * Struct containing basic information related to Fonts. Not part of the public
 * interface.
 */
struct FontInfo
{
	unsigned int textureId{0u};
	unsigned int pt_size{0u};
	int height{0};
	int ascent{0};
	int refCount{0};
	Vector<int> glyph_size;
	GlyphMetricsList metrics;
};

}
