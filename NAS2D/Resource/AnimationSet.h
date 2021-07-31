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
#include "../Renderer/Vector.h"
#include "../Renderer/Rectangle.h"
#include "../Xml/Xml.h"

#include <map>
#include <vector>
#include <string>


namespace NAS2D
{

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

		explicit AnimationSet(std::string fileName);
		explicit AnimationSet(const Xml::XmlElement& element);
		AnimationSet(std::string fileName, std::map<std::string, std::string> imageSheetMap, std::map<std::string, std::vector<Frame>> actions);

		std::vector<std::string> actionNames() const;
		const std::vector<Frame>& frames(const std::string& actionName) const;

	private:
		void loadFromXml(const Xml::XmlElement& element);

		std::string mFileName{};
		std::map<std::string, std::string> mImageSheetMap{};
		std::map<std::string, std::vector<Frame>> mActions{};
	};

} // namespace
