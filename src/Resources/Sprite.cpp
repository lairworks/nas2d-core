// ==================================================================================
// = NAS2D
// = Copyright © 2008 - 2017 New Age Software
// ==================================================================================
// = NAS2D is distributed under the terms of the zlib license. You are free to copy,
// = modify and distribute the software under the terms of the zlib license.
// = 
// = Acknowledgement of your use of NAS2D is appriciated but is not required.
// ==================================================================================

#include "NAS2D/Resources/Sprite.h"
#include "NAS2D/XML/XmlAttributeParser.h"

#include <iostream>

using namespace std;
using namespace NAS2D;
using namespace NAS2D::Xml;

const string NAS2D::SPRITE_VERSION("0.99");
const string DEFAULT_ACTION("default");

const int FRAME_PAUSE = -1;


// Adds a row/name tag to the end of messages.
string endTag(int row, const std::string& name)
{
	return " (Row: " + std::to_string(row) + ", " + name + ")";
}


/**
 * Default c'tor.
 * 
 * Provided to allow sprites to be used in STL containers.
 * 
 * \warning	Generally speaking, Sprites should not be instantiated
 *			with the default c'tor.
 */
Sprite::Sprite():	mSpriteName("Default Constructed"),
					mCurrentAction(DEFAULT_ACTION),
					mCurrentFrame(0),
					mColor(COLOR_NORMAL),
					mRotationAngle(0.0f),
					mPaused(false)
{
	addDefaultAction();
}


/**
 * C'tor.
 *
 * \param filePath	File path of the Sprite definition file.
 */
Sprite::Sprite(const std::string& filePath):	mSpriteName(filePath),
												mCurrentAction(DEFAULT_ACTION),
												mCurrentFrame(0),
												mColor(COLOR_NORMAL),
												mRotationAngle(0.0f),
												mPaused(false)
{
	addDefaultAction();
	parseXml(filePath);
}


/**
 * Copy C'tor
 */
Sprite::Sprite(const Sprite &sprite):	mImageSheets(sprite.mImageSheets),
										mActions(sprite.mActions),
										mSpriteName(sprite.mSpriteName),
										mCurrentAction(sprite.mCurrentAction),
										mCurrentFrame(sprite.mCurrentFrame),
										mColor(sprite.mColor),
										mRotationAngle(sprite.mRotationAngle),
										mPaused(sprite.mPaused)
{}


/**
 * Assignment Operator
 */
Sprite& Sprite::operator=(const Sprite &rhs)
{
	mImageSheets	= rhs.mImageSheets;
	mActions		= rhs.mActions;
	mSpriteName		= rhs.mSpriteName;
	mCurrentAction	= rhs.mCurrentAction;
	mCurrentFrame	= rhs.mCurrentFrame;
	mColor			= rhs.mColor;
	mRotationAngle	= rhs.mRotationAngle;
	mPaused			= rhs.mPaused;

	return *this;
}


/**
 * Plays an action animation.
 *
 * \param	action	Name of the action to use for animation. Action's are not
 *			case sensitive. "Case", "caSe", "CASE", etc. will all be viewed
 *			as identical.
 *
 * \note	If the named action doesn't exist, a warning message will
 *			be written to the log and the default action will be used
 *			instead.
 */
void Sprite::play(const std::string& action)
{
	// Set the current frame list to the defined action. If action
	// isn't found, set to default and reset frame counter.
	ActionList::iterator actionIt = mActions.find(toLowercase(action));
	if(actionIt == mActions.end())
	{
		cout << "Named action '" << action << "' is not defined in '" << name() << "'." << endl;
		mCurrentAction = DEFAULT_ACTION;
		mCurrentFrame = 0;
	}
	else if(mCurrentAction == toLowercase(action))
	{
		// Reset the frame counter.
		mCurrentFrame = 0;
	}
	else
	{
		// Set the current action to the named action.
		mCurrentFrame = 0;	
		mCurrentAction = toLowercase(action);
	}

	mTimer.reset();
	resume();
}


