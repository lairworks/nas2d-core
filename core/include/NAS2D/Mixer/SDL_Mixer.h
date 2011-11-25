// ==================================================================================
// = NAS2D
// = Copyright © 2008 - 2011 New Age Software
// ==================================================================================
// = NAS2D is distributed under the terms of the zlib license. You are free to copy,
// = modify and distribute the software under the terms of the zlib license.
// = 
// = Acknowledgement of your use of NAS2D is appriciated but is not required.
// ==================================================================================

#ifndef _NAS_SDL_MIXER_
#define _NAS_SDL_MIXER_

#include "Mixer.h"

#include "SDL/SDL.h"

#ifdef __APPLE__
#include "SDL_mixer/SDL_mixer.h"

#else
#include "SDL/SDL_mixer.h"
#endif

/**
 * \class SDL_Mixer
 * \brief SDL Mixer.
 *
 * Implements all Mixer functions with the SDL API.
 *
 * \warning	Ensure that the mixer is not playing any audio resources
 *			before freeing them. Not doing so results in crashes.
 */
class SDL_Mixer: public Mixer
{
public:
	SDL_Mixer();
	~SDL_Mixer();
	
	// Sound Functions
	void playSound(Sound& sound);
	void stopSound();
	void pauseSound();
	void resumeSound();

	// Music Functions
	void playMusic(Music& music);
	void playMusic(Music& intro, Music& loop);
	void stopMusic();
	void pauseMusic();
	void resumeMusic();

	void fadeInMusic(Music& music, int loops, int delay);
	void fadeOutMusic(int delay);

	// Global Functions
	void setSfxVolume(int level);
	void setMusVolume(int level);

	void mute();
	void unmute();

	void update();

protected:
	friend void notifyMixer();
	
	void updateTwoPartTrackState();

private:
	SDL_Mixer(const SDL_Mixer&);			// Intentionally left undefined;
	SDL_Mixer& operator=(const SDL_Mixer&);	// Intentionally left undefined;

	void init();
	
	bool		mIsMuted;		/**< Flag indicating that the mixer is in a muted state. */
	bool		mActAsNull;		/**< Act as a NULL mixer in the event of an audio failure. */
	bool		mMusicStopped;	/**< Flag indicating that the stopMusic function was called. */

	bool		mIntroFinished;	/**< Flag indicating that the 'intro' section of a two part music track is finished playing. */

	Music*		mBody;			/**< Pointer to the 'body' section of a two part music track. */
};

#endif
