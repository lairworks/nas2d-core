// ==================================================================================
// = NAS2D
// = Copyright © 2008 - 2020 New Age Software
// ==================================================================================
// = NAS2D is distributed under the terms of the zlib license. You are free to copy,
// = modify and distribute the software under the terms of the zlib license.
// =
// = Acknowledgment of your use of NAS2D is appreciated but is not required.
// ==================================================================================

#include "MixerNull.h"

namespace NAS2D
{

	void MixerNull::playSound(const Sound& /*sound*/)
	{}

	void MixerNull::stopSound()
	{}

	void MixerNull::pauseSound()
	{}

	void MixerNull::resumeSound()
	{}

	void MixerNull::fadeInMusic(const Music& /*music*/, Duration /*fadeInTime*/ /*= Mixer::DefaultFadeTime*/)
	{}

	void MixerNull::fadeOutMusic(Duration /*fadeOutTime*/ /*= Mixer::DefaultFadeTime*/)
	{}

	void MixerNull::stopMusic()
	{}

	void MixerNull::pauseMusic()
	{}

	void MixerNull::resumeMusic()
	{}

	bool MixerNull::musicPlaying() const
	{
		return false;
	}

	void MixerNull::soundVolume(Volume /*level*/)
	{}

	void MixerNull::musicVolume(Volume /*level*/)
	{}

	Volume MixerNull::soundVolume() const
	{
		return 0;
	}

	Volume MixerNull::musicVolume() const
	{
		return 0;
	}

	void MixerNull::addMusicCompleteHandler(Delegate<void()> /*musicCompleteHandler*/)
	{}

	void MixerNull::removeMusicCompleteHandler(Delegate<void()> /*musicCompleteHandler*/)
	{}

} // namespace NAS2D
