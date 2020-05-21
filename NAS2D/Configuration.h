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

#include "Dictionary.h"
#include <string>
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
	static const Dictionary defaultAudio;
	static const Dictionary defaultGraphics;


	Configuration() = default;
	Configuration(std::map<std::string, Dictionary> defaults);
	Configuration(const Configuration&) = delete;
	Configuration& operator=(const Configuration&) = delete;
	Configuration(Configuration&&) = delete;
	Configuration& operator=(Configuration&&) = delete;
	~Configuration() = default;

	const Dictionary& operator[](const std::string& key) const { return mSettings.at(key); }
	Dictionary& operator[](const std::string& key) { return mSettings.at(key); }

	void loadData(const std::string& fileData);
	void load(const std::string& filePath);
	std::string saveData() const;
	void save(const std::string& filePath) const;

	// Video Options
	int graphicsWidth() const;
	int graphicsHeight() const;
	int graphicsColorDepth() const;
	bool fullscreen() const;
	bool vsync() const;

	void graphicsWidth(int width);
	void graphicsHeight(int height);
	void graphicsColorDepth(int bpp);
	void fullscreen(bool fullscreen);
	void vsync(bool vsync);


	// Audio Options
	int audioMixRate() const;
	int audioStereoChannels() const;
	int audioSfxVolume() const;
	int audioMusicVolume() const;
	int audioBufferSize() const;
	std::string mixer() const;

	void audioMixRate(int mixrate);
	void audioStereoChannels(int channels);
	void audioSfxVolume(int volume);
	void audioMusicVolume(int volume);
	void audioBufferSize(int size);
	void mixer(const std::string& mixer);

	void option(const std::string& option, const std::string& value, bool overwrite = true);
	std::string option(const std::string& key);
	void deleteOption(const std::string& option);

	void setDefaultValues();

protected:
private:
	void parseGraphics(const Dictionary& dictionary);
	void parseAudio(const Dictionary& dictionary);

	const std::map<std::string, Dictionary> mDefaults{};
	std::map<std::string, Dictionary> mLoadedSettings{};
	std::map<std::string, Dictionary> mSettings{};
};

} // namespace