/**
 * Pauses animation for this Sprite.
 */
void Sprite::pause()
{
	mPaused = true;
}


/**
 * Resumes the action of the Sprite.
 */
void Sprite::resume()
{
	mPaused = false;
}


/**
 * Skips animation playback frames.
 * 
 * \param	frames	Number of frames to skip.
 */
void Sprite::skip(int frames)
{
	if(mActions.find(toLowercase(mCurrentAction)) == mActions.end())
		return;

	mCurrentFrame = frames % mActions[mCurrentAction].size();
}


/**
 * Updates the Sprite and draws it to the screen at specified coordinaes.
 *
 * \param	x	X-Screen Coordinate to render the Sprite.
 * \param	y	X-Screen Coordinate to render the Sprite.
 */
void Sprite::update(float x, float y)
{
	SpriteFrame frame = mActions[mCurrentAction][mCurrentFrame];

	if(!mPaused && (frame.frameDelay() != FRAME_PAUSE))
	{
		while(frame.frameDelay() > 0 && static_cast<int>(mTimer.accumulator()) >= frame.frameDelay())
		{
			mTimer.adjust_accumulator(frame.frameDelay());
			mCurrentFrame++;
		}

		// Check that our frame count is within bounds.
		if(mCurrentFrame >= mActions[mCurrentAction].size())
		{
			mCurrentFrame = 0;
			mFrameCallback();		// Notifiy any frame listeners that the animation sequence has completed.
		}
	}
	else if(frame.frameDelay() == FRAME_PAUSE)
		mFrameCallback();

	Utility<Renderer>::get().drawSubImageRotated(mImageSheets[frame.sheetId()], x - frame.anchorX(), y - frame.anchorY(), static_cast<float>(frame.x()), static_cast<float>(frame.y()), static_cast<float>(frame.width()), static_cast<float>(frame.height()), mRotationAngle, mColor);
}


/**
 * Sets the rotation angle of the Sprite.
 * 
 * \param	angle	Angle of rotation in degrees.
 */
void Sprite::rotation(float angle)
{
	mRotationAngle = angle;
}


/**
 * Gets the rotation angle of the Sprite.
 * 
 * \return	Angle of rotation in degrees.
 */
float Sprite::rotation() const
{
	return mRotationAngle;
}


/**
 * Gets a list of Actions available for the Sprite.
 *
 * \return	StringList of actions.
 */
StringList Sprite::actions()
{
	StringList list;

	for (auto it = mActions.begin(); it != mActions.end(); ++it)
		list.push_back(it->first);

	return list;
}


/**
 * Increments the frame counter.
 */
void Sprite::incrementFrame()
{
	++mCurrentFrame;
	if (mCurrentFrame >= mActions[mCurrentAction].size())
		mCurrentFrame = 0;
}


/**
 * Decrements the frame counter.
 */
void Sprite::decrementFrame()
{
	if (mCurrentFrame == 0)
		mCurrentFrame = mActions[mCurrentAction].size();
	
	--mCurrentFrame;
}


/**
 * Parses a Sprite XML Definition File.
 *
 * \param filePath	File path of the sprite XML definition file.
 */
