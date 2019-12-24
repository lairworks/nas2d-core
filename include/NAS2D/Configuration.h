// ==================================================================================
// = NAS2D
// = Copyright © 2008 - 2019 New Age Software
// ==================================================================================
// = NAS2D is distributed under the terms of the zlib license. You are free to copy,
// = modify and distribute the software under the terms of the zlib license.
// =
// = Acknowledgement of your use of NAS2D is appriciated but is not required.
// ==================================================================================
#pragma once

#include "Exception.h"

#include <map>

namespace NAS2D {


/**
 * \class Configuration
 * \brief Configuration Parser.
 *
 * Parses and interprets Configuration data stored in XML files (e.g., config.xml).
 */
class Configuration
{
public:

	Configuration() = default;
	Configuration(const Configuration&) = delete;
	Configuration& operator=(const Configuration&) = delete;
	Configuration(Configuration&&) = delete;
	Configuration& operator=(Configuration&&) = delete;
	~Configuration();

	void save();
	void load(const std::string& filePath);

	// Video Options
	int graphicsWidth() const;
	int graphicsHeight() const;
	int graphicsColorDepth() const;

	bool fullscreen() const;
	void fullscreen(bool fullscreen);

	bool vsync() const;
	void vsync(bool vsync);

	void graphicsWidth(int width);
	void graphicsHeight(int height);
	void graphicsColorDepth(int bpp);


	// Audio Options
	int audioMixRate() const;
	int audioStereoChannels() const;
	int audioSfxVolume() const;
	int audioMusicVolume() const;
	int audioBufferSize() const;
	const std::string& mixer() const;

	void audioMixRate(int mixrate);
	void audioStereoChannels(int channels);
	void audioSfxVolume(int volume);
	void audioMusicVolume(int volume);
	void audioBufferSize(int size);
	void mixer(const std::string& mixer);

	void option(const std::string& option, const std::string& value, bool overwrite = true);
	const std::string& option(const std::string& key);
	void deleteOption(const std::string& option);

	void setDefaultValues();

	/**
	 * Indicates that an option has changed since the
	 * Configuration file was loaded.
	 */
	bool optionChanged() const { return mOptionChanged; }

protected:
private:
	typedef std::map<std::string, std::string> Options;

	bool readConfig(const std::string& filePath);

	void parseGraphics(void *node);
	void parseAudio(void *node);
	void parseOptions(void *node);

	Options mOptions{};

	int mScreenWidth{800};
	int mScreenHeight{600};
	int mScreenBpp{32};

	bool mFullScreen{false};
	bool mVSync{false};

	int mMixRate{22050};
	int mStereoChannels{2};
	int mSfxVolume{128};
	int mMusicVolume{100};
	int mBufferLength{1024};
	std::string mMixerName{"SDL"};

	std::string mConfigPath{};

	bool mOptionChanged{false};
};

} // namespace
