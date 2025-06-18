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

		void fadeInMusic(const Music& music, Duration fadeInTime = Mixer::DefaultFadeTime) override;
		void fadeOutMusic(Duration fadeOutTime = Mixer::DefaultFadeTime) override;
		void stopMusic() override;
		void pauseMusic() override;
		void resumeMusic() override;
		bool musicPlaying() const override;

		void soundVolume(Volume level) override;
		void musicVolume(Volume level) override;
		Volume soundVolume() const override;
		Volume musicVolume() const override;

		void addMusicCompleteHandler(Delegate<void()> musicCompleteHandler) override;
		void removeMusicCompleteHandler(Delegate<void()> musicCompleteHandler) override;

	protected:
	private:
	};

} // namespace NAS2D
