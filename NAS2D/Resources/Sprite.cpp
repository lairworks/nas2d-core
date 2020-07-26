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
#include "../ContainerUtils.h"
#include "../Version.h"
#include "../Xml/Xml.h"

#include <iostream>
#include <stdexcept>


using namespace std;
using namespace NAS2D;
using namespace NAS2D::Xml;


namespace NAS2D {
	const string SPRITE_VERSION("0.99");
}


namespace {
	const auto FRAME_PAUSE = unsigned(-1);

	// Adds a row/name tag to the end of messages.
	string endTag(int row, const std::string& name)
	{
		return " (Row: " + std::to_string(row) + ", " + name + ")";
	}
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


Vector<int> Sprite::size() const
{
	return mActions.at(mCurrentAction)[mCurrentFrame].bounds.size();
}


Point<int> Sprite::origin(Point<int> point) const
{
	return point - mActions.at(mCurrentAction)[mCurrentFrame].anchorOffset;
}


/**
 * Gets a list of Actions available for the Sprite.
 *
 * \return	StringList of actions.
 */
StringList Sprite::actions() const
{
	return getKeys(mActions);
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
		throw std::runtime_error("Sprite::play called on undefined action: '" + action + "' : " + name());
	}

	mCurrentAction = normalizedAction;
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
	mCurrentFrame = frameIndex % mActions[mCurrentAction].size();
}


/**
 * Increments the frame counter.
 */
void Sprite::incrementFrame()
{
	setFrame(mCurrentFrame + 1);
}


/**
 * Decrements the frame counter.
 */
void Sprite::decrementFrame()
{
	setFrame(mCurrentFrame - 1);
}


