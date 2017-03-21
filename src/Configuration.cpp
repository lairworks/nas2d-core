// ==================================================================================
// = NAS2D
// = Copyright © 2008 - 2017 New Age Software
// ==================================================================================
// = NAS2D is distributed under the terms of the zlib license. You are free to copy,
// = modify and distribute the software under the terms of the zlib license.
// = 
// = Acknowledgement of your use of NAS2D is appriciated but is not required.
// ==================================================================================

#include "NAS2D/Configuration.h"
#include "NAS2D/Filesystem.h"
#include "NAS2D/Utility.h"
#include "NAS2D/Xml/XmlAttributeParser.h"

#include <iostream>

using namespace NAS2D;
using namespace NAS2D::Xml;

// Set some basic constants.
const int				AUDIO_LOW_QUALITY			= 11025;
const int				AUDIO_MEDIUM_QUALITY		= 22050;
const int				AUDIO_HIGH_QUALITY			= 44100;
const int				AUDIO_CHANNELS				= 16;
const int				AUDIO_SFX_MIN_VOLUME		= 0;
const int				AUDIO_SFX_MAX_VOLUME		= 128;
const int				AUDIO_SFX_VOLUME			= 128;
const int				AUDIO_MUSIC_MIN_VOLUME		= 0;
const int				AUDIO_MUSIC_MAX_VOLUME		= 128;
const int				AUDIO_MUSIC_VOLUME			= 100;
const int				AUDIO_BUFFER_SIZE			= 1024;
const int				AUDIO_MONO					= 1;
const int				AUDIO_STEREO				= 2;
const std::string		AUDIO_MIXER					= "SDL";

const int				GRAPHICS_WIDTH				= 800;
const int				GRAPHICS_HEIGHT				= 600;
const int				GRAPHICS_BITDEPTH			= 32;
const std::string		GRAPHICS_VSYNC				= "false";
const GraphicsQuality	GRAPHICS_TEXTURE_QUALITY	= GRAPHICS_GOOD;
const bool				GRAPHICS_FULLSCREEN			= false;


/**
 * C'tor
 */
Configuration::Configuration():	mScreenWidth(GRAPHICS_WIDTH),
								mScreenHeight(GRAPHICS_HEIGHT),
								mScreenBpp(GRAPHICS_BITDEPTH),
								mTextureQuality(GRAPHICS_TEXTURE_QUALITY),
								mFullScreen(false),
								mMixRate(AUDIO_MEDIUM_QUALITY),
								mStereoChannels(AUDIO_STEREO),
								mSfxVolume(AUDIO_SFX_VOLUME),
								mMusicVolume(AUDIO_MUSIC_VOLUME),
								mBufferLength(AUDIO_BUFFER_SIZE),
								mOptionChanged(false),
								mMixerName("SDL")
{
}


/**
 * D'tor
 */
Configuration::~Configuration()
{
	if(mOptionChanged)
		save();

	std::cout << "Configuration Terminated." << std::endl;
}


/**
 * Loads a configuration file.
 *
 * \param	filePath	A string indicating the file to load and process for configuration.
 */
void Configuration::load(const std::string& filePath)
{
	std::cout << "Initializing Configuration... ";

	mConfigPath = filePath;
	
	if(!Utility<Filesystem>::get().exists(filePath))
	{
		std::cout << "configuration file '" << filePath << "' does not exist. Using default options." << std::endl;
		mOptionChanged = true;
	}
	// Read in the Config File.
	else if(!readConfig(filePath))
	{
		mOptionChanged = true;
		std::cout << "unable to process '" << filePath << "'. Using default options." << std::endl;
	}
	else
		std::cout << "done." << std::endl;

}

void Configuration::save()
{
	XmlDocument doc;

	XmlComment* comment = new XmlComment("Automatically generated Configuration file.");
	doc.linkEndChild(comment);

	XmlElement* root = new XmlElement("configuration");
	doc.linkEndChild(root);

	XmlElement* graphics = new XmlElement("graphics");
	graphics->setAttribute("screenwidth", mScreenWidth);
	graphics->setAttribute("screenheight", mScreenHeight);
	graphics->setAttribute("bitdepth", mScreenBpp);

	if(mFullScreen)	graphics->setAttribute("fullscreen", "true");
	else graphics->setAttribute("fullscreen", "false");
	
	if(mVSync)	graphics->setAttribute("vsync", "true");
	else graphics->setAttribute("vsync", "false");

	if(mTextureQuality == GL_NEAREST) graphics->setAttribute("texturequality", "fast");
	else if(mTextureQuality == GL_LINEAR) graphics->setAttribute("texturequality", "good");

	root->linkEndChild(graphics);

	XmlElement* audio = new XmlElement("audio");
	audio->setAttribute("mixrate", mMixRate);
	audio->setAttribute("channels", mStereoChannels);
	audio->setAttribute("sfxvolume", mSfxVolume);
	audio->setAttribute("musicvolume", mMusicVolume);
	audio->setAttribute("bufferlength", mBufferLength);
	audio->setAttribute("mixer", mMixerName);
	root->linkEndChild(audio);

	// Options
	XmlElement* options = new XmlElement("options");
	root->linkEndChild(options);
    
	for (auto op : mOptions)
	{
		XmlElement* option = new XmlElement("option");
		option->setAttribute("name", op.first);
		option->setAttribute("value", op.second);
		options->linkEndChild(option);
	}

	// Write out the XML file.
	XmlMemoryBuffer buff;
	doc.accept(&buff);

	Utility<Filesystem>::get().write(File(buff.buffer(), mConfigPath));
}


