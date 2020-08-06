// ==================================================================================
// = NAS2D
// = Copyright © 2008 - 2019 New Age Software
// ==================================================================================
// = NAS2D is distributed under the terms of the zlib license. You are free to copy,
// = modify and distribute the software under the terms of the zlib license.
// =
// = Acknowledgement of your use of NAS2D is appriciated but is not required.
// ==================================================================================
#include "Music.h"
#include "MusicInfo.h"

#include "../Filesystem.h"
#include "../Utility.h"

#include <SDL2/SDL.h>
#include <SDL2/SDL_mixer.h>

#include <iostream>
#include <string>
#include <stdexcept>


using namespace NAS2D;

std::map<std::string, MusicInfo> MUSIC_REF_MAP; /**< Lookup table for music resource references. */

void updateMusicReferenceCount(const std::string& name);

/**
 * C'tor.
 *
 * \param filePath	Path of the music file to load.
 */
Music::Music(const std::string& filePath) :
	mResourceName{filePath}
{
	load();
}


/**
 * Copy c'tor.
 */
Music::Music(const Music& rhs) :
	mResourceName{rhs.mResourceName}
{
	auto it = MUSIC_REF_MAP.find(mResourceName);
	if (it != MUSIC_REF_MAP.end())
	{
		it->second.refCount++;
	}
}


/**
 * Copy operator.
 */
Music& Music::operator=(const Music& rhs)
{
	if (this == &rhs) { return *this; }

	updateMusicReferenceCount(mResourceName);

	auto it = MUSIC_REF_MAP.find(mResourceName);
	if (it != MUSIC_REF_MAP.end())
	{
		it->second.refCount++;
	}

	mResourceName = rhs.mResourceName;

	return *this;
}


/**
 * D'tor.
 */
Music::~Music()
{
	updateMusicReferenceCount(mResourceName);
}


void* Music::music() const
{
	return MUSIC_REF_MAP[mResourceName].music;
}


/**
 * Loads a specified music file.
 *
 * \note	This function is called internally during instantiation.
 */
void Music::load()
{
	if (MUSIC_REF_MAP.find(mResourceName) != MUSIC_REF_MAP.end())
	{
		MUSIC_REF_MAP.find(mResourceName)->second.refCount++;
		return;
	}

	File* file = new File(Utility<Filesystem>::get().open(mResourceName));
	if (file->empty())
	{
		delete file;
		throw std::runtime_error("Music file is empty: " + mResourceName);
	}

	Mix_Music* music = Mix_LoadMUS_RW(SDL_RWFromConstMem(file->raw_bytes(), static_cast<int>(file->size())), 0);
	if (!music)
	{
		throw std::runtime_error("Music::load() error: " + std::string{Mix_GetError()});
	}

	auto& record = MUSIC_REF_MAP[mResourceName];
	record.buffer = file;
	record.music = music;
	record.refCount++;
}


// ==================================================================================
// = Unexposed module-level functions defined here that don't need to be part of the
// = API interface.
// ==================================================================================

/**
* Internal function used to clean up references to fonts when the Music
* destructor or copy assignment operators are called.
*
* \param	name	Name of the Music to check against.
*/
void updateMusicReferenceCount(const std::string& name)
{
	auto it = MUSIC_REF_MAP.find(name);
	if (it == MUSIC_REF_MAP.end())
	{
		return;
	}

	--it->second.refCount;

	// No more references to this resource.
	if (it->second.refCount < 1)
	{
		if (it->second.music)
		{
			Mix_FreeMusic(static_cast<Mix_Music*>(it->second.music));
		}

		if (it->second.buffer)
		{
			delete static_cast<File*>(it->second.buffer);
		}

		MUSIC_REF_MAP.erase(it);
	}
}
