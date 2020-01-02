#include "NAS2D/Mixer/Mixer.h"

using namespace NAS2D;


Mixer::Mixer(const std::string& name): mName(name)
{}


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


const std::string& Mixer::name() const
{
	return mName;
}


Signals::Signal0<>& Mixer::musicComplete()
{
	return mMusicComplete;
}


void Mixer::addMusicCompleteHandler(NAS2D::Signals::Signal0<>::_Delegate handler)
{
	return mMusicComplete.connect(handler);
}


void Mixer::removeMusicCompleteHandler(NAS2D::Signals::Signal0<>::_Delegate handler)
{
	return mMusicComplete.disconnect(handler);
}
