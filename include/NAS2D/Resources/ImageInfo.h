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
 * Struct containing basic information related to Images. Not part of the public
 * interface.
 */
struct ImageInfo
{
	ImageInfo() : pixels(nullptr), texture_id(0), fbo_id(0), w(0), h(0), ref_count(0) {}

	void*	pixels;

	size_t	texture_id;
	size_t	fbo_id;
	int		w;
	int		h;
	int		ref_count;
};
