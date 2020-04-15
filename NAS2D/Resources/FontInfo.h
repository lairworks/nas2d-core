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

#include "../Renderer/Point.h"

#include <iostream>


namespace NAS2D {

struct GlyphMetrics
{
	float uvX{0.0f};
	float uvY{0.0f};
	float uvW{0.0f};
	float uvH{0.0f};
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
	unsigned int texture_id{0u};
	unsigned int pt_size{0u};
	int height{0};
	int ascent{0};
	int ref_count{0};
	Point<int> glyph_size;
	GlyphMetricsList metrics;
};

}
