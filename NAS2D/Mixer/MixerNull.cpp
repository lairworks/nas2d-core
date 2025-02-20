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

	void MixerNull::stopMusic()
	{}

	void MixerNull::pauseMusic()
	{}

	void MixerNull::resumeMusic()
	{}

	void MixerNull::fadeInMusic(const Music& /*music*/, Duration /*fadeInTime*/ /*= Mixer::DefaultFadeTime*/)
	{}

	void MixerNull::fadeOutMusic(Duration /*fadeOutTime*/ /*= Mixer::DefaultFadeTime*/)
	{}

	bool MixerNull::musicPlaying() const
	{
		return false;
	}

	void MixerNull::soundVolume(int /*level*/)
	{}

	void MixerNull::musicVolume(int /*level*/)
	{}

	int MixerNull::soundVolume() const
	{
		return 0;
	}

	int MixerNull::musicVolume() const
	{
		return 0;
	}

} // namespace NAS2D
