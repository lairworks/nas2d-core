// ==================================================================================
// = NAS2D
// = Copyright © 2008 - 2020 New Age Software
// ==================================================================================
// = NAS2D is distributed under the terms of the zlib license. You are free to copy,
// = modify and distribute the software under the terms of the zlib license.
// =
// = Acknowledgment of your use of NAS2D is appreciated but is not required.
// ==================================================================================

#include "Sound.h"

#include "../Filesystem.h"
#include "../Utility.h"

#include <SDL2/SDL.h>
#include <SDL2/SDL_mixer.h>

#include <string>
#include <stdexcept>


using namespace NAS2D;

Sound::Sound(const std::string& filePath) :
	mFilePath{filePath}
{
	auto data = Utility<Filesystem>::get().read(mFilePath);
	if (data.empty())
	{
		throw std::runtime_error("Sound file is empty: " + mFilePath);
	}

	mMixChunk = Mix_LoadWAV_RW(SDL_RWFromConstMem(data.c_str(), static_cast<int>(data.size())), 1);
	if (!mMixChunk)
	{
		throw std::runtime_error("Sound file could not be loaded: " + mFilePath + " : " + std::string{Mix_GetError()});
	}
}


Sound::~Sound()
{
	Mix_FreeChunk(mMixChunk);
}


Mix_Chunk* Sound::sound() const
{
	return mMixChunk;
}
