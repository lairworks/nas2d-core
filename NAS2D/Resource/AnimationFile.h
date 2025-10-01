#pragma once

#include <string_view>
#include <string>
#include <vector>


namespace NAS2D
{
	struct AnimationImageSheetReference;
	struct AnimationAction;
	struct AnimationFileData;
	class AnimationSequence;
	class Image;
	template <typename Resource, typename... Params> class ResourceCache;
	using ImageCache = ResourceCache<Image, std::string>;


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

		std::size_t imageSheetReferenceCount() const;
		std::size_t actionCount() const;

		std::size_t imageSheetReferenceIndex(const std::string& id) const;
		std::size_t actionIndex(const std::string& name) const;

		const AnimationImageSheetReference& imageSheetReference(std::size_t index) const;
		const AnimationAction& action(std::size_t index) const;
		const std::string& actionName(std::size_t index) const;

		AnimationSequence animationSequence(std::size_t actionIndex, ImageCache& imageCache) const;

	private:
		std::string mBasePath;
		std::vector<AnimationImageSheetReference> mImageSheetReferences;
		std::vector<AnimationAction> mActions;
	};


	AnimationFile loadAnimationFile(std::string_view filePath);
}
