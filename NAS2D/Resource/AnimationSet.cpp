
#include "AnimationSet.h"

#include "ResourceCache.h"
#include "../Utility.h"
#include "../Filesystem.h"
#include "../ContainerUtils.h"
#include "../StringUtils.h"
#include "../Version.h"
#include "../Xml/Xml.h"


using namespace NAS2D;


namespace
{
	constexpr std::string_view SPRITE_VERSION{"0.99"};

	using ImageCache = ResourceCache<Image, std::string>;
	ImageCache animationImageCache;


	// Adds a row tag to the end of messages.
	std::string endTag(int row)
	{
		return " (Row: " + std::to_string(row) + ")";
	}

	AnimationSet processXml(std::string filePath, ImageCache& imageCache);
	std::map<std::string, std::string> processImageSheets(const std::string& basePath, const Xml::XmlElement* element, ImageCache& imageCache);
	std::map<std::string, std::vector<AnimationSet::Frame>> processActions(const std::map<std::string, std::string>& imageSheetMap, const Xml::XmlElement* element, ImageCache& imageCache);
	std::vector<AnimationSet::Frame> processFrames(const std::map<std::string, std::string>& imageSheetMap, const std::string& action, const Xml::XmlNode* node, ImageCache& imageCache);
}


AnimationSet::AnimationSet(std::string fileName) : AnimationSet{processXml(std::move(fileName), animationImageCache)}
{
}


AnimationSet::AnimationSet(std::string fileName, std::map<std::string, std::string> imageSheetMap, std::map<std::string, std::vector<Frame>> actions) :
	mFileName{std::move(fileName)},
	mImageSheetMap{std::move(imageSheetMap)},
	mActions{std::move(actions)}
{
}


std::vector<std::string> AnimationSet::actionNames() const
{
	return getKeys(mActions);
}


const std::vector<AnimationSet::Frame>& AnimationSet::frames(const std::string& actionName) const
{
	if (mActions.find(actionName) == mActions.end())
	{
		throw std::runtime_error("Sprite::play called on undefined action: " + actionName + "  (" + mFileName + ")");
	}

	return mActions.at(actionName);
}


namespace
{

