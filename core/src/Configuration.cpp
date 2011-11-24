// ==================================================================================
// = NAS2D
// = Copyright © 2008 New Age Software
// ==================================================================================
// = NAS2D is distributed under the terms of the zlib license. You are free to copy,
// = modify and distribute the software as long under the terms of the zlib license.
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
		writeConfig();

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
void Configuration::loadConfig(const std::string& filePath)
{
	cout << "Initializing Configuration..." << endl;
	
	// Read in the Config File.
	if(!readConfig(filePath))
	{
		mOptionChanged = true;
		cout << "Unable to read '" << filePath << "'. Using default options." << endl;
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
void Configuration::writeConfig()
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

	/*
	// GUI Options
	TiXmlElement *gui_options = new TiXmlElement("gui-options");
	root->LinkEndChild(gui_options);
    
	// Iterate through the gui options list
	GuiOptionTable::iterator optIt = mGuiOptions.begin();
	while(optIt != mGuiOptions.end())
	{
		TiXmlElement *option = new TiXmlElement("option");
		option->SetAttribute("name", optIt->first);
		option->SetAttribute("value", optIt->second);
		gui_options->LinkEndChild(option);
		optIt++;
	}

	// Iterate through the gui positions list
	GuiPositionTable::iterator posIt = mGuiPositions.begin();
	while(posIt != mGuiPositions.end())
	{
		TiXmlElement *position = new TiXmlElement("position");
		position->SetAttribute("name", posIt->first);
		position->SetAttribute("x", posIt->second.first);
		position->SetAttribute("y", posIt->second.second);
		gui_options->LinkEndChild(position);
		posIt++;
	}
	*/
	
	
	// Write out the XML file.
	TiXmlPrinter printer;
	doc->Accept(&printer);

	Singleton<Filesystem>::get().write(File(printer.Str(), "config.xml"));


	delete doc;
	doc = 0;
}


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


void Configuration::saveConfig()
{
	cout << "Saving configuration changes." << endl;
	writeConfig();
}


/**
 * Parse the <graphics> tab.
 *
 * \todo	Check for sane configurations, particularly screen resolution.
 */
void Configuration::parseGraphics(TiXmlNode *node)
{
	XmlAttributeParser parser;

	mScreenWidth = parser.getIntAttribute(node, "screenwidth");
	mScreenHeight = parser.getIntAttribute(node, "screenheight");
	mScreenBpp = parser.getIntAttribute(node, "bitdepth");

	string fs = parser.getStringAttribute(node, "fullscreen");
	(toLowercase(fs) == "true") ? mFullScreen = true : mFullScreen = false;


	string vsync = parser.getStringAttribute(node, "vsync");
	if(toLowercase(vsync) != "true" && toLowercase(vsync) != "false")
		setGraphicsVSync(true);
	else
		toLowercase(vsync) == "true" ? mVSync = true : mVSync = false;

	mRendererName = parser.getStringAttribute(node, "renderer");
	if(toLowercase(mRendererName) != "sdl" && toLowercase(mRendererName) != "ogl")
		setRenderer(GRAPHICS_RENDERER);

	setGraphicsTextureQuality(parser.getStringAttribute(node, "texturequality"));
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

	mMixRate = parser.getIntAttribute(node, "mixrate");
	if(mMixRate == 0)
		setAudioMixRate(AUDIO_MEDIUM_QUALITY);

	mStereoChannels = parser.getIntAttribute(node, "channels");
	if(mStereoChannels != AUDIO_MONO && mStereoChannels != AUDIO_STEREO)
		setAudioStereoChannels(AUDIO_STEREO);

	mSfxVolume = parser.getIntAttribute(node, "sfxvolume");
	if(mSfxVolume < AUDIO_SFX_MIN_VOLUME || mSfxVolume > AUDIO_SFX_MAX_VOLUME)
		setAudioSfxVolume(AUDIO_SFX_VOLUME);


	mMusicVolume = parser.getIntAttribute(node, "musicvolume");
	if(mMusicVolume < AUDIO_MUSIC_MIN_VOLUME || mMusicVolume > AUDIO_MUSIC_MAX_VOLUME)
		setAudioSfxVolume(AUDIO_MUSIC_VOLUME);


	mBufferLength = parser.getIntAttribute(node, "bufferlength");


	mMixerName = parser.getStringAttribute(node, "mixer");
	if(mMixerName != "SDL")
		setAudioMixer(AUDIO_MIXER);
}


/*
bool Configuration::parseGuiOptions(TiXmlNode *node)
{
	TiXmlNode *xmlNode = 0;
	while(xmlNode = node->IterateChildren(xmlNode))
	{
		if(xmlNode->ValueStr() == "option")
		{
			parseGuiOption(xmlNode);
				//return false;
		}
		else if(xmlNode->ValueStr() == "position")
		{
			parseGuiPosition(xmlNode);
				//return false;
		}
		else
			cout << "Unexpected tag '<" << xmlNode->ValueStr() << ">' found in configuration file on row " << xmlNode->Row() << "." << endl;
	}

	return true;
}


bool Configuration::parseGuiOption(TiXmlNode *node)
{
	// ===========================================
	// Option Name
	// ===========================================
	const char *name = node->ToElement()->Attribute("name");
	if(name == NULL)
	{
		cout << "Attribute 'name' in tag '<option>' contains invalid data on row " << node->Row() << " in file '" << mConfigFile->ValueStr() << "'." << endl;
		return false;
	}
	if(strlen(name) < 1)
	{
		cout << "Attribute 'name' in tag '<option>' is empty." << endl;
		return false;
	}

	// ===========================================
	// Option Value
	// ===========================================
	const char *value = node->ToElement()->Attribute("value");
	if(value == NULL)
	{
		Logger::log << Logger::warning() << "Attribute 'value' in tag '<option>' contains invalid data on row " << node->Row() << " in file '" << mConfigFile->ValueStr() << "'." << endl;
		return false;
	}
	if(strlen(value) < 1)
	{
		Logger::log << Logger::warning() << "Attribute 'value' in tag '<option>' is empty." << endl;
		return false;
	}

	setGuiOption(name, value);

	return true;
}


bool Configuration::parseGuiPosition(TiXmlNode *node)
{
	int x = 0, y = 0;

	int result = TIXML_SUCCESS;

	// ===========================================
	// Position Name
	// ===========================================
	const char *name = node->ToElement()->Attribute("name");
	if(name == NULL)
	{
		Logger::log << Logger::warning() << "Attribute 'name' in tag '<position>' contains invalid data on row " << node->Row() << " in file '" << mConfigFile->ValueStr() << "'." << endl;
		return false;
	}
	if(strlen(name) < 1)
	{
		Logger::log << Logger::warning() << "Attribute 'name' in tag '<position>' is empty." << endl;
		return false;
	}

	// ===========================================
	// Position X Value
	// ===========================================
	result = node->ToElement()->QueryIntAttribute("x", &x);
	if(result != TIXML_SUCCESS)
	{
		if(result == TIXML_NO_ATTRIBUTE)
		{
			Logger::log << Logger::warning() << "Tag '<position>' is missing attribute 'x' on row " << node->Row() << "." << endl;
			return false;
		}
		else if(result == TIXML_WRONG_TYPE)
		{
			Logger::log << Logger::warning() << "Attribute 'x' in tag '<position>' contains invalid data on row " << node->Row() << "." << endl;
			return false;
		}
		else
		{
			Logger::log << Logger::error() << "Unspecified error." << endl;
			return false;
		}
	}

	// ===========================================
	// Position Y Value
	// ===========================================
	result = node->ToElement()->QueryIntAttribute("y", &y);
	if(result != TIXML_SUCCESS)
	{
		if(result == TIXML_NO_ATTRIBUTE)
		{
			Logger::log << Logger::warning() << "Tag '<position>' is missing attribute 'y' on row " << node->Row() << "." << endl;
			return false;
		}
		else if(result == TIXML_WRONG_TYPE)
		{
			Logger::log << Logger::warning() << "Attribute 'y' in tag '<position>' contains invalid data on row " << node->Row() << "." << endl;
			return false;
		}
		else
		{
			Logger::log << Logger::error() << "Unspecified error." << endl;
			return false;
		}
	}

	setGuiPosition(name, AbsCoordinate(x, y));

	return true;
}
*/

/**
 * Returns screen Width.
 */
int Configuration::getGraphicsWidth() const
{
	return mScreenWidth;
}


/**
 * Returns screen Height.
 */
int Configuration::getGraphicsHeight() const
{
	return mScreenHeight;
}


/**
 * Returns Color Depth.
 */
int Configuration::getGraphicsColorDepth() const
{
	return mScreenBpp;
}


/**
 * Returns texture render quality.
 *
 * \note	Equivalent to GL_NEAREST or GL_LINEAR
 */
GraphicsQuality Configuration::getGraphicsTextureQuality() const
{
	return mTextureQuality;
}


/**
 * Returns true if fullscreen mode is requested.
 */
bool Configuration::isGraphicsFullscreen() const
{
	return mFullScreen;
}


/**
 * Returns true if vsync mode is requested.
 */
bool Configuration::isVsyncEnabled() const
{
	return mVSync;
}


/**
 * Returns the name of a Renderer.
 */
std::string Configuration::getRenderer() const
{
	return mRendererName;
}


/**
 * Sets the Renderer driver to use.
 */
void Configuration::setRenderer(const string& renderer)
{
	mRendererName = renderer;
}


/**
 * Returns the Audio Rate that should be used by the Mixer.
 */
int Configuration::getAudioMixRate() const
{
	return mMixRate;
}


/**
 * Returns Stereo or Mono
 */
int Configuration::getAudioStereoChannels() const
{
	return mStereoChannels;
}


/**
 * Returns the Volume Level for Sound Effects.
 */
int Configuration::getAudioSfxVolume() const
{
	return mSfxVolume;
}


/**
 * Returns the Volume Level for Music.
 */
int Configuration::getAudioMusicVolume() const
{
	return mMusicVolume;
}


/**
 * Returns the audio Buffer Length.
 */
int Configuration::getAudioBufferSize() const
{
	return mBufferLength;
}


/**
 * Sets the screen width.
 *
 * \todo	Needs proper value and error checking.
 */
void Configuration::setGraphicsWidth(int width)
{
	mScreenWidth = width;
	mOptionChanged = true;
}


/**
 * Sets the screen height.
 *
 * \todo	Needs proper value and error checking.
 */
void Configuration::setGraphicsHeight(int height)
{
	mScreenHeight = height;
	mOptionChanged = true;
}


/**
 * Sets the screen depth.
 *
 * \todo	Needs proper value and error checking.
 */
void Configuration::setGraphicsColorDepth(int bpp)
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
void Configuration::setGraphicsTextureQuality(const string& quality)
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
void Configuration::setGraphicsFullscreen(bool isFullscreen)
{
	mFullScreen = isFullscreen;
	mOptionChanged = true;
}


/**
 * Toggles the use of Vertical Sync.
 */
void Configuration::setGraphicsVSync(bool isVsync)
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
void Configuration::setAudioMixRate(int mixrate)
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
void Configuration::setAudioMixer(const std::string& mixer)
{
	mMixerName = mixer;
	mOptionChanged = true;
}


/**
 * Sets the number of sound channels.
 * 
 * \param	channels	Number of audio channels. Can either be AUDIO_MONO or AUDIO_STEREO.
 */
void Configuration::setAudioStereoChannels(int channels)
{
	mStereoChannels = clamp(channels, AUDIO_MONO, AUDIO_STEREO);
	mOptionChanged = true;
}


/**
 * Sets the Sound Effects volume.
 * 
 * \param	volume	Sound volume. Can be anywhere between AUDIO_SFX_MIN_VOLUME and AUDIO_SFX_MAX_VOLUME.
 */
void Configuration::setAudioSfxVolume(int volume)
{
	mSfxVolume = clamp(volume, AUDIO_SFX_MIN_VOLUME, AUDIO_SFX_MAX_VOLUME);
	mOptionChanged = true;
}


/**
 * Sets the Music volume.
 * 
 * \param	volume	Music volume. Can be anywhere between AUDIO_MUSIC_MIN_VOLUME and AUDIO_MUSIC_MAX_VOLUME.
 */
void Configuration::setAudioMusicVolume(int volume)
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
void Configuration::setAudioBufferSize(int size)
{
	mBufferLength = size;
	mOptionChanged = true;
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