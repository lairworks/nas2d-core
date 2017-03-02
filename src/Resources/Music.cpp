// ==================================================================================
// = NAS2D
// = Copyright © 2008 - 2017 New Age Software
// ==================================================================================
// = NAS2D is distributed under the terms of the zlib license. You are free to copy,
// = modify and distribute the software under the terms of the zlib license.
// = 
// = Acknowledgement of your use of NAS2D is appriciated but is not required.
// ==================================================================================

#include "NAS2D/Resources/Music.h"

#include <string>

using namespace std;

using namespace NAS2D;

Music::MusicReferenceMap Music::_RefMap;

/**
 * Default c'tor.
 */
Music::Music():	Resource()//,
				//mMusic(NULL)
{}


/**
 * C'tor.
 * 
 * \param filePath	Path of the music file to load.
 */
Music::Music(const std::string& filePath):	Resource(filePath)//,
											//mMusic(NULL)
{
	load();
}


/**
 * Copy c'tor.
 */
Music::Music(const Music& _m): Resource(_m.name())
{
	MusicReferenceMap::iterator it = Music::_RefMap.find(name());
	if(it != Music::_RefMap.end())
	{
		it->second.ref_count++;
	}

	loaded(_m.loaded());
}


/**
 * Copy operator.
 */
Music& Music::operator=(const Music& _m)
{
	MusicReferenceMap::iterator it = Music::_RefMap.find(name());
	if(it != Music::_RefMap.end())
	{
		it->second.ref_count++;
	}

	name(_m.name());
	loaded(_m.loaded());

	return *this;
}


/**
 * D'tor.
 */
Music::~Music()
{
	// Is this check necessary?
	MusicReferenceMap::iterator it = Music::_RefMap.find(name());
	if(it == Music::_RefMap.end() || it->second.music == nullptr)
		return;

	it->second.ref_count--;

	// No more references to this resource.
	if(it->second.ref_count < 1)
	{
		if(it->second.music)
		{
			Mix_FreeMusic(it->second.music);
			it->second.music = nullptr;
		}

		if(it->second.buffer)
		{
			delete it->second.buffer;
			it->second.buffer = nullptr;
		}

		_RefMap.erase(it);
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
	// Check the reference map to see if we've already loaded this file.
	MusicReferenceMap::iterator it = Music::_RefMap.find(name());
	if(it != Music::_RefMap.end())
	{
		it->second.ref_count++;
		loaded(true);
		return;
	}

	// File was not previously loaded, load it.
	Music::_RefMap[name()].buffer = new File(Utility<Filesystem>::get().open(name()));
	it = Music::_RefMap.find(name());

	// Empty File
	if(it->second.buffer->empty())
	{
		Music::_RefMap.erase(it);
		return;
	}

	// Load failed
	it->second.music = Mix_LoadMUS_RW(SDL_RWFromConstMem(it->second.buffer->raw_bytes(), it->second.buffer->size()), 0);
	if(!it->second.music) 
	{
		cout << "(ERROR) Music::load(): " << Mix_GetError() << endl;
		Music::_RefMap.erase(it);
		return;
	}

	it->second.ref_count++;

	loaded(true);
}


/**
 * Retrieves a pointer to an SDL_Mixer Mix_Music.
 * 
 * \note	This function is accessible by the Mixer
 *			only.
 */
Mix_Music *Music::music() const
{
	MusicReferenceMap::iterator it = _RefMap.find(name());

	if(it == _RefMap.end())
		return nullptr;

	return it->second.music;
}

