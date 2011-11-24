// ==================================================================================
// = NAS2D
// = Copyright © 2008 New Age Software
// ==================================================================================
// = NAS2D is distributed under the terms of the zlib license. You are free to copy,
// = modify and distribute the software as long under the terms of the zlib license.
// = 
// = Acknowledgement of your use of NAS2D is appriciated but is not required.
// ==================================================================================

#include "NAS2D/Resources/Sprite.h"

#include <iostream>

using namespace std;

/**
 * Constructor.
 *
 * \param renderer		A pointer to a Renderer object.
 * \param filePath		A string containing the file path of the Sprite definition file to use.
 */
Sprite::Sprite(const string& filePath):	mTimer(new Timer()),
										mDocXml(new TiXmlDocument()),
										mSpriteName(filePath),
										mErrorMessage(""),
										mCurrentAction("default"),
										mCurrentFrame(0),
										mLastFrameTick(0),
										mAlpha(255),
										mPaused(false)
{
	if(!parseXml(filePath))
		cout << "Sprite '" << filePath << "' not loaded: " << mErrorMessage << endl;
	//else
		//cout << "Loaded Sprite '" << filePath << "' successfully." << endl;

	play("default");
}


/**
 * Clean up memory allocated for animation frames and such.
 */
Sprite::~Sprite()
{
	ActionList::iterator actionIt = mActions.begin();
	while(actionIt != mActions.end())
	{
		for(size_t i = 0; i < actionIt->second.size(); i++)
		{
			delete actionIt->second[i];
			actionIt->second[i] = NULL;
		}

		actionIt++;
	}

	SheetContainer::iterator sheetIt = mSpriteSheets.begin();
	for(sheetIt; sheetIt != mSpriteSheets.end(); sheetIt++)
	{
		delete sheetIt->second;
		sheetIt->second = NULL;
	}

	delete mTimer;
	mTimer = NULL;

	if(mDocXml)
	{
		delete mDocXml;
		mDocXml = NULL;
	}
}


/**
 * Copy Constructor
 */
Sprite::Sprite(const Sprite &sprite):	mTimer(new Timer()),
										mDocXml(NULL),
										mSpriteName(sprite.mSpriteName),
										mErrorMessage(sprite.mErrorMessage),
										mCurrentAction(sprite.mCurrentAction),
										mCurrentFrame(sprite.mCurrentFrame),
										mLastFrameTick(sprite.mLastFrameTick),
										mPaused(sprite.mPaused)
{

	// Copy frame lists. Note that we don't need to copy the list of
	// frame sheets as the sprite frames are already loaded and we can
	// simply copy them.
	ActionList::const_iterator actionIt = sprite.mActions.begin();
	while(actionIt != sprite.mActions.end())
	{
		FrameList frameList;
		for(size_t i = 0; i < actionIt->second.size(); i++)
			frameList.push_back(new spriteFrame(*actionIt->second[i]));

		//Logger::log << "Adding " << frameList.size() << " frames to action '" << actionIt->first << "'." << endl;
		mActions[actionIt->first] = frameList;
		actionIt++;
	}

	play(mCurrentAction);

}


/**
 * Assignment Operator
 */
Sprite& Sprite::operator=(const Sprite &rhs)
{
	if(mTimer)
		delete mTimer;
	mTimer = new Timer;

	if(mDocXml)
		delete mDocXml;
	mDocXml = NULL;
	
	mSpriteName = rhs.mSpriteName;
	mErrorMessage = rhs.mErrorMessage;
	mCurrentAction = rhs.mCurrentAction;
	mCurrentFrame = rhs.mCurrentFrame;
	mLastFrameTick = rhs.mLastFrameTick;
	mPaused = rhs.mPaused;

	// Free memory from old action list if one exists.
	if(mActions.size() > 0)
	{
		ActionList::iterator actionIt = mActions.begin();
		while(actionIt != mActions.begin())
		{
			for(size_t i = 0; i < actionIt->second.size(); i++)
			{
				delete actionIt->second[i];
				actionIt->second[i] = NULL;
			}

			actionIt++;
		}
	}

	// Copy actions and their associated frames from the provided Sprite.
	ActionList::const_iterator actionIt = rhs.mActions.begin();
	while(actionIt != rhs.mActions.end())
	{
		FrameList frameList;
		for(size_t i = 0; i < actionIt->second.size(); i++)
			frameList.push_back(new spriteFrame(*actionIt->second[i]));

		//Logger::log << "Adding " << frameList.size() << " frames to action '" << actionIt->first << "'." << endl;
		mActions[actionIt->first] = frameList;

		actionIt++;
	}

	play(mCurrentAction);

	return *this;
}


