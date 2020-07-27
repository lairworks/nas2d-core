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

#include <utility>
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

	// Adds a row tag to the end of messages.
	string endTag(int row)
	{
		return " (Row: " + std::to_string(row) + ")";
	}

	Sprite::SpriteAnimations processXml(const std::string& filePath);
	std::map<std::string, Image> processImageSheets(const std::string& basePath, const XmlElement* element);
	std::map<std::string, std::vector<Sprite::SpriteFrame>> processActions(const std::map<std::string, Image>& imageSheets, const XmlElement* element);
	std::vector<Sprite::SpriteFrame> processFrames(const std::map<std::string, Image>& imageSheets, const std::string& action, const XmlNode* node);
}


/**
 * C'tor.
 *
 * \param filePath	File path of the Sprite definition file.
 */
Sprite::Sprite(const std::string& filePath) :
	mSpriteName(filePath)
{
	try
	{
		mSpriteAnimations = processXml(filePath);
	}
	catch(const std::runtime_error& error)
	{
		throw std::runtime_error("Error parsing Sprite file: " + filePath + "\nError: " + error.what());
	}
}


Vector<int> Sprite::size() const
{
	return mSpriteAnimations.actions.at(mCurrentAction)[mCurrentFrame].bounds.size();
}


Point<int> Sprite::origin(Point<int> point) const
{
	return point - mSpriteAnimations.actions.at(mCurrentAction)[mCurrentFrame].anchorOffset;
}


/**
 * Gets a list of Actions available for the Sprite.
 *
 * \return	StringList of actions.
 */
StringList Sprite::actions() const
{
	return getKeys(mSpriteAnimations.actions);
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
	if (mSpriteAnimations.actions.find(normalizedAction) == mSpriteAnimations.actions.end())
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
	mCurrentFrame = frameIndex % mSpriteAnimations.actions[mCurrentAction].size();
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
	const auto& frame = mSpriteAnimations.actions[mCurrentAction][mCurrentFrame];

	if (!mPaused && (frame.frameDelay != FRAME_PAUSE))
	{
		while (frame.frameDelay > 0 && mTimer.accumulator() >= frame.frameDelay)
		{
			mTimer.adjust_accumulator(frame.frameDelay);
			mCurrentFrame++;
		}

		if (mCurrentFrame >= mSpriteAnimations.actions[mCurrentAction].size())
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
	Utility<Renderer>::get().drawSubImageRotated(frame.image, drawPosition, frameBounds, mRotationAngle, mColor);
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


namespace {

/**
 * Parses a Sprite XML Definition File.
 *
 * \param filePath	File path of the sprite XML definition file.
 */
Sprite::SpriteAnimations processXml(const std::string& filePath)
{
	auto& filesystem = Utility<Filesystem>::get();
	const auto basePath = filesystem.workingPath(filePath);

	XmlDocument xmlDoc;
	xmlDoc.parse(filesystem.open(filePath).raw_bytes());

	if (xmlDoc.error())
	{
		throw std::runtime_error("Sprite file has malformed XML: Row: " + std::to_string(xmlDoc.errorRow()) + " Column: " + std::to_string(xmlDoc.errorCol()) + " : " + xmlDoc.errorDesc());
	}

	// Find the Sprite node.
	const XmlElement* xmlRootElement = xmlDoc.firstChildElement("sprite");
	if (!xmlRootElement)
	{
		throw std::runtime_error("Sprite file does not contain required <sprite> tag");
	}

	// Get the Sprite version.
	const XmlAttribute* version = xmlRootElement->firstAttribute();
	if (!version || version->value().empty())
	{
		throw std::runtime_error("Sprite file's root element does not specify a version");
	}
	if (version->value() != SPRITE_VERSION)
	{
		throw std::runtime_error("Sprite version mismatch. Expected: " + SPRITE_VERSION + " Actual: " + versionString());
	}

	// Note:
	// Here instead of going through each element and calling a processing function to handle
	// it, we just iterate through all nodes to find sprite sheets. This allows us to define
	// image sheets anywhere in the sprite file.
	Sprite::SpriteAnimations spriteAnimations;
	spriteAnimations.imageSheets = processImageSheets(basePath, xmlRootElement);
	spriteAnimations.actions = processActions(spriteAnimations.imageSheets, xmlRootElement);
	return spriteAnimations;
}


/**
 * Iterates through all elements of a Sprite XML definition looking
 * for 'imagesheet' elements and processes them.
 *
 * \note	Since 'imagesheet' elements are processed before any other
 *			element in a sprite definition, these elements can appear
 *			anywhere in a Sprite XML definition.
 */
std::map<std::string, Image> processImageSheets(const std::string& basePath, const XmlElement* element)
{
	std::map<std::string, Image> imageSheets;

	string id, src;
	for (const XmlNode* node = element->iterateChildren(nullptr);
		node != nullptr;
		node = element->iterateChildren(node))
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
				throw std::runtime_error("Sprite imagesheet definition has `id` of length zero: " + endTag(node->row()));
			}

			if (src.empty())
			{
				throw std::runtime_error("Sprite imagesheet definition has `src` of length zero: " + endTag(node->row()));
			}

			if (imageSheets.find(toLowercase(id)) != imageSheets.end())
			{
				throw std::runtime_error("Sprite image sheet redefinition: id: '" + id + "' " + endTag(node->row()));
			}

			const string imagePath = basePath + src;
			imageSheets.try_emplace(id, imagePath);
		}
	}

	return imageSheets;
}


