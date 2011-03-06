// ==================================================================================
// = NAS2D
// = Copyright © 2008 New Age Software
// ==================================================================================
// = NAS2D is distributed under the terms of the zlib license. You are free to copy,
// = modify and distribute the software as long under the terms of the zlib license.
// = 
// = Acknowledgement of your use of NAS2D is appriciated but is not required.
// ==================================================================================

#ifndef _NAS_MUSICRESOURCE_
#define _NAS_MUSICRESOURCE_

#include "Resource.h"
#include "SDL/SDL.h"

#ifdef __APPLE__
#include "SDL_mixer/SDL_mixer.h"
#else
#include "SDL/SDL_mixer.h"
#endif

/**
 *  \class Music
 *  \brief Music Resource.
 *
 *  Derived from the Resource class.
 */
class Music : public Resource
{
public:

	Music(const std::string& filePath);

	Mix_Music *getMusic() const;			/**< Should this be private and just friend the Mixer class? */

private:
	void load();

	File		mMusicBuffer;			/**< Persistent memory buffer for Mix_Music. */
	Mix_Music	*mMusic;
};
#endif