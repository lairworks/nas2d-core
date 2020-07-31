// ==================================================================================
// = NAS2D
// = Copyright © 2008 - 2019 New Age Software
// ==================================================================================
// = NAS2D is distributed under the terms of the zlib license. You are free to copy,
// = modify and distribute the software under the terms of the zlib license.
// =
// = Acknowledgement of your use of NAS2D is appriciated but is not required.
// ==================================================================================

#include "Sound.h"

#include "../Filesystem.h"
#include "../Utility.h"

#include <SDL2/SDL.h>
#include <SDL2/SDL_mixer.h>

#include <string>
#include <stdexcept>


using namespace NAS2D;

/**
 * C'tor.
 *
 * \param	filePath	File path of the sound file to load.
 */
Sound::Sound(const std::string& filePath) :
	mResourceName{filePath}
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
	File soundFile = Utility<Filesystem>::get().open(mResourceName);
	if (soundFile.empty())
	{
		throw std::runtime_error("Sound file is empty: " + mResourceName);
	}

	_chunk = Mix_LoadWAV_RW(SDL_RWFromConstMem(soundFile.raw_bytes(), static_cast<int>(soundFile.size())), 0);
	if (!_chunk)
	{
		throw std::runtime_error("Sound file could not be loaded: " + mResourceName + " : " + std::string{Mix_GetError()});
	}
}


/**
 * Gets a pointer to sound buffer.
 */
void* Sound::sound() const
{
	return _chunk;
}
