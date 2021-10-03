#include "NAS2D/Mixer/MixerSDL.h"

#include <gtest/gtest.h>


TEST(MixerSDL, InvalidToDefault) {
	{
		const auto options = NAS2D::MixerSDL::InvalidToDefault({});
		EXPECT_EQ(22050, options.mixRate);
		EXPECT_EQ(1, options.numChannels);
		EXPECT_EQ(0, options.sfxVolume);
		EXPECT_EQ(0, options.musicVolume);
		EXPECT_EQ(256, options.bufferSize);
	}

	const NAS2D::MixerSDL::Options highEndOptions{44100, 2, 128, 128, 4096};
	const auto options = NAS2D::MixerSDL::InvalidToDefault(highEndOptions);
	EXPECT_EQ(highEndOptions.mixRate, options.mixRate);
	EXPECT_EQ(highEndOptions.numChannels, options.numChannels);
	EXPECT_EQ(highEndOptions.sfxVolume, options.sfxVolume);
	EXPECT_EQ(highEndOptions.musicVolume, options.musicVolume);
	EXPECT_EQ(highEndOptions.bufferSize, options.bufferSize);
}
