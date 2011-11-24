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
	mMusicBuffer = Singleton<Filesystem>::get().open(name());
	if(mMusicBuffer.empty())
	{
		errorMessage(Singleton<Filesystem>::get().lastError());
		return;
	}

	mMusic = Mix_LoadMUS_RW(SDL_RWFromConstMem(mMusicBuffer.raw_bytes(), mMusicBuffer.size()));
	if(!mMusic) 
	{
		// Get the error message and return false.
		errorMessage(Mix_GetError());
		return;
	}
	
	loaded(true);
	errorMessage("");
}


Mix_Music *Music::music() const
{
	return mMusic;
}