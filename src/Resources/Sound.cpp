// ==================================================================================
// = NAS2D
// = Copyright © 2008 - 2018 New Age Software
// ==================================================================================
// = NAS2D is distributed under the terms of the zlib license. You are free to copy,
// = modify and distribute the software under the terms of the zlib license.
// = 
// = Acknowledgement of your use of NAS2D is appriciated but is not required.
// ==================================================================================

#include "NAS2D/Resources/Sound.h"

#include "NAS2D/Filesystem.h"
#include "NAS2D/Utility.h"

#include <SDL.h>
#include <SDL_mixer.h>

using namespace NAS2D;

/**
 * Default C'tor.
 */
Sound::Sound(): Resource(),
				_chunk(nullptr)
{}

/**
 * C'tor.
 * 
 * \param	filePath	File path of the sound file to load.
 */
Sound::Sound(const std::string& filePath):	Resource(filePath),
_chunk(nullptr)
{ 
	load();
}


/**
 * D'tor.
 */
Sound::~Sound()
{
	if (_chunk)
	{
		Mix_FreeChunk(static_cast<Mix_Chunk*>(_chunk));
		_chunk = nullptr;
	}
}


/**
 * Attempts to load a specified sound file.
 * 
 * \note	This function is called internally during
 *			instantiation.
 */
void Sound::load()
{
	File soundFile = Utility<Filesystem>::get().open(name());
	if (soundFile.empty())
	{
		return;
	}

	_chunk = Mix_LoadWAV_RW(SDL_RWFromConstMem(soundFile.raw_bytes(), static_cast<int>(soundFile.size())), 0);
	if (!_chunk)
	{
		return;
	}

	loaded(true);
}


/**
 * Gets a pointer to sound buffer.
 */
void* Sound::sound() const
{
	return _chunk;
}