/**
 * Plays an action animation.
 *
 * \param action	Name of the action to use for animation.
 *
 * \note	If the named action doesn't exist, a warning message will
 *			be written to the log and the default action will be used
 *			instead.
 */
void Sprite::play(const string& action)
{
	string actionString = toLowercase(action);

	// Check the list for the action.
	ActionList::iterator actionIt = mActions.find(actionString);
	if(actionIt == mActions.end())
	{
		cout << "Action '" << action << "' does not exist in Sprite '" << mSpriteName << "'." << endl;
		// set the current action to default.
		mCurrentAction = "default";
		// Reset the current frame counter.
		mCurrentFrame = 0;
		return;
	}
	// If the named action is the current action.
	else if(mCurrentAction == actionString)
	{
		//mPlayingAction = &actionIt->second;
		return;
	}
	// Set the current action to the named action.
	else
	{
		mCurrentFrame = 0;	
		mCurrentAction = actionString;
	}
}


/**
 * Pauses animation for this Sprite.
 */
void Sprite::pause(bool pause)
{
	mPaused = pause;
}


/**
 * Instructs the Sprite to update itself.
 *
 * Updates the internal state of the Sprite, pushing forward
 * animation frames as necessary.
 *
 * \param x	X-Screen Coordinate to render the Sprite.
 * \param y	X-Screen Coordinate to render the Sprite.
 */
void Sprite::update(int x, int y)
{
	spriteFrame *frame = mActions[mCurrentAction][mCurrentFrame]; // Not as safe as at() but a good bit faster.

	if(!mPaused)
	{
		// Draw frame
		Singleton<Renderer>::get().drawImage(*frame->mFrameImage, x - frame->mAnchorX, y - frame->mAnchorY);

		unsigned int currentTick = mTimer->ms();
		if(currentTick - mLastFrameTick > frame->mFrameDelay)
		{
			mLastFrameTick = currentTick;
			mCurrentFrame++;
		}

		// Check that our frame count is within bounds.
		if(mCurrentFrame > mActions[mCurrentAction].size() - 1)
			mCurrentFrame = 0;
	}
	else
		Singleton<Renderer>::get().drawImage(*frame->mFrameImage, x - frame->mAnchorX, y - frame->mAnchorY);

}


/**
 * Gets a pointer to the current frame image.
 *
 * \warning	Returned pointer is owned by the Sprite. Do
 *			not free it.

Image* Sprite::currentFrameImage()
{
	spriteFrame *frame = mActions[mCurrentAction][mCurrentFrame];
	return frame->mFrameImage;
}
 */

/**
 * Parses a Sprite XML Definition File.
 *
 * \param filePath	File path of the sprite XML definition file.
 *
 * \return	True on success. False when if an error occurs.
 */
