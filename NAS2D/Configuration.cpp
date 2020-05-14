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
#include <utility>

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
	std::map<std::string, Dictionary> merge(const std::map<std::string, Dictionary>& defaults, const std::map<std::string, Dictionary>& priorityValues)
	{
		std::map<std::string, Dictionary> results = defaults;
		for (const auto& [key, dictionary] : priorityValues)
		{
			results[key] += dictionary;
		}
		return results;
	}


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


	XmlElement* DictionaryToXmlElementAttributes(const std::string& tagName, const Dictionary& dictionary)
	{
		XmlElement* element = new XmlElement(tagName.c_str());

		for (const auto& key : dictionary.keys())
		{
			element->attribute(key, dictionary.get(key));
		}

		return element;
	}


	XmlElement* DictionaryToXmlElementOptions(const std::string& tagName, const Dictionary& dictionary)
	{
		XmlElement* element = new XmlElement(tagName.c_str());

		for (const auto& key : dictionary.keys())
		{
			XmlElement* option = new XmlElement("option");
			option->attribute("name", key);
			option->attribute("value", dictionary.get(key));
			element->linkEndChild(option);
		}

		return element;
	}


	XmlElement* SectionsToXmlElement(const std::string& tagName, const std::map<std::string, Dictionary>& sections)
	{
		XmlElement* element = new XmlElement(tagName);

		for (const auto& [key, dictionary] : sections)
		{
			element->linkEndChild(DictionaryToXmlElementAttributes(key, dictionary));
		}

		return element;
	}
}


Configuration::Configuration(std::map<std::string, Dictionary> defaults) :
	mDefaults{std::move(defaults)},
	mSettings{mDefaults}
{
	if (mDefaults.find("graphics") != mDefaults.end())
	{
		parseGraphics(mDefaults.at("graphics"));
	}
	if (mDefaults.find("audio") != mDefaults.end())
	{
		parseAudio(mDefaults.at("audio"));
	}
	if (mDefaults.find("options") != mDefaults.end())
	{
		parseOptions(mDefaults.at("options"));
	}
}


/**
 * Reads a given XML configuration file.
 *
 * \param fileData	Name of an XML Configuration file to be read.
 */
void Configuration::loadData(const std::string& fileData)
{
	// Start parsing through the Config.xml file.
	mLoadedSettings = ParseXmlSections(fileData, "configuration");
	mSettings = merge(mDefaults, mLoadedSettings);
	ReportProblemNames(getKeys(mSettings), {"graphics", "audio", "options"});

	parseGraphics(mSettings.at("graphics"));
	parseAudio(mSettings.at("audio"));
	parseOptions(mSettings.at("options"));
}


/**
 * Loads a configuration file.
 *
 * \param	filePath	A string indicating the file to load and process for configuration.
 */
void Configuration::load(const std::string& filePath)
{
	std::cout << "Initializing Configuration... ";

	if (!Utility<Filesystem>::get().exists(filePath))
	{
		std::cout << "configuration file '" << filePath << "' does not exist. Using default options." << std::endl;
	}
	else
	{
		try {
			// Read in the Config File.
			File xmlFile = Utility<Filesystem>::get().open(filePath);
			loadData(xmlFile.raw_bytes());
			std::cout << "done." << std::endl;
		}
		catch (const std::runtime_error& e) {
			std::cout << "unable to process '" << filePath << "'. Using default options. Error: " << e.what() << std::endl;
		}
	}

}


/**
 * Saves the Configuration to an XML string
 */
