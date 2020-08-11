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

#include <string>


struct Mix_Chunk;


namespace NAS2D {

/**
 *  \class Sound
 *  \brief Sound Resource.
 *
 *  Represents a Sound.
 */

class Sound
{
public:
	Sound(const Sound& other) = default;
	Sound& operator=(const Sound& rhs) = default;
	Sound(Sound&& other) = default;
	Sound& operator=(Sound&& other) = default;
	explicit Sound(const std::string& filePath);
	~Sound();

	const std::string& name() const { return mResourceName; }

protected:
	friend class MixerSDL;

	Mix_Chunk* sound() const;

private:
	std::string mResourceName{"Default Resource"}; /**< File path and internal identifier. */
	Mix_Chunk* mMixChunk{nullptr};
};

} // namespace
