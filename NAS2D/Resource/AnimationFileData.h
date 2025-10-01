#pragma once

#include "../Duration.h"
#include "../Math/Vector.h"
#include "../Math/Rectangle.h"

#include <string>
#include <vector>


namespace NAS2D
{
	struct AnimationImageSheetReference
	{
		std::string id;
		std::string filePath;
	};

	struct AnimationFrameData
	{
		std::string id;
		Rectangle<int> imageBounds;
		Vector<int> anchorOffset;
		Duration frameDelay;
	};

	struct AnimationAction
	{
		std::string name;
		std::vector<AnimationFrameData> frames;
	};

	struct AnimationFileData
	{
		std::vector<AnimationImageSheetReference> imageSheetReferences;
		std::vector<AnimationAction> actions;
	};
}
