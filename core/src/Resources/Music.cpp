// ==================================================================================
// = NAS2D
// = Copyright © 2008 - 2012 New Age Software
// ==================================================================================
// = NAS2D is distributed under the terms of the zlib license. You are free to copy,
// = modify and distribute the software under the terms of the zlib license.
// = 
// = Acknowledgement of your use of NAS2D is appriciated but is not required.
// ==================================================================================

#include "NAS2D/Resources/Music.h"

#include <string>

using namespace std;


/**
 * Default c'tor.
 */
Music::Music():	Resource(),
				mMusic(NULL)
{}


/**
 * C'tor.
 * 
 * \param filePath	Path of the music file to load.
 */
Music::Music(const std::string& filePath):	Resource(filePath),
											mMusic(NULL)
{
	load();
}


/**
 * D'tor.
 */
Music::~Music()
{
	if(mMusic)
	{
		Mix_FreeMusic(mMusic);
		mMusic = NULL;
	}
}


/**
 * Attempts to load a specified music file.
 * 
 * \note	This function is called internally during
 *			instantiation.
 */
void Music::load()
{
	mMusicBuffer = Utility<Filesystem>::get().open(name());
	if(mMusicBuffer.empty())
	{
		//errorMessage(Utility<Filesystem>::get().lastError());
		cout << "(ERROR) Music::load(): " << Utility<Filesystem>::get().lastError() << endl;
		return;
	}

	mMusic = Mix_LoadMUS_RW(SDL_RWFromConstMem(mMusicBuffer.raw_bytes(), mMusicBuffer.size()));
	if(!mMusic) 
	{
		// Get the error message and return false.
		//errorMessage(Mix_GetError());
		cout << "(ERROR) Music::load(): " << Mix_GetError() << endl;
		return;
	}
	
	loaded(true);
	errorMessage("");
}


/**
 * Retrieves a pointer to an SDL_Mixer Mix_Music.
 * 
 * \note	This function is accessible by the Mixer
 *			only.
 */
Mix_Music *Music::music() const
{
	return mMusic;
}