void Sprite::update(Point<float> position)
{
	const auto& frame = mActions[mCurrentAction][mCurrentFrame];

	if (!mPaused && (frame.frameDelay != FRAME_PAUSE))
	{
		while (frame.frameDelay > 0 && mTimer.accumulator() >= frame.frameDelay)
		{
			mTimer.adjust_accumulator(frame.frameDelay);
			mCurrentFrame++;
		}

		if (mCurrentFrame >= mActions[mCurrentAction].size())
		{
			mCurrentFrame = 0;
			mAnimationCompleteCallback();
		}
	}
	else if (frame.frameDelay == FRAME_PAUSE)
	{
		mAnimationCompleteCallback();
	}

	const auto drawPosition = position - frame.anchorOffset.to<float>();
	const auto frameBounds = frame.bounds.to<float>();
	Utility<Renderer>::get().drawSubImageRotated(mImageSheets.at(frame.sheetId), drawPosition, frameBounds, mRotationAngle, mColor);
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
 * Sets the alpha value for the Sprite.
 *
 * \param	alpha	Alpha value to set between 0 - 255.
 */
void Sprite::alpha(uint8_t alpha)
{
	mColor.alpha = alpha;
}


/**
 * Gets the alpha value for the Sprite.
 */
uint8_t Sprite::alpha() const
{
	return mColor.alpha;
}


/**
 * Sets the color tint of the Sprite.
 */
void Sprite::color(const Color& color)
{
	mColor = color;
}


/**
 * Gets the color tint of the Sprite.
 */
const Color& Sprite::color() const
{
	return mColor;
}


Sprite::Callback& Sprite::frameCallback()
{
	return mAnimationCompleteCallback;
}


const std::string& Sprite::name() const
{
	return mSpriteName;
}


/**
 * Parses a Sprite XML Definition File.
 *
 * \param filePath	File path of the sprite XML definition file.
 */
void Sprite::processXml(const std::string& filePath)
{
	XmlDocument xmlDoc;
	xmlDoc.parse(Utility<Filesystem>::get().open(filePath).raw_bytes());

	if (xmlDoc.error())
	{
		throw std::runtime_error("Sprite file has malformed XML: (" + name() + ") Row: " + std::to_string(xmlDoc.errorRow()) + " Column: " + std::to_string(xmlDoc.errorCol()) + " : " + xmlDoc.errorDesc());
	}

	// Find the Sprite node.
	const XmlElement* xmlRootElement = xmlDoc.firstChildElement("sprite");
	if (!xmlRootElement)
	{
		throw std::runtime_error("Sprite file does not contain required <sprite> tag: " + filePath);
	}

	// Get the Sprite version.
	const XmlAttribute* version = xmlRootElement->firstAttribute();
	if (!version || version->value().empty())
	{
		throw std::runtime_error("Sprite file's root element does not specify a version: " + filePath);
	}
	if (version->value() != SPRITE_VERSION)
	{
		throw std::runtime_error("Sprite version mismatch. Expected: " + SPRITE_VERSION + " Actual: " + versionString());
	}

	// Note:
	// Here instead of going through each element and calling a processing function to handle
	// it, we just iterate through all nodes to find sprite sheets. This allows us to define
	// image sheets anywhere in the sprite file.
	processImageSheets(xmlRootElement);
	processActions(xmlRootElement);
}


/**
 * Iterates through all elements of a Sprite XML definition looking
 * for 'imagesheet' elements and processes them.
 *
 * \note	Since 'imagesheet' elements are processed before any other
 *			element in a sprite definition, these elements can appear
 *			anywhere in a Sprite XML definition.
 */
void Sprite::processImageSheets(const void* root)
{
	const XmlElement* e = static_cast<const XmlElement*>(root);

	string id, src;
	for (const XmlNode* node = e->iterateChildren(nullptr);
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
				throw std::runtime_error("Sprite imagesheet definition has `id` of length zero: " + endTag(node->row(), name()));
			}

			if (src.empty())
			{
				throw std::runtime_error("Sprite imagesheet definition has `src` of length zero: " + endTag(node->row(), name()));
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
void Sprite::addImageSheet(const std::string& id, const std::string& src, const void* node)
{
	Filesystem& fs = Utility<Filesystem>::get();

	if (mImageSheets.find(toLowercase(id)) != mImageSheets.end())
	{
		throw std::runtime_error("Sprite image sheet redefinition: id: '" + id + "' " + endTag(static_cast<const XmlNode*>(node)->row(), name()));
	}

	const string imagePath = fs.workingPath(mSpriteName) + src;
	if (!fs.exists(imagePath))
	{
		throw std::runtime_error("Sprite image path not found: Sprite: '" + name() + "' Image: '" + imagePath + "'");
	}
	mImageSheets.try_emplace(id, imagePath);
}


/**
 * Iterates through all elements of a Sprite XML definition looking
 * for 'action' elements and processes them.
 *
 * \note	Action names are not case sensitive. "Case", "caSe",
 *			"CASE", etc. will all be viewed as identical.
 */
void Sprite::processActions(const void* root)
{
	const XmlElement* element = static_cast<const XmlElement*>(root);

	for (const XmlNode* node = element->iterateChildren(nullptr);
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
				throw std::runtime_error("Sprite Action definition has 'name' of length zero: " + endTag(node->row(), name()));
			}
			if (mActions.find(toLowercase(action_name)) != mActions.end())
			{
				throw std::runtime_error("Sprite Action redefinition: '" + action_name + "' " + endTag(node->row(), name()));
			}

			processFrames(action_name, node);
		}
	}
}


/**
 * Parses through all <frame> tags within an <action> tag in a Sprite Definition.
 */
void Sprite::processFrames(const std::string& action, const void* _node)
{
	const XmlNode* node = static_cast<const XmlNode*>(_node);

	FrameList frameList;

	for (const XmlNode* frame = node->iterateChildren(nullptr);
		frame != nullptr;
		frame = node->iterateChildren(frame))
	{
		int currentRow = frame->row();

		if (frame->value() != "frame" || !frame->toElement())
		{
			throw std::runtime_error("Sprite frame tag unexpected: <" + frame->value() + "> : " + endTag(currentRow, name()));
		}

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
			else {
				throw std::runtime_error("Sprite frame attribute unexpected: '" + attribute->name() + "' : " + endTag(currentRow, name()));
			}

			attribute = attribute->next();
		}

		if (sheetId.empty())
		{
			throw std::runtime_error("Sprite Frame definition has 'sheetid' of length zero: " + endTag(currentRow, name()));
		}
		const auto iterator = mImageSheets.find(sheetId);
		if (iterator == mImageSheets.end())
		{
			throw std::runtime_error("Sprite Frame definition references undefined imagesheet: '" + sheetId + "' " + endTag(currentRow, name()));
		}
		const auto& image = iterator->second;
		if (!image.loaded())
		{
			throw std::runtime_error("Sprite Frame definition references imagesheet that failed to load: '" + sheetId + "' " + endTag(currentRow, name()));
		}

		// X-Coordinate
		if (x < 0 || x > image.size().x)
		{
			throw std::runtime_error("Sprite frame attribute 'x' is out of bounds: " + endTag(currentRow, name()));
		}

		// Y-Coordinate
		if (y < 0 || y > image.size().y)
		{
			throw std::runtime_error("Sprite frame attribute 'y' is out of bounds: " + endTag(currentRow, name()));
		}

		// Width
		if (width <= 0 || width > image.size().x - x)
		{
			throw std::runtime_error("Sprite frame attribute 'width' is out of bounds: " + endTag(currentRow, name()));
		}

		// Height
		if (height <= 0 || height > image.size().y - y)
		{
			throw std::runtime_error("Sprite frame attribute 'height' is out of bounds: " + endTag(currentRow, name()));
		}

		const auto bounds = Rectangle<int>::Create(Point<int>{x, y}, Vector{width, height});
		const auto anchorOffset = Vector{anchorx, anchory};
		frameList.push_back(SpriteFrame{sheetId, bounds, anchorOffset, static_cast<unsigned int>(delay)});
	}

	if (frameList.size() <= 0)
	{
		throw std::runtime_error("Sprite Action contains no valid frames: " + action + " (" + name() + ")");
	}

	mActions[toLowercase(action)] = frameList;
}
