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

#include "NAS2D/Signal.h"

#include "NAS2D/Resources/Sound.h"
#include "NAS2D/Resources/Music.h"

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
	/**
	 * C'Tor
	 */
	Mixer() {}

	/**
	 * D'tor
	 */
	virtual ~Mixer() {}

	/**
	 * Plays a sound on the first available sound channel.
	 *
	 * \param	sound	A reference to a Sound Resource.
	 */
	virtual void playSound(Sound& sound) {}

	/**
	 * Stops playing all sounds on all channels.
	 */
	virtual void stopSound() {}

	/**
	 * Pauses sound on all channels.
	 */
	virtual void pauseSound() {}

	/**
	 * Resumes sound on all channels.
	 */
	virtual void resumeSound() {}

	/**
	 * Starts playing a Music track.
	 *
	 * \param music	Reference to a Music Resource.
	 * \param loops	Number of times to repeat the music.
	 */
	void playMusic(Music& music, int loops = Mixer::CONTINUOUS) { fadeInMusic(music, loops, 0); }

	/**
	 * Stops all playing music.
	 */
	virtual void stopMusic() {}

	/**
	 * Pauses the currently playing Music.
	 */
	virtual void pauseMusic() {}

	/**
	 * Resumes the currently paused Music.
	 *
	 * \note	It is safe to call this function if the music is stopped or already playing.
	 */
	virtual void resumeMusic() {}

	/**
	 * Starts a Music track and fades it in to the current Music volume.
	 *
	 * \param	music	Reference to a Music Resource.
	 * \param	loops	Number of times the Music should be repeated. -1 for continuous loop.
	 * \param	time	Time, in miliseconds, for the fade to last. Default is 500.
	 */
	virtual void fadeInMusic(Music& music, int loops = Mixer::CONTINUOUS, int time = Mixer::DEFAULT_FADE_TIME) {}

	/**
	 * Fades out the currently playing Music track.
	 *
	 * \param	time	Time, in miliseconds, for the fade to last. Default is 500.
	 */
	virtual void fadeOutMusic(int time = Mixer::DEFAULT_FADE_TIME) {}

	/**
	 * Gets whether or not music is currently playing.
	 */
	virtual bool musicPlaying() const { return false; }

	/**
	 * Mutes all audio.
	 */
	virtual void mute() {}

	/**
	 * Unmutes all audio.
	 */
	virtual void unmute() {}

	/**
	 * Stops all music and sound.
	 */
	void stopAllAudio() { stopMusic(); stopSound(); }

	/**
	 * Pauses all music and sound.
	 */
	void pauseAllAudio() { pauseMusic(); pauseSound(); }

	/**
	 * Resumes all paused music and sound.
	 */
	void resumeAllAudio() { resumeMusic(); resumeSound(); }

	/**
	 * Sets the sound volume.
	 *
	 * \param	level	Volume level to set. Valid values are 0 - 128.
	 */
	virtual void soundVolume(int level) {};

	/**
	 * Sets the music volume.
	 *
	 * \param	level	Volume level to set. Valid values are 0 - 128.
	 */
	virtual void musicVolume(int level) {};

	/**
	 * Gets the name of the Mixer.
	 *
	 * \return	A /c std::string containing the name of the Mixer.
	 */
	const std::string& name() const { return mName; }

	/**
	 * Gets a reference to a NAS2D::Signals::Signal0<void>, a signal raised
	 * when a Music track has finished playing.
	 */
	NAS2D::Signals::Signal0<void>& musicComplete() { return _music_complete; }

protected:
	/**
	 * Internal constructor used by derived types to set the name of the Mixer.
	 *
	 * This c'tor is not public and can't be invoked externally.
	 */
	Mixer(const std::string& name) : mName(name)
	{}

protected:
	NAS2D::Signals::Signal0<void>	_music_complete; /**< Callback used when music finished playing. */

private:
	// No default copy constructor or copy operator
	// Calling these any any time should result in an error
	Mixer(const Mixer&) = delete;
	Mixer& operator=(const Mixer&) = delete;

private:
	std::string		mName;		/**< Internal name of the Renderer. */
};

} // namespace
