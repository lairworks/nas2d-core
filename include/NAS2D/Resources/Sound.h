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

#include "Resource.h"

namespace NAS2D {

/**
 *  \class Sound
 *  \brief Sound Resource.
 *
 *  Represents a Sound.
 */

class Sound: public Resource
{
public:

	Sound() = default;
	explicit Sound(const std::string& filePath);
	Sound& operator=(const Sound& rhs) = default;
	Sound(Sound&& other) = default;
	Sound& operator=(Sound&& other) = default;
	virtual ~Sound();

protected:
	friend class MixerSDL;

	void* sound() const;

private:
	void load();

	void* _chunk{nullptr};
};

} // namespace
