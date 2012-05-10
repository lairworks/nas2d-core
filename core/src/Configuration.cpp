// ==================================================================================
// = NAS2D
// = Copyright © 2008 - 2012 New Age Software
// ==================================================================================
// = NAS2D is distributed under the terms of the zlib license. You are free to copy,
// = modify and distribute the software under the terms of the zlib license.
// = 
// = Acknowledgement of your use of NAS2D is appriciated but is not required.
// ==================================================================================

#include "NAS2D/Configuration.h"
#include "NAS2D/Filesystem.h"
#include "NAS2D/Singleton.h"
#include "NAS2D/XmlAttributeParser.h"

// Set some basic constants.
#define AUDIO_LOW_QUALITY			11025
#define AUDIO_MEDIUM_QUALITY		22050
#define AUDIO_HIGH_QUALITY			44100
#define AUDIO_CHANNELS				16
#define AUDIO_SFX_MIN_VOLUME		0
#define AUDIO_SFX_MAX_VOLUME		128
#define AUDIO_SFX_VOLUME			128
#define AUDIO_MUSIC_MIN_VOLUME		0
#define AUDIO_MUSIC_MAX_VOLUME		128
#define AUDIO_MUSIC_VOLUME			100
#define AUDIO_BUFFER_SIZE			1024
#define AUDIO_MONO					1
#define AUDIO_STEREO				2
#define	AUDIO_MIXER					"SDL"

#define GRAPHICS_WIDTH				800
#define GRAPHICS_HEIGHT				600
#define GRAPHICS_BITDEPTH			32
#define GRAPHICS_RENDERER			"OGL"
#define GRAPHICS_VSYNC				"false"
#define GRAPHICS_TEXTURE_QUALITY	GRAPHICS_GOOD
#define GRAPHICS_FULLSCREEN			false


/**
 * C'tor
 */
Configuration::Configuration():	mConfigFile(new TiXmlDocument()),
								mScreenWidth(GRAPHICS_WIDTH),
								mScreenHeight(GRAPHICS_HEIGHT),
								mScreenBpp(GRAPHICS_BITDEPTH),
								mTextureQuality(GRAPHICS_TEXTURE_QUALITY),
								mFullScreen(false),
								mRendererName(GRAPHICS_RENDERER),
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

	if(mConfigFile)
	{
		delete mConfigFile;
		mConfigFile = NULL;
	}

	cout << "Configuration Terminated." << endl;
}


/**
 * Loads a configuration file.
 *
 * \param	filePath	A string indicating the file to load and process for configuration.
 */
void Configuration::load(const std::string& filePath)
{
	cout << "Initializing Configuration..." << endl;
	
	// Read in the Config File.
	if(!readConfig(filePath))
	{
		mOptionChanged = true;
		cout << "Unable to read '" << filePath << "'. Using default options." << endl;
	}
}


