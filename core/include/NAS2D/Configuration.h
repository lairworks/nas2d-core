// ==================================================================================
// = NAS2D
// = Copyright © 2008 New Age Software
// ==================================================================================
// = NAS2D is distributed under the terms of the zlib license. You are free to copy,
// = modify and distribute the software as long under the terms of the zlib license.
// = 
// = Acknowledgement of your use of NAS2D is appriciated but is not required.
// ==================================================================================

#ifndef _NAS_CONFIGURATION_
#define _NAS_CONFIGURATION_

#ifdef __APPLE__
#include <OpenGL/OpenGL.h>
#elif WIN32
#include <windows.h>
#include "GLee.h"
#else
#include <GL/gl.h>
#endif

#include <map>

#include "Common.h"
#include "Exception.h"

#include "tinyxml/tinyxml.h"

enum ConfigOptions
{
	AUDIO_QUALITY
};


enum GraphicsQuality
{
	GRAPHICS_FAST = GL_NEAREST,
	GRAPHICS_GOOD = GL_LINEAR
};


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

	void saveConfig();

	void loadConfig(const std::string& filePath);

	// Video Options
	int getGraphicsWidth() const;
	int getGraphicsHeight() const;
	int getGraphicsColorDepth() const;
	GraphicsQuality getGraphicsTextureQuality() const;
	bool isGraphicsFullscreen() const;
	bool isVsyncEnabled() const;

	void setGraphicsWidth(int width);
	void setGraphicsHeight(int height);
	void setGraphicsColorDepth(int bpp);
	void setGraphicsTextureQuality(const std::string& quality);
	void setGraphicsFullscreen(bool isFullscreen);
	void setGraphicsVSync(bool isVsync);

	void setRenderer(const std::string& renderer);
	std::string getRenderer() const;

	// Audio Options
	int getAudioMixRate() const;
	int getAudioStereoChannels() const;
	int getAudioSfxVolume() const;
	int getAudioMusicVolume() const;
	int getAudioBufferSize() const;

	void setAudioMixRate(int mixrate);
	void setAudioStereoChannels(int channels);
	void setAudioSfxVolume(int volume);
	void setAudioMusicVolume(int volume);
	void setAudioBufferSize(int size);
	void setAudioMixer(const std::string& mixer);

	// GUI Options
	//std::string getGuiOption(const std::string& key);
	//AbsCoordinate getGuiPosition(const std::string& key);

	//void setGuiOption(const std::string& key, const std::string& value);
	//void setGuiPosition(const std::string& key, const AbsCoordinate& position);

	//void clearGuiOption(const std::string& key);
	//void clearGuiPosition(const std::string& key);

	void setDefaultValues();

	bool optionChanged() const { return mOptionChanged; }

private:
	Configuration(const Configuration&);			// Intentionally left undefined.
	Configuration& operator=(const Configuration&);	// Intentionally left undefined.

	//typedef std::map<string, string> GuiOptionTable;
	//typedef std::map<string, AbsCoordinate> GuiPositionTable;

	void writeConfig();
	bool readConfig(const std::string& filePath);

	void parseGraphics(TiXmlNode *node);
	void parseAudio(TiXmlNode *node);

	//bool parseGuiOptions(TiXmlNode *node);
	//bool parseGuiOption(TiXmlNode *node);
	//bool parseGuiPosition(TiXmlNode *node);

	//GuiOptionTable		mGuiOptions;					/**< Table of GUI Options. */
	//GuiPositionTable	mGuiPositions;					/**< Table of GUI Positions. */

	TiXmlDocument		*mConfigFile;					/**<  */

	int					mScreenWidth, mScreenHeight;	/**< Screen Resolution */
	int					mScreenBpp;						/**< Color Depth */
	GraphicsQuality		mTextureQuality;				/**< Texture render quality -- equivalent to GL_NEAREST and GL_LINEAR */

	bool				mFullScreen;					/**< Screen Mode */
	bool				mVSync;							/**< Vertical Sync */
	std::string			mRendererName;					/**< Renderer to use. */

    int					mMixRate;						/**< */
	int					mStereoChannels;				/**< Either AUDIO_STEREO or AUDIO_MONO */
	int					mSfxVolume, mMusicVolume;		/**< Audio Volume Levels */
	unsigned int		mBufferLength;					/**< Audio Buffer Length */
	std::string			mMixerName;						/**< Renderer to use. */

	bool				mOptionChanged;					/**< Flags whether or not an option was changed. */
};
#endif
