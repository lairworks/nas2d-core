// ==================================================================================
// = NAS2D
// = Copyright © 2008 - 2019 New Age Software
// ==================================================================================
// = NAS2D is distributed under the terms of the zlib license. You are free to copy,
// = modify and distribute the software under the terms of the zlib license.
// =
// = Acknowledgement of your use of NAS2D is appriciated but is not required.
// ==================================================================================

#include "Sprite.h"
#include "../Version.h"
#include "../Xml/Xml.h"

#include <iostream>

using namespace std;
using namespace NAS2D;
using namespace NAS2D::Xml;

const string NAS2D::SPRITE_VERSION("0.99");

const int FRAME_PAUSE = -1;


namespace {
	// Adds a row/name tag to the end of messages.
	string endTag(int row, const std::string& name)
	{
		return " (Row: " + std::to_string(row) + ", " + name + ")";
	}
}


/**
 * Default c'tor.
 *
 * Provided to allow sprites to be used in STL containers.
 *
 * \warning	Generally speaking, Sprites should not be instantiated
 *			with the default c'tor.
 */
Sprite::Sprite()
{
	addDefaultAction();
}


/**
 * C'tor.
 *
 * \param filePath	File path of the Sprite definition file.
 */
Sprite::Sprite(const std::string& filePath) :
	mSpriteName(filePath)
{
	processXml(filePath);
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
	const auto normalizedAction = toLowercase(action);
	if (mActions.find(normalizedAction) == mActions.end())
	{
		cout << "Named action '" << action << "' is not defined in '" << name() << "'." << endl;
		mCurrentAction = DEFAULT_ACTION;
	}
	else
	{
		mCurrentAction = normalizedAction;
	}

	mCurrentFrame = 0;
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
 * Sets the animation playback frame.
 *
 * \param	frameIndex	New frame index
 */
void Sprite::setFrame(std::size_t frameIndex)
{
	if (mActions.find(mCurrentAction) != mActions.end())
	{
		mCurrentFrame = frameIndex % mActions[mCurrentAction].size();
	}
}


void Sprite::update(Point<float> position)
{
	const auto& frame = mActions[mCurrentAction][mCurrentFrame];

	if (!mPaused && (frame.frameDelay != FRAME_PAUSE))
	{
		while (frame.frameDelay > 0 && static_cast<int>(mTimer.accumulator()) >= frame.frameDelay)
		{
			mTimer.adjust_accumulator(frame.frameDelay);
			mCurrentFrame++;
		}

		// Check that our frame count is within bounds.
		if (mCurrentFrame >= mActions[mCurrentAction].size())
		{
			mCurrentFrame = 0;
			mFrameCallback();		// Notifiy any frame listeners that the animation sequence has completed.
		}
	}
	else if (frame.frameDelay == FRAME_PAUSE)
	{
		mFrameCallback();
	}

	const auto drawPosition = position - frame.anchorOffset;
	const auto frameBounds = frame.bounds.to<float>();
	Utility<Renderer>::get().drawSubImageRotated(mImageSheets[frame.sheetId], drawPosition.x(), drawPosition.y(), frameBounds.x(), frameBounds.y(), frameBounds.width(), frameBounds.height(), mRotationAngle, mColor);
}


/**
 * Updates the Sprite and draws it to the screen at specified coordinaes.
 *
 * \param	x	X-Screen Coordinate to render the Sprite.
 * \param	y	X-Screen Coordinate to render the Sprite.
 */
void Sprite::update(float x, float y)
{
	update(Point{x, y});
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
StringList Sprite::actions() const
{
	StringList list;

	for (const auto& pair : mActions)
	{
		list.push_back(pair.first);
	}

	return list;
}


/**
 * Increments the frame counter.
 */
void Sprite::incrementFrame()
{
	++mCurrentFrame;
	if (mCurrentFrame >= mActions[mCurrentAction].size())
	{
		mCurrentFrame = 0;
	}
}


/**
 * Decrements the frame counter.
 */
void Sprite::decrementFrame()
{
	if (mCurrentFrame == 0)
	{
		mCurrentFrame = mActions[mCurrentAction].size();
	}

	--mCurrentFrame;
}


/**
 * Parses a Sprite XML Definition File.
 *
 * \param filePath	File path of the sprite XML definition file.
 */
void Sprite::processXml(const std::string& filePath)
{
	Filesystem& fs = Utility<Filesystem>::get();

	if (!fs.exists(filePath))
	{
		cout << "Sprite file '" << filePath << "' doesn't exist." << endl;
		addDefaultAction();
		return;
	}

	File xmlFile = fs.open(filePath);

	// Load the file
	if (xmlFile.empty())
	{
		cout << "Sprite file '" << filePath << "' is empty." << endl;
		addDefaultAction();
		return;
	}

	XmlDocument docXml;
	XmlElement *xmlRootElement = nullptr;

	// Load the XML document and handle any errors if occuring
	docXml.parse(xmlFile.raw_bytes());
	if (docXml.error())
	{
		cout << "Malformed XML. Row: " << docXml.errorRow() << " Column: " << docXml.errorCol() << ": " << docXml.errorDesc() << " (" << name() << ")" << endl;
		addDefaultAction();
		return;
	}
	else
	{
		// Find the Sprite node.
		xmlRootElement = docXml.firstChildElement("sprite");
		if (!xmlRootElement)
		{
			cout << "Specified file '" << filePath << "' doesn't contain a <sprite> tag." << endl;
			addDefaultAction();
			return;
		}

		// Get the Sprite version.
		const XmlAttribute* version = xmlRootElement->firstAttribute();
		if (!version || version->value().empty())
		{
			cout << "Root element in sprite file '" << filePath << "' doesn't define a version." << endl;
			addDefaultAction();
			return;
		}
		else if (version->value() != SPRITE_VERSION)
		{
			cout << "Sprite version mismatch (" << versionString() << ") in '" << filePath << "'. Expected (" << SPRITE_VERSION << ")." << endl;
			addDefaultAction();
			return;
		}

		// Note:
		// Here instead of going through each element and calling a processing function to handle
		// it, we just iterate through all nodes to find sprite sheets. This allows us to define
		// image sheets anywhere in the sprite file.
		processImageSheets(xmlRootElement);
		processActions(xmlRootElement);
	}
}


/**
 * Iterates through all elements of a Sprite XML definition looking
 * for 'imagesheet' elements and processes them.
 *
 * \note	Since 'imagesheet' elements are processed before any other
 *			element in a sprite definition, these elements can appear
 *			anywhere in a Sprite XML definition.
 */
void Sprite::processImageSheets(void* root)
{
	XmlElement* e = static_cast<XmlElement*>(root);

	string id, src;
	for (XmlNode* node = e->iterateChildren(nullptr);
		node != nullptr;
		node = e->iterateChildren(node))
	{
		if (node->value() == "imagesheet" && node->toElement())
		{
			const XmlAttribute* attribute = node->toElement()->firstAttribute();
			while (attribute)
			{
				if (toLowercase(attribute->name()) == "id") { id = attribute->value(); }
				else if (toLowercase(attribute->name()) == "src") { src = attribute->value(); }

				attribute = attribute->next();
			}

			if (id.empty())
			{
				cout << "Zero-length 'id' value in Imagesheet definition." << endTag(node->row(), name()) << endl;
				continue;
			}

			if (src.empty())
			{
				cout << "Zero-length 'src' value in Imagesheet definition." << endTag(node->row(), name()) << endl;
				continue;
			}

			addImageSheet(id, src, node);
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
void Sprite::addImageSheet(const std::string& id, const std::string& src, void* node)
{
	Filesystem& fs = Utility<Filesystem>::get();

	// Search for an image sheet with 'id'. If not found, add it.
	if (mImageSheets.find(toLowercase(id)) == mImageSheets.end())
	{
		string imagePath = fs.workingPath(mSpriteName);
		imagePath += src;
		if (!fs.exists(imagePath))
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
		cout << "Image-sheet redefinition '" << id << "'." << endTag(static_cast<XmlNode*>(node)->row(), name()) << ". Imagesheet ignored." << endl;
	}
}


/**
 * Iterates through all elements of a Sprite XML definition looking
 * for 'action' elements and processes them.
 *
 * \note	Action names are not case sensitive. "Case", "caSe",
 *			"CASE", etc. will all be viewed as identical.
 */
void Sprite::processActions(void* root)
{
	XmlElement* element = static_cast<XmlElement*>(root);

	for (XmlNode* node = element->iterateChildren(nullptr);
		node != nullptr;
		node = element->iterateChildren(node))
	{
		if (toLowercase(node->value()) == "action" && node->toElement())
		{

			string action_name;
			const XmlAttribute* attribute = node->toElement()->firstAttribute();
			while (attribute)
			{
				if (toLowercase(attribute->name()) == "name")
				{
					action_name = attribute->value();
				}

				attribute = attribute->next();
			}

			if (action_name.empty())
			{
				cout << "Zero-length 'name' value in Action definition." << endTag(node->row(), name()) << endl;
				continue;
			}

			if (mActions.find(toLowercase(action_name)) == mActions.end())
			{
				processFrames(action_name, node);
			}
			else
			{
				cout << "Redefinition of action '" << action_name << "'. First definition will be used." << endTag(node->row(), name()) << endl;
			}
		}
	}
}


/**
 * Parses through all <frame> tags within an <action> tag in a Sprite Definition.
 */
void Sprite::processFrames(const std::string& action, void* _node)
{
	XmlNode* node = static_cast<XmlNode*>(_node);

	FrameList frameList;

	for (XmlNode* frame = node->iterateChildren(nullptr);
		frame != nullptr;
		frame = node->iterateChildren(frame))
	{
		int currentRow = frame->row();

		if (frame->value() == "frame" && frame->toElement())
		{
			string sheetId;
			int delay = 0;
			int x = 0, y = 0;
			int width = 0, height = 0;
			int anchorx = 0, anchory = 0;

			const XmlAttribute* attribute = frame->toElement()->firstAttribute();
			while (attribute)
			{
				if (toLowercase(attribute->name()) == "sheetid") { sheetId = attribute->value(); }
				else if (toLowercase(attribute->name()) == "delay") { attribute->queryIntValue(delay); }
				else if (toLowercase(attribute->name()) == "x") { attribute->queryIntValue(x); }
				else if (toLowercase(attribute->name()) == "y") { attribute->queryIntValue(y); }
				else if (toLowercase(attribute->name()) == "width") { attribute->queryIntValue(width); }
				else if (toLowercase(attribute->name()) == "height") { attribute->queryIntValue(height); }
				else if (toLowercase(attribute->name()) == "anchorx") { attribute->queryIntValue(anchorx); }
				else if (toLowercase(attribute->name()) == "anchory") { attribute->queryIntValue(anchory); }
				else { std::cout << "Unexpected attribute '" << attribute->name() << "' found on row " << currentRow << std::endl; }

				attribute = attribute->next();
			}

			if (!validateSheetId(sheetId, currentRow))
			{
				continue;
			}

			// X-Coordinate
			if ( x < 0 || x > mImageSheets.find(sheetId)->second.width())
			{
				cout << "Value 'x' is out of bounds." << endTag(currentRow, name()) << endl;
				continue;
			}

			// Y-Coordinate
			if (y < 0 || y > mImageSheets.find(sheetId)->second.height())
			{
				cout << "Value 'y' is out of bounds." << endTag(currentRow, name()) << endl;
				continue;
			}

			// Width
			if (width < 1)
			{
				width = 1;
				cout << "'width' value must be greater than 0." << endTag(currentRow, name()) << endl;
				continue;
			}
			else if (x + width > mImageSheets.find(sheetId)->second.width())
			{
				cout << "'x' + 'width' value exceeds dimensions of specified imagesheet." << endTag(currentRow, name()) << endl;
				continue;
			}

			// Height
			if (height < 1)
			{
				height = 1;
				cout << "'height' value must be greater than 0." << endTag(currentRow, name()) << endl;
				continue;
			}
			else if (y + height > mImageSheets.find(sheetId)->second.height())
			{
				cout << "'y' + 'height' value exceeds dimensions of specified imagesheet." << endTag(currentRow, name()) << endl;
				continue;
			}

			const auto bounds = NAS2D::Rectangle<int>::Create(NAS2D::Point<int>{x, y}, NAS2D::Vector{width, height});
			const auto anchorOffset = NAS2D::Vector{anchorx, anchory};
			frameList.push_back(SpriteFrame{sheetId, bounds, anchorOffset, delay});
		}
		else
		{
			cout << "Unexpected tag '<" << frame->value() << ">'." << endTag(currentRow, name()) << endl;
		}
	}

	// Add the frame list to the action container.
	if (frameList.size() > 0)
	{
		mActions[toLowercase(action)] = frameList;
	}
	else
	{
		cout << "Action '" << action << "' contains no valid frames. (" << name() << ")" << endl;
	}
}


bool Sprite::validateSheetId(const std::string& sheetId, int row)
{
	if (sheetId.empty())
	{
		cout << "Frame definition has a zero-length 'sheetid' value. Frame is being ignored." << endTag(row, name()) << endl;
		return false;
	}
	else if (mImageSheets.find(sheetId) == mImageSheets.end())
	{
		cout << "Frame definition references an undefined imagesheet '" << sheetId << "'." << endTag(row, name()) << endl;
		return false;
	}
	else if (!mImageSheets.find(sheetId)->second.loaded())
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
	if (mActions.find("default") == mActions.end())
	{
		auto& imageSheet = mImageSheets["default"]; // Adds a default sheet. //-V607

		const auto size = imageSheet.size();
		const auto bounds = NAS2D::Rectangle<int>::Create(NAS2D::Point{0, 0}, size);
		const auto anchorOffset = size / 2;

		FrameList frameList{SpriteFrame{"default", bounds, anchorOffset, FRAME_PAUSE}};
		mActions["default"] = frameList;
	}
}


Vector<int> Sprite::size() const
{
	return mActions.at(mCurrentAction)[mCurrentFrame].bounds.size();
}


Point<int> Sprite::origin(Point<int> point) const
{
	return point - mActions.at(mCurrentAction)[mCurrentFrame].anchorOffset;
}
