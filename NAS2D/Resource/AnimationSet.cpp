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

#include "AnimationFile.h"
#include "AnimationFrame.h"
#include "Image.h"
#include "ResourceCache.h"
#include "../Duration.h"
#include "../Utility.h"
#include "../Filesystem.h"
#include "../ContainerUtils.h"
#include "../StringFrom.h"
#include "../ParserHelper.h"
#include "../Xml/XmlNode.h"
#include "../Xml/XmlElement.h"
#include "../Xml/XmlDocument.h"
#include "../Math/Vector.h"
#include "../Math/Rectangle.h"

#include <utility>
#include <stdexcept>


using namespace NAS2D;


namespace
{
	constexpr std::string_view SpriteVersion{"0.99"};

	using ImageCache = ResourceCache<Image, std::string>;
	ImageCache animationImageCache;

	using ImageSheets = AnimationSet::ImageSheets;
	using Actions = AnimationSet::Actions;


	struct AnimationFileIndexedData
	{
		ImageSheets imageSheets;
		Actions actions;
	};

	[[noreturn]] void throwLoadError(std::string_view message, const Xml::XmlNode* node);
	AnimationFile readAnimationFile(std::string_view filePath);
	AnimationFileData readAnimationFileData(std::string_view fileData);
	std::vector<AnimationImageSheetReference> readImageSheetReferences(const Xml::XmlElement* element);
	std::vector<AnimationAction> readActions(const Xml::XmlElement* element);
	std::vector<AnimationFrameData> readFrames(const Xml::XmlElement* element);

	AnimationFileIndexedData readAndIndexAnimationFile(std::string_view filePath, ImageCache& imageCache);
	ImageSheets loadImages(const std::vector<AnimationImageSheetReference>& imageSheetReferences, const std::string& basePath, ImageCache& imageCache);
	Actions indexActions(const std::vector<AnimationAction>& actionDefinitions, const ImageSheets& imageSheets, ImageCache& imageCache);
	AnimationSequence buildAnimationSequences(std::vector<AnimationFrameData> frameDefinitions, const ImageSheets& imageSheets, ImageCache& imageCache);


	void throwLoadError(std::string_view message, const Xml::XmlNode* node)
	{
		throw std::runtime_error(std::string{message} + " (Line: " + std::to_string(node->row()) + ")");
	}


	AnimationFile readAnimationFile(std::string_view filePath)
	{
		auto& filesystem = Utility<Filesystem>::get();
		return {
			Filesystem::parentPath(filePath),
			readAnimationFileData(filesystem.readFile(VirtualPath{filePath})),
		};
	}


	AnimationFileData readAnimationFileData(std::string_view fileData)
	{
		Xml::XmlDocument xmlDoc;
		xmlDoc.parse(fileData.data());

		if (xmlDoc.error())
		{
			throw std::runtime_error("Malformed XML: Line: " + std::to_string(xmlDoc.errorRow()) + " Column: " + std::to_string(xmlDoc.errorCol()) + " : " + xmlDoc.errorDesc());
		}

		const auto* spriteElement = xmlDoc.firstChildElement("sprite");
		if (!spriteElement)
		{
			throw std::runtime_error("Missing required <sprite> tag");
		}

		const auto version = spriteElement->attribute("version");
		if (version.empty())
		{
			throwLoadError("No version specified", spriteElement);
		}
		if (version != SpriteVersion)
		{
			throwLoadError("Unsupported version: Expected: " + std::string{SpriteVersion} + " Actual: " + version, spriteElement);
		}

		return {
			readImageSheetReferences(spriteElement),
			readActions(spriteElement),
		};
	}


	std::vector<AnimationImageSheetReference> readImageSheetReferences(const Xml::XmlElement* element)
	{
		std::vector<AnimationImageSheetReference> imageSheetReferences;
		for (const auto* node = element->firstChildElement("imagesheet"); node; node = node->nextSiblingElement("imagesheet"))
		{
			const auto dictionary = attributesToDictionary(*node);
			const auto& imageSheetReference = imageSheetReferences.emplace_back(
				dictionary.get("id"),
				dictionary.get("src")
			);

			if (imageSheetReference.id.empty())
			{
				throwLoadError("Image sheet definition has `id` of length zero", node);
			}

			if (imageSheetReference.filePath.empty())
			{
				throwLoadError("Image sheet definition has `src` of length zero", node);
			}
		}
		return imageSheetReferences;
	}


	std::vector<AnimationAction> readActions(const Xml::XmlElement* element)
	{
		std::vector<AnimationAction> actionDefinitions;
		for (const auto* action = element->firstChildElement("action"); action; action = action->nextSiblingElement("action"))
		{
			const auto dictionary = attributesToDictionary(*action);
			const auto& actionDefinition = actionDefinitions.emplace_back(
				dictionary.get("name"),
				readFrames(action)
			);

			if (actionDefinition.name.empty())
			{
				throwLoadError("Action definition has 'name' of length zero", action);
			}
		}
		return actionDefinitions;
	}


