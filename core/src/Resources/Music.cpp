// ==================================================================================
// = NAS2D
// = Copyright © 2008 New Age Software
// ==================================================================================
// = NAS2D is distributed under the terms of the zlib license. You are free to copy,
// = modify and distribute the software as long under the terms of the zlib license.
// = 
// = Acknowledgement of your use of NAS2D is appriciated but is not required.
// ==================================================================================

#include "NAS2D/Resources/Music.h"

#include <string>

using namespace std;

Music::Music(const string& filePath):	Resource(filePath),
										mMusic(0)
{
	load();
}


void Music::load()
{
	mMusicBuffer = Singleton<Filesystem>::get().getFile(mResourceName);
	if(mMusicBuffer.empty())
	{
		mErrorDescription = Singleton<Filesystem>::get().getLastError();
		return;
	}

	mMusic = Mix_LoadMUS_RW(SDL_RWFromConstMem(mMusicBuffer.raw_bytes(), mMusicBuffer.size()));
	if(!mMusic) 
	{
		// Get the error message and return false.
		mErrorDescription = Mix_GetError();
		return;
	}
	
	mIsLoaded = true;
	mErrorDescription = "";
}


Mix_Music *Music::getMusic() const
{
	return mMusic;
}