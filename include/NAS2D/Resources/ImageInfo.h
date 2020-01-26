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


/**
 * Struct containing basic information related to Images. Not part of the public
 * interface.
 */
struct ImageInfo
{
	void* pixels{nullptr};
	unsigned int texture_id{0u};
	unsigned int fbo_id{0u};
	int w{0};
	int h{0};
	int ref_count{0};
};
