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
	FontInfo() : textureId(0), fontSize(0), ref_count(0) {}
	FontInfo(unsigned int id, unsigned int fontSize) : textureId(id), fontSize(fontSize), ref_count(0) {}

	void operator()(unsigned int id, unsigned int size) { textureId = id; fontSize = size; }

	unsigned int textureId;
	unsigned int fontSize;
	int ref_count;

	GlyphMetricsList	metrics;
};
