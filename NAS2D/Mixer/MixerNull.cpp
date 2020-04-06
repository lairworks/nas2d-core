#include "MixerNull.h"

namespace NAS2D
{

	void MixerNull::playSound(Sound& /*sound*/)
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

	void MixerNull::fadeInMusic(Music& /*music*/, int /*loops*/ /*= Mixer::CONTINUOUS*/, int /*time*/ /*= Mixer::DEFAULT_FADE_TIME*/)
	{}

	void MixerNull::fadeOutMusic(int /*time*/ /*= Mixer::DEFAULT_FADE_TIME*/)
	{}

	bool MixerNull::musicPlaying() const
	{
		return false;
	}

	void MixerNull::mute()
	{}

	void MixerNull::unmute()
	{}

	void MixerNull::soundVolume(int /*level*/)
	{}

	void MixerNull::musicVolume(int /*level*/)
	{}

} // namespace NAS2D
