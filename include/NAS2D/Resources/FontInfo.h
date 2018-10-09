// ==================================================================================
// = NAS2D
// = Copyright © 2008 - 2018 New Age Software
// ==================================================================================
// = NAS2D is distributed under the terms of the zlib license. You are free to copy,
// = modify and distribute the software under the terms of the zlib license.
// = 
// = Acknowledgement of your use of NAS2D is appriciated but is not required.
// ==================================================================================
#pragma once

#include "NAS2D/Renderer/Primitives.h"

#include <iostream>

struct GlyphMetrics
{
	GlyphMetrics() : uvX(0.0f), uvY(0.0f), uvW(0.0f), uvH(0.0f), minX(0), minY(0), maxX(0), maxY(0), advance(0) {}

	float uvX, uvY;	// Texture coordinates.
	float uvW, uvH; // Texture coordinates.
	int minX, minY;
	int maxX, maxY;
	int advance;
};

typedef std::vector<GlyphMetrics> GlyphMetricsList;

/**
 * Struct containing basic information related to Fonts. Not part of the public
 * interface.
 */
struct FontInfo
{
	FontInfo() : texture_id(0), pt_size(0), height(0), ascent(0), ref_count(0)
	{}

	unsigned int		texture_id;
	unsigned int		pt_size;
	
	int					height;
	int					ascent;

	int					ref_count;
	
	NAS2D::Point_2d		glyph_size;
	GlyphMetricsList	metrics;
};