/**
 * Sets default values for all important values.
 */
void Configuration::setDefaultValues()
{
	mScreenWidth = GRAPHICS_WIDTH;
	mScreenHeight = GRAPHICS_HEIGHT;
	mScreenBpp = GRAPHICS_BITDEPTH;
	mFullScreen = GRAPHICS_FULLSCREEN;
	mTextureQuality = GRAPHICS_TEXTURE_QUALITY;
	
	mMixRate = AUDIO_MEDIUM_QUALITY;
	mStereoChannels = AUDIO_STEREO;
	mSfxVolume = AUDIO_SFX_VOLUME;
	mMusicVolume = AUDIO_MUSIC_VOLUME;
	mBufferLength = AUDIO_BUFFER_SIZE;
}


/**
 * Reads a given XML configuration file.
 *
 * \param filePath	Name of an XML Configuration file to be read.
 */
bool Configuration::readConfig(const std::string& filePath)
{
	File xmlFile = Utility<Filesystem>::get().open(filePath);

	XmlDocument config;
	config.parse(xmlFile.raw_bytes());
	if(config.error())
	{
		std::cout << "Error parsing configuration file '" << filePath << "' on Row " << config.errorRow() << ", Column " << config.errorCol() << ": " << config.errorDesc() << std::endl;
		return false;
	}
	else
	{
		XmlElement* root = config.firstChildElement("configuration");
		if(!root)
		{
			std::cout << "'" << filePath << "' doesn't contain a '<configuration>' tag." << std::endl;
			return false;
		}


		// Start parsing through the Config.xml file.
		int result = 0;
		
		XmlNode *xmlNode = nullptr;
		while(xmlNode = root->iterateChildren(xmlNode))
		{
			if (xmlNode->value() == "graphics")
				parseGraphics(xmlNode);
			else if (xmlNode->value() == "audio")
				parseAudio(xmlNode);
			else if (xmlNode->value() == "options")
				parseOptions(xmlNode);
			else if (xmlNode->type() == XmlNode::XML_COMMENT)
				; // ignore comments
			else
				std::cout << "Unexpected tag '<" << xmlNode->value() << ">' found in '" << filePath << "' on row " << xmlNode->row() << "." << std::endl;
		}
	}

    return true;
}


/**
 * Parse the <graphics> tab.
 *
 * \todo	Check for sane configurations, particularly screen resolution.
 * 
 * \note	Use of void pointer in declaration to avoid implementation details in header.
 */
void Configuration::parseGraphics(void* _n)
{
	XmlAttributeParser parser;
	XmlNode* node = static_cast<XmlNode*>(_n);

	mScreenWidth = parser.intAttribute(node, "screenwidth");
	mScreenHeight = parser.intAttribute(node, "screenheight");
	mScreenBpp = parser.intAttribute(node, "bitdepth");

	std::string fs = parser.stringAttribute(node, "fullscreen");
	(toLowercase(fs) == "true") ? fullscreen(true) : fullscreen(false);


	std::string vSync = parser.stringAttribute(node, "vsync");
	if(toLowercase(vSync) != "true" && toLowercase(vSync) != "false")
		vsync(true);
	else
		toLowercase(vSync) == "true" ? vsync(true) : vsync(false);

	graphicsTextureQuality(parser.stringAttribute(node, "texturequality"));
}

/**
 * Parses audio information from an XML node.
 * 
 * \note	If any values are invalid or non-existant, this
 *			function will set default values.
 * 
 * \note	Use of void pointer in declaration to avoid implementation details in header.
 */
