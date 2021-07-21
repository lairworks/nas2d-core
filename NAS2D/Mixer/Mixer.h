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

#include "../Signal/Signal.h"


namespace NAS2D
{

	class Sound;
	class Music;

	/**
	 * \brief Mixer base class.
	 *
	 * Provides a standard Mixer interface.
	 * The base Mixer can be used but will act as a NULL interface.
	*/
	class Mixer
	{
	public:
		static const int CONTINUOUS = -1;
		static const int DEFAULT_FADE_TIME = 500;

	public:
		Mixer() = default;
		Mixer(const Mixer&) = default;
		Mixer& operator=(const Mixer&) = default;
		Mixer(Mixer&&) = default;
		Mixer& operator=(Mixer&&) = default;
		virtual ~Mixer() = default;

	public:
		virtual void playSound(const Sound& sound) = 0;
		virtual void stopSound() = 0;
		virtual void pauseSound() = 0;
		virtual void resumeSound() = 0;
		void playMusic(const Music& music, int loops = Mixer::CONTINUOUS);
		virtual void stopMusic() = 0;
		virtual void pauseMusic() = 0;
		virtual void resumeMusic() = 0;

		/**
		 * Starts a Music track and fades it in to the current Music volume.
		 *
		 * \param	music	Reference to a Music Resource.
		 * \param	loops	Number of times the Music should be repeated. -1 for continuous loop.
		 * \param	time	Time, in milliseconds, for the fade to last. Default is 500.
		 */
		virtual void fadeInMusic(const Music& music, int loops = Mixer::CONTINUOUS, int time = Mixer::DEFAULT_FADE_TIME) = 0;

		/**
		 * Fades out the currently playing Music track.
		 *
		 * \param	time	Time, in milliseconds, for the fade to last. Default is 500.
		 */
		virtual void fadeOutMusic(int time = Mixer::DEFAULT_FADE_TIME) = 0;

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

		SignalSource<>& musicCompleteSignalSource();

	protected:
		Signal<> mMusicComplete;
	};

} // namespace