/**
 * Iterates through all elements of a Sprite XML definition looking
 * for 'action' elements and processes them.
 *
 * \note	Action names are not case sensitive. "Case", "caSe",
 *			"CASE", etc. will all be viewed as identical.
 */
std::map<std::string, std::vector<Sprite::SpriteFrame>> processActions(const std::map<std::string, Image>& imageSheets, const XmlElement* element)
{
	std::map<std::string, std::vector<Sprite::SpriteFrame>> actions;

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
				throw std::runtime_error("Sprite Action definition has 'name' of length zero: " + endTag(node->row()));
			}
			if (actions.find(toLowercase(action_name)) != actions.end())
			{
				throw std::runtime_error("Sprite Action redefinition: '" + action_name + "' " + endTag(node->row()));
			}

			actions[toLowercase(action_name)] = processFrames(imageSheets, action_name, node);
		}
	}

	return actions;
}


/**
 * Parses through all <frame> tags within an <action> tag in a Sprite Definition.
 */
std::vector<Sprite::SpriteFrame> processFrames(const std::map<std::string, Image>& imageSheets, const std::string& action, const XmlNode* node)
{
	std::vector<Sprite::SpriteFrame> frameList;

	for (const XmlNode* frame = node->iterateChildren(nullptr);
		frame != nullptr;
		frame = node->iterateChildren(frame))
	{
		int currentRow = frame->row();

		if (frame->value() != "frame" || !frame->toElement())
		{
			throw std::runtime_error("Sprite frame tag unexpected: <" + frame->value() + "> : " + endTag(currentRow));
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
				throw std::runtime_error("Sprite frame attribute unexpected: '" + attribute->name() + "' : " + endTag(currentRow));
			}

			attribute = attribute->next();
		}

		if (sheetId.empty())
		{
			throw std::runtime_error("Sprite Frame definition has 'sheetid' of length zero: " + endTag(currentRow));
		}
		const auto iterator = imageSheets.find(sheetId);
		if (iterator == imageSheets.end())
		{
			throw std::runtime_error("Sprite Frame definition references undefined imagesheet: '" + sheetId + "' " + endTag(currentRow));
		}
		const auto& image = iterator->second;
		if (!image.loaded())
		{
			throw std::runtime_error("Sprite Frame definition references imagesheet that failed to load: '" + sheetId + "' " + endTag(currentRow));
		}

		// X-Coordinate
		if (x < 0 || x > image.size().x)
		{
			throw std::runtime_error("Sprite frame attribute 'x' is out of bounds: " + endTag(currentRow));
		}

		// Y-Coordinate
		if (y < 0 || y > image.size().y)
		{
			throw std::runtime_error("Sprite frame attribute 'y' is out of bounds: " + endTag(currentRow));
		}

		// Width
		if (width <= 0 || width > image.size().x - x)
		{
			throw std::runtime_error("Sprite frame attribute 'width' is out of bounds: " + endTag(currentRow));
		}

		// Height
		if (height <= 0 || height > image.size().y - y)
		{
			throw std::runtime_error("Sprite frame attribute 'height' is out of bounds: " + endTag(currentRow));
		}

		const auto bounds = Rectangle<int>::Create(Point<int>{x, y}, Vector{width, height});
		const auto anchorOffset = Vector{anchorx, anchory};
		frameList.push_back(Sprite::SpriteFrame{imageSheets.at(sheetId), bounds, anchorOffset, static_cast<unsigned int>(delay)});
	}

	if (frameList.size() <= 0)
	{
		throw std::runtime_error("Sprite Action contains no valid frames: " + action);
	}

	return frameList;
}

} // namespace
