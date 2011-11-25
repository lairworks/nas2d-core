// ==================================================================================
// = NAS2D
// = Copyright © 2008 - 2011 New Age Software
// ==================================================================================
// = NAS2D is distributed under the terms of the zlib license. You are free to copy,
// = modify and distribute the software under the terms of the zlib license.
// = 
// = Acknowledgement of your use of NAS2D is appriciated but is not required.
// ==================================================================================

#include "NAS2D/Resources/Sound.h"

using namespace std;

Sound::Sound(const string& filePath):	Resource(filePath),
										mChunk(0)
{ 
	load();
}

void Sound::load()
{
	File soundFile = Singleton<Filesystem>::get().open(name());
	if(soundFile.empty())
	{
		errorMessage(Singleton<Filesystem>::get().lastError());
		return;
	}

	mChunk = Mix_LoadWAV_RW(SDL_RWFromConstMem(soundFile.raw_bytes(), soundFile.size()), 0);
	if(!mChunk) 
	{
		// Get the error message and return false.
		errorMessage(Mix_GetError());
		return;
	}

	loaded(true);
}

Mix_Chunk *Sound::sound() const
{
	return mChunk;
}