void Configuration::save()
{
	TiXmlDocument *doc = new TiXmlDocument();

	TiXmlComment *comment = new TiXmlComment("Automatically generated Configuration file. This is best left untouched.");
	doc->LinkEndChild(comment);

	TiXmlElement *root = new TiXmlElement("configuration");
	doc->LinkEndChild(root);

	TiXmlElement *graphics = new TiXmlElement("graphics");
	graphics->SetAttribute("screenwidth", mScreenWidth);
	graphics->SetAttribute("screenheight", mScreenHeight);
	graphics->SetAttribute("bitdepth", mScreenBpp);
	// Fullscreen Attribute
	if(mFullScreen)
		graphics->SetAttribute("fullscreen", "true");
	else
		graphics->SetAttribute("fullscreen", "false");
	
	if(mVSync)
		graphics->SetAttribute("vsync", "true");
	else
		graphics->SetAttribute("vsync", "false");

	if(mTextureQuality == GL_NEAREST)
		graphics->SetAttribute("texturequality", "fast");
	else if(mTextureQuality == GL_LINEAR)
		graphics->SetAttribute("texturequality", "good");

	graphics->SetAttribute("renderer", mRendererName);
	root->LinkEndChild(graphics);

	TiXmlElement *audio = new TiXmlElement("audio");
	audio->SetAttribute("mixrate", mMixRate);
	audio->SetAttribute("channels", mStereoChannels);
	audio->SetAttribute("sfxvolume", mSfxVolume);
	audio->SetAttribute("musicvolume", mMusicVolume);
	audio->SetAttribute("bufferlength", mBufferLength);
	audio->SetAttribute("mixer", mMixerName);
	root->LinkEndChild(audio);

	// Options
	TiXmlElement *options = new TiXmlElement("options");
	root->LinkEndChild(options);
    
	// Iterate through the gui options list
	Options::iterator optIt = mOptions.begin();
	while(optIt != mOptions.end())
	{
		TiXmlElement *option = new TiXmlElement("option");
		option->SetAttribute("name", optIt->first);
		option->SetAttribute("value", optIt->second);
		options->LinkEndChild(option);
		optIt++;
	}

	// Write out the XML file.
	TiXmlPrinter printer;
	doc->Accept(&printer);

	Singleton<Filesystem>::get().write(File(printer.Str(), "config.xml"));

	delete doc;
	doc = 0;
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
 * Writes all stored values to disk.
 */



/**
 * Reads a given XML configuration file.
 *
 * \param filePath	Name of an XML Configuration file to be read.
 */
bool Configuration::readConfig(const string& filePath)
{
	File xmlFile = Singleton<Filesystem>::get().open(filePath);

	TiXmlElement  *xmlRootElement;

	// Load the XML document and handle any errors if occuring
	mConfigFile->Parse(xmlFile.raw_bytes());
	if(mConfigFile->Error())
	{
		cout << "Configuration malformed. Error on Row " << mConfigFile->ErrorRow() << ", Column " << mConfigFile->ErrorCol() << ": " << mConfigFile->ErrorDesc() << endl;
		return false;
	}
	else
	{
		// Check that we're using an XML Document.
		if (mConfigFile->ToDocument() != 0)
		{
			// Find the <configuration> tag
			xmlRootElement = mConfigFile->FirstChildElement("configuration");
			if(xmlRootElement == 0)
			{
				cout << "'" << filePath << "' doesn't contain a '<configuration>' tag." << endl;
				return false;
			}
			
		} 
		else 
		{
			cout << "Configuration parsing train wrecked."  << endl;
			setDefaultValues();
			return false;
		}

		// Start parsing through the Config.xml file.
		int result = 0;
		
		TiXmlNode *xmlNode = 0;
		while(xmlNode = xmlRootElement->IterateChildren(xmlNode))
		{
			if(xmlNode->ValueStr() == "graphics")
			{
				parseGraphics(xmlNode);
			}
			else if(xmlNode->ValueStr() == "audio")
			{
				parseAudio(xmlNode);
			}
			else if(xmlNode->ValueStr() == "options")
			{
				parseOptions(xmlNode);
			}
			else if(xmlNode->ValueStr() == "gui-options")
			{
				//if(!parseGuiOptions(xmlNode))
					//return false;
				// Add stuff here.
				cout << "GUI options are not supported at this time." << endl;
			}
			else
				cout << "Unexpected tag '<" << xmlNode->ValueStr() << ">' found in '" << filePath << "' on row " << xmlNode->Row() << "." << endl;
		}
	}

    return true;
}


/**
 * Parse the <graphics> tab.
 *
 * \todo	Check for sane configurations, particularly screen resolution.
 */
void Configuration::parseGraphics(TiXmlNode *node)
{
	XmlAttributeParser parser;

	mScreenWidth = parser.intAttribute(node, "screenwidth");
	mScreenHeight = parser.intAttribute(node, "screenheight");
	mScreenBpp = parser.intAttribute(node, "bitdepth");

	string fs = parser.stringAttribute(node, "fullscreen");
	(toLowercase(fs) == "true") ? fullscreen(true) : fullscreen(false);


	string vSync = parser.stringAttribute(node, "vsync");
	if(toLowercase(vSync) != "true" && toLowercase(vSync) != "false")
		vsync(true);
	else
		toLowercase(vSync) == "true" ? vsync(true) : vsync(false);

	mRendererName = parser.stringAttribute(node, "renderer");
	if(toLowercase(mRendererName) != "sdl" && toLowercase(mRendererName) != "ogl")
		renderer(GRAPHICS_RENDERER);

	graphicsTextureQuality(parser.stringAttribute(node, "texturequality"));
}

/**
 * Parses audio information from an XML node.
 * 
 * \note	If any values are invalid or non-existant, this
 *			function will set default values.
 */
void Configuration::parseAudio(TiXmlNode *node)
{
	XmlAttributeParser parser;

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
 */
void Configuration::parseOptions(TiXmlNode *node)
{
	XmlAttributeParser parser;

	TiXmlNode *xmlNode = 0;
	while(xmlNode = node->IterateChildren(xmlNode))
	{
		if(xmlNode->ValueStr() == "option")
		{
			// Ensure that there is a 'name' attribute.
			string option = parser.stringAttribute(xmlNode, "name");

			if(!option.empty())
				mOptions[option] = parser.stringAttribute(xmlNode, "value");
			else
				cout << "Option tag is missing a name attribute on row " << xmlNode->Row() << "." << endl;
		}
		else
			cout << "Unexpected tag '<" << xmlNode->ValueStr() << ">' found in configuration on row " << xmlNode->Row() << "." << endl;
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
 * Gets the name of a Renderer.
 */
std::string Configuration::renderer() const
{
	return mRendererName;
}


/**
 * Sets the Renderer driver to use.
 */
void Configuration::renderer(const string& renderer)
{
	mRendererName = renderer;
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
const string& Configuration::mixer() const
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
void Configuration::graphicsTextureQuality(const string& quality)
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
	if(mixrate != AUDIO_LOW_QUALITY || mixrate != AUDIO_MEDIUM_QUALITY || mixrate != AUDIO_HIGH_QUALITY)
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
void Configuration::option(const string option, const string& value, bool overwrite)
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


/**
 * Gets a GUI option value given a \c key.
 *
 * \param	key		An alphanumeric key to search for.
 *
 * \return	A string containing the requested value. An empty
 *			string if the requested value was not found.
 *
 * \note	The search is \a case sensistive.
 *
string Configuration::getGuiOption(const std::string& key)
{
	GuiOptionTable::iterator it = mGuiOptions.find(key);

	if(it != mGuiOptions.end())
		return it->second;

	return string();
}


/**
 * Gets a GUI Position value given a \c key.
 *
 * \param	key		An alphanumeric key to search for.
 *
 * \return	An AbsCoordinate object. An empty AbsCoordinate object
 *			if the requested value was not found.
 *
 * \note	The search is \a case sensistive.
 *
AbsCoordinate Configuration::getGuiPosition(const std::string& key)
{
	GuiPositionTable::iterator it = mGuiPositions.find(key);

	if(it != mGuiPositions.end())
		return it->second;

	return AbsCoordinate();
}


/**
 * Sets a GUI Option.
 *
 * \param	key		Name of the option to set.
 * \param	value	Value to give the named option.
 *
void Configuration::setGuiOption(const std::string& key, const std::string& value)
{
	mGuiOptions[key] = value;
	mOptionChanged = true;
}


/**
 * Sets a GUI Position.
 *
 * \param	key			Name of the option to set.
 * \param	position	Coordinates of a named object.
 *
void Configuration::setGuiPosition(const std::string& key, const AbsCoordinate& position)
{
	mGuiPositions[key] = position;
	mOptionChanged = true;
}


void Configuration::clearGuiOption(const std::string& key)
{
	GuiOptionTable::iterator it = mGuiOptions.find(key);

	if(it != mGuiOptions.end())
		mGuiOptions.erase(it);

	mOptionChanged = true;
}


void Configuration::clearGuiPosition(const std::string& key)
{
	GuiPositionTable::iterator it = mGuiPositions.find(key);

	if(it != mGuiPositions.end())
		mGuiPositions.erase(it);

	mOptionChanged = true;
}
*/