bool Sprite::parseXml(const string& filePath)
{
	if(!Singleton<Filesystem>::get().exists(filePath))
	{
		cout << "Sprite file '" << filePath << "' doesn't exist." << endl;
		addDefaultAction();
		return false;
	}

	File xmlFile = Singleton<Filesystem>::get().open(filePath);

	// Load the file
	if(xmlFile.size() == 0)
	{
		cout << "Sprite file '" << filePath << "' is empty!" << endl;
		addDefaultAction();
		return false;
	}

	TiXmlElement  *spriteRootElement = 0;

	// Load the XML document and handle any errors if occuring
	mDocXml->Parse(xmlFile.raw_bytes(), 0, TIXML_DEFAULT_ENCODING);
	if(mDocXml->Error())
	{
		cout << "Sprite definition error in '" << filePath << "' at Row " << mDocXml->ErrorRow() << ", Column " << mDocXml->ErrorCol() << ": " << mDocXml->ErrorDesc() << endl;
		addDefaultAction();
		return false;
	}
	else
	{
		// Check that we're using an XML Document.
		if(mDocXml->ToDocument() != 0)
		{
			// We've got ourselves a document, find the Sprite node.
			spriteRootElement = mDocXml->FirstChildElement("sprite");
			
			// Validate the node
			if (spriteRootElement == 0)
			{
				cout << "ERROR: '" << filePath << "' doesn't contain a '<sprite>' tag!" << endl;
				addDefaultAction();
				return false;
			}
		} 
		else 
		{
			cout << "Sprite parsing train wrecked."  << endl;
			addDefaultAction();
			return false;
		}

		// Get Sprite Version
		const char *versionString = spriteRootElement->Attribute("version");
		if(versionString == NULL)
		{
			cout << "ERROR: Unable to determine sprite version." << endl;
			addDefaultAction();
			return false;
		}
		std::string vsnStr = versionString;
		if(vsnStr != SPRITE_VERSION)
		{
			cout << "ERROR: Sprite format is version '" << versionString <<"'. Expected version '" << SPRITE_VERSION << "'." << endl;
			addDefaultAction();
			return false;
		}
			
        
		// NOTE:
		// Parse through all image sheets within the Sprite definition so that a user
		// can define a spritesheet wherever they want to within the XML definition.
		if(!parseImageSheets(spriteRootElement))
			return false;
		if(!parseActions(spriteRootElement))
			return false;

		return true;
	}
	
	// This should never be reached but just in case, add a default frame.
	throw Exception(0, "Sprite XML Processing", "An unexpected code path was followed during XML processing.");
	//addDefaultAction();
	//return false;
}


/**
 * Parses through and interpretes <imagesheet> tags within a Sprite
 * XML Definition File.
 *
 * \todo	Make use of mErrorMessage.
 */
bool Sprite::parseImageSheets(TiXmlElement *root)
{
	// Iterate through all child elements of <sprite> to find all <imagesheet> tags
	// then push them into a list of imagesheet's identified by a string constant.
	TiXmlNode *xmlNode = 0;
	while(xmlNode = root->IterateChildren(xmlNode))
	{
		if(xmlNode->ValueStr() == "imagesheet")
		{
			const char *id = xmlNode->ToElement()->Attribute("id");
			const char *src = xmlNode->ToElement()->Attribute("src");

			// Check that <imagesheet> has all expected attributes and that they aren't zero-length.
			if(id == NULL)
			{
				cout << "<imagesheet> definition in Sprite '" << mSpriteName << "' is missing exected attribute 'id' on row " << xmlNode->Row() << "." << endl;
				return false;
			}
			else if(strlen(id) == 0)
			{
				cout << "<imagesheet> definition in Sprite '" << mSpriteName << "' has a zero-length value in attribute 'id' on row " << xmlNode->Row() << "." <<  endl;
				return false;
			}
			else if(src == NULL)
			{
				cout << "<imagesheet> definition in Sprite '" << mSpriteName << "' is missing exected attribute 'src' on row " << xmlNode->Row() << "." << endl;
				return false;
			}
			else if(strlen(src) == 0)
			{
				cout << "<imagesheet> definition in Sprite '" << mSpriteName << "' has a zero-length value in attribute 'src' on row " << xmlNode->Row() << "." << endl;
				return false;
			}

			// If an <imagesheet> with 'id' doesn't exist in our list, add it.
			if(mSpriteSheets.find(toLowercase(id)) == mSpriteSheets.end())
			{
				std::string fileStr = Singleton<Filesystem>::get().workingPath(mSpriteName);
				fileStr += src;
				if(!Singleton<Filesystem>::get().exists(fileStr))
				{
					cout << "Couldn't find '" << fileStr << "'." << endl;
					mSpriteSheets[toLowercase(id)] = new Image();
				}
				else
				{
					if(mSpriteSheets.find(id) != mSpriteSheets.end())
						delete mSpriteSheets[id];

					mSpriteSheets[id] = new Image(fileStr);
				}
			}
			else
				cout << "<imagesheet> redefinition in Sprite '" << mSpriteName << "'on row " << xmlNode->Row() << ", <imagesheet id='" << id << "' src='" << src << "' />. This <imagesheet> tag will be ignored." << endl;
		}
	}

	return true;
}


/**
 * Parses through and interpretes <action> tags within a Sprite
 * XML Definition File.
 * 
 * \todo	Make use of mErrorMessage.
 */
