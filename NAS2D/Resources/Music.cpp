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

#include "../Filesystem.h"
#include "../Utility.h"

#include <SDL2/SDL.h>
#include <SDL2/SDL_mixer.h>

#include <iostream>
#include <string>
#include <stdexcept>


using namespace NAS2D;


namespace {
	struct MusicInfo
	{
		const File* buffer{nullptr};
		Mix_Music* music{nullptr};
		int refCount{0};
	};

	std::map<std::string, MusicInfo> MUSIC_REF_MAP; /**< Lookup table for music resource references. */

	void updateMusicReferenceCount(const std::string& name);
}


/**
 * C'tor.
 *
 * \param filePath	Path of the music file to load.
 */
Music::Music(const std::string& filePath) :
	mResourceName{filePath}
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


namespace {
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
				Mix_FreeMusic(it->second.music);
			}
			delete it->second.buffer;

			MUSIC_REF_MAP.erase(it);
		}
	}
}
