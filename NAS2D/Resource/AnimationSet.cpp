// ==================================================================================
// = NAS2D
// = Copyright © 2008 - 2020 New Age Software
// ==================================================================================
// = NAS2D is distributed under the terms of the zlib license. You are free to copy,
// = modify and distribute the software under the terms of the zlib license.
// =
// = Acknowledgment of your use of NAS2D is appreciated but is not required.
// ==================================================================================


#include "AnimationSet.h"

#include "ResourceCache.h"
#include "../Utility.h"
#include "../Filesystem.h"
#include "../ContainerUtils.h"
#include "../ParserHelper.h"
#include "../Version.h"
#include "../Xml/Xml.h"

#include <tuple>
#include <utility>


using namespace NAS2D;


namespace
{
	constexpr std::string_view SpriteVersion{"0.99"};

	using ImageCache = ResourceCache<Image, std::string>;
	ImageCache animationImageCache;


	// Adds a row tag to the end of messages.
	std::string endTag(int row)
	{
		return " (Row: " + std::to_string(row) + ")";
	}

	using ImageSheetMap = AnimationSet::ImageSheetMap;
	using ActionsMap = AnimationSet::ActionsMap;


	std::tuple<ImageSheetMap, ActionsMap> processXml(const std::string& filePath, ImageCache& imageCache);
	ImageSheetMap processImageSheets(const std::string& basePath, const Xml::XmlElement* element, ImageCache& imageCache);
	ActionsMap processActions(const ImageSheetMap& imageSheetMap, const Xml::XmlElement* element, ImageCache& imageCache);
	std::vector<AnimationSet::Frame> processFrames(const ImageSheetMap& imageSheetMap, const Xml::XmlElement* element, ImageCache& imageCache);
}


bool AnimationSet::Frame::isStopFrame() const
{
	// We want to redefine the sentinel value for stop frames from -1 to 0
	// Neither value makes sense as a delay, though the field is unsigned
	// Using 0 would also help simplify some code, so makes the most sense
	// Temporarily handle -1 for backwards compatibility during transition
	return (frameDelay == 0) || (frameDelay == unsigned(-1));
}


AnimationSet::AnimationSet(std::string fileName) :
	AnimationSet{std::move(fileName), animationImageCache}
{
}


AnimationSet::AnimationSet(std::string fileName, ImageCache& imageCache) :
	mImageSheetMap{},
	mActions{}
{
	auto [imageSheetMap, actions] = processXml(fileName, imageCache);
	mImageSheetMap = std::move(imageSheetMap);
	mActions = std::move(actions);
}


AnimationSet::AnimationSet(ImageSheetMap imageSheetMap, ActionsMap actions) :
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
		throw std::runtime_error("AnimationSet::frames called on undefined action: " + actionName);
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
	std::tuple<ImageSheetMap, ActionsMap> processXml(const std::string& filePath, ImageCache& imageCache)
	{
		try
		{
			auto& filesystem = Utility<Filesystem>::get();
			const auto basePath = Filesystem::parentPath(filePath);

			Xml::XmlDocument xmlDoc;
			xmlDoc.parse(filesystem.readFile(filePath).c_str());

			if (xmlDoc.error())
			{
				throw std::runtime_error("Sprite file has malformed XML: Row: " + std::to_string(xmlDoc.errorRow()) + " Column: " + std::to_string(xmlDoc.errorCol()) + " : " + xmlDoc.errorDesc());
			}

			const auto* xmlRootElement = xmlDoc.firstChildElement("sprite");
			if (!xmlRootElement)
			{
				throw std::runtime_error("Sprite file does not contain required <sprite> tag");
			}

			const auto version = xmlRootElement->attribute("version");
			if (version.empty())
			{
				throw std::runtime_error("Sprite file's root element does not specify a version");
			}
			if (version != SpriteVersion)
			{
				throw std::runtime_error("Sprite version mismatch. Expected: " + std::string{SpriteVersion} + " Actual: " + versionString());
			}

			// Note:
			// Here instead of going through each element and calling a processing function to handle
			// it, we just iterate through all nodes to find sprite sheets. This allows us to define
			// image sheets anywhere in the sprite file.
			auto imageSheetMap = processImageSheets(basePath, xmlRootElement, imageCache);
			auto actions = processActions(imageSheetMap, xmlRootElement, imageCache);
			return std::tuple{std::move(imageSheetMap), std::move(actions)};
		}
		catch (const std::runtime_error& error)
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
	ImageSheetMap processImageSheets(const std::string& basePath, const Xml::XmlElement* element, ImageCache& imageCache)
	{
		ImageSheetMap imageSheetMap;

		for (const auto* node = element->firstChildElement("imagesheet"); node; node = node->nextSiblingElement("imagesheet"))
		{
			const auto dictionary = attributesToDictionary(*node);
			const auto id = dictionary.get("id");
			const auto src = dictionary.get("src");

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

		return imageSheetMap;
	}


	/**
	 * Iterates through all elements of a Sprite XML definition looking
	 * for 'action' elements and processes them.
	 */
	ActionsMap processActions(const ImageSheetMap& imageSheetMap, const Xml::XmlElement* element, ImageCache& imageCache)
	{
		ActionsMap actions;

		for (const auto* action = element->firstChildElement("action"); action; action = action->nextSiblingElement("action"))
		{
			const auto dictionary = attributesToDictionary(*action);
			const auto actionName = dictionary.get("name");

			if (actionName.empty())
			{
				throw std::runtime_error("Sprite Action definition has 'name' of length zero: " + endTag(action->row()));
			}
			if (actions.find(actionName) != actions.end())
			{
				throw std::runtime_error("Sprite Action redefinition: '" + actionName + "' " + endTag(action->row()));
			}

			actions[actionName] = processFrames(imageSheetMap, action, imageCache);

			if (actions[actionName].empty())
			{
				throw std::runtime_error("Sprite Action contains no valid frames: " + actionName);
			}
		}

		return actions;
	}


	/**
	 * Parses through all <frame> tags within an <action> tag in a Sprite Definition.
	 */
	std::vector<AnimationSet::Frame> processFrames(const ImageSheetMap& imageSheetMap, const Xml::XmlElement* element, ImageCache& imageCache)
	{
		std::vector<AnimationSet::Frame> frameList;

		for (const auto* frame = element->firstChildElement("frame"); frame; frame = frame->nextSiblingElement("frame"))
		{
			int currentRow = frame->row();

			const auto dictionary = attributesToDictionary(*frame);
			reportMissingOrUnexpected(dictionary.keys(), {"sheetid", "x", "y", "width", "height", "anchorx", "anchory"}, {"delay"});

			const auto sheetId = dictionary.get("sheetid");
			const auto delay = dictionary.get<unsigned int>("delay", 0);
			const auto x = dictionary.get<int>("x");
			const auto y = dictionary.get<int>("y");
			const auto width = dictionary.get<int>("width");
			const auto height = dictionary.get<int>("height");
			const auto anchorx = dictionary.get<int>("anchorx");
			const auto anchory = dictionary.get<int>("anchory");

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

			const auto frameRect = Rectangle<int>{{x, y}, {width, height}};
			const auto imageRect = Rectangle{{0, 0}, image.size()};
			if (!imageRect.contains(frameRect))
			{
				throw std::runtime_error("Sprite frame bounds exceeds image sheet bounds: " + endTag(currentRow));
			}

			const auto anchorOffset = Vector{anchorx, anchory};
			frameList.push_back(AnimationSet::Frame{image, frameRect, anchorOffset, delay});
		}

		return frameList;
	}

}
