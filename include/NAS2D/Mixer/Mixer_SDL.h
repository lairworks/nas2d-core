// ==================================================================================
// = NAS2D
// = Copyright © 2008 - 2014 New Age Software
// ==================================================================================
// = NAS2D is distributed under the terms of the zlib license. You are free to copy,
// = modify and distribute the software under the terms of the zlib license.
// = 
// = Acknowledgement of your use of NAS2D is appriciated but is not required.
// ==================================================================================

#ifndef _NAS_SDL_MIXER_
#define _NAS_SDL_MIXER_

#ifdef __APPLE__
#include <SDL2/SDL.h>
#include "SDL2_mixer/SDL_mixer.h"
#elif __linux__
#include "SDL2/SDL.h"
#include "SDL2/SDL_mixer.h"
#elif WINDOWS
#include "SDL.h"
#include "SDL_mixer.h"
#endif


#include "Mixer.h"


namespace NAS2D {

/**
 * \class SDL_Mixer
 * \brief SDL Mixer.
 *
 * Implements all Mixer functions with the SDL API.
 *
 * \warning	Ensure that the mixer is not playing any audio resources
 *			before freeing them. Not doing so results in crashes.
 */
	class Mixer_SDL : public Mixer
	{
	public:
		Mixer_SDL();
		~Mixer_SDL();

		// Sound Functions
		void playSound(Sound& sound);
		void stopSound();
		void pauseSound();
		void resumeSound();

		// Music Functions
		void playMusic(Music& music);
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

	private:
		Mixer_SDL(const Mixer_SDL&) {}			// Intentionally left undefined;
		Mixer_SDL& operator=(const Mixer_SDL&) {}	// Intentionally left undefined;

	void init();

	bool		mActAsNull;		/**< Act as a NULL mixer in the event of an audio failure. */
};

};

#endif
