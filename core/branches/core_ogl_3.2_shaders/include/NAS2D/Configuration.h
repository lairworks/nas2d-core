// ==================================================================================
// = NAS2D
// = Copyright © 2008 - 2012 New Age Software
// ==================================================================================
// = NAS2D is distributed under the terms of the zlib license. You are free to copy,
// = modify and distribute the software under the terms of the zlib license.
// = 
// = Acknowledgement of your use of NAS2D is appriciated but is not required.
// ==================================================================================

#ifndef _NAS_CONFIGURATION_
#define _NAS_CONFIGURATION_

#ifdef __APPLE__
#include <OpenGL/gl3.h>
#elif WIN32
#include "GLee.h"
#define NO_SDL_GLEXT
#include <windows.h>
#include "SDL/SDL_opengl.h"
#else
#include <GL/gl3.h>
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

	void save();
	void load(const std::string& filePath);

	// Video Options
	int graphicsWidth() const;
	int graphicsHeight() const;
	int graphicsColorDepth() const;
	GraphicsQuality graphicsTextureQuality() const;

	bool fullscreen() const;
	void fullscreen(bool isFullscreen);

	bool vsync() const;
	void vsync(bool isVsync);

	void graphicsWidth(int width);
	void graphicsHeight(int height);
	void graphicsColorDepth(int bpp);
	void graphicsTextureQuality(const std::string& quality);


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

	bool optionChanged() const { return mOptionChanged; }

private:
	typedef std::map<std::string, std::string> Options;
	
	Configuration(const Configuration&);			// Intentionally left undefined.
	Configuration& operator=(const Configuration&);	// Intentionally left undefined.

	bool readConfig(const std::string& filePath);

	void parseGraphics(TiXmlNode *node);
	void parseAudio(TiXmlNode *node);

	void parseOptions(TiXmlNode *node);

	TiXmlDocument		*mConfigFile;					/**< XML Document. */

	Options				mOptions;						/**< Options table containing option/value pairs. */

	int					mScreenWidth, mScreenHeight;	/**< Screen Resolution */
	int					mScreenBpp;						/**< Color Depth */
	GraphicsQuality		mTextureQuality;				/**< Texture render quality -- equivalent to GL_NEAREST and GL_LINEAR */

	bool				mFullScreen;					/**< Screen Mode */
	bool				mVSync;							/**< Vertical Sync */

    int					mMixRate;						/**< */
	int					mStereoChannels;				/**< Either AUDIO_STEREO or AUDIO_MONO */
	int					mSfxVolume, mMusicVolume;		/**< Audio Volume Levels */
	unsigned int		mBufferLength;					/**< Audio Buffer Length */
	std::string			mMixerName;						/**< Renderer to use. */

	bool				mOptionChanged;					/**< Flags whether or not an option was changed. */
};
#endif
