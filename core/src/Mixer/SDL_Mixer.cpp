// ==================================================================================
// = NAS2D
// = Copyright © 2008 New Age Software
// ==================================================================================
// = NAS2D is distributed under the terms of the zlib license. You are free to copy,
// = modify and distribute the software as long under the terms of the zlib license.
// = 
// = Acknowledgement of your use of NAS2D is appriciated but is not required.
// ==================================================================================

#include "NAS2D/Mixer/SDL_Mixer.h"

#include <iostream>

/**
 * This function is called when a music track stops playing. It's used only when a
 * two-part audio track is set.
 */
void notifyMixer()
{
	static_cast<SDL_Mixer&>(Singleton<Mixer>::get()).updateTwoPartTrackState();
}


/*
 * C'tor.
 */
SDL_Mixer::SDL_Mixer():	Mixer("SDL Mixer"),
						mIsMuted(false),
						mActAsNull(false),
						mMusicStopped(false),
						mIntroFinished(false),
						mBody(NULL)
{
	init();
}


/*
 * D'tor.
 */
SDL_Mixer::~SDL_Mixer()
{
	// Save current volume levels in the Configuration.
	Singleton<Configuration>::get().setAudioSfxVolume(Mix_Volume(-1, -1));
	Singleton<Configuration>::get().setAudioMusicVolume(Mix_VolumeMusic(-1));

	SDL_QuitSubSystem(SDL_INIT_AUDIO);
}


void SDL_Mixer::init()
{
	// Initialize SDL's Audio Subsystems.
	if(SDL_Init(SDL_INIT_AUDIO) < 0)
	{
		cout << "\tAudio driver not initialized: " << SDL_GetError() << endl;
		cout << "\tSetting to NULL mode. No audio will be played." << endl;
		mActAsNull = true;
		return;
	}
	
    // Initialize the Audio Mixer
    if(Mix_OpenAudio(Singleton<Configuration>::get().getAudioMixRate(), MIX_DEFAULT_FORMAT, Singleton<Configuration>::get().getAudioStereoChannels(), Singleton<Configuration>::get().getAudioBufferSize()))
	{
		cout << "\tAudio driver not initialized: " << SDL_GetError() << endl;
		cout << "\tSetting to NULL mode. No audio will be played." << endl;
		mActAsNull = true;
		return;
	}

	setSfxVolume(Singleton<Configuration>::get().getAudioSfxVolume());
	setMusVolume(Singleton<Configuration>::get().getAudioMusicVolume());
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

	Mix_HookMusicFinished(NULL);
	Mix_PlayMusic(music.music(), -1);
	mMusicStopped = false;
	mBody = NULL;
}


void SDL_Mixer::playMusic(Music& intro, Music& loop)
{
	if(mActAsNull)
		return;

	if(!intro.loaded())
		return;
	if(!loop.loaded())
		return;

	mBody = &loop;

	Mix_HookMusicFinished(notifyMixer);

	Mix_PlayMusic(intro.music(), 1);
	mMusicStopped = false;
}


void SDL_Mixer::stopMusic()
{
	Mix_HaltMusic();
	mMusicStopped = true;
	mBody = NULL;
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

	mIsMuted = true;
}


void SDL_Mixer::unmute()
{
	if(mActAsNull)
		return;

	setMusVolume(Mix_VolumeMusic(-1));
	setSfxVolume(Mix_Volume(-1, -1));

	mIsMuted = false;
}


/**
 * Used internally for the SDL_Mixer.
 * 
 * Changes the state of the SDL_Mixer so it knows to start playing
 * the 'body' section of a two part track.
 */
void SDL_Mixer::updateTwoPartTrackState()
{
	mIntroFinished = true;
}


void SDL_Mixer::update()
{
	if(mIntroFinished)
	{
		playMusic(*mBody);
		mIntroFinished = false;
	}
}