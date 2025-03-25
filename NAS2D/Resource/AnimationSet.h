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

#include "Image.h"
#include "../Math/Vector.h"
#include "../Math/Rectangle.h"

#include <map>
#include <vector>
#include <string>


namespace NAS2D
{
	template <typename Resource, typename... Params>
	class ResourceCache;


	class AnimationSet
	{
	public:
		struct Frame
		{
			const Image& image;
			Rectangle<int> bounds;
			Vector<int> anchorOffset;
			unsigned int frameDelay;

			bool isStopFrame() const;
		};

		using ImageSheets = std::map<std::string, std::string>;
		using Actions = std::map<std::string, std::vector<Frame>>;


		explicit AnimationSet(std::string fileName);
		AnimationSet(std::string fileName, ResourceCache<Image, std::string>& imageCache);
		AnimationSet(ImageSheets imageSheets, Actions actions);

		std::vector<std::string> actionNames() const;
		const std::vector<Frame>& frames(const std::string& actionName) const;

	private:
		ImageSheets mImageSheets;
		Actions mActions;
	};

} // namespace
