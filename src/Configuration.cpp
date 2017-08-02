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
#include "NAS2D/Xml/Xml.h"

#include <iostream>

using namespace NAS2D;
using namespace NAS2D::Xml;

// Set some basic constants.
const int				AUDIO_LOW_QUALITY			= 11025;
const int				AUDIO_MEDIUM_QUALITY		= 22050;
const int				AUDIO_HIGH_QUALITY			= 44100;
const int				AUDIO_SFX_MIN_VOLUME		= 0;
const int				AUDIO_SFX_MAX_VOLUME		= 128;
const int				AUDIO_SFX_VOLUME			= 128;
const int				AUDIO_MUSIC_MIN_VOLUME		= 0;
const int				AUDIO_MUSIC_MAX_VOLUME		= 128;
const int				AUDIO_MUSIC_VOLUME			= 100;
const int				AUDIO_BUFFER_SIZE			= 1024;
const int				AUDIO_BUFFER_MIN_SIZE		= 256;
const int				AUDIO_BUFFER_MAX_SIZE		= 4096;
const int				AUDIO_MONO					= 1;
const int				AUDIO_STEREO				= 2;
const std::string		AUDIO_MIXER					= "SDL";

const std::string		AUDIO_CFG_MIXRATE			= "mixrate";
const std::string		AUDIO_CFG_CHANNELS			= "channels";
const std::string		AUDIO_CFG_SFX_VOLUME		= "sfxvolume";
const std::string		AUDIO_CFG_MUS_VOLUME		= "musicvolume";
const std::string		AUDIO_CFG_BUFFER_SIZE		= "bufferlength";
const std::string		AUDIO_CFG_MIXER				= "mixer";

const int				GRAPHICS_WIDTH				= 800;
const int				GRAPHICS_HEIGHT				= 600;
const int				GRAPHICS_BITDEPTH			= 32;
const bool				GRAPHICS_FULLSCREEN			= false;

const std::string		GRAPHICS_CFG_SCREEN_WIDTH	= "screenwidth";
const std::string		GRAPHICS_CFG_SCREEN_HEIGHT	= "screenheight";
const std::string		GRAPHICS_CFG_SCREEN_DEPTH	= "bitdepth";
const std::string		GRAPHICS_CFG_FULLSCREEN		= "fullscreen";
const std::string		GRAPHICS_CFG_VSYNC			= "vsync";


/**
 * C'tor
 */
Configuration::Configuration():	mScreenWidth(GRAPHICS_WIDTH),
								mScreenHeight(GRAPHICS_HEIGHT),
								mScreenBpp(GRAPHICS_BITDEPTH),
								mFullScreen(false),
								mMixRate(AUDIO_MEDIUM_QUALITY),
								mStereoChannels(AUDIO_STEREO),
								mSfxVolume(AUDIO_SFX_VOLUME),
								mMusicVolume(AUDIO_MUSIC_VOLUME),
								mBufferLength(AUDIO_BUFFER_SIZE),
								mMixerName("SDL"),
								mOptionChanged(false)
{}


/**
 * D'tor
 */
Configuration::~Configuration()
{
	if (mOptionChanged)
	{
		save();
	}

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

	if (!Utility<Filesystem>::get().exists(filePath))
	{
		std::cout << "configuration file '" << filePath << "' does not exist. Using default options." << std::endl;
		mOptionChanged = true;
	}
	else if (!readConfig(filePath)) // Read in the Config File.
	{
		mOptionChanged = true;
		std::cout << "unable to process '" << filePath << "'. Using default options." << std::endl;
	}
	else
	{
		std::cout << "done." << std::endl;
	}

}


/**
 * Saves the Configuration to an XML file.
 */