void Configuration::parseAudio(void* _n)
{
	XmlAttributeParser parser;
	XmlNode* node = static_cast<XmlNode*>(_n);

	mMixRate = parser.intAttribute(node, "mixrate");
	if(mMixRate == 0)
		audioMixRate(AUDIO_MEDIUM_QUALITY);

	mStereoChannels = parser.intAttribute(node, "channels");
	if(mStereoChannels != AUDIO_MONO && mStereoChannels != AUDIO_STEREO)
		audioStereoChannels(AUDIO_STEREO);

	mSfxVolume = parser.intAttribute(node, "sfxvolume");
	if(mSfxVolume < AUDIO_SFX_MIN_VOLUME || mSfxVolume > AUDIO_SFX_MAX_VOLUME)
		audioSfxVolume(AUDIO_SFX_VOLUME);


	mMusicVolume = parser.intAttribute(node, "musicvolume");
	if(mMusicVolume < AUDIO_MUSIC_MIN_VOLUME || mMusicVolume > AUDIO_MUSIC_MAX_VOLUME)
		audioSfxVolume(AUDIO_MUSIC_VOLUME);


	audioBufferSize(parser.intAttribute(node, "bufferlength"));


	mixer(parser.stringAttribute(node, "mixer"));
	if(mixer() != "SDL")
		mixer(AUDIO_MIXER);
}


/**
 * Parses program options from an XML node.
 * 
 * \note	Use of void pointer in declaration to avoid implementation details in header.
 */
void Configuration::parseOptions(void* _n)
{
	XmlAttributeParser parser;
	XmlNode* node = static_cast<XmlNode*>(_n);

	XmlNode *xmlNode = nullptr;
	while(xmlNode = node->iterateChildren(xmlNode))
	{
		if(xmlNode->value() == "option")
		{
			// Ensure that there is a 'name' attribute.
			std::string option = parser.stringAttribute(xmlNode, "name");

			if(!option.empty())
				mOptions[option] = parser.stringAttribute(xmlNode, "value");
			else
				std::cout << "Option tag is missing a name attribute on row " << xmlNode->row() << "." << std::endl;
		}
		else
			std::cout << "Unexpected tag '<" << xmlNode->value() << ">' found in configuration on row " << xmlNode->row() << "." << std::endl;
	}
}


/**
 * Gets screen Width.
 */
int Configuration::graphicsWidth() const
{
	return mScreenWidth;
}


/**
 * Gets screen Height.
 */
int Configuration::graphicsHeight() const
{
	return mScreenHeight;
}


/**
 * Gets Color Depth.
 */
int Configuration::graphicsColorDepth() const
{
	return mScreenBpp;
}


/**
 * Gets texture render quality.
 *
 * \note	Equivalent to GL_NEAREST or GL_LINEAR
 */
GraphicsQuality Configuration::graphicsTextureQuality() const
{
	return mTextureQuality;
}


/**
 * Gets true if fullscreen mode is requested.
 */
bool Configuration::fullscreen() const
{
	return mFullScreen;
}


/**
 * Gets true if vsync mode is requested.
 */
bool Configuration::vsync() const
{
	return mVSync;
}


/**
 * Gets the Audio Rate that should be used by the Mixer.
 */
int Configuration::audioMixRate() const
{
	return mMixRate;
}


/**
 * Gets Stereo or Mono
 */
int Configuration::audioStereoChannels() const
{
	return mStereoChannels;
}


/**
 * Gets the Volume Level for Sound Effects.
 */
int Configuration::audioSfxVolume() const
{
	return mSfxVolume;
}


/**
 * Gets the Volume Level for Music.
 */
int Configuration::audioMusicVolume() const
{
	return mMusicVolume;
}


/**
 * Gets the audio Buffer Length.
 */
int Configuration::audioBufferSize() const
{
	return mBufferLength;
}


/**
 * Gets name of the audio mixer.
 */
const std::string& Configuration::mixer() const
{
	return mMixerName;
}


/**
 * Sets the screen width.
 *
 * \todo	Needs proper value and error checking.
 */
void Configuration::graphicsWidth(int width)
{
	mScreenWidth = width;
	mOptionChanged = true;
}


/**
 * Sets the screen height.
 *
 * \todo	Needs proper value and error checking.
 */
void Configuration::graphicsHeight(int height)
{
	mScreenHeight = height;
	mOptionChanged = true;
}


/**
 * Sets the screen depth.
 *
 * \todo	Needs proper value and error checking.
 */
void Configuration::graphicsColorDepth(int bpp)
{
	mScreenBpp = bpp;
	mOptionChanged = true;
}


/**
 * Sets the texture quality.
 * 
 * \param	quality		Can either be GRAPHICS_FAST or GRAPHICS_GOOD.
 * 
 * \note	If the specified quality is not one of the two valid choices,
 *			GRAPHICS_FAST is used instead.
 */