bool Sprite::parseActions(TiXmlElement *root)
{
	// Iterate through all child elements of <sprite> to find all <imagesheet> tags
	// then push them into a list of imagesheet's identified by a string constant.
	TiXmlNode *actionNode = 0;
	while(actionNode = root->IterateChildren(actionNode))
	{
		if(actionNode->ValueStr() == "action")
		{
			const char *name = actionNode->ToElement()->Attribute("name");
			if(name == NULL)
			{
				cout << "<action> definition in Sprite '" << mSpriteName << "' is missing exected attribute 'name' on row " << actionNode->Row() << "." << endl;
				return false;
			}
			else if(strlen(name) == 0)
			{
				cout << "<action> definition in Sprite '" << mSpriteName << "' has a zero-length value in attribute 'name' on row " << actionNode->Row() << "." << endl;
				return false;
			}

			if(mActions.find(toLowercase(name)) == mActions.end())
				parseFrames(actionNode, name);
			else
				cout << "Multiple definitions of action '" << name << "' on row " << actionNode->Row() << "." << endl;
		}
	}

	return true;
}


/**
 * Parses through all <frame> tags within an <action> tag in a Sprite Definition.
 *
 * \todo	Make use of mErrorMessage.
 */
void Sprite::parseFrames(TiXmlNode *node, const string& action)
{
	TiXmlNode *frameNode = 0;

	FrameList frameList;

	while(frameNode = node->IterateChildren(frameNode))
	{
		if(frameNode->ValueStr() == "frame")
		{
			int width = 0, height = 0, anchorx = 0, anchory = 0, delay = 0;

			// ===========================================
			// Image Sheet ID
			// ===========================================
			const char *sheetid = frameNode->ToElement()->Attribute("sheetid");
			if(sheetid == NULL)
			{
				cout << "<action> definition in Sprite '" << mSpriteName << "' is missing exected attribute 'name' on row " << frameNode->Row() << "." << endl;
				addDefaultFrame(frameList);
				continue;
			}
			else if(strlen(sheetid) == 0)
			{
				cout << "<action> definition in Sprite '" << mSpriteName << "' has a zero-length value in attribute 'name' on row " << frameNode->Row() << "." << endl;
				addDefaultFrame(frameList);
				continue;
			}
			else if(mSpriteSheets.find(sheetid) == mSpriteSheets.end())
			{
				cout << "<frame> definition in Sprite '" << mSpriteName << "' references an undefined <imagesheet> '" << sheetid << "' at row " << frameNode->Row() << "." << endl;
				addDefaultFrame(frameList);
				continue;
			}
			else if(!mSpriteSheets.find(sheetid)->second->loaded())
			{
				cout << "<frame> definition in Sprite '" << mSpriteName << "' references an <imagesheet> that couldn't load its source image at row " << frameNode->Row() << "." << endl;
				addDefaultFrame(frameList);
				continue;
			}

			// ===========================================
			// Delay Value
			// ===========================================
			if(frameNode->ToElement()->QueryIntAttribute("delay", &delay) != TIXML_SUCCESS)
			{
				cout << "Invalid 'delay' value in Sprite '" << mSpriteName << "' in <frame> tag on row " << frameNode->Row() << "." << endl;
				addDefaultFrame(frameList);
				continue;
			}

			// ===========================================
			// X-Coordinate
			// ===========================================
			int x = 0;
			if(frameNode->ToElement()->QueryIntAttribute("x", &x) != TIXML_SUCCESS)
			{
				cout << "Invalid 'x' value in Sprite '" << mSpriteName << "' in <frame> tag on row " << frameNode->Row() << "." << endl;
				addDefaultFrame(frameList);
				continue;
			}
			else if( x < 0 || x > mSpriteSheets.find(sheetid)->second->width())
			{
				cout << "Value 'x' in Sprite '" << mSpriteName << "' in <frame> tag on row " << frameNode->Row() << " contains a value that is out of bounds." << endl;
				addDefaultFrame(frameList);
				continue;
			}

			// ===========================================
			// Y-Coordinate
			// ===========================================
			int y = 0;
			if(frameNode->ToElement()->QueryIntAttribute("y", &y) != TIXML_SUCCESS)
			{
				cout << "Invalid 'y' value in Sprite '" << mSpriteName << "' in <frame> tag on row " << frameNode->Row() << "." << endl;
				addDefaultFrame(frameList);
				continue;
			}
			else if( y < 0 || y > mSpriteSheets.find(sheetid)->second->height())
			{
				cout << "Value 'y' in Sprite '" << mSpriteName << "' in <frame> tag on row " << frameNode->Row() << " contains a value that is out of bounds." << endl;
				addDefaultFrame(frameList);
				continue;
			}
			
			// ===========================================
			// Width
			// ===========================================
			if(frameNode->ToElement()->QueryIntAttribute("width", &width) != TIXML_SUCCESS)
			{
				cout << "Invalid 'width' value in Sprite '" << mSpriteName << "' in <frame> tag on row " << frameNode->Row() << "." << endl;
				addDefaultFrame(frameList);
				continue;
			}
			else if(width < 1)
			{
				cout << "'width' value must be larger than 1. Sprite '" << mSpriteName << "',  Row " << frameNode->Row() << "." << endl;
				addDefaultFrame(frameList);
				continue;
			}
			else if(width + x > mSpriteSheets.find(sheetid)->second->width())
			{
				cout << "'x' + 'width' value exceeds dimensions of specified sprite sheet. Sprite '" << mSpriteName << "',  Row " << frameNode->Row() << "." << endl;
				addDefaultFrame(frameList);
				continue;
			}
			
			// ===========================================
			// Height
			// ===========================================
			if(frameNode->ToElement()->QueryIntAttribute("height", &height) != TIXML_SUCCESS)
			{
				cout << "Invalid 'height' value in Sprite '" << mSpriteName << "' in <frame> tag on row " << frameNode->Row() << "." << endl;
				addDefaultFrame(frameList);
				continue;
			}
			else if(height < 1)
			{
				cout << "'width' value must be larger than 1. Sprite '" << mSpriteName << "',  Row " << frameNode->Row() << "." << endl;
				addDefaultFrame(frameList);
				continue;
			}
			else if(height + y > mSpriteSheets.find(sheetid)->second->height())
			{
				cout << "'y' + 'height' value exceeds dimensions of specified sprite sheet. Sprite '" << mSpriteName << "',  Row " << frameNode->Row() << "." << endl;
				addDefaultFrame(frameList);
				continue;
			}
			
			// ===========================================
			// Anchor X-Coordinate
			// ===========================================
			if(frameNode->ToElement()->QueryIntAttribute("anchorx", &anchorx) != TIXML_SUCCESS)
			{
				cout << "Invalid 'anchorx' value in Sprite '" << mSpriteName << "' in <frame> tag on row " << frameNode->Row() << "." << endl;
				addDefaultFrame(frameList);
				continue;
			}
			
			// ===========================================
			// Anchor X-Coordinate
			// ===========================================
			if(frameNode->ToElement()->QueryIntAttribute("anchory", &anchory) != TIXML_SUCCESS)
			{
				cout << "Invalid 'anchory' value in Sprite '" << mSpriteName << "' in <frame> tag on row " << frameNode->Row() << "." << endl;
				addDefaultFrame(frameList);
				continue;
			}

			frameList.push_back(new spriteFrame(mSpriteSheets[sheetid], x, y, width, height, anchorx, anchory, delay));
		}
		else
			cout << "Unexpected child node '<" << frameNode->Value() << ">' found in <action> on row " << frameNode->Row() << "." << endl;
	}

	// Add the frame list to the action container.
	if(frameList.size() > 0)
	{
		mActions[toLowercase(action)] = frameList;
	}
	else
	{
		cout << "Action '" << action << "' contains no frames!" << endl;

		for(size_t i = 0; i < frameList.size(); i++)
		{
			delete frameList[i];
			frameList[i] = NULL;
		}
	}
}


