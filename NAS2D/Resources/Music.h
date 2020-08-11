// ==================================================================================
// = NAS2D
// = Copyright © 2008 - 2019 New Age Software
// ==================================================================================
// = NAS2D is distributed under the terms of the zlib license. You are free to copy,
// = modify and distribute the software under the terms of the zlib license.
// =
// = Acknowledgement of your use of NAS2D is appriciated but is not required.
// ==================================================================================

#pragma once

#include "../File.h"

#include <string>
#include <map>


typedef struct _Mix_Music Mix_Music;


namespace NAS2D {

/**
 *  \class Music
 *  \brief Music Resource.
 */
class Music
{
public:
	explicit Music(const std::string& filePath);

	Music(const Music& rhs) = delete;
	Music& operator=(const Music& rhs) = delete;

	~Music();

	const std::string& name() const { return mResourceName; }

	// Temporary method that may be removed in the future
	// Intended only to be used by MixerSDL
	void* music() const;

private:
	std::string mResourceName; /**< File path */
	const File mBuffer;
	Mix_Music* mMusic{nullptr};
};

} // namespace
