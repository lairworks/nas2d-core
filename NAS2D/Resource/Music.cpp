// ==================================================================================
// = NAS2D
// = Copyright © 2008 - 2020 New Age Software
// ==================================================================================
// = NAS2D is distributed under the terms of the zlib license. You are free to copy,
// = modify and distribute the software under the terms of the zlib license.
// =
// = Acknowledgment of your use of NAS2D is appreciated but is not required.
// ==================================================================================
#include "Music.h"

#include "../Filesystem.h"
#include "../Utility.h"

#include <SDL2/SDL.h>

#include <string>
#include <stdexcept>


using namespace NAS2D;


Music::Music(std::string_view filePath) :
	mBuffer{Utility<Filesystem>::get().readFile(filePath)}
{
	if (mBuffer.empty())
	{
		throw std::runtime_error("Music file is empty: " + filePath);
	}

	mMusic = Mix_LoadMUS_RW(SDL_RWFromConstMem(mBuffer.c_str(), static_cast<int>(mBuffer.size())), 1);
	if (!mMusic)
	{
		throw std::runtime_error("Music load error: " + std::string{Mix_GetError()});
	}
}


Music::~Music()
{
	Mix_FreeMusic(mMusic);
}


Mix_Music* Music::music() const
{
	return mMusic;
}