/**
 *	Adds a default frame image to a frame list.
 *
 * \param	frmList		A reference to a FrameList.
 * \param	frmDelay	Frame delay, in miliseconds. Default: 25.
 *
 * \note	This function does not check for validity of parameters
 *			passed in.
 */
void Sprite::addDefaultFrame(FrameList &frmList, unsigned int frmDelay)
{
	Image *tmpImage = new Image();
	frmList.push_back(new spriteFrame(tmpImage, 0, 0, tmpImage->width(), tmpImage->height(), tmpImage->width() / 2, tmpImage->height() / 2, frmDelay));
	delete tmpImage;
}


/**
 * Adds a 'default' action to the action list.
 *
 * \note	This function will not override a 'default' action if one has
 *			has already been specified.
 */
void Sprite::addDefaultAction()
{
	if(mActions.find("default") == mActions.end())
	{
		FrameList frameList;
		addDefaultFrame(frameList);
		mActions["default"] = frameList;
	}
}


/**
 * Spits out a bit of debug info on the Sprite.
 */
void Sprite::debug()
{
	cout << endl;
	cout << "=== Sprite Info: " << mSpriteName << " ===" << endl;
	cout << "Sheets: " << mSpriteSheets.size() << endl;
	
	SheetContainer::iterator sheetIt = mSpriteSheets.begin();
	while(sheetIt != mSpriteSheets.end())
	{
		cout << "   " << sheetIt->first << ": '" << sheetIt->second->name() << "'" << endl;
		sheetIt++;
	}

	cout << "Actions: " << mActions.size() << endl;
	
	unsigned int frameCount = 0;
	ActionList::iterator actionIt = mActions.begin();
	while(actionIt != mActions.end())
	{
		frameCount += actionIt->second.size();
		actionIt++;
	}
	cout << "Frames: " << frameCount << endl;
	cout << endl;
}