void Sprite::parseXml(const std::string& filePath)
{
	Filesystem& fs = Utility<Filesystem>::get();

	if(!fs.exists(filePath))
	{
		cout << "Sprite file '" << filePath << "' doesn't exist." << endl;
		addDefaultAction();
		return;
	}

	File xmlFile = fs.open(filePath);

	// Load the file
	if(xmlFile.size() == 0)
	{
		cout << "Sprite file '" << filePath << "' is empty." << endl;
		addDefaultAction();
		return;
	}

	XmlDocument docXml;
	XmlElement *xmlRootElement = 0;

	// Load the XML document and handle any errors if occuring
	docXml.Parse(xmlFile.raw_bytes(), 0);
	if(docXml.Error())
	{
		cout << "Malformed XML. Row: " << docXml.ErrorRow() << " Column: " << docXml.ErrorCol() << ": " << docXml.ErrorDesc() << " (" << name() << ")" << endl;
		addDefaultAction();
		return;
	}
	else
	{
		// Find the Sprite node.
		xmlRootElement = docXml.FirstChildElement("sprite");
		if(!xmlRootElement)
		{
			cout << "Specified file '" << filePath << "' doesn't contain a <sprite> tag." << endl;
			addDefaultAction();
			return;
		}

		// Get the Sprite version.
		XmlAttributeParser parser;
		string versionString = parser.stringAttribute(xmlRootElement, "version");
		if(versionString != SPRITE_VERSION)
		{
			if(versionString.empty())
				cout << "Sprite file '" << filePath << "' doesn't define a version." << endl;
			else
				cout << "Sprite version mismatch (" << versionString <<") in '" << filePath << "'. Expected (" << SPRITE_VERSION << ")." << endl;
			
			addDefaultAction();
		}

		// Note:
		// Here instead of going through each element and calling a processing function to handle
		// it, we just iterate through all nodes to find sprite sheets. This allows us to define
		// image sheets anywhere in the sprite file.
		parseImageSheets(xmlRootElement);
		parseActions(xmlRootElement);
	}
}


/**
 * Parses through and interpretes <imagesheet> tags within a Sprite
 * XML Definition File.
 *
 * \todo	Make use of mErrorMessage.
 */
void Sprite::parseImageSheets(XmlElement *root)
{
	XmlAttributeParser parser;

	// Iterate through all child elements of <sprite> to find all <imagesheet> tags
	// then push them into a list of imagesheet's identified by a string id.
	XmlNode* xmlNode = 0;
	while(xmlNode = root->IterateChildren(xmlNode))
	{
		if(xmlNode->Value() == "imagesheet")
		{
			string id = parser.stringAttribute(xmlNode, "id");
			string src = parser.stringAttribute(xmlNode, "src");

			if(id.empty())
			{
				cout << "Zero-length 'id' value in Imagesheet definition." << endTag(xmlNode->row(), name()) << endl;
				return;
			}

			if(src.empty())
			{
				cout << "Zero-length 'src' value in Imagesheet definition." << endTag(xmlNode->row(), name()) << endl;
				return;
			}

			addImageSheet(id, src, xmlNode);
		}
	}
}


/**
 * Adds an image sheet to the Sprite.
 * 
 * \note	Imagesheet ID's are not case sensitive. "Case", "caSe",
 *			"CASE", etc. will all be viewed as identical.
 * 
 * \param	id		String ID for the image sheet.
 * \param	src		Image sheet file path.
 * \param	node	XML Node (for error information).
 */
void Sprite::addImageSheet(const std::string& id, const std::string& src, XmlNode* node)
{
	Filesystem& fs = Utility<Filesystem>::get();

	// Search for an image sheet with 'id'. If not found, add it.
	if(mImageSheets.find(toLowercase(id)) == mImageSheets.end())
	{
		string imagePath = fs.workingPath(mSpriteName);
		imagePath += src;
		if(!fs.exists(imagePath))
		{
			cout << "Couldn't find '" << imagePath << "' defined in sprite file '" << name() << "'." << endl;
			mImageSheets[toLowercase(id)]; // Add a default image
		}
		else
		{
			mImageSheets[id] = Image(imagePath);
		}
	}
	else
	{
		cout << "Image-sheet redefinition '" << id << "'." << endTag(node->row(), name()) << ". Imagesheet ignored." << endl;
	}
}



/**
 * Parses through and interpretes <action> tags within a Sprite
 * XML Definition File.
 * 
 * \note	Action names are not case sensitive. "Case", "caSe",
 *			"CASE", etc. will all be viewed as identical.
 */
