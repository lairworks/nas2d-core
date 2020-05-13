#include "NAS2D/Configuration.h"

#include <gmock/gmock.h>
#include <gtest/gtest.h>


TEST(Configuration, loadData) {
	NAS2D::Configuration config{
		std::map<std::string, NAS2D::Dictionary>{
			{
				{
					"graphics",
					{{
						{"screenwidth", 1000},
						{"screenheight", 700},
						{"bitdepth", 32},
						{"fullscreen", false},
						{"vsync", true}
					}}
				},
				{
					"options",
					{{
						{"skip-splash", false},
						{"maximized", true},
						{"Key1", "Some string value"},
						{"Key2", true},
						{"Key3", -1}
					}}
				}
			}
		}
	};

	// Defaults (before data load)
	EXPECT_EQ("Some string value", config.option("Key1"));
	EXPECT_EQ("true", config.option("Key2"));
	EXPECT_EQ("-1", config.option("Key3"));

	config.loadData(
		R"(
			<!--Automatically generated Configuration file.-->
			<configuration>
				<audio mixer="SDL" musicvolume="100" sfxvolume="128" channels="2" mixrate="22050" bufferlength="1024" />
				<graphics screenheight="700" screenwidth="1000" bitdepth="32" fullscreen="false" vsync="false" />
				<options Key3="1" Key4="0" />
			</configuration>
		)"
	);

	// Defaults (after data load)
	EXPECT_EQ("Some string value", config.option("Key1"));
	EXPECT_EQ("true", config.option("Key2"));
	// Default overwritten by data load
	EXPECT_EQ("1", config.option("Key3"));

	// Fresh loaded values
	EXPECT_EQ("SDL", config.mixer());
	EXPECT_EQ(100, config.audioMusicVolume());
	EXPECT_EQ(128, config.audioSfxVolume());
	EXPECT_EQ(22050, config.audioMixRate());
	EXPECT_EQ(1024, config.audioBufferSize());

	// Nonexistent keys return empty string
	EXPECT_EQ("", config.option("NonExistentKey"));
}
