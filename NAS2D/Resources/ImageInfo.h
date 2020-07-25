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

#include <SDL2/SDL_image.h>


namespace NAS2D {

/**
 * Struct containing basic information related to Images. Not part of the public
 * interface.
 */
struct ImageInfo
{
	SDL_Surface* surface{nullptr};
	unsigned int textureId{0u};
	unsigned int frameBufferObjectId{0u};
	Vector<int> size{0, 0};
	int ref_count{0};
};

} // namespace
