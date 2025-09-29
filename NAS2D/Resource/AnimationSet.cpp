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
#include "../ContainerUtils.h"
#include "../StringFrom.h"
#include "../Math/Rectangle.h"

#include <utility>
#include <stdexcept>


using namespace NAS2D;


namespace
{
	using ImageCache = ResourceCache<Image, std::string>;
	ImageCache animationImageCache;

	using ImageSheets = AnimationSet::ImageSheets;
	using Actions = AnimationSet::Actions;


	struct AnimationFileIndexedData
	{
		ImageSheets imageSheets;
		Actions actions;
	};

	AnimationFileIndexedData readAndIndexAnimationFile(std::string_view filePath, ImageCache& imageCache);
	ImageSheets loadImages(const std::vector<AnimationImageSheetReference>& imageSheetReferences, const std::string& basePath, ImageCache& imageCache);
	Actions indexActions(const std::vector<AnimationAction>& actionDefinitions, const ImageSheets& imageSheets, ImageCache& imageCache);
	AnimationSequence buildAnimationSequences(std::vector<AnimationFrameData> frameDefinitions, const ImageSheets& imageSheets, ImageCache& imageCache);


	AnimationFileIndexedData readAndIndexAnimationFile(std::string_view filePath, ImageCache& imageCache)
	{
		try
		{
			const auto& [basePath, animationFileData] = loadAnimationFile(filePath);
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
