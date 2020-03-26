// ==================================================================================
// = NAS2D
// = Copyright © 2008 - 2019 New Age Software
// ==================================================================================
// = NAS2D is distributed under the terms of the zlib license. You are free to copy,
// = modify and distribute the software under the terms of the zlib license.
// =
// = Acknowledgement of your use of NAS2D is appriciated but is not required.
// ==================================================================================

#include "NAS2D/Mixer/MixerSDL.h"
#include "NAS2D/Configuration.h"
#include "NAS2D/Exception.h"
#include "NAS2D/Resources/MusicInfo.h"
#include "NAS2D/Utility.h"

#include <SDL2/SDL.h>
#include <SDL2/SDL_mixer.h>

#include <iostream>
#include <functional>
#include <algorithm>

using namespace NAS2D;
using namespace NAS2D::Exception;

extern std::map<std::string, MusicInfo>	MUSIC_REF_MAP;


namespace {
	// ==================================================================================
	// INTEROP WITH SDL2_MIXER
	// ==================================================================================
	// Global so it can be accessed without capturing `this`
	NAS2D::Signals::Signal<> musicFinished;
	// ==================================================================================
}


/*
 * C'tor.
 */
MixerSDL::MixerSDL()
{
	std::cout << "Initializing Mixer... ";

	if (SDL_Init(SDL_INIT_AUDIO) < 0)
	{
		throw mixer_backend_init_failure(SDL_GetError());
	}

	Configuration& c = Utility<Configuration>::get();

	if (Mix_OpenAudio(c.audioMixRate(), MIX_DEFAULT_FORMAT, c.audioStereoChannels(), c.audioBufferSize()))
	{
		throw mixer_backend_init_failure(Mix_GetError());
	}

	soundVolume(c.audioSfxVolume());
	musicVolume(c.audioMusicVolume());

	musicFinished.connect(this, &MixerSDL::onMusicFinished);
	Mix_HookMusicFinished([](){ musicFinished(); });

	std::cout << "done." << std::endl;
}


/*
 * D'tor.
 */
MixerSDL::~MixerSDL()
{
	// Save current volume levels in the Configuration.
	Utility<Configuration>::get().audioSfxVolume(Mix_Volume(-1, -1));
	Utility<Configuration>::get().audioMusicVolume(Mix_VolumeMusic(-1));

	stopAllAudio();

	Mix_CloseAudio();

	Mix_HookMusicFinished(nullptr);
	musicFinished.disconnect(this, &MixerSDL::onMusicFinished);

	SDL_QuitSubSystem(SDL_INIT_AUDIO);

	std::cout << "Mixer Terminated." << std::endl;
}

void MixerSDL::onMusicFinished()
{
	mMusicComplete.emit();
}


void MixerSDL::playSound(Sound& sound)
{
	if (!sound.loaded())
	{
		return;
	}

	Mix_PlayChannel(-1, static_cast<Mix_Chunk*>(sound.sound()), 0);
}


void MixerSDL::stopSound()
{
	Mix_HaltChannel(-1);
}


void MixerSDL::pauseSound()
{
	Mix_Pause(-1);
}


void MixerSDL::resumeSound()
{
	Mix_Resume(-1);
}


void MixerSDL::stopMusic()
{
	Mix_HaltMusic();
}


void MixerSDL::pauseMusic()
{
	Mix_PauseMusic();
}


void MixerSDL::resumeMusic()
{
	Mix_ResumeMusic();
}


void MixerSDL::fadeInMusic(Music& music, int loops, int time)
{
	if (!music.loaded())
	{
		return;
	}

	Mix_FadeInMusic(static_cast<Mix_Music*>(MUSIC_REF_MAP[music.name()].music), loops, time);
}


void MixerSDL::fadeOutMusic(int delay)
{
	Mix_FadeOutMusic(delay);
}


bool MixerSDL::musicPlaying() const
{
	return Mix_PlayingMusic() == 1;
}


void MixerSDL::soundVolume(int volume)
{
	Mix_Volume(-1, std::clamp(volume, 0, SDL_MIX_MAXVOLUME));
}


void MixerSDL::musicVolume(int volume)
{
	Mix_VolumeMusic(std::clamp(volume, 0, SDL_MIX_MAXVOLUME));
}


void MixerSDL::mute()
{
	musicVolume(0);
	soundVolume(0);
}


void MixerSDL::unmute()
{
	musicVolume(Mix_VolumeMusic(-1));
	soundVolume(Mix_Volume(-1, -1));
}
