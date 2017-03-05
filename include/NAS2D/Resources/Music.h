// ==================================================================================
// = NAS2D
// = Copyright © 2008 - 2017 New Age Software
// ==================================================================================
// = NAS2D is distributed under the terms of the zlib license. You are free to copy,
// = modify and distribute the software under the terms of the zlib license.
// = 
// = Acknowledgement of your use of NAS2D is appriciated but is not required.
// ==================================================================================

#pragma once

#include "Resource.h"

class Mixer_SDL;

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
	friend class Mixer_SDL;

	void *music() const;			/**< Internal function used only by Mixer classes. */

private:
	struct MusicInfo
	{
		MusicInfo(): buffer(nullptr), music(nullptr), ref_count(0)
		{}

		File*	buffer;
		void*	music;
		int		ref_count;
	};

private:
	typedef std::map<std::string, MusicInfo> MusicReferenceMap;
	
private:
	void load();

private:
	static MusicReferenceMap			_RefMap;		/*< Lookup table for music resource references. */
};

} // namespace
