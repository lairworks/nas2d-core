#pragma once

#include "Image.h"
#include "../Renderer/Vector.h"
#include "../Renderer/Rectangle.h"

#include <map>
#include <vector>
#include <string>


namespace NAS2D {

class AnimationSet
{
public:
	struct Frame
	{
		const Image& image;
		Rectangle<int> bounds;
		Vector<int> anchorOffset;
		unsigned int frameDelay;
	};

	AnimationSet(std::string fileName, std::map<std::string, Image> imageSheets, std::map<std::string, std::vector<Frame>> actions);

	std::vector<std::string> actionNames() const;
	const std::vector<Frame>& frames(const std::string& actionName) const;

private:
	std::string mFileName;
	std::map<std::string, Image> mImageSheets;
	std::map<std::string, std::vector<Frame>> mActions;
};

} // namespace
