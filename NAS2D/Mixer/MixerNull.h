#pragma once

#include "Mixer.h"

namespace NAS2D
{

	class MixerNull : public Mixer
	{
	public:
		void playSound(Sound& sound) override;
		void stopSound() override;
		void pauseSound() override;
		void resumeSound() override;

		void stopMusic() override;
		void pauseMusic() override;
		void resumeMusic() override;

		void fadeInMusic(Music& music, int loops = Mixer::CONTINUOUS, int time = Mixer::DEFAULT_FADE_TIME) override;
		void fadeOutMusic(int time = Mixer::DEFAULT_FADE_TIME) override;

		bool musicPlaying() const override;

		void mute() override;
		void unmute() override;

		void soundVolume(int level) override;
		void musicVolume(int level) override;

		int soundVolume() const override;
		int musicVolume() const override;

	protected:
	private:
	};

} // namespace NAS2D
