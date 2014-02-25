// ==================================================================================
// = NAS2D
// = Copyright © 2008 - 2014 New Age Software
// ==================================================================================
// = NAS2D is distributed under the terms of the zlib license. You are free to copy,
// = modify and distribute the software under the terms of the zlib license.
// = 
// = Acknowledgement of your use of NAS2D is appriciated but is not required.
// ==================================================================================

#ifndef _NAS_MIXER_
#define _NAS_MIXER_

#include "NAS2D/Exception.h"
#include "NAS2D/Common.h"
#include "NAS2D/Configuration.h"
#include "NAS2D/Utility.h"

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
	 * \param	music	A reference to a Music Resource.
	 */
	virtual void playMusic(Music& music) {}


	/**
	 * Stops all playing music.
	 */
	virtual void stopMusic() {}

	/**
	 * Pauses the currently playing Music track.
	 */
	virtual void pauseMusic() {}

	/**
	 * Resumes the currently paused Music track.
	 * 
	 * \note	It is safe to call this function if the music is stopped or already playing.
	 */
	virtual void resumeMusic() {}
	
	/**
	 * Starts a Music track and fades it in to the current Music volume.
	 */
	virtual void fadeInMusic(Music& music, int loops = -1, int delay = 500) {}

	/**
	 * Fades out the currently playing Music track.
	 */
	virtual void fadeOutMusic(int delay = 500) {}

	/**
	 * Mutes all music and audio.
	 */
	virtual void mute() {}

	/**
	 * Unmute's all music and sound and returns their volumes to their previous levels.
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
	 */
	void setSfxVolume(int level) {};

	/**
	 * Sets the music volume.
	 */
	void setMusVolume(int level) {};

	/**
	 * Gets the name of the derived Mixer.
	 * 
	 * \return	A /c std::string containing the name of the Mixer.
	 */
	const std::string& getName() const { return mMixerName; }

protected:
	/**
	 * Internal constructor used by derived types to set the name of the Mixer.
	 * 
	 * This c'tor is not public and can't be invoked externally.
	 */
	Mixer(const std::string& mixerName):	mMixerName(mixerName)
	{}

	std::string mMixerName;		/**< Internal name of the Renderer. */

private:
	/**
	 * Copy c'tor.
	 * 
	 * The copy c'tor is intentionally private and undefined. It should
	 * never be invoked at any time.
	 */
	Mixer(const Mixer&);

	/**
	 * Copy operator.
	 * 
	 * The copy operator is intentionally private and undefined. It should
	 * never be invoked at any time.
	 */
	Mixer& operator=(const Mixer&);
};

} // namespace

#endif