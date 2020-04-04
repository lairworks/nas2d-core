#include "NAS2D/Mixer/Mixer.h"

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


Signals::Signal<>& Mixer::musicComplete()
{
	return mMusicComplete;
}


void Mixer::addMusicCompleteHandler(NAS2D::Signals::Signal<>::DelegateType handler)
{
	return mMusicComplete.connect(handler);
}


void Mixer::removeMusicCompleteHandler(NAS2D::Signals::Signal<>::DelegateType handler)
{
	return mMusicComplete.disconnect(handler);
}
