#include "NAS2D/Mixer/NullMixer.h"

namespace NAS2D
{

	void NullMixer::playSound(Sound& /*sound*/)
	{}

	void NullMixer::stopSound()
	{}

	void NullMixer::pauseSound()
	{}

	void NullMixer::resumeSound()
	{}

	void NullMixer::stopMusic()
	{}

	void NullMixer::pauseMusic()
	{}

	void NullMixer::resumeMusic()
	{}

	void NullMixer::fadeInMusic(Music& /*music*/, int /*loops*/ /*= Mixer::CONTINUOUS*/, int /*time*/ /*= Mixer::DEFAULT_FADE_TIME*/)
	{}

	void NullMixer::fadeOutMusic(int /*time*/ /*= Mixer::DEFAULT_FADE_TIME*/)
	{}

	bool NullMixer::musicPlaying() const
	{
		return false;
	}

	void NullMixer::mute()
	{}

	void NullMixer::unmute()
	{}

	void NullMixer::soundVolume(int /*level*/)
	{}

	void NullMixer::musicVolume(int /*level*/)
	{}

} // namespace NAS2D