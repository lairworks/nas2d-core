// ==================================================================================
// = NAS2D
// = Copyright © 2008 - 2019 New Age Software
// ==================================================================================
// = NAS2D is distributed under the terms of the zlib license. You are free to copy,
// = modify and distribute the software under the terms of the zlib license.
// =
// = Acknowledgement of your use of NAS2D is appriciated but is not required.
// ==================================================================================

#include "Configuration.h"
#include "Dictionary.h"
#include "ContainerUtils.h"
#include "Filesystem.h"
#include "Utility.h"
#include "Xml/Xml.h"

#include <iostream>
#include <algorithm>

using namespace NAS2D;
using namespace NAS2D::Xml;

// Set some basic constants.
const int AUDIO_LOW_QUALITY = 11025;
const int AUDIO_MEDIUM_QUALITY = 22050;
const int AUDIO_HIGH_QUALITY = 44100;
const int AUDIO_SFX_MIN_VOLUME = 0;
const int AUDIO_SFX_MAX_VOLUME = 128;
const int AUDIO_SFX_VOLUME = 128;
const int AUDIO_MUSIC_MIN_VOLUME = 0;
const int AUDIO_MUSIC_MAX_VOLUME = 128;
const int AUDIO_MUSIC_VOLUME = 100;
const int AUDIO_BUFFER_SIZE = 1024;
const int AUDIO_BUFFER_MIN_SIZE = 256;
const int AUDIO_BUFFER_MAX_SIZE = 4096;
const int AUDIO_MONO = 1;
const int AUDIO_STEREO = 2;
const std::string AUDIO_MIXER = "SDL";

const std::string AUDIO_CFG_MIXRATE = "mixrate";
const std::string AUDIO_CFG_CHANNELS = "channels";
const std::string AUDIO_CFG_SFX_VOLUME = "sfxvolume";
const std::string AUDIO_CFG_MUS_VOLUME = "musicvolume";
const std::string AUDIO_CFG_BUFFER_SIZE = "bufferlength";
const std::string AUDIO_CFG_MIXER = "mixer";

const int GRAPHICS_WIDTH = 800;
const int GRAPHICS_HEIGHT = 600;
const int GRAPHICS_BITDEPTH = 32;
const bool GRAPHICS_FULLSCREEN = false;
const bool GRAPHICS_VSYNC = false;

const std::string GRAPHICS_CFG_SCREEN_WIDTH = "screenwidth";
const std::string GRAPHICS_CFG_SCREEN_HEIGHT = "screenheight";
const std::string GRAPHICS_CFG_SCREEN_DEPTH = "bitdepth";
const std::string GRAPHICS_CFG_FULLSCREEN = "fullscreen";
const std::string GRAPHICS_CFG_VSYNC = "vsync";


namespace {
	void ReportProblemNames(
		const std::vector<std::string>& names,
		const std::vector<std::string>& required,
		const std::vector<std::string>& optional = {}
	) {
		using namespace ContainerOperators;

		const auto expected = required + optional;

		const auto missing = names - required;
		const auto unexpected = names - expected;

		if (!missing.empty() || !unexpected.empty())
		{
			throw std::runtime_error("Missing required names: {" + join(missing, ", ") +"}, unexpected names: {" + join(unexpected, ", ") + "}");
		}
	}


	Dictionary ParseXmlElementAttributesToDictionary(const XmlElement& element)
	{
		Dictionary dictionary;

		for (const auto* attribute = element.firstAttribute(); attribute; attribute = attribute->next())
		{
			dictionary.set(attribute->name(), attribute->value());
		}

		return dictionary;
	}


