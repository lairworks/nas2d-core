// ==================================================================================
// = NAS2D
// = Copyright © 2008 - 2020 New Age Software
// ==================================================================================
// = NAS2D is distributed under the terms of the zlib license. You are free to copy,
// = modify and distribute the software under the terms of the zlib license.
// =
// = Acknowledgment of your use of NAS2D is appreciated but is not required.
// ==================================================================================

#pragma once

#include "Mixer.h"

namespace NAS2D
{

	class MixerNull : public Mixer
	{
	public:
		void playSound(const Sound& sound) override;
		void stopSound() override;
		void pauseSound() override;
		void resumeSound() override;

		void stopMusic() override;
		void pauseMusic() override;
		void resumeMusic() override;

		void fadeInMusic(const Music& music, std::chrono::milliseconds fadeInTime = Mixer::DEFAULT_FADE_TIME) override;
		void fadeOutMusic(std::chrono::milliseconds fadeOutTime = Mixer::DEFAULT_FADE_TIME) override;

		bool musicPlaying() const override;

		void soundVolume(int level) override;
		void musicVolume(int level) override;

		int soundVolume() const override;
		int musicVolume() const override;

	protected:
	private:
	};

} // namespace NAS2D
