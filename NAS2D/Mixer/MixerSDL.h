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

	/**
	 * SDL Mixer.
	 *
	 * Implements all Mixer functions with the SDL API.
	 *
	 * \warning	Ensure that the mixer is not playing any audio resources
	 *			before freeing them. Not doing so results in crashes.
	 */
	class MixerSDL : public Mixer
	{
	public:
		struct Options
		{
			int mixRate;
			int numChannels;
			int sfxVolume;
			int musicVolume;
			int bufferSize;
		};

		static Options InvalidToDefault(const Options& options);
		static Options ReadConfigurationOptions();
		static void WriteConfigurationOptions(const Options& options);

		MixerSDL();
		MixerSDL(const MixerSDL&) = delete;
		MixerSDL(MixerSDL&&) = default;
		explicit MixerSDL(const Options& options);
		explicit MixerSDL(const Options& options, Delegate<void()> musicCompleteHandler);
		explicit MixerSDL(Delegate<void()> musicCompleteHandler);
		~MixerSDL() override;

		MixerSDL& operator=(const MixerSDL&) = delete;
		MixerSDL& operator=(MixerSDL&&) = default;

		void playSound(const Sound& sound) override;
		void stopSound() override;
		void pauseSound() override;
		void resumeSound() override;

		void stopMusic() override;
		void pauseMusic() override;
		void resumeMusic() override;

		void fadeInMusic(const Music& music, Duration fadeInTime) override;
		void fadeOutMusic(Duration fadeOutTime) override;

		bool musicPlaying() const override;

		void soundVolume(int level) override;
		void musicVolume(int level) override;

		int soundVolume() const override;
		int musicVolume() const override;

	private:
		void onMusicFinished();
	};

}
