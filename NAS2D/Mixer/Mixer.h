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
#include "../Signal/Signal.h"
#include "../Signal/Delegate.h"


namespace NAS2D
{
	class Sound;
	class Music;


	class Mixer
	{
	public:
		static constexpr Duration DefaultFadeTime{500};

	public:
		Mixer() = default;
		Mixer(const Mixer&) = default;
		Mixer(Mixer&&) = default;
		explicit Mixer(Delegate<void()> musicCompleteHandler);
		virtual ~Mixer();

		Mixer& operator=(const Mixer&) = default;
		Mixer& operator=(Mixer&&) = default;

	public:
		virtual void playSound(const Sound& sound) = 0;
		virtual void stopSound() = 0;
		virtual void pauseSound() = 0;
		virtual void resumeSound() = 0;

		/**
		 * Starts playing a Music track.
		 *
		 * \param music Reference to a Music Resource.
		 */
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

		/**
		 * \param level Volume level, valid values are in the range [0, 128]
		*/
		virtual void soundVolume(int level) = 0;


		/**
		 * \param level Volume level, valid values are in the range [0, 128]
		*/
		virtual void musicVolume(int level) = 0;


		/**
		 * \return The volume level in the range [0, 128]
		*/
		virtual int soundVolume() const = 0;

		/**
		 * \return The volume level in the range [0, 128]
		*/
		virtual int musicVolume() const = 0;

		virtual void addMusicCompleteHandler(Delegate<void()> musicCompleteHandler);
		virtual void removeMusicCompleteHandler(Delegate<void()> musicCompleteHandler);

		SignalSource<>& musicCompleteSignalSource();

	protected:
		Signal<> mMusicComplete{};
	};

} // namespace