void Sprite::parseActions(XmlElement *root)
{
	XmlAttributeParser parser;

	// Iterate through all child elements of <sprite> to find all <action> tags
	// then push them into a list of actions identified by a string id.
	XmlNode *actionNode = 0;
	while(actionNode = root->IterateChildren(actionNode))
	{
		if(actionNode->Value() == "action")
		{
			string action_name = parser.stringAttribute(actionNode, "name");

			if(action_name.empty())
			{
				cout << "Zero-length 'name' value in Action definition." << endTag(actionNode->row(), name()) << endl;
				continue;
			}

			if(mActions.find(toLowercase(action_name)) == mActions.end())
				parseFrames(action_name, actionNode);
			else
				cout << "Redefinition of action '" << action_name << "'. First definition will be used." << endTag(actionNode->row(), name()) << endl;
		}
	}
}


/**
 * Parses through all <frame> tags within an <action> tag in a Sprite Definition.
 *
 * \todo	Make use of mErrorMessage.
 */
void Sprite::parseFrames(const std::string& action, XmlNode *node)
{
	XmlAttributeParser parser;
	
	FrameList frameList;

	XmlNode *frameNode = 0;
	while(frameNode = node->IterateChildren(frameNode))
	{
		int currentRow = frameNode->row();
		
		if(frameNode->Value() == "frame")
		{
			// Imagesheet ID
			string sheetId = toLowercase(parser.stringAttribute(frameNode, "sheetid")); // normalized
			if(!validateSheetId(sheetId, currentRow))
			{
				continue;
			}

			// Delay Value
			int delay = parser.intAttribute(frameNode, "delay");

			// X-Coordinate
			int x = parser.intAttribute(frameNode, "x");
			if( x < 0 || x > mImageSheets.find(sheetId)->second.width())
			{
				cout << "Value 'x' is out of bounds." << endTag(currentRow, name()) << endl;
				continue;
			}

			// Y-Coordinate
			int y = parser.intAttribute(frameNode, "y");
			if(y < 0 || y > mImageSheets.find(sheetId)->second.height())
			{
				cout << "Value 'y' is out of bounds." << endTag(currentRow, name()) << endl;
				continue;
			}
			
			// Width
			int width = parser.intAttribute(frameNode, "width");
			if(width < 1)
			{
				width = 1;
				cout << "'width' value must be non-zero. Defaulting to 1." << endTag(currentRow, name()) << endl;
			}
			else if(x + width > mImageSheets.find(sheetId)->second.width())
			{
				cout << "'x' + 'width' value exceeds dimensions of specified imagesheet." << endTag(currentRow, name()) << endl;
				continue;
			}
			
			// Height
			int height = parser.intAttribute(frameNode, "height");
			if(height < 1)
			{
				height = 1;
				cout << "'height' value must be non-zero. Defaulting to 1." << endTag(currentRow, name()) << endl;
			}
			else if(y + height > mImageSheets.find(sheetId)->second.height())
			{
				cout << "'y' + 'height' value exceeds dimensions of specified imagesheet." << endTag(currentRow, name()) << endl;
				continue;
			}

			// Anchor Coordinates
			int anchorx	= parser.intAttribute(frameNode, "anchorx");
			int anchory	= parser.intAttribute(frameNode, "anchory");

			frameList.push_back(SpriteFrame(sheetId, x, y, width, height, anchorx, anchory, delay));
		}
		else
			cout << "Unexpected tag '<" << frameNode->Value() << ">'." << endTag(currentRow, name()) << endl;
	}

	// Add the frame list to the action container.
	if(frameList.size() > 0)
		mActions[toLowercase(action)] = frameList;
	else
		cout << "Action '" << action << "' contains no valid frames. (" << name() << ")" << endl;
}


