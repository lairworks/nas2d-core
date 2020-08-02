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

	void MixerNull::fadeInMusic(const Music& /*music*/, int /*loops*/ /*= Mixer::CONTINUOUS*/, int /*time*/ /*= Mixer::DEFAULT_FADE_TIME*/)
	{}

	void MixerNull::fadeOutMusic(int /*time*/ /*= Mixer::DEFAULT_FADE_TIME*/)
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
