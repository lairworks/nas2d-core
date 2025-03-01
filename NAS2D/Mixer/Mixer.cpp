// ==================================================================================
// = NAS2D
// = Copyright © 2008 - 2020 New Age Software
// ==================================================================================
// = NAS2D is distributed under the terms of the zlib license. You are free to copy,
// = modify and distribute the software under the terms of the zlib license.
// =
// = Acknowledgment of your use of NAS2D is appreciated but is not required.
// ==================================================================================

#include "Mixer.h"


using namespace NAS2D;


Mixer::~Mixer()
{
}


void Mixer::playMusic(const Music& music)
{
	fadeInMusic(music, Duration{0});
}


void Mixer::stopAllAudio()
{
	stopMusic();
	stopSound();
}


void Mixer::pauseAllAudio()
{
	pauseMusic();
	pauseSound();
}


void Mixer::resumeAllAudio()
{
	resumeMusic();
	resumeSound();
}


SignalSource<>& Mixer::musicCompleteSignalSource()
{
	return mMusicComplete;
}
