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

#include "Mixer.h"

namespace NAS2D {

/**
 * \class Mixer_SDL
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
	void stopMusic();
	void pauseMusic();
	void resumeMusic();

	void fadeInMusic(Music& music, int loops, int time);
	void fadeOutMusic(int time);

	virtual bool musicPlaying() const;

	// Global Functions
	virtual void soundVolume(int level);
	virtual void musicVolume(int level);

	void mute();
	void unmute();

private:
	// No default copy constructor or copy operator
	Mixer_SDL(const Mixer_SDL&) = delete;
	Mixer_SDL& operator=(const Mixer_SDL&) = delete;

	void init();
	void music_finished_hook();
};

};
