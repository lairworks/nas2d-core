// ==================================================================================
// = NAS2D
// = Copyright © 2008 - 2013 New Age Software
// ==================================================================================
// = NAS2D is distributed under the terms of the zlib license. You are free to copy,
// = modify and distribute the software under the terms of the zlib license.
// = 
// = Acknowledgement of your use of NAS2D is appriciated but is not required.
// ==================================================================================

#ifndef _NAS_SOUNDRESOURCE_
#define _NAS_SOUNDRESOURCE_

#include "Resource.h"

#ifdef __APPLE__
#include <SDL2/SDL.h>
#elif __linux__
#include "SDL2/SDL.h"
#else
#include "SDL/SDL.h"
#endif

#ifdef __APPLE__
#include <SDL2/SDL_mixer.h>
#elif __linux__
#include "SDL2/SDL_mixer.h"
#else
#include "SDL/SDL_mixer.h"
#endif

namespace NAS2D {

/**
 *  \class Sound
 *  \brief Sound Resource.
 *
 *  Represents a Sound.
 */

class Sound: public Resource
{
public:

	Sound();
	Sound(const std::string& filePath);

	~Sound();

protected:
	friend class Mixer;
	friend class SDL_Mixer;

	Mix_Chunk *sound() const;			/**< Internal function only used by the Mixer. */

private:
	void load();

	Mix_Chunk	*mChunk;
};

} // namespace

#endif