int Sprite::width()
{
	spriteFrame *frame = mActions[mCurrentAction][mCurrentFrame];
	return frame->mWidth;
}


int Sprite::height()
{
	spriteFrame *frame = mActions[mCurrentAction][mCurrentFrame];
	return frame->mHeight;
}


int Sprite::originX(int x)
{
	spriteFrame *frame = mActions[mCurrentAction][mCurrentFrame];
	return x - frame->mAnchorX;
}


int Sprite::originY(int y)
{
	spriteFrame *frame = mActions[mCurrentAction][mCurrentFrame];
	return y - frame->mAnchorY;
}



// ==================================================================================
// = spriteFrame member function definitions.
// ==================================================================================

/**
 * Constructor
 *
 * \param	src	A pointer to a source Image to copy from.
 * \param	x	X-Coordinte of the area to copy from the source Image object.
 * \param	y	Y-Coordinte of the area to copy from the source Image object.
 * \param	w	Width of the area to copy from the source Image object.
 * \param	h	Height of the area to copy from the source Image object.
 * \param	aX	X-Axis of the Anchor Point for this spriteFrame.
 * \param	aY	Y-Axis of the Anchor Point for this spriteFrame.
 * \param	d	Length of time milliseconds to display this spriteFrame during animation playback.
 */
Sprite::spriteFrame::spriteFrame(Image *src, int x, int y, int w, int h, int aX, int aY, int d) :	mFrameImage(new Image(src, x, y, w, h) ),
																									mWidth(mFrameImage->width()),
																									mHeight(mFrameImage->height()),
																									mAnchorX(aX), mAnchorY(aY),
																									mFrameDelay(d)
{}


/**
 * Copy Constructor
 */
Sprite::spriteFrame::spriteFrame(const spriteFrame &spriteframe) :	mFrameImage(new Image(*spriteframe.mFrameImage)),
																	mWidth(spriteframe.mWidth),
																	mHeight(spriteframe.mHeight),
																	mAnchorX(spriteframe.mAnchorX),
																	mAnchorY(spriteframe.mAnchorY),
																	mFrameDelay(spriteframe.mFrameDelay)
{}


/**
 * Assignment Operator
 */
Sprite::spriteFrame& Sprite::spriteFrame::operator=(const spriteFrame &rhs)
{
	if(mFrameImage)
		delete mFrameImage;

	mFrameImage = new Image(*rhs.mFrameImage);

	mWidth = rhs.mWidth;
	mHeight = rhs.mHeight;
	mAnchorX = rhs.mAnchorX;
	mAnchorY = rhs.mAnchorY;
	mFrameDelay = rhs.mFrameDelay;
	
	return *this;
}


/**
 * Free memory allocated for the sprite frame.
 */
Sprite::spriteFrame::~spriteFrame()
{
	delete mFrameImage;
	mFrameImage = NULL;
}
