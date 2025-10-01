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

#include "AnimationSequence.h"

#include <string_view>
#include <string>
#include <vector>
#include <map>


namespace NAS2D
{
	class Image;
	template <typename Resource, typename... Params> class ResourceCache;


	class AnimationSet
	{
	public:
		using Actions = std::map<std::string, AnimationSequence>;


		explicit AnimationSet(std::string_view fileName);
		AnimationSet(std::string_view fileName, ResourceCache<Image, std::string>& imageCache);
		AnimationSet(Actions actions);

		std::vector<std::string> actionNames() const;
		const AnimationSequence& frames(const std::string& actionName) const;

	private:
		Actions mActions;
	};

} // namespace