void Configuration::graphicsTextureQuality(const std::string& quality)
{
	if(toLowercase(quality) == "fast")
		mTextureQuality = GRAPHICS_FAST;
	else if(toLowercase(quality) == "good")
		mTextureQuality = GRAPHICS_GOOD;
	else
		mTextureQuality = GRAPHICS_FAST;

	mOptionChanged = true;
}


/**
 * Toggles between windowed and fullscreen mode.
 */
void Configuration::fullscreen(bool isFullscreen)
{
	mFullScreen = isFullscreen;
	mOptionChanged = true;
}


/**
 * Toggles the use of Vertical Sync.
 */
void Configuration::vsync(bool isVsync)
{
	mVSync = isVsync;
	mOptionChanged = true;
}


/**
 * Sets the audio mixrate.
 *
 * \param	mixrate		Audio mix quality. Can either be AUDIO_LOW_QUALITY, AUDIO_MEDIUM_QUALITY or AUDIO_HIGH_QUALITY.
 * 
 * \note	If the specified mixrate is not one of the three valid values, AUDIO_MEDIUM_QUALITY will be used instead.
 */
void Configuration::audioMixRate(int mixrate)
{
	if(mixrate != AUDIO_LOW_QUALITY && mixrate != AUDIO_MEDIUM_QUALITY && mixrate != AUDIO_HIGH_QUALITY)
		mixrate = AUDIO_MEDIUM_QUALITY;

	mMixRate = mixrate;
	mOptionChanged = true;
}


/**
 * Sets the audio mixer.
 *
 * \todo	Needs proper value and error checking.
 */
void Configuration::mixer(const std::string& mixer)
{
	mMixerName = mixer;
	mOptionChanged = true;
}


/**
 * Sets the number of sound channels.
 * 
 * \param	channels	Number of audio channels. Can either be AUDIO_MONO or AUDIO_STEREO.
 */
void Configuration::audioStereoChannels(int channels)
{
	mStereoChannels = clamp(channels, AUDIO_MONO, AUDIO_STEREO);
	mOptionChanged = true;
}


/**
 * Sets the Sound Effects volume.
 * 
 * \param	volume	Sound volume. Can be anywhere between AUDIO_SFX_MIN_VOLUME and AUDIO_SFX_MAX_VOLUME.
 */
void Configuration::audioSfxVolume(int volume)
{
	mSfxVolume = clamp(volume, AUDIO_SFX_MIN_VOLUME, AUDIO_SFX_MAX_VOLUME);
	mOptionChanged = true;
}


/**
 * Sets the Music volume.
 * 
 * \param	volume	Music volume. Can be anywhere between AUDIO_MUSIC_MIN_VOLUME and AUDIO_MUSIC_MAX_VOLUME.
 */
void Configuration::audioMusicVolume(int volume)
{
	mMusicVolume = clamp(volume, AUDIO_MUSIC_MIN_VOLUME, AUDIO_MUSIC_MAX_VOLUME);
	mOptionChanged = true;
}


/**
 * Sets the size of the audio buffer.
 * 
 * \param	size	Size, in bytes, of the audio buffer. Setting this value too high can
 *					cause noticable lag while setting it too low can cause popping and
 *					crackling. A good all-around value is 1024.
 */
void Configuration::audioBufferSize(int size)
{
	mBufferLength = size;
	mOptionChanged = true;
}


/**
 * Sets a program option.
 * 
 * \param	option		Name of the option.
 * \param	value		Value of the option.
 * \param	overwrite	Overwrites an existing option's value. Default is \c true.
 * 
 * \note	Option values are stored and read as strings. How the program
 *			interprets the data contained in value is up to the programmer.
 */
void Configuration::option(const std::string option, const std::string& value, bool overwrite)
{
	if(!overwrite && mOptions.find(option) != mOptions.end())
		return;
		
	mOptions[option] = value;
	mOptionChanged = true;
}


/**
 * Gets an option's value.
 * 
 * \note	If the option was not defined when loading the configuration
 *			file, calling this function will create the option with an
 *			empty value.
 * 
 * \note	Option values are stored and read as strings. How the program
 *			interprets the data contained in value is up to the programmer.
 */
const std::string& Configuration::option(const std::string& key)
{
	if(mOptions.find(key) != mOptions.end())
		mOptionChanged = true;

	return mOptions[key];
}


/**
 * Erases an option.
 * 
 * \param	option	Name of the option to erase.
 * 
 * \note	It is safe to call this function if the named
 *			option has not been defined.
 */
void Configuration::deleteOption(const std::string option)
{
	Options::iterator it = mOptions.find(option);
	
	if(it != mOptions.end())
	{
		mOptions.erase(it);
		mOptionChanged = true;
	}
}