void Configuration::save()
{
	XmlDocument doc;

	XmlComment* comment = new XmlComment("Automatically generated Configuration file.");
	doc.linkEndChild(comment);

	XmlElement* root = new XmlElement("configuration");
	doc.linkEndChild(root);

	XmlElement* graphics = new XmlElement("graphics");
	graphics->attribute("screenwidth", mScreenWidth);
	graphics->attribute("screenheight", mScreenHeight);
	graphics->attribute("bitdepth", mScreenBpp);

	if (mFullScreen) { graphics->attribute("fullscreen", "true"); }
	else { graphics->attribute("fullscreen", "false"); }

	if (mVSync) { graphics->attribute("vsync", "true"); }
	else { graphics->attribute("vsync", "false"); }

	root->linkEndChild(graphics);

	XmlElement* audio = new XmlElement("audio");
	audio->attribute("mixrate", mMixRate);
	audio->attribute("channels", mStereoChannels);
	audio->attribute("sfxvolume", mSfxVolume);
	audio->attribute("musicvolume", mMusicVolume);
	audio->attribute("bufferlength", static_cast<int>(mBufferLength));
	audio->attribute("mixer", mMixerName);
	root->linkEndChild(audio);

	// Options
	XmlElement* options = new XmlElement("options");
	root->linkEndChild(options);

	for (auto op : mOptions)
	{
		XmlElement* option = new XmlElement("option");
		option->attribute("name", op.first);
		option->attribute("value", op.second);
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
	if (config.error())
	{
		std::cout << "Error parsing configuration file '" << filePath << "' on Row " << config.errorRow() << ", Column " << config.errorCol() << ": " << config.errorDesc() << std::endl;
		return false;
	}
	else
	{
		XmlElement* root = config.firstChildElement("configuration");
		if (!root)
		{
			std::cout << "'" << filePath << "' doesn't contain a '<configuration>' tag." << std::endl;
			return false;
		}


		// Start parsing through the Config.xml file.
		XmlNode *xmlNode = nullptr;
		while ((xmlNode = root->iterateChildren(xmlNode)))
		{
			if (xmlNode->value() == "graphics") { parseGraphics(xmlNode); }
			else if (xmlNode->value() == "audio") { parseAudio(xmlNode); }
			else if (xmlNode->value() == "options") { parseOptions(xmlNode); }
			else if (xmlNode->type() == XmlNode::XML_COMMENT) {} // Ignore comments
			else
			{
				std::cout << "Unexpected tag '<" << xmlNode->value() << ">' found in '" << filePath << "' on row " << xmlNode->row() << "." << std::endl;
			}
		}
	}

	return true;
}


/**
 * Parse the <graphics> tab.
 *
 * \todo	Check for sane configurations, particularly screen resolution.
 */
void Configuration::parseGraphics(void* _n)
{
	// NOTE: Void pointer used to avoid implementation details in the class declaration.

	XmlElement* element = static_cast<XmlNode*>(_n)->toElement();

	// Probably not a necessary check but here for robustness.
	if (!element)
	{
		std::cout << "Unexpected XML tag '" << static_cast<XmlNode*>(_n)->value() << "' found in configuration file while processing the '<graphics>' element." << std::endl;
		return;
	}

	XmlAttribute* attribute = element->firstAttribute();
	while (attribute)
	{
		if (attribute->name() == GRAPHICS_CFG_SCREEN_WIDTH) { attribute->queryIntValue(mScreenWidth); }
		else if (attribute->name() == GRAPHICS_CFG_SCREEN_HEIGHT) { attribute->queryIntValue(mScreenHeight); }
		else if (attribute->name() == GRAPHICS_CFG_SCREEN_DEPTH) { attribute->queryIntValue(mScreenBpp); }
		else if (attribute->name() == GRAPHICS_CFG_FULLSCREEN) { fullscreen(toLowercase(attribute->value()) == "true"); }
		else if (attribute->name() == GRAPHICS_CFG_VSYNC) { vsync(toLowercase(attribute->value()) == "true"); }
		else { std::cout << "Unexpected attribute '" << attribute->name() << "' found in '" << element->value() << "'." << std::endl; }

		attribute = attribute->next();
	}
}


/**
 * Parses audio information from an XML node.
 * 
 * \note	If any values are invalid or non-existant, this
 *			function will set default values.
 */
void Configuration::parseAudio(void* _n)
{
	// NOTE: Void pointer used to avoid implementation details in the class declaration.

	XmlElement* element = static_cast<XmlNode*>(_n)->toElement();

	// Probably not a necessary check but here for robustness.
	if (!element)
	{
		std::cout << "Unexpected XML tag '" << static_cast<XmlNode*>(_n)->value() << "' found in configuration file while processing the '<audio>' element." << std::endl;
		return;
	}

	XmlAttribute* attribute = element->firstAttribute();
	while (attribute)
	{
		if (attribute->name() == AUDIO_CFG_MIXRATE)
		{
			attribute->queryIntValue(mMixRate);
			if (mMixRate != AUDIO_LOW_QUALITY && mMixRate != AUDIO_MEDIUM_QUALITY && mMixRate != AUDIO_HIGH_QUALITY)
			{
				std::cout << "Invalid audio mixrate setting '" << mMixRate << "'. Expected 11025, 22050 or 44100. Setting to default of 22050." << std::endl;
				audioMixRate(AUDIO_MEDIUM_QUALITY);
			}
		}
		else if (attribute->name() == AUDIO_CFG_CHANNELS)
		{
			attribute->queryIntValue(mStereoChannels);

			if (mStereoChannels != AUDIO_MONO && mStereoChannels != AUDIO_STEREO)
			{
				std::cout << "Invalid audio channels setting '" << mStereoChannels << "'. Expected 1 or 2. Setting to default of 2." << std::endl;
				audioStereoChannels(AUDIO_STEREO);
			}
		}
		else if (attribute->name() == AUDIO_CFG_SFX_VOLUME)
		{
			attribute->queryIntValue(mSfxVolume);

			if (mSfxVolume < AUDIO_SFX_MIN_VOLUME || mSfxVolume > AUDIO_SFX_MAX_VOLUME)
			{
				audioSfxVolume(clamp(mSfxVolume, AUDIO_SFX_MIN_VOLUME, AUDIO_SFX_MAX_VOLUME));
			}
		}
		else if (attribute->name() == AUDIO_CFG_MUS_VOLUME)
		{
			attribute->queryIntValue(mMusicVolume);

			if (mMusicVolume < AUDIO_SFX_MIN_VOLUME || mMusicVolume > AUDIO_SFX_MAX_VOLUME)
			{
				audioSfxVolume(clamp(mMusicVolume, AUDIO_SFX_MIN_VOLUME, AUDIO_SFX_MAX_VOLUME));
			}
		}
		else if (attribute->name() == AUDIO_CFG_BUFFER_SIZE)
		{
			attribute->queryIntValue(mBufferLength);
			if (mBufferLength < AUDIO_BUFFER_MIN_SIZE || mBufferLength > AUDIO_BUFFER_MAX_SIZE)
			{
				audioBufferSize(clamp(mBufferLength, AUDIO_BUFFER_MIN_SIZE, AUDIO_BUFFER_MAX_SIZE));
			}
		}
		else if (attribute->name() == AUDIO_CFG_MIXER)
		{
			mMixerName = attribute->value();
		}
		else
		{
			std::cout << "Unexpected attribute '" << attribute->name() << "' found in '" << element->value() << "'." << std::endl;
		}

		attribute = attribute->next();
	}
}


/**
 * Parses program options from an XML node.
 * 
 * \note	Use of void pointer in declaration to avoid implementation details in header.
 */
void Configuration::parseOptions(void* _n)
{
	// NOTE: Void pointer used to avoid implementation details in the class declaration.
	XmlElement* element = static_cast<XmlNode*>(_n)->toElement();

	// Probably not a necessary check but here for robustness.
	if (!element)
	{
		std::cout << "Unexpected XML tag '" << static_cast<XmlNode*>(_n)->value() << "' found in configuration file while processing the '<graphics>' element." << std::endl;
		return;
	}

	XmlNode *node = nullptr;
	while ((node = element->iterateChildren(node)))
	{
		if (node->value() == "option")
		{
			XmlAttribute* attribute = node->toElement()->firstAttribute();

			std::string name, value;
			while (attribute)
			{
				if (attribute->name() == "name")
				{
					name = attribute->value();
				}
				else if (attribute->name() == "value")
				{
					value = attribute->value();
				}
				else
				{
					std::cout << "Unexpected attribute '" << attribute->name() << "' found in '" << element->value() << "'." << std::endl;
				}

				attribute = attribute->next();
			}

			if (name.empty() || value.empty())
			{
				std::cout << "Invalid name/value pair in <option> tag in configuration file on row " << node->row() << ". This option will be ignored." << std::endl;
			}
			else
			{
				mOptions[name] = value;
			}
		}
		else
		{
			std::cout << "Unexpected tag '<" << node->value() << ">' found in configuration on row " << node->row() << "." << std::endl;
		}
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
 * Sets fullscreen mode.
 * 
 * \param	fullscreen	Sets fullscreen mode when \c true.
 */
void Configuration::fullscreen(bool fullscreen)
{
	mFullScreen = fullscreen;
	mOptionChanged = true;
}


/**
 * Toggles vertical sync.
 * 
 * \param	vsync	Turns on vertical synch when \c true.
 * 
 * \note	This option does nothing when the graphics driver
 *			is set to a forced mode outside of the application.
 *			This is commonly seen on nVidia GPU's with global
 *			settings in place.
 */
void Configuration::vsync(bool vsync)
{
	mVSync = vsync;
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
	if (mixrate != AUDIO_LOW_QUALITY && mixrate != AUDIO_MEDIUM_QUALITY && mixrate != AUDIO_HIGH_QUALITY)
	{
		mixrate = AUDIO_MEDIUM_QUALITY;
	}

	mMixRate = mixrate;
	mOptionChanged = true;
}


/**
 * Sets the audio mixer.
 * 
 * \param	mixer	Sets the name of the mixer driver to use.
 * 
 * \note	The following values are available: "SDL"
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
 * \param	volume	Music volume. Can be anywhere between AUDIO_MUSIC_MIN_VOLUME
 *					and AUDIO_MUSIC_MAX_VOLUME.
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
void Configuration::option(const std::string& option, const std::string& value, bool overwrite)
{
	if (!overwrite && mOptions.find(option) != mOptions.end())
	{
		return;
	}

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
	if (mOptions.find(key) != mOptions.end())
	{
		mOptionChanged = true;
	}

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
void Configuration::deleteOption(const std::string& option)
{
	Options::iterator it = mOptions.find(option);

	if (it != mOptions.end())
	{
		mOptions.erase(it);
		mOptionChanged = true;
	}
}
