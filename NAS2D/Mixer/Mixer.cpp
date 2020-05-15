#include "Mixer.h"

using namespace NAS2D;


void Mixer::playMusic(Music& music, int loops /*= Mixer::CONTINUOUS*/)
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


void Mixer::addMusicCompleteHandler(Signals::Signal<>::DelegateType handler)
{
	return mMusicComplete.connect(handler);
}


void Mixer::removeMusicCompleteHandler(Signals::Signal<>::DelegateType handler)
{
	return mMusicComplete.disconnect(handler);
}
