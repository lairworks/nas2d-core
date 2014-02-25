// ==================================================================================
// = NAS2D
// = Copyright © 2008 - 2012 New Age Software
// ==================================================================================
// = NAS2D is distributed under the terms of the zlib license. You are free to copy,
// = modify and distribute the software under the terms of the zlib license.
// = 
// = Acknowledgement of your use of NAS2D is appriciated but is not required.
// ==================================================================================

#include "NAS2D/Mixer/SDL_Mixer.h"

#include <iostream>


/*
 * C'tor.
 */
SDL_Mixer::SDL_Mixer():	Mixer("SDL Mixer"),
						mActAsNull(false)
{
	init();
}


/*
 * D'tor.
 */
SDL_Mixer::~SDL_Mixer()
{
	// Save current volume levels in the Configuration.
	Utility<Configuration>::get().audioSfxVolume(Mix_Volume(-1, -1));
	Utility<Configuration>::get().audioMusicVolume(Mix_VolumeMusic(-1));

	stopAllAudio();

	SDL_QuitSubSystem(SDL_INIT_AUDIO);
}


void SDL_Mixer::init()
{
	cout << "Initializing Mixer... ";
	// Initialize SDL's Audio Subsystems.
	if(SDL_Init(SDL_INIT_AUDIO) < 0)
	{
		cout << endl << "\tAudio driver not initialized: " << SDL_GetError() << endl;
		cout << "\tSetting to NULL mode. No audio will be played." << endl;
		mActAsNull = true;
		return;
	}
	
    // Initialize the Audio Mixer
    if(Mix_OpenAudio(Utility<Configuration>::get().audioMixRate(), MIX_DEFAULT_FORMAT, Utility<Configuration>::get().audioStereoChannels(), Utility<Configuration>::get().audioBufferSize()))
	{
		cout << endl << "\tAudio driver not initialized: " << Mix_GetError() << endl;
		cout << "\tSetting to NULL mode. No audio will be played." << endl;
		mActAsNull = true;
		return;
	}

	setSfxVolume(Utility<Configuration>::get().audioSfxVolume());
	setMusVolume(Utility<Configuration>::get().audioMusicVolume());

	cout << "done." << endl;
}


void SDL_Mixer::playSound(Sound& sound)
{
	if(mActAsNull)
		return;

	if(!sound.loaded())
		return;

	Mix_PlayChannel(-1, sound.sound(), 0);
}


void SDL_Mixer::stopSound()
{
	Mix_HaltChannel(-1);
}


void SDL_Mixer::pauseSound()
{
	Mix_Pause(-1);
}


void SDL_Mixer::resumeSound()
{
	Mix_Resume(-1);
}


void SDL_Mixer::playMusic(Music& music)
{
	if(mActAsNull)
		return;

	if(!music.loaded())
		return;

	Mix_PlayMusic(music.music(), -1);
}


void SDL_Mixer::stopMusic()
{
	Mix_HaltMusic();
}



void SDL_Mixer::pauseMusic()
{
	Mix_PauseMusic();
}


void SDL_Mixer::resumeMusic()
{
	Mix_ResumeMusic();
}


void SDL_Mixer::fadeInMusic(Music& music, int loops, int delay)
{
	if(mActAsNull)
		return;

	Mix_FadeInMusic(music.music(), loops, delay);
}


void SDL_Mixer::fadeOutMusic(int delay)
{
	if(mActAsNull)
		return;
	
	Mix_FadeOutMusic(delay);
}


void SDL_Mixer::setSfxVolume(int volume)
{
	if(mActAsNull)
		return;

	Mix_Volume(-1, clamp(volume, 0, SDL_MIX_MAXVOLUME));
}


void SDL_Mixer::setMusVolume(int volume)
{
	if(mActAsNull)
		return;

	Mix_VolumeMusic(clamp(volume, 0, SDL_MIX_MAXVOLUME));
}


void SDL_Mixer::mute()
{
	if(mActAsNull)
		return;

	setMusVolume(0);
	setSfxVolume(0);
}


void SDL_Mixer::unmute()
{
	if(mActAsNull)
		return;

	setMusVolume(Mix_VolumeMusic(-1));
	setSfxVolume(Mix_Volume(-1, -1));
}

