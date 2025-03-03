// ==================================================================================
// = NAS2D
// = Copyright © 2008 - 2020 New Age Software
// ==================================================================================
// = NAS2D is distributed under the terms of the zlib license. You are free to copy,
// = modify and distribute the software under the terms of the zlib license.
// =
// = Acknowledgment of your use of NAS2D is appreciated but is not required.
// ==================================================================================

#include "MixerSDL.h"

#include "../Resource/Sound.h"
#include "../Resource/Music.h"

#include "../Configuration.h"
#include "../Utility.h"
#include "../ContainerUtils.h"

#include <SDL2/SDL.h>

#if defined(__XCODE_BUILD__)
#include <SDL2_mixer/SDL_mixer.h>
#else
#include <SDL2/SDL_mixer.h>
#endif

#include <array>
#include <algorithm>
#include <stdexcept>
#include <string>


using namespace NAS2D;


namespace
{
	constexpr int AudioVolumeMin = 0;
	constexpr int AudioVolumeMax = 128;

	constexpr int AudioNumChannelsMin = 1;
	constexpr int AudioNumChannelsMax = 2;

	constexpr int AudioQualityLow = 11025;
	constexpr int AudioQualityMedium = 22050;
	constexpr int AudioQualityHigh = 44100;

	constexpr auto AllowedMixRate = std::array{AudioQualityLow, AudioQualityMedium, AudioQualityHigh};

	constexpr int AudioBufferSizeMin = 256;
	constexpr int AudioBufferSizeMax = 4096;

	// Global so it can be accessed without capturing `this`
	Delegate<void()> musicFinished;


	void onMusicFinished()
	{
		if (musicFinished)
		{
			musicFinished();
		}
	}
}


MixerSDL::Options MixerSDL::InvalidToDefault(const Options& options)
{
	return {
		has(AllowedMixRate, options.mixRate) ? options.mixRate : AudioQualityMedium,
		std::clamp(options.numChannels, AudioNumChannelsMin, AudioNumChannelsMax),
		std::clamp(options.sfxVolume, AudioVolumeMin, AudioVolumeMax),
		std::clamp(options.musicVolume, AudioVolumeMin, AudioVolumeMax),
		std::clamp(options.bufferSize, AudioBufferSizeMin, AudioBufferSizeMax),
	};
}

MixerSDL::Options MixerSDL::ReadConfigurationOptions()
{
	const auto& configuration = Utility<Configuration>::get();
	const auto& audio = configuration["audio"];
	return {
		audio.get<int>("mixrate"),
		audio.get<int>("channels"),
		audio.get<int>("sfxvolume"),
		audio.get<int>("musicvolume"),
		audio.get<int>("bufferlength"),
	};
}

void MixerSDL::WriteConfigurationOptions(const Options& options)
{
	auto& configuration = Utility<Configuration>::get();
	auto& audio = configuration["audio"];
	audio.set("mixrate", options.mixRate);
	audio.set("channels", options.numChannels);
	audio.set("sfxvolume", options.sfxVolume);
	audio.set("musicvolume", options.musicVolume);
	audio.set("bufferlength", options.bufferSize);
}


MixerSDL::MixerSDL() :
	MixerSDL(InvalidToDefault(ReadConfigurationOptions()))
{
}


MixerSDL::MixerSDL(const Options& options)
{
	if (SDL_InitSubSystem(SDL_INIT_AUDIO) < 0)
	{
		throw std::runtime_error(std::string{"Error initializing SDL audio: "} + SDL_GetError());
	}

	if (Mix_OpenAudio(options.mixRate, MIX_DEFAULT_FORMAT, options.numChannels, options.bufferSize))
	{
		throw std::runtime_error(std::string{"Error opening audio mixer: "} + Mix_GetError());
	}

	soundVolume(options.sfxVolume);
	musicVolume(options.musicVolume);

	musicFinished = Delegate{this, &MixerSDL::onMusicFinished};
	Mix_HookMusicFinished(&::onMusicFinished);
}


MixerSDL::MixerSDL(const Options& options, Delegate<void()> musicCompleteHandler) :
	MixerSDL(options)
{
	mMusicComplete.connect(musicCompleteHandler);
}


MixerSDL::MixerSDL(Delegate<void()> musicCompleteHandler) :
	MixerSDL()
{
	mMusicComplete.connect(musicCompleteHandler);
}


MixerSDL::~MixerSDL()
{
	stopAllAudio();

	Mix_CloseAudio();

	Mix_HookMusicFinished(nullptr);
	musicFinished.clear();

	SDL_QuitSubSystem(SDL_INIT_AUDIO);
}

void MixerSDL::onMusicFinished()
{
	mMusicComplete.emit();
}


void MixerSDL::playSound(const Sound& sound)
{
	Mix_PlayChannel(-1, sound.sound(), 0);
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


void MixerSDL::fadeInMusic(const Music& music, Duration fadeInTime)
{
	Mix_FadeInMusic(music.music(), 0, static_cast<int>(fadeInTime.milliseconds));
}


void MixerSDL::fadeOutMusic(Duration fadeOutTime)
{
	Mix_FadeOutMusic(static_cast<int>(fadeOutTime.milliseconds));
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


int MixerSDL::soundVolume() const
{
	return Mix_Volume(-1, -1);
}


int MixerSDL::musicVolume() const
{
	return Mix_VolumeMusic(-1);
}
