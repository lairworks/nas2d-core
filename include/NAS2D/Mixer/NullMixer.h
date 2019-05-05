#pragma once

#include "NAS2D/Mixer/Mixer.h"

namespace NAS2D
{

	class NullMixer : public Mixer
	{
	  public:
		NullMixer() = default;
		NullMixer(const NullMixer& other) = default;
		NullMixer(NullMixer&& other) = default;
		NullMixer& operator=(const NullMixer& other) = default;
		NullMixer& operator=(NullMixer&& other) = default;
		virtual ~NullMixer() = default;

		virtual void playSound(Sound& sound) override;

		virtual void stopSound() override;

		virtual void pauseSound() override;

		virtual void resumeSound() override;

		virtual void stopMusic() override;

		virtual void pauseMusic() override;

		virtual void resumeMusic() override;

		virtual void fadeInMusic(Music& music, int loops = Mixer::CONTINUOUS, int time = Mixer::DEFAULT_FADE_TIME) override;

		virtual void fadeOutMusic(int time = Mixer::DEFAULT_FADE_TIME) override;

		virtual bool musicPlaying() const override;

		virtual void mute() override;

		virtual void unmute() override;

		virtual void soundVolume(int level) override;

		virtual void musicVolume(int level) override;

	  protected:
	  private:
	};

} // namespace NAS2D