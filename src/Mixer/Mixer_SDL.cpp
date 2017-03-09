// ==================================================================================
// = NAS2D
// = Copyright © 2008 - 2017 New Age Software
// ==================================================================================
// = NAS2D is distributed under the terms of the zlib license. You are free to copy,
// = modify and distribute the software under the terms of the zlib license.
// = 
// = Acknowledgement of your use of NAS2D is appriciated but is not required.
// ==================================================================================

#include "NAS2D/Mixer/Mixer_SDL.h"

#include "NAS2D/Configuration.h"
#include "NAS2D/Exception.h"

#include "NAS2D/Resources/MusicInfo.h"

#include <iostream>
#include <functional>

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

using namespace NAS2D;
using namespace NAS2D::exception;

extern std::map<std::string, MusicInfo>	MUSIC_REF_MAP;



// ==================================================================================
// INTEROP WITH SDL2_MIXER
// ==================================================================================
NAS2D::Signals::Signal0<void> MIXER_HOOK_CALLBACK_SIGNAL;
void MIXER_HOOK() { MIXER_HOOK_CALLBACK_SIGNAL(); }
// ==================================================================================




/*
 * C'tor.
 */
Mixer_SDL::Mixer_SDL():	Mixer("SDL Mixer")
{
	init();
}


/*
 * D'tor.
 */
Mixer_SDL::~Mixer_SDL()
{
	// Save current volume levels in the Configuration.
	Utility<Configuration>::get().audioSfxVolume(Mix_Volume(-1, -1));
	Utility<Configuration>::get().audioMusicVolume(Mix_VolumeMusic(-1));

	stopAllAudio();

	Mix_CloseAudio();

	MIXER_HOOK_CALLBACK_SIGNAL.disconnect(this, &Mixer_SDL::music_finished_hook);
	Mix_HookMusicFinished(nullptr);

	SDL_QuitSubSystem(SDL_INIT_AUDIO);

	std::cout << "Mixer Terminated." << std::endl;
}


void Mixer_SDL::init()
{
	std::cout << "Initializing Mixer... ";

	if(SDL_Init(SDL_INIT_AUDIO) < 0)
		throw mixer_backend_init_failure(SDL_GetError());
	
	Configuration& c = Utility<Configuration>::get();

    if(Mix_OpenAudio(c.audioMixRate(), MIX_DEFAULT_FORMAT, c.audioStereoChannels(), c.audioBufferSize()))
		throw mixer_backend_init_failure(Mix_GetError());

	soundVolume(c.audioSfxVolume());
	musicVolume(c.audioMusicVolume());

	Mix_HookMusicFinished(MIXER_HOOK);
	MIXER_HOOK_CALLBACK_SIGNAL.connect(this, &Mixer_SDL::music_finished_hook);

	std::cout << "done." << std::endl;
}


void Mixer_SDL::music_finished_hook()
{
	musicComplete().emit();
}


void Mixer_SDL::playSound(Sound& sound)
{
	if(!sound.loaded())
		return;

	Mix_PlayChannel(-1, static_cast<Mix_Chunk*>(sound.sound()), 0);
}


void Mixer_SDL::stopSound()
{
	Mix_HaltChannel(-1);
}


void Mixer_SDL::pauseSound()
{
	Mix_Pause(-1);
}


void Mixer_SDL::resumeSound()
{
	Mix_Resume(-1);
}


void Mixer_SDL::stopMusic()
{
	Mix_HaltMusic();
}



void Mixer_SDL::pauseMusic()
{
	Mix_PauseMusic();
}


void Mixer_SDL::resumeMusic()
{
	Mix_ResumeMusic();
}


void Mixer_SDL::fadeInMusic(Music& music, int loops, int time)
{
	if (!music.loaded())
		return;

	Mix_FadeInMusic(static_cast<Mix_Music*>(MUSIC_REF_MAP[music.name()].music), loops, time);
}


void Mixer_SDL::fadeOutMusic(int delay)
{
	Mix_FadeOutMusic(delay);
}


bool Mixer_SDL::musicPlaying() const
{
	return Mix_PlayingMusic() == 1;
}


void Mixer_SDL::soundVolume(int volume)
{
	Mix_Volume(-1, clamp(volume, 0, SDL_MIX_MAXVOLUME));
}


void Mixer_SDL::musicVolume(int volume)
{
	Mix_VolumeMusic(clamp(volume, 0, SDL_MIX_MAXVOLUME));
}


void Mixer_SDL::mute()
{
	musicVolume(0);
	soundVolume(0);
}


void Mixer_SDL::unmute()
{
	musicVolume(Mix_VolumeMusic(-1));
	soundVolume(Mix_Volume(-1, -1));
}
