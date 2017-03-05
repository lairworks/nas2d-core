// ==================================================================================
// = NAS2D
// = Copyright © 2008 - 2017 New Age Software
// ==================================================================================
// = NAS2D is distributed under the terms of the zlib license. You are free to copy,
// = modify and distribute the software under the terms of the zlib license.
// = 
// = Acknowledgement of your use of NAS2D is appriciated but is not required.
// ==================================================================================

#include "NAS2D/Resources/Sound.h"

using namespace std;
using namespace NAS2D;

/**
 * Default C'tor.
 */
Sound::Sound(): Resource(),
				mChunk(nullptr)
{}

/**
 * C'tor.
 * 
 * \param	filePath	File path of the sound file to load.
 */
Sound::Sound(const std::string& filePath):	Resource(filePath),
											mChunk(nullptr)
{ 
	load();
}


/**
 * D'tor.
 */
Sound::~Sound()
{
	if(mChunk)
	{
		Mix_FreeChunk(mChunk);
		mChunk = nullptr;
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
	if(soundFile.empty())
		return;

	mChunk = Mix_LoadWAV_RW(SDL_RWFromConstMem(soundFile.raw_bytes(), static_cast<int>(soundFile.size())), 0);
	if(!mChunk)
		return;

	loaded(true);
}


/**
 * Retrieves a pointer to an SDL_Mixer Mix_Chunk.
 * 
 * \note	This function is accessible by the Mixer
 *			only.
 */
Mix_Chunk *Sound::sound() const
{
	return mChunk;
}