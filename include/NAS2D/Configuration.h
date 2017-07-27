// ==================================================================================
// = NAS2D
// = Copyright © 2008 - 2017 New Age Software
// ==================================================================================
// = NAS2D is distributed under the terms of the zlib license. You are free to copy,
// = modify and distribute the software under the terms of the zlib license.
// = 
// = Acknowledgement of your use of NAS2D is appriciated but is not required.
// ==================================================================================
#pragma once

#include <map>

#include "Common.h"
#include "Exception.h"

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

	Configuration();
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

	void option(const std::string option, const std::string& value, bool overwrite = true);
	const std::string& option(const std::string& key);
	void deleteOption(const std::string option);

	void setDefaultValues();

	/**
	 * Indicates that an option has changed since the
	 * Configuration file was loaded.
	 */
	bool optionChanged() const { return mOptionChanged; }

private:
	typedef std::map<std::string, std::string> Options;
	
	Configuration(const Configuration&);			// Intentionally left undefined.
	Configuration& operator=(const Configuration&);	// Intentionally left undefined.

	bool readConfig(const std::string& filePath);

	void parseGraphics(void *node);
	void parseAudio(void *node);
	void parseOptions(void *node);

private:
	Options				mOptions;						/**< Options table containing option/value pairs. */

	int					mScreenWidth, mScreenHeight;	/**< Screen Resolution */
	int					mScreenBpp;						/**< Color Depth */

	bool				mFullScreen;					/**< Screen Mode */
	bool				mVSync;							/**< Vertical Sync */

    int					mMixRate;						/**< */
	int					mStereoChannels;				/**< Either AUDIO_STEREO or AUDIO_MONO */
	int					mSfxVolume, mMusicVolume;		/**< Audio Volume Levels */
	int					mBufferLength;					/**< Audio Buffer Length */
	std::string			mMixerName;						/**< Renderer to use. */

	std::string			mConfigPath;					/**< Path for the config file. */

	bool				mOptionChanged;					/**< Flags whether or not an option was changed. */
};

} // namespace
