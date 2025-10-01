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
	using Actions = AnimationSet::Actions;
	using ImageCache = ResourceCache<Image, std::string>;
	ImageCache animationImageCache;


	Actions readAndIndexAnimationFile(std::string_view filePath, ImageCache& imageCache);
	Actions indexActions(const AnimationFile& animationFile, ImageCache& imageCache);


	Actions readAndIndexAnimationFile(std::string_view filePath, ImageCache& imageCache)
	{
		try
		{
			const auto animationFile = AnimationFile{filePath};
			return indexActions(animationFile, imageCache);
		}
		catch (const std::runtime_error& error)
		{
			throw std::runtime_error("Error loading Sprite file: " + std::string{filePath} + "\n" + error.what());
		}
	}


	Actions indexActions(const AnimationFile& animationFile, ImageCache& imageCache)
	{
		Actions actions;
		for (std::size_t actionIndex = 0; actionIndex < animationFile.actionCount(); ++actionIndex)
		{
			const auto& actionName = animationFile.actionName(actionIndex);
			if (actions.contains(actionName))
			{
				throw std::runtime_error("Action redefinition: " + actionName);
			}

			actions.try_emplace(actionName, animationFile.animationSequence(actionIndex, imageCache));
		}
		return actions;
	}
}


AnimationSet::AnimationSet(std::string_view fileName) :
	AnimationSet{fileName, animationImageCache}
{
}


AnimationSet::AnimationSet(std::string_view fileName, ImageCache& imageCache) :
	mActions{readAndIndexAnimationFile(fileName, imageCache)}
{
}


AnimationSet::AnimationSet(Actions actions) :
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