	/**
	 * Parses a Sprite XML Definition File.
	 *
	 * \param filePath	File path of the sprite XML definition file.
	 */
	AnimationSet processXml(std::string filePath, ImageCache& imageCache)
	{
		try
		{
			auto& filesystem = Utility<Filesystem>::get();
			const auto basePath = filesystem.workingPath(filePath);

			Xml::XmlDocument xmlDoc;
			xmlDoc.parse(filesystem.open(filePath).raw_bytes());

			if (xmlDoc.error())
			{
				throw std::runtime_error("Sprite file has malformed XML: Row: " + std::to_string(xmlDoc.errorRow()) + " Column: " + std::to_string(xmlDoc.errorCol()) + " : " + xmlDoc.errorDesc());
			}

			// Find the Sprite node.
			const auto* xmlRootElement = xmlDoc.firstChildElement("sprite");
			if (!xmlRootElement)
			{
				throw std::runtime_error("Sprite file does not contain required <sprite> tag");
			}

			// Get the Sprite version.
			const auto* version = xmlRootElement->firstAttribute();
			if (!version || version->value().empty())
			{
				throw std::runtime_error("Sprite file's root element does not specify a version");
			}
			if (version->value() != SPRITE_VERSION)
			{
				throw std::runtime_error("Sprite version mismatch. Expected: " + std::string{SPRITE_VERSION} + " Actual: " + versionString());
			}

			// Note:
			// Here instead of going through each element and calling a processing function to handle
			// it, we just iterate through all nodes to find sprite sheets. This allows us to define
			// image sheets anywhere in the sprite file.
			auto imageSheetMap = processImageSheets(basePath, xmlRootElement, imageCache);
			auto actions = processActions(imageSheetMap, xmlRootElement, imageCache);
			return {std::move(filePath), std::move(imageSheetMap), std::move(actions)};
		}
		catch(const std::runtime_error& error)
		{
			throw std::runtime_error("Error parsing Sprite file: " + filePath + "\nError: " + error.what());
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
	std::map<std::string, std::string> processImageSheets(const std::string& basePath, const Xml::XmlElement* element, ImageCache& imageCache)
	{
		std::map<std::string, std::string> imageSheetMap;

		for (const auto* node = element->iterateChildren(nullptr);
			node != nullptr;
			node = element->iterateChildren(node))
		{
			if (node->value() == "imagesheet" && node->toElement())
			{
				std::string id, src;
				const auto* attribute = node->toElement()->firstAttribute();
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

				if (imageSheetMap.find(id) != imageSheetMap.end())
				{
					throw std::runtime_error("Sprite image sheet redefinition: id: '" + id + "' " + endTag(node->row()));
				}

				const auto imagePath = basePath + src;
				imageSheetMap.try_emplace(id, imagePath);
				imageCache.load(imagePath);
			}
		}

		return imageSheetMap;
	}


	/**
	 * Iterates through all elements of a Sprite XML definition looking
	 * for 'action' elements and processes them.
	 *
	 * \note	Action names are not case sensitive. "Case", "caSe",
	 *			"CASE", etc. will all be viewed as identical.
	 */
	std::map<std::string, std::vector<AnimationSet::Frame>> processActions(const std::map<std::string, std::string>& imageSheetMap, const Xml::XmlElement* element, ImageCache& imageCache)
	{
		std::map<std::string, std::vector<AnimationSet::Frame>> actions;

		for (const auto* node = element->iterateChildren(nullptr);
			node != nullptr;
			node = element->iterateChildren(node))
		{
			if (toLowercase(node->value()) == "action" && node->toElement())
			{

				std::string actionName;
				const auto* attribute = node->toElement()->firstAttribute();
				while (attribute)
				{
					if (toLowercase(attribute->name()) == "name")
					{
						actionName = attribute->value();
					}

					attribute = attribute->next();
				}

				if (actionName.empty())
				{
					throw std::runtime_error("Sprite Action definition has 'name' of length zero: " + endTag(node->row()));
				}
				if (actions.find(actionName) != actions.end())
				{
					throw std::runtime_error("Sprite Action redefinition: '" + actionName + "' " + endTag(node->row()));
				}

				actions[actionName] = processFrames(imageSheetMap, actionName, node, imageCache);
			}
		}

		return actions;
	}


	/**
	 * Parses through all <frame> tags within an <action> tag in a Sprite Definition.
	 */
	std::vector<AnimationSet::Frame> processFrames(const std::map<std::string, std::string>& imageSheetMap, const std::string& action, const Xml::XmlNode* node, ImageCache& imageCache)
	{
		std::vector<AnimationSet::Frame> frameList;

		for (const auto* frame = node->iterateChildren(nullptr);
			frame != nullptr;
			frame = node->iterateChildren(frame))
		{
			int currentRow = frame->row();

			if (frame->value() != "frame" || !frame->toElement())
			{
				throw std::runtime_error("Sprite frame tag unexpected: <" + frame->value() + "> : " + endTag(currentRow));
			}

			std::string sheetId;
			int delay = 0;
			int x = 0, y = 0;
			int width = 0, height = 0;
			int anchorx = 0, anchory = 0;

			const auto* attribute = frame->toElement()->firstAttribute();
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
				else
				{
					throw std::runtime_error("Sprite frame attribute unexpected: '" + attribute->name() + "' : " + endTag(currentRow));
				}

				attribute = attribute->next();
			}

			if (sheetId.empty())
			{
				throw std::runtime_error("Sprite Frame definition has 'sheetid' of length zero: " + endTag(currentRow));
			}
			const auto iterator = imageSheetMap.find(sheetId);
			if (iterator == imageSheetMap.end())
			{
				throw std::runtime_error("Sprite Frame definition references undefined imagesheet: '" + sheetId + "' " + endTag(currentRow));
			}

			const auto& image = imageCache.load(iterator->second);
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
			frameList.push_back(AnimationSet::Frame{image, bounds, anchorOffset, static_cast<unsigned int>(delay)});
		}

		if (frameList.size() <= 0)
		{
			throw std::runtime_error("Sprite Action contains no valid frames: " + action);
		}

		return frameList;
	}

}
