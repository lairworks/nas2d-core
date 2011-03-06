// ==================================================================================
// = NAS2D
// = Copyright © 2008 New Age Software
// ==================================================================================
// = NAS2D is distributed under the terms of the zlib license. You are free to copy,
// = modify and distribute the software as long under the terms of the zlib license.
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
	File soundFile = Singleton<Filesystem>::get().getFile(mResourceName);
	if(soundFile.empty())
	{
		mErrorDescription = Singleton<Filesystem>::get().getLastError();
		return;
	}

	mChunk = Mix_LoadWAV_RW(SDL_RWFromConstMem(soundFile.raw_bytes(), soundFile.size()), 0);
	if(!mChunk) 
	{
		// Get the error message and return false.
		mErrorDescription = Mix_GetError();
		return;
	}

	mIsLoaded = true;
}

Mix_Chunk *Sound::getSound() const
{
	return mChunk;
}