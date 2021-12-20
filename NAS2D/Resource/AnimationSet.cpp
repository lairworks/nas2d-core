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
#include "../StringUtils.h"
#include "../ParserHelper.h"
#include "../Version.h"
#include "../Xml/Xml.h"
#include "../Parsing/XmlFile.h"

#include <tuple>


using namespace NAS2D;


namespace
{
	constexpr std::string_view SPRITE_VERSION{"0.99"};

	using ImageCache = ResourceCache<Image, std::string>;
	ImageCache animationImageCache;

	using ImageSheetMap = AnimationSet::ImageSheetMap;
	using ActionsMap = AnimationSet::ActionsMap;


	std::tuple<ImageSheetMap, ActionsMap> processXml(const std::string& filePath, ImageCache& imageCache);
	ImageSheetMap processImageSheets(const std::string& basePath, const XmlSection& spriteSection, ImageCache& imageCache);
	ActionsMap processActions(const ImageSheetMap& imageSheetMap, const XmlSection& spriteSection, ImageCache& imageCache);
	std::vector<AnimationSet::Frame> processFrames(const ImageSheetMap& imageSheetMap, const XmlSection& actionSection, ImageCache& imageCache);
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
	mFileName{std::move(fileName)}
{
	auto [imageSheetMap, actions] = processXml(mFileName, imageCache);
	mImageSheetMap = std::move(imageSheetMap);
	mActions = std::move(actions);
}


AnimationSet::AnimationSet(std::string fileName, ImageSheetMap imageSheetMap, ActionsMap actions) :
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
	std::tuple<ImageSheetMap, ActionsMap> processXml(const std::string& filePath, ImageCache& imageCache)
	{
		try
		{
			auto& filesystem = Utility<Filesystem>::get();
			const auto basePath = filesystem.parentPath(filePath);

			const auto xmlFile = XmlFile{filesystem.read(filePath)};
			const auto spriteSection = xmlFile.root("sprite");

			const auto version = spriteSection.value("version", std::string{""});
			if (version.empty())
			{
				throw std::runtime_error("Sprite file's root element does not specify a version");
			}
			if (version != SPRITE_VERSION)
			{
				throw std::runtime_error("Sprite version mismatch. Expected: " + std::string{SPRITE_VERSION} + " Actual: " + versionString());
			}

			// Note:
			// Here instead of going through each element and calling a processing function to handle
			// it, we just iterate through all nodes to find sprite sheets. This allows us to define
			// image sheets anywhere in the sprite file.
			auto imageSheetMap = processImageSheets(basePath, spriteSection, imageCache);
			auto actions = processActions(imageSheetMap, spriteSection, imageCache);
			return std::tuple{std::move(imageSheetMap), std::move(actions)};
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
	ImageSheetMap processImageSheets(const std::string& basePath, const XmlSection& spriteSection, ImageCache& imageCache)
	{
		ImageSheetMap imageSheetMap;

		for (const auto& section : spriteSection.subSections("imagesheet"))
		{
			const auto id = section.value("id");
			const auto src = section.value("src");

			if (id.empty())
			{
				section.throwError("Sprite imagesheet definition has `id` of length zero");
			}

			if (src.empty())
			{
				section.throwError("Sprite imagesheet definition has `src` of length zero");
			}

			if (imageSheetMap.find(id) != imageSheetMap.end())
			{
				section.throwError("Sprite image sheet redefinition: id: '" + id + "'");
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
	ActionsMap processActions(const ImageSheetMap& imageSheetMap, const XmlSection& spriteSection, ImageCache& imageCache)
	{
		ActionsMap actions;

		for (const auto& section : spriteSection.subSections("action"))
		{
			const auto actionName = section.value("name");

			if (actionName.empty())
			{
				section.throwError("Sprite Action definition has 'name' of length zero");
			}
			if (actions.find(actionName) != actions.end())
			{
				section.throwError("Sprite Action redefinition: '" + actionName + "'");
			}

			actions[actionName] = processFrames(imageSheetMap, section, imageCache);

			if (actions[actionName].empty())
			{
				section.throwError("Sprite Action contains no valid frames: " + actionName);
			}
		}

		return actions;
	}


	/**
	 * Parses through all <frame> tags within an <action> tag in a Sprite Definition.
	 */
	std::vector<AnimationSet::Frame> processFrames(const ImageSheetMap& imageSheetMap, const XmlSection& actionSection, ImageCache& imageCache)
	{
		std::vector<AnimationSet::Frame> frameList;

		for (const auto& section : actionSection.subSections("frame"))
		{
			section.verifyKeys({"sheetid", "x", "y", "width", "height", "anchorx", "anchory"}, {"delay"});

			const auto sheetId = section.value("sheetid");
			const auto delay = section.value<unsigned int>("delay", 0);
			const auto x = section.value<int>("x");
			const auto y = section.value<int>("y");
			const auto width = section.value<int>("width");
			const auto height = section.value<int>("height");
			const auto anchorx = section.value<int>("anchorx");
			const auto anchory = section.value<int>("anchory");

			if (sheetId.empty())
			{
				section.throwError("Sprite Frame definition has 'sheetid' of length zero");
			}
			const auto iterator = imageSheetMap.find(sheetId);
			if (iterator == imageSheetMap.end())
			{
				section.throwError("Sprite Frame definition references undefined imagesheet: '" + sheetId + "'");
			}

			const auto& image = imageCache.load(iterator->second);
			// X-Coordinate
			if (x < 0 || x > image.size().x)
			{
				section.throwError("Sprite frame attribute 'x' is out of bounds");
			}
			// Y-Coordinate
			if (y < 0 || y > image.size().y)
			{
				section.throwError("Sprite frame attribute 'y' is out of bounds");
			}
			// Width
			if (width <= 0 || width > image.size().x - x)
			{
				section.throwError("Sprite frame attribute 'width' is out of bounds");
			}
			// Height
			if (height <= 0 || height > image.size().y - y)
			{
				section.throwError("Sprite frame attribute 'height' is out of bounds");
			}

			const auto bounds = Rectangle<int>::Create(Point<int>{x, y}, Vector{width, height});
			const auto anchorOffset = Vector{anchorx, anchory};
			frameList.push_back(AnimationSet::Frame{image, bounds, anchorOffset, delay});
		}

		return frameList;
	}

}