	Dictionary ParseOptionsToDictionary(const XmlElement& element)
	{
		Dictionary dictionary;

		for (const auto* setting = element.firstChildElement(); setting; setting = setting->nextSiblingElement())
		{
			if (setting->value() != "option")
			{
				throw std::runtime_error("Unexpected tag found in configuration on row: " + std::to_string(setting->row()) + "  <" + setting->value() + ">");
			}

			const auto optionDictionary = ParseXmlElementAttributesToDictionary(*setting);
			ReportProblemNames(optionDictionary.keys(), {"name", "value"});

			const auto name = optionDictionary.get("name");
			const auto value = optionDictionary.get("value");

			if (name.empty() || value.empty())
			{
				throw std::runtime_error("Invalid name/value pair in <option> tag in configuration file on row: " + std::to_string(setting->row()));
			}

			dictionary.set(name, value);
		}

		return dictionary;
	}


	std::map<std::string, Dictionary> ParseXmlSections(const XmlElement& element)
	{
		std::map<std::string, Dictionary> sections;

		for (auto childElement = element.firstChildElement(); childElement; childElement = childElement->nextSiblingElement())
		{
			if (childElement->type() == XmlNode::NodeType::XML_COMMENT) { continue; } // Ignore comments

			sections[childElement->value()] = ParseXmlElementAttributesToDictionary(*childElement) + ParseOptionsToDictionary(*childElement);
		}

		return sections;
	}


	std::map<std::string, Dictionary> ParseXmlSections(const std::string& xmlString, const std::string& sectionName)
	{
		XmlDocument xmlDocument;
		xmlDocument.parse(xmlString.c_str());

		if (xmlDocument.error())
		{
			throw std::runtime_error("Error parsing XML file on (Row " + std::to_string(xmlDocument.errorRow()) + ", Column " + std::to_string(xmlDocument.errorCol()) + "): " + xmlDocument.errorDesc());
		}

		XmlElement* root = xmlDocument.firstChildElement(sectionName);
		if (!root)
		{
			throw std::runtime_error("XML file does not contain tag: " + sectionName);
		}

		return ParseXmlSections(*root);
	}
}


/**
 * D'tor
 */
Configuration::~Configuration()
{
	if(mOptionChanged)
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
	graphics->attribute("fullscreen", mFullScreen ? "true" : "false");
	graphics->attribute("vsync", mVSync ? "true" : "false");
	root->linkEndChild(graphics);

	XmlElement* audio = new XmlElement("audio");
	audio->attribute("mixrate", mMixRate);
	audio->attribute("channels", mStereoChannels);
	audio->attribute("sfxvolume", mSfxVolume);
	audio->attribute("musicvolume", mMusicVolume);
	audio->attribute("bufferlength", mBufferLength);
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
	mVSync = GRAPHICS_VSYNC;

	mMixRate = AUDIO_MEDIUM_QUALITY;
	mStereoChannels = AUDIO_STEREO;
	mSfxVolume = AUDIO_SFX_VOLUME;
	mMusicVolume = AUDIO_MUSIC_VOLUME;
	mBufferLength = AUDIO_BUFFER_SIZE;
	mOptionChanged = true;
}


/**
 * Reads a given XML configuration file.
 *
 * \param filePath	Name of an XML Configuration file to be read.
 */
bool Configuration::readConfig(const std::string& filePath)
{
	File xmlFile = Utility<Filesystem>::get().open(filePath);

	// Start parsing through the Config.xml file.
	const auto sections = ParseXmlSections(xmlFile.raw_bytes(), "configuration");
	ReportProblemNames(getKeys(sections), {"graphics", "audio", "options"});

	parseGraphics(sections.at("graphics"));
	parseAudio(sections.at("audio"));
	parseOptions(sections.at("options"));

	return true;
}


