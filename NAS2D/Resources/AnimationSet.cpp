
#include "AnimationSet.h"

#include "../ContainerUtils.h"


using namespace NAS2D;


AnimationSet::AnimationSet(std::string fileName, std::map<std::string, std::string> imageSheetMap, std::map<std::string, std::vector<AnimationSet::Frame>> actions) :
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