std::string Configuration::saveData() const
{
	XmlDocument doc;

	XmlComment* comment = new XmlComment("Automatically generated Configuration file.");
	doc.linkEndChild(comment);

	Dictionary graphics;
	graphics.set("screenwidth", mScreenWidth);
	graphics.set("screenheight", mScreenHeight);
	graphics.set("bitdepth", mScreenBpp);
	graphics.set("fullscreen", mFullScreen);
	graphics.set("vsync", mVSync);

	Dictionary audio;
	audio.set("mixrate", mMixRate);
	audio.set("channels", mStereoChannels);
	audio.set("sfxvolume", mSfxVolume);
	audio.set("musicvolume", mMusicVolume);
	audio.set("bufferlength", mBufferLength);
	audio.set("mixer", mMixerName);

	auto* root = SectionsToXmlElement("configuration", std::map<std::string, Dictionary>{{"graphics", graphics}, {"audio", audio}});
	root->linkEndChild(DictionaryToXmlElementOptions("options", mOptions));
	doc.linkEndChild(root);

	// Write out the XML file.
	XmlMemoryBuffer buff;
	doc.accept(&buff);

	return buff.buffer();
}


/**
 * Saves the Configuration to an XML file.
 */
void Configuration::save(const std::string& filePath) const
{
	try
	{
		Utility<Filesystem>::get().write(File(saveData(), filePath));
	}
	catch (const std::runtime_error& e)
	{
		throw std::runtime_error("Error saving configuration file: '" + filePath + "'  Error: " + e.what());
	}
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
		audioMixRate(AUDIO_MEDIUM_QUALITY);
	}
	if (mStereoChannels != AUDIO_MONO && mStereoChannels != AUDIO_STEREO)
	{
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
		audioBufferSize(mBufferLength);
	}
}


void Configuration::parseOptions(const Dictionary& dictionary)
{
	mOptions = dictionary;
}


/**
 * Sets the screen width.
 *
 * \todo	Needs proper value and error checking.
 */
void Configuration::graphicsWidth(int width)
{
	mScreenWidth = width;
	mSettings["graphics"].set(GRAPHICS_CFG_SCREEN_WIDTH, width);
}


/**
 * Sets the screen height.
 *
 * \todo	Needs proper value and error checking.
 */
void Configuration::graphicsHeight(int height)
{
	mScreenHeight = height;
	mSettings["graphics"].set(GRAPHICS_CFG_SCREEN_HEIGHT, height);
}


/**
 * Sets the screen depth.
 *
 * \todo	Needs proper value and error checking.
 */
void Configuration::graphicsColorDepth(int bpp)
{
	mScreenBpp = bpp;
	mSettings["graphics"].set(GRAPHICS_CFG_SCREEN_DEPTH, bpp);
}


/**
 * Sets fullscreen mode.
 *
 * \param	fullscreen	Sets fullscreen mode when \c true.
 */
void Configuration::fullscreen(bool fullscreen)
{
	mFullScreen = fullscreen;
	mSettings["graphics"].set(GRAPHICS_CFG_FULLSCREEN, fullscreen);
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
	mSettings["graphics"].set(GRAPHICS_CFG_VSYNC, vsync);
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
}


/**
 * Sets the number of sound channels.
 *
 * \param	channels	Number of audio channels. Can either be AUDIO_MONO or AUDIO_STEREO.
 */
void Configuration::audioStereoChannels(int channels)
{
	mStereoChannels = std::clamp(channels, AUDIO_MONO, AUDIO_STEREO);
}


/**
 * Sets the Sound Effects volume.
 *
 * \param	volume	Sound volume. Can be anywhere between AUDIO_SFX_MIN_VOLUME and AUDIO_SFX_MAX_VOLUME.
 */
void Configuration::audioSfxVolume(int volume)
{
	mSfxVolume = std::clamp(volume, AUDIO_SFX_MIN_VOLUME, AUDIO_SFX_MAX_VOLUME);
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
	mBufferLength = std::clamp(size, AUDIO_BUFFER_MIN_SIZE, AUDIO_BUFFER_MAX_SIZE);
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
	if (!overwrite && mOptions.has(option))
	{
		return;
	}

	mOptions.set(option, value);
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
std::string Configuration::option(const std::string& key)
{
	if (!mOptions.has(key))
	{
		mOptions.set(key, std::string{});
	}

	return mOptions.get(key);
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
	if (mOptions.has(option))
	{
		mOptions.erase(option);
	}
}