void Configuration::parseGraphics(const Dictionary& dictionary)
{
	ReportProblemNames(dictionary.keys(), {GRAPHICS_CFG_SCREEN_WIDTH, GRAPHICS_CFG_SCREEN_HEIGHT, GRAPHICS_CFG_SCREEN_DEPTH, GRAPHICS_CFG_FULLSCREEN, GRAPHICS_CFG_VSYNC});

	mScreenWidth = dictionary.get<int>(GRAPHICS_CFG_SCREEN_WIDTH);
	mScreenHeight = dictionary.get<int>(GRAPHICS_CFG_SCREEN_HEIGHT);
	mScreenBpp = dictionary.get<int>(GRAPHICS_CFG_SCREEN_DEPTH);
	fullscreen(dictionary.get<bool>(GRAPHICS_CFG_FULLSCREEN));
	vsync(dictionary.get<bool>(GRAPHICS_CFG_VSYNC));
}


void Configuration::parseAudio(const Dictionary& dictionary)
{
	ReportProblemNames(dictionary.keys(), {AUDIO_CFG_MIXRATE, AUDIO_CFG_CHANNELS, AUDIO_CFG_SFX_VOLUME, AUDIO_CFG_MUS_VOLUME, AUDIO_CFG_BUFFER_SIZE, AUDIO_CFG_MIXER});

	mMixRate = dictionary.get<int>(AUDIO_CFG_MIXRATE);
	mStereoChannels = dictionary.get<int>(AUDIO_CFG_CHANNELS);
	mSfxVolume = dictionary.get<int>(AUDIO_CFG_SFX_VOLUME);
	mMusicVolume = dictionary.get<int>(AUDIO_CFG_MUS_VOLUME);
	mBufferLength = dictionary.get<int>(AUDIO_CFG_BUFFER_SIZE);
	mMixerName = dictionary.get(AUDIO_CFG_MIXER);

	if (mMixRate != AUDIO_LOW_QUALITY && mMixRate != AUDIO_MEDIUM_QUALITY && mMixRate != AUDIO_HIGH_QUALITY)
	{
		std::cout << "Invalid audio mixrate setting '" << mMixRate << "'. Expected 11025, 22050 or 44100. Setting to default of 22050." << std::endl;
		audioMixRate(AUDIO_MEDIUM_QUALITY);
	}
	if (mStereoChannels != AUDIO_MONO && mStereoChannels != AUDIO_STEREO)
	{
		std::cout << "Invalid audio channels setting '" << mStereoChannels << "'. Expected 1 or 2. Setting to default of 2." << std::endl;
		audioStereoChannels(AUDIO_STEREO);
	}
	if (mSfxVolume < AUDIO_SFX_MIN_VOLUME || mSfxVolume > AUDIO_SFX_MAX_VOLUME)
	{
		audioSfxVolume(mSfxVolume);
	}
	if (mMusicVolume < AUDIO_MUSIC_MIN_VOLUME || mMusicVolume > AUDIO_MUSIC_MAX_VOLUME)
	{
		audioMusicVolume(mMusicVolume);
	}
	if (mBufferLength < AUDIO_BUFFER_MIN_SIZE || mBufferLength > AUDIO_BUFFER_MAX_SIZE)
	{
		audioBufferSize(std::clamp(mBufferLength, AUDIO_BUFFER_MIN_SIZE, AUDIO_BUFFER_MAX_SIZE));
	}
}


void Configuration::parseOptions(const Dictionary& dictionary)
{
	for (const auto& key : dictionary.keys())
	{
		mOptions[key] = dictionary.get(key);
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
	mStereoChannels = std::clamp(channels, AUDIO_MONO, AUDIO_STEREO);
	mOptionChanged = true;
}


/**
 * Sets the Sound Effects volume.
 *
 * \param	volume	Sound volume. Can be anywhere between AUDIO_SFX_MIN_VOLUME and AUDIO_SFX_MAX_VOLUME.
 */
void Configuration::audioSfxVolume(int volume)
{
	mSfxVolume = std::clamp(volume, AUDIO_SFX_MIN_VOLUME, AUDIO_SFX_MAX_VOLUME);
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
	mMusicVolume = std::clamp(volume, AUDIO_MUSIC_MIN_VOLUME, AUDIO_MUSIC_MAX_VOLUME);
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
