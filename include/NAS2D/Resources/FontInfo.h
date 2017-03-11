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

/**
 * Struct containing basic information related to Fonts. Not part of the public
 * interface.
 */
struct FontInfo
{
	FontInfo() : textureId(0), fontSize(0) {}
	FontInfo(unsigned int id, unsigned int fontSize) : textureId(id), fontSize(fontSize) {}

	void operator()(unsigned int id, unsigned int size) { textureId = id; fontSize = size; }

	unsigned int textureId;
	unsigned int fontSize;
	int ref_count;
};
