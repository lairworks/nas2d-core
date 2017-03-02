// ==================================================================================
// = NAS2D
// = Copyright © 2008 - 2017 New Age Software
// ==================================================================================
// = NAS2D is distributed under the terms of the zlib license. You are free to copy,
// = modify and distribute the software under the terms of the zlib license.
// = 
// = Acknowledgement of your use of NAS2D is appriciated but is not required.
// ==================================================================================

#ifndef _NAS_MUSICRESOURCE_
#define _NAS_MUSICRESOURCE_

#include "Resource.h"

#ifdef __APPLE__
#include <SDL2/SDL.h>
#include <SDL2/SDL_mixer.h>
#elif __linux__
#include "SDL2/SDL.h"
#include "SDL2/SDL_mixer.h"
#else
#include "SDL.h"
#include "SDL_mixer.h"
#endif

#include <map>

namespace NAS2D {

/**
 *  \class Music
 *  \brief Music Resource.
 *
 *  Derived from the Resource class.
 */
class Music: public Resource
{
public:

	Music();
	Music(const std::string& filePath);

	Music(const Music& _m);
	Music& operator=(const Music& _m);

	~Music();

protected:
	friend class Mixer;
	friend class Mixer_SDL;

	Mix_Music *music() const;			/**< Internal function used only by Mixer classes. */

private:
	struct MusicInfo
	{
		MusicInfo(): buffer(NULL), music(NULL), ref_count(0)
		{}

		File*		buffer;
		Mix_Music*	music;
		int			ref_count;
	};

	typedef std::map<std::string, MusicInfo> MusicReferenceMap;


	void load();


	//File		mMusicBuffer;			/**< Persistent memory buffer for Mix_Music. */
	//Mix_Music	*mMusic;



	static MusicReferenceMap			_RefMap;		/*< Lookup table for music resource references. */
};

} // namespace

#endif
