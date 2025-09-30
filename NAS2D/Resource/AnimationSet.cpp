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
	Actions indexActions(const AnimationFile& animationFile, ImageCache& imageCache);


	AnimationFileIndexedData readAndIndexAnimationFile(std::string_view filePath, ImageCache& imageCache)
	{
		try
		{
			const auto animationFile = AnimationFile{filePath};
			return {
				loadImages(animationFile.imageSheetReferences(), animationFile.basePath(), imageCache),
				indexActions(animationFile, imageCache)
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


	Actions indexActions(const AnimationFile& animationFile, ImageCache& imageCache)
	{
		Actions actions;
		for (std::size_t actionIndex = 0; actionIndex < animationFile.actionCount(); ++actionIndex)
		{
			const auto& action = animationFile.action(actionIndex);
			if (actions.contains(action.name))
			{
				throw std::runtime_error("Action redefinition: " + action.name);
			}

			actions.try_emplace(action.name, animationFile.animationSequence(actionIndex, imageCache));
		}
		return actions;
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
