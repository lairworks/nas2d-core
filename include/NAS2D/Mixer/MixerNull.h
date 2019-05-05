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

#include "Mixer.h"

namespace NAS2D {

class MixerNull : public Mixer
{
public:
	MixerNull() = default;
	~MixerNull() = default;

	// Sound Functions
	void playSound(Sound& sound) override;
	void stopSound() override;
	void pauseSound() override;
	void resumeSound() override;

	// Music Functions
	void stopMusic() override;
	void pauseMusic() override;
	void resumeMusic() override;

	void fadeInMusic(Music& music, int loops = Mixer::CONTINUOUS, int time = Mixer::DEFAULT_FADE_TIME) override;
	void fadeOutMusic(int time = Mixer::DEFAULT_FADE_TIME) override;

	bool musicPlaying() const override;

	// Global Functions
	void soundVolume(int level) override;
	void musicVolume(int level) override;

	void mute() override;
	void unmute() override;
};

}