bool Sprite::validateSheetId(const std::string& sheetId, int row)
{
	if(sheetId.empty())
	{
		cout << "Frame definition has a zero-length 'name' value. Frame is being ignored." << endTag(row, name()) << endl;
		return false;
	}
	else if(mImageSheets.find(sheetId) == mImageSheets.end())
	{
		cout << "Frame definition references an undefined imagesheet '" << sheetId << "'." << endTag(row, name()) << endl;
		return false;
	}
	else if(!mImageSheets.find(sheetId)->second.loaded())
	{
		cout << "Frame definition references an imagesheet that failed to load." << endTag(row, name()) << endl;
		return false;
	}

	return true;
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
		mImageSheets["default"];	// Adds a default sheet. //-V607

		int width = mImageSheets["default"].width();
		int height = mImageSheets["default"].height();

		FrameList frameList;
		frameList.push_back(SpriteFrame("default", 0, 0, width, height, width / 2, height / 2, -1));
		mActions["default"] = frameList;
	}
}


/**
 * Spits out a bit of debug info on the Sprite.
 */
void Sprite::debug()
{
	cout << endl;
	cout << "=== Sprite Info: " << name() << " ===" << endl;
	cout << "Sheets: " << mImageSheets.size() << endl;
	
	SheetList::iterator sheetIt = mImageSheets.begin();
	while(sheetIt != mImageSheets.end())
	{
		cout << "   " << sheetIt->first << ": '" << sheetIt->second.name() << "'" << endl;
		sheetIt++;
	}

	cout << endl << endl << "Actions: " << mActions.size() << endl;
	
	size_t frameCount = 0;
	ActionList::iterator actionIt = mActions.begin();
	while(actionIt != mActions.end())
	{
		cout << "   " << actionIt->first << endl;
		frameCount += actionIt->second.size();
		actionIt++;
	}
	cout << endl << endl << "Frames: " << frameCount << endl << endl;
}


int Sprite::width()
{
	return mActions[mCurrentAction][mCurrentFrame].width();
}


int Sprite::height()
{
	return mActions[mCurrentAction][mCurrentFrame].height();
}


int Sprite::originX(int x)
{
	return x - mActions[mCurrentAction][mCurrentFrame].anchorX();
}


int Sprite::originY(int y)
{
	return y - mActions[mCurrentAction][mCurrentFrame].anchorY();
}



// ==================================================================================
// = spriteFrame member function definitions.
// ==================================================================================

/**
 * Constructor
 *
 * \param	sId	Sprite sheet ID.
 * \param	x	X-Coordinte of the area to copy from the source Image object.
 * \param	y	Y-Coordinte of the area to copy from the source Image object.
 * \param	w	Width of the area to copy from the source Image object.
 * \param	h	Height of the area to copy from the source Image object.
 * \param	aX	X-Axis of the Anchor Point for this spriteFrame.
 * \param	aY	Y-Axis of the Anchor Point for this spriteFrame.
 * \param	d	Length of time milliseconds to display this spriteFrame during animation playback.
 */
Sprite::SpriteFrame::SpriteFrame(const std::string& sId, int x, int y, int w, int h, int aX, int aY, int d):	mSheetId(sId),
																										mFrameDelay(d),
																										mAnchorX(aX),
																										mAnchorY(aY),
																										mRect(x, y, w, h)
{}


/**
 * Copy C'tor
 */
Sprite::SpriteFrame::SpriteFrame(const SpriteFrame &sf):	mSheetId(sf.mSheetId),
															mFrameDelay(sf.mFrameDelay),
															mAnchorX(sf.mAnchorX),
															mAnchorY(sf.mAnchorY),
															mRect(sf.mRect)
{}


/**
 * Assignment Operator
 */
Sprite::SpriteFrame& Sprite::SpriteFrame::operator=(const SpriteFrame &rhs)
{
	mSheetId = rhs.mSheetId;
	mFrameDelay = rhs.mFrameDelay;
	mAnchorX = rhs.mAnchorX;
	mAnchorY = rhs.mAnchorY;
	mRect = rhs.mRect;
	
	return *this;
}

