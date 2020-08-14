// ==================================================================================
// = NAS2D
// = Copyright © 2008 - 2020 New Age Software
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
 * \param	filePath	File path of the sound file to load.
 */
Sound::Sound(const std::string& filePath) :
	mResourceName{filePath}
{
	File soundFile = Utility<Filesystem>::get().open(mResourceName);
	if (soundFile.empty())
	{
		throw std::runtime_error("Sound file is empty: " + mResourceName);
	}

	mMixChunk = Mix_LoadWAV_RW(SDL_RWFromConstMem(soundFile.raw_bytes(), static_cast<int>(soundFile.size())), 0);
	if (!mMixChunk)
	{
		throw std::runtime_error("Sound file could not be loaded: " + mResourceName + " : " + std::string{Mix_GetError()});
	}
}


Sound::~Sound()
{
	Mix_FreeChunk(mMixChunk);
}


/**
 * Gets a pointer to sound buffer.
 */
Mix_Chunk* Sound::sound() const
{
	return mMixChunk;
}
