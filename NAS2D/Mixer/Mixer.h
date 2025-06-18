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

#include "../Duration.h"
#include "../Signal/Delegate.h"


namespace NAS2D
{
	class Sound;
	class Music;

	// Volume level, valid values are in the range [0, 128]
	using Volume = int;


	class Mixer
	{
	public:
		static constexpr Duration DefaultFadeTime{500};

	public:
		Mixer() = default;
		Mixer(const Mixer&) = default;
		Mixer(Mixer&&) = default;
		virtual ~Mixer();

		Mixer& operator=(const Mixer&) = default;
		Mixer& operator=(Mixer&&) = default;

	public:
		virtual void playSound(const Sound& sound) = 0;
		virtual void stopSound() = 0;
		virtual void pauseSound() = 0;
		virtual void resumeSound() = 0;

		void playMusic(const Music& music);
		virtual void stopMusic() = 0;
		virtual void pauseMusic() = 0;
		virtual void resumeMusic() = 0;

		virtual void fadeInMusic(const Music& music, Duration fadeInTime = Mixer::DefaultFadeTime) = 0;

		virtual void fadeOutMusic(Duration fadeOutTime = Mixer::DefaultFadeTime) = 0;

		virtual bool musicPlaying() const = 0;

		void stopAllAudio();
		void pauseAllAudio();
		void resumeAllAudio();

		virtual void soundVolume(Volume level) = 0;
		virtual void musicVolume(Volume level) = 0;

		virtual Volume soundVolume() const = 0;
		virtual Volume musicVolume() const = 0;

		virtual void addMusicCompleteHandler(Delegate<void()> musicCompleteHandler) = 0;
		virtual void removeMusicCompleteHandler(Delegate<void()> musicCompleteHandler) = 0;

	protected:
	};

} // namespace
