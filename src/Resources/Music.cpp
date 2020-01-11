// ==================================================================================
// = NAS2D
// = Copyright © 2008 - 2019 New Age Software
// ==================================================================================
// = NAS2D is distributed under the terms of the zlib license. You are free to copy,
// = modify and distribute the software under the terms of the zlib license.
// =
// = Acknowledgement of your use of NAS2D is appriciated but is not required.
// ==================================================================================
#include "NAS2D/Resources/Music.h"
#include "NAS2D/Resources/MusicInfo.h"

#include "NAS2D/Filesystem.h"
#include "NAS2D/Utility.h"

#include <SDL.h>
#include <SDL_mixer.h>

#include <iostream>
#include <string>

using namespace NAS2D;

std::map<std::string, MusicInfo>	MUSIC_REF_MAP;		/*< Lookup table for music resource references. */

void updateMusicReferenceCount(const std::string& name);

/**
 * Default c'tor.
 */
Music::Music() : Resource()
{}


/**
 * C'tor.
 *
 * \param filePath	Path of the music file to load.
 */
Music::Music(const std::string& filePath) : Resource(filePath)
{
	load();
}


/**
 * Copy c'tor.
 */
Music::Music(const Music& rhs) : Resource(rhs.name())
{
	auto it = MUSIC_REF_MAP.find(name());
	if (it != MUSIC_REF_MAP.end())
	{
		it->second.ref_count++;
	}

	loaded(rhs.loaded());
}


/**
 * Copy operator.
 */
Music& Music::operator=(const Music& rhs)
{
	if (this == &rhs) { return *this; }

	updateMusicReferenceCount(name());

	name(rhs.name());

	auto it = MUSIC_REF_MAP.find(name());
	if (it != MUSIC_REF_MAP.end())
	{
		it->second.ref_count++;
		loaded(rhs.loaded());
	}
	else
	{
		loaded(false);
	}

	return *this;
}


/**
 * D'tor.
 */
Music::~Music()
{
	updateMusicReferenceCount(name());
}


/**
 * Loads a specified music file.
 *
 * \note	This function is called internally during instantiation.
 */
void Music::load()
{
	if (MUSIC_REF_MAP.find(name()) != MUSIC_REF_MAP.end())
	{
		MUSIC_REF_MAP.find(name())->second.ref_count++;
		loaded(true);
		return;
	}

	File* file = new File(Utility<Filesystem>::get().open(name()));
	if (file->empty())
	{
		delete file;
		return;
	}

	Mix_Music* music = Mix_LoadMUS_RW(SDL_RWFromConstMem(file->raw_bytes(), static_cast<int>(file->size())), 0);
	if (!music)
	{
		std::cout << "Music::load(): " << Mix_GetError() << std::endl;
		return;
	}

	auto& record = MUSIC_REF_MAP[name()];
	record.buffer = file;
	record.music = music;
	record.ref_count++;

	loaded(true);
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

	--it->second.ref_count;

	// No more references to this resource.
	if (it->second.ref_count < 1)
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
