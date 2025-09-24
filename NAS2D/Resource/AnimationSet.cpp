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

#include "AnimationFrame.h"
#include "Image.h"
#include "ResourceCache.h"
#include "../Duration.h"
#include "../Utility.h"
#include "../Filesystem.h"
#include "../ContainerUtils.h"
#include "../ParserHelper.h"
#include "../Xml/Xml.h"
#include "../Math/Vector.h"
#include "../Math/Rectangle.h"

#include <tuple>
#include <utility>


using namespace NAS2D;


namespace
{
	constexpr std::string_view SpriteVersion{"0.99"};

	using ImageCache = ResourceCache<Image, std::string>;
	ImageCache animationImageCache;

	using ImageSheets = AnimationSet::ImageSheets;
	using Actions = AnimationSet::Actions;


	std::tuple<ImageSheets, Actions> processXml(std::string_view filePath, ImageCache& imageCache);
	ImageSheets processImageSheets(const std::string& basePath, const Xml::XmlElement* element, ImageCache& imageCache);
	Actions processActions(const ImageSheets& imageSheets, const Xml::XmlElement* element, ImageCache& imageCache);
	AnimationSequence processFrames(const ImageSheets& imageSheets, const Xml::XmlElement* element, ImageCache& imageCache);
	[[noreturn]] void throwLoadError(std::string_view message, const Xml::XmlElement* element);
}


AnimationSet::AnimationSet(std::string_view fileName) :
	AnimationSet{fileName, animationImageCache}
{
}


AnimationSet::AnimationSet(std::string_view fileName, ImageCache& imageCache) :
	mImageSheets{},
	mActions{}
{
	auto [imageSheets, actions] = processXml(fileName, imageCache);
	mImageSheets = std::move(imageSheets);
	mActions = std::move(actions);
}


AnimationSet::AnimationSet(ImageSheets imageSheets, Actions actions) :
	mImageSheets{std::move(imageSheets)},
	mActions{std::move(actions)}
{
}


std::vector<std::string> AnimationSet::actionNames() const
{
	return getKeys(mActions);
}


const AnimationSequence& AnimationSet::frames(const std::string& actionName) const
{
	if (!mActions.contains(actionName))
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
	std::tuple<ImageSheets, Actions> processXml(std::string_view filePath, ImageCache& imageCache)
	{
		try
		{
			auto& filesystem = Utility<Filesystem>::get();
			const auto basePath = Filesystem::parentPath(filePath);

			Xml::XmlDocument xmlDoc;
			xmlDoc.parse(filesystem.readFile(VirtualPath{filePath}).c_str());

			if (xmlDoc.error())
			{
				throw std::runtime_error("Malformed XML: Row: " + std::to_string(xmlDoc.errorRow()) + " Column: " + std::to_string(xmlDoc.errorCol()) + " : " + xmlDoc.errorDesc());
			}

			const auto* xmlRootElement = xmlDoc.firstChildElement("sprite");
			if (!xmlRootElement)
			{
				throw std::runtime_error("Missing required <sprite> tag");
			}

			const auto version = xmlRootElement->attribute("version");
			if (version.empty())
			{
				throw std::runtime_error("No version specified");
			}
			if (version != SpriteVersion)
			{
				throw std::runtime_error("Unsupported version: Expected: " + std::string{SpriteVersion} + " Actual: " + version);
			}

			// Note:
			// Here instead of going through each element and calling a processing function to handle
			// it, we just iterate through all nodes to find sprite sheets. This allows us to define
			// image sheets anywhere in the sprite file.
			auto imageSheets = processImageSheets(basePath, xmlRootElement, imageCache);
			auto actions = processActions(imageSheets, xmlRootElement, imageCache);
			return std::tuple{std::move(imageSheets), std::move(actions)};
		}
		catch (const std::runtime_error& error)
		{
			throw std::runtime_error("Error loading Sprite file: " + filePath + "\n" + error.what());
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
	ImageSheets processImageSheets(const std::string& basePath, const Xml::XmlElement* element, ImageCache& imageCache)
	{
		ImageSheets imageSheets;

		for (const auto* node = element->firstChildElement("imagesheet"); node; node = node->nextSiblingElement("imagesheet"))
		{
			const auto dictionary = attributesToDictionary(*node);
			const auto id = dictionary.get("id");
			const auto src = dictionary.get("src");

			if (id.empty())
			{
				throwLoadError("Image sheet definition has `id` of length zero: ", node);
			}

			if (src.empty())
			{
				throwLoadError("Image sheet definition has `src` of length zero: ", node);
			}

			if (imageSheets.contains(id))
			{
				throwLoadError("Image sheet redefinition: id: '" + id + "' ", node);
			}

			const auto imagePath = basePath + src;
			imageSheets.try_emplace(id, imagePath);
			imageCache.load(imagePath);
		}

		return imageSheets;
	}


	/**
	 * Iterates through all elements of a Sprite XML definition looking
	 * for 'action' elements and processes them.
	 */
	Actions processActions(const ImageSheets& imageSheets, const Xml::XmlElement* element, ImageCache& imageCache)
	{
		Actions actions;

		for (const auto* action = element->firstChildElement("action"); action; action = action->nextSiblingElement("action"))
		{
			const auto dictionary = attributesToDictionary(*action);
			const auto actionName = dictionary.get("name");

			if (actionName.empty())
			{
				throwLoadError("Action definition has 'name' of length zero: ", action);
			}
			if (actions.find(actionName) != actions.end())
			{
				throwLoadError("Action redefinition: '" + actionName + "' ", action);
			}

			actions.try_emplace(actionName, processFrames(imageSheets, action, imageCache));

			if (actions.at(actionName).empty())
			{
				throw std::runtime_error("Action contains no valid frames: " + actionName);
			}
		}

		return actions;
	}


	/**
	 * Parses through all <frame> tags within an <action> tag in a Sprite Definition.
	 */
	AnimationSequence processFrames(const ImageSheets& imageSheets, const Xml::XmlElement* element, ImageCache& imageCache)
	{
		std::vector<AnimationFrame> frameList;

		for (const auto* frame = element->firstChildElement("frame"); frame; frame = frame->nextSiblingElement("frame"))
		{
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
				throwLoadError("Frame definition has 'sheetid' of length zero: ", frame);
			}
			const auto iterator = imageSheets.find(sheetId);
			if (iterator == imageSheets.end())
			{
				throwLoadError("Frame definition references undefined imagesheet: '" + sheetId + "' ", frame);
			}

			const auto& image = imageCache.load(iterator->second);

			const auto frameRect = Rectangle<int>{{x, y}, {width, height}};
			const auto imageRect = Rectangle{{0, 0}, image.size()};
			if (!imageRect.contains(frameRect))
			{
				throwLoadError("Frame bounds exceeds image sheet bounds: ", frame);
			}

			const auto anchorOffset = Vector{anchorx, anchory};
			frameList.push_back(AnimationFrame{image, frameRect, anchorOffset, {delay}});
		}

		return {frameList};
	}


	void throwLoadError(std::string_view message, const Xml::XmlElement* element)
	{
		throw std::runtime_error(message + " (Line: " + std::to_string(element->row()) + ")");
	}
}