	std::vector<AnimationFrameData> readFrames(const Xml::XmlElement* element)
	{
		std::vector<AnimationFrameData> frameDefinitions;
		for (const auto* frame = element->firstChildElement("frame"); frame; frame = frame->nextSiblingElement("frame"))
		{
			const auto dictionary = attributesToDictionary(*frame);
			reportMissingOrUnexpected(dictionary.keys(), {"sheetid", "x", "y", "width", "height", "anchorx", "anchory"}, {"delay"});

			const auto& animationFrameData = frameDefinitions.emplace_back(
				dictionary.get("sheetid"),
				Rectangle{
					Point{
						dictionary.get<int>("x"),
						dictionary.get<int>("y"),
					},
					Vector{
						dictionary.get<int>("width"),
						dictionary.get<int>("height"),
					}
				},
				Vector{
					dictionary.get<int>("anchorx"),
					dictionary.get<int>("anchory"),
				},
				Duration{dictionary.get<unsigned int>("delay", 0)}
			);

			if (animationFrameData.id.empty())
			{
				throwLoadError("Frame definition has 'sheetid' of length zero", frame);
			}
		}
		return frameDefinitions;
	}


	AnimationFileIndexedData readAndIndexAnimationFile(std::string_view filePath, ImageCache& imageCache)
	{
		try
		{
			const auto& [basePath, animationFileData] = readAnimationFile(filePath);
			auto imageSheets = loadImages(animationFileData.imageSheetReferences, basePath, imageCache);
			auto actions = indexActions(animationFileData.actions, imageSheets, imageCache);
			return {
				std::move(imageSheets),
				std::move(actions)
			};
		}
		catch (const std::runtime_error& error)
		{
			throw std::runtime_error("Error loading Sprite file: " + std::string{filePath} + "\n" + error.what());
		}
	}


	ImageSheets loadImages(const std::vector<AnimationImageSheetReference>& imageSheetReferences, const std::string& basePath, ImageCache& imageCache)
	{
		ImageSheets imageSheets;
		for (const auto& imageSheetReference : imageSheetReferences)
		{
			if (imageSheets.contains(imageSheetReference.id))
			{
				throw std::runtime_error("Image sheet redefinition: id: " + imageSheetReference.id);
			}

			const auto imagePath = basePath + imageSheetReference.filePath;
			imageSheets.try_emplace(imageSheetReference.id, imagePath);
			imageCache.load(imagePath);
		}
		return imageSheets;
	}


	Actions indexActions(const std::vector<AnimationAction>& actionDefinitions, const ImageSheets& imageSheets, ImageCache& imageCache)
	{
		Actions actions;
		for (const auto& action : actionDefinitions)
		{
			if (actions.contains(action.name))
			{
				throw std::runtime_error("Action redefinition: " + action.name);
			}

			actions.try_emplace(action.name, buildAnimationSequences(action.frames, imageSheets, imageCache));

			if (actions.at(action.name).empty())
			{
				throw std::runtime_error("Action contains no valid frames: " + action.name);
			}
		}
		return actions;
	}


	AnimationSequence buildAnimationSequences(std::vector<AnimationFrameData> frameDefinitions, const ImageSheets& imageSheets, ImageCache& imageCache)
	{
		std::vector<AnimationFrame> frameList;
		for (const auto& animationFrameData : frameDefinitions)
		{
			if (!imageSheets.contains(animationFrameData.id))
			{
				throw std::runtime_error("Frame definition references undefined imagesheet: " + animationFrameData.id);
			}

			const auto& filePath = imageSheets.at(animationFrameData.id);
			const auto& image = imageCache.load(filePath);

			const auto imageRect = Rectangle{{0, 0}, image.size()};
			if (!imageRect.contains(animationFrameData.imageBounds))
			{
				throw std::runtime_error("Frame bounds exceeds image sheet bounds: " + animationFrameData.id + " : " + stringFrom(animationFrameData.imageBounds));
			}

			frameList.push_back(AnimationFrame{image, animationFrameData.imageBounds, animationFrameData.anchorOffset, animationFrameData.frameDelay});
		}
		return {frameList};
	}
}


AnimationSet::AnimationSet(std::string_view fileName) :
	AnimationSet{fileName, animationImageCache}
{
}


AnimationSet::AnimationSet(std::string_view fileName, ImageCache& imageCache) :
	mImageSheets{},
	mActions{}
{
	auto [imageSheets, actions] = readAndIndexAnimationFile(fileName, imageCache);
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
