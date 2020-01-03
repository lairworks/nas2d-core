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

#include "../Signal.h"

#include "../Resources/Sound.h"
#include "../Resources/Music.h"

namespace NAS2D {

/**
 * \class Mixer
 * \brief Mixer base class.
 *
 * Provides a standard Mixer interface. The base Mixer can be used
 * but will act as a NULL interface.
 */
class Mixer
{
public:
	static const int CONTINUOUS = -1; /**< Constant representing a continuous loop. */
	static const int DEFAULT_FADE_TIME = 500; /**< Default fade time. */

public:
	Mixer() = default;
	Mixer(const Mixer&) = default;
	Mixer& operator=(const Mixer&) = default;
	Mixer(Mixer&&) = default;
	Mixer& operator=(Mixer&&) = default;
	virtual ~Mixer() = default;

public:
	/**
	 * Plays a sound on the first available sound channel.
	 *
	 * \param	sound	A reference to a Sound Resource.
	 */
	virtual void playSound(Sound& sound) = 0;

	/**
	 * Stops playing all sounds on all channels.
	 */
	virtual void stopSound() = 0;

	/**
	 * Pauses sound on all channels.
	 */
	virtual void pauseSound() = 0;

	/**
	 * Resumes sound on all channels.
	 */
	virtual void resumeSound() = 0;

	/**
	 * Starts playing a Music track.
	 *
	 * \param music	Reference to a Music Resource.
	 * \param loops	Number of times to repeat the music.
	 */
	void playMusic(Music& music, int loops = Mixer::CONTINUOUS);

	/**
	 * Stops all playing music.
	 */
	virtual void stopMusic() = 0;

	/**
	 * Pauses the currently playing Music.
	 */
	virtual void pauseMusic() = 0;

	/**
	 * Resumes the currently paused Music.
	 *
	 * \note	It is safe to call this function if the music is stopped or already playing.
	 */
	virtual void resumeMusic() = 0;

	/**
	 * Starts a Music track and fades it in to the current Music volume.
	 *
	 * \param	music	Reference to a Music Resource.
	 * \param	loops	Number of times the Music should be repeated. -1 for continuous loop.
	 * \param	time	Time, in miliseconds, for the fade to last. Default is 500.
	 */
	virtual void fadeInMusic(Music& music, int loops = Mixer::CONTINUOUS, int time = Mixer::DEFAULT_FADE_TIME) = 0;

	/**
	 * Fades out the currently playing Music track.
	 *
	 * \param	time	Time, in miliseconds, for the fade to last. Default is 500.
	 */
	virtual void fadeOutMusic(int time = Mixer::DEFAULT_FADE_TIME) = 0;

	/**
	 * Gets whether or not music is currently playing.
	 */
	virtual bool musicPlaying() const = 0;

	/**
	 * Mutes all audio.
	 */
	virtual void mute() = 0;

	/**
	 * Unmutes all audio.
	 */
	virtual void unmute() = 0;

	/**
	 * Stops all music and sound.
	 */
	void stopAllAudio();

	/**
	 * Pauses all music and sound.
	 */
	void pauseAllAudio();

	/**
	 * Resumes all paused music and sound.
	 */
	void resumeAllAudio();

	/**
	 * Sets the sound volume.
	 *
	 * \param	level	Volume level to set. Valid values are 0 - 128.
	 */
	virtual void soundVolume(int level) = 0;

	/**
	 * Sets the music volume.
	 *
	 * \param	level	Volume level to set. Valid values are 0 - 128.
	 */
	virtual void musicVolume(int level) = 0;

	/**
	 * Gets a reference to a NAS2D::Signals::Signal0<void>, a signal raised
	 * when a Music track has finished playing.
	 */
	 [[deprecated("Deprecated: Please use addMusicCompleteHandler and removeMusicCompleteHandler")]]
	NAS2D::Signals::Signal0<>& musicComplete();

	void addMusicCompleteHandler(NAS2D::Signals::Signal0<>::_Delegate handler);
	void removeMusicCompleteHandler(NAS2D::Signals::Signal0<>::_Delegate handler);

protected:
	NAS2D::Signals::Signal0<> mMusicComplete; /**< Callback used when music finished playing. */
};

} // namespace
