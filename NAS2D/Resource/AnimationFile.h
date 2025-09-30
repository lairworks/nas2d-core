#pragma once

#include "../Duration.h"
#include "../Math/Vector.h"
#include "../Math/Rectangle.h"

#include <string_view>
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


	class AnimationFile
	{
	public:
		explicit AnimationFile(std::string_view filePath);
		AnimationFile(std::string basePath, AnimationFileData animationFileData);
		AnimationFile(const AnimationFile&) = default;
		AnimationFile(AnimationFile&&) = default;
		~AnimationFile();

		const std::string& basePath() const;
		const std::vector<AnimationImageSheetReference>& imageSheetReferences() const;
		const std::vector<AnimationAction>& actions() const;

	private:
		std::string mBasePath;
		std::vector<AnimationImageSheetReference> mImageSheetReferences;
		std::vector<AnimationAction> mActions;
	};


	AnimationFile loadAnimationFile(std::string_view filePath);
}
