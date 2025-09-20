// ==================================================================================
// = NAS2D
// = Copyright © 2008 - 2020 New Age Software
// ==================================================================================
// = NAS2D is distributed under the terms of the zlib license. You are free to copy,
// = modify and distribute the software under the terms of the zlib license.
// =
// = Acknowledgment of your use of NAS2D is appreciated but is not required.
// ==================================================================================

#pragma once

#include "../Duration.h"
#include "../Math/Vector.h"
#include "../Math/Rectangle.h"

#include <map>
#include <vector>
#include <string>


namespace NAS2D
{
	struct AnimationFrame;
	class Image;
	template <typename Resource, typename... Params> class ResourceCache;


	class AnimationSet
	{
	public:
		using ImageSheets = std::map<std::string, std::string>;
		using Actions = std::map<std::string, std::vector<AnimationFrame>>;


		explicit AnimationSet(std::string fileName);
		AnimationSet(std::string fileName, ResourceCache<Image, std::string>& imageCache);
		AnimationSet(ImageSheets imageSheets, Actions actions);

		std::vector<std::string> actionNames() const;
		const std::vector<AnimationFrame>& frames(const std::string& actionName) const;

	private:
		ImageSheets mImageSheets;
		Actions mActions;
	};

} // namespace
