#include "Mixer.h"


using namespace NAS2D;


void Mixer::playMusic(const Music& music, int loops /*= Mixer::CONTINUOUS*/)
{
	fadeInMusic(music, loops, 0);
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
