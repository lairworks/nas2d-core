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

#include "ResourceCache.h"
#include "../Utility.h"
#include "../Filesystem.h"
#include "../ContainerUtils.h"
#include "../StringUtils.h"
#include "../ParserHelper.h"
#include "../Version.h"
#include "../Xml/Xml.h"


using namespace NAS2D;


namespace
{
	constexpr std::string_view SPRITE_VERSION{"0.99"};

	using ImageCache = ResourceCache<Image, std::string>;
	ImageCache animationImageCache;


	// Adds a row tag to the end of messages.
	std::string endTag(int row)
	{
		return " (Row: " + std::to_string(row) + ")";
	}
}


bool AnimationSet::Frame::isStopFrame() const
{
	// We want to redefine the sentinel value for stop frames from -1 to 0
	// Neither value makes sense as a delay, though the field is unsigned
	// Using 0 would also help simplify some code, so makes the most sense
	// Temporarily handle -1 for backwards compatibility during transition
	return (frameDelay == 0) || (frameDelay == unsigned(-1));
}


AnimationSet::AnimationSet(std::string fileName) :
	mFileName{fileName}
{
	try {
		const auto& filesystem = Utility<Filesystem>::get();
		const auto basePath = filesystem.parentPath(mFileName);

		Xml::XmlDocument xmlDoc{};
		xmlDoc.parse(filesystem.read(mFileName).c_str());

		if(xmlDoc.error())
		{
			throw std::runtime_error("Sprite file has malformed XML: Row: " + std::to_string(xmlDoc.errorRow()) + " Column: " + std::to_string(xmlDoc.errorCol()) + " : " + xmlDoc.errorDesc());
		}
		loadFromXml(*xmlDoc.rootElement());
	} catch(...) {
		throw;
	}
}

AnimationSet::AnimationSet(const Xml::XmlElement& element)
{
	try {
		loadFromXml(element);
	}
	catch(...)
	{
		throw;
	}
}

AnimationSet::AnimationSet(std::string fileName, std::map<std::string, std::string> imageSheetMap, std::map<std::string, std::vector<Frame>> actions) :
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

void AnimationSet::loadFromXml(const Xml::XmlElement& element)
{
	try
	{
		if (const auto* xml_sprite = element.firstChildElement("sprite"); xml_sprite == nullptr)
		{
			throw std::runtime_error("Sprite file does not contain required <sprite> tag");
		}
		else
		{
			if(const auto version = element.attribute("version"); version.empty())
			{
				throw std::runtime_error("Sprite file's root element does not specify a version");
			} else
			{
				if(version != SPRITE_VERSION)
				{
					throw std::runtime_error("Sprite version mismatch. Expected: " + std::string{SPRITE_VERSION} + " Actual: " + versionString());
				}
			}
		}
		mImageSheetMap = [&]() //IIIL
		{
			std::map<std::string, std::string> imageSheetMap;

			for (const auto* node = element.firstChildElement("imagesheet"); node != nullptr; node = node->nextSiblingElement("imagesheet"))
			{
				const auto dictionary = attributesToDictionary(*node);
				const auto id = dictionary.get("id");
				const auto src = dictionary.get("src");

				if (id.empty())
				{
					throw std::runtime_error("Sprite imagesheet definition has `id` of length zero: " + endTag(node->row()));
				}

				if (src.empty())
				{
					throw std::runtime_error("Sprite imagesheet definition has `src` of length zero: " + endTag(node->row()));
				}

				if (imageSheetMap.find(id) != imageSheetMap.end())
				{
					throw std::runtime_error("Sprite image sheet redefinition: id: '" + id + "' " + endTag(node->row()));
				}

				const auto& filesystem = Utility<Filesystem>::get();
				const auto basePath = filesystem.parentPath(mFileName);

				const auto path = basePath + src;
				imageSheetMap.try_emplace(id, path);
				animationImageCache.load(path);
			}
			return imageSheetMap;
		}(); //IIIL

		mActions = [&]() //IIIL
		{
			std::map<std::string, std::vector<AnimationSet::Frame>> actions;

			for (const auto* action = element.firstChildElement("action"); action != nullptr; action = action->nextSiblingElement("action"))
			{
				const auto dictionary = attributesToDictionary(*action);
				const auto actionName = dictionary.get("name");

				if (actionName.empty())
				{
					throw std::runtime_error("Sprite Action definition has 'name' of length zero: " + endTag(action->row()));
				}
				if (actions.find(actionName) != actions.end())
				{
					throw std::runtime_error("Sprite Action redefinition: '" + actionName + "' " + endTag(action->row()));
				}

				actions[actionName] = [&]() //IIIL
				{
					std::vector<AnimationSet::Frame> frameList;

					for (const auto* frame = element.firstChildElement("frame"); frame; frame = frame->nextSiblingElement("frame"))
					{
						const auto currentRow = frame->row();

						const auto dictionary = attributesToDictionary(*frame);
						reportMissingOrUnexpected(dictionary.keys(), {"sheetid", "x", "y", "width", "height", "anchorx", "anchory"}, {"delay"});

						const auto sheetId = dictionary.get("sheetid");
						const auto delay = dictionary.get<unsigned int>("delay", 0);
						const auto x = dictionary.get<int>("x");
						const auto y = dictionary.get<int>("y");
						const auto width = dictionary.get<int>("width");
						const auto height = dictionary.get<int>("height");
						const auto anchorx = dictionary.get<int>("anchorx");
						const auto anchory = dictionary.get<int>("anchory");

						if (sheetId.empty())
						{
							throw std::runtime_error("Sprite Frame definition has 'sheetid' of length zero: " + endTag(currentRow));
						}
						if (const auto iterator = mImageSheetMap.find(sheetId); iterator != mImageSheetMap.end())
						{
							const auto& image = animationImageCache.load(iterator->second);
							// X-Coordinate
							if (x < 0 || x > image.size().x)
							{
								throw std::runtime_error("Sprite frame attribute 'x' is out of bounds: " + endTag(currentRow));
							}
							// Y-Coordinate
							if (y < 0 || y > image.size().y)
							{
								throw std::runtime_error("Sprite frame attribute 'y' is out of bounds: " + endTag(currentRow));
							}
							// Width
							if (width <= 0 || width > image.size().x - x)
							{
								throw std::runtime_error("Sprite frame attribute 'width' is out of bounds: " + endTag(currentRow));
							}
							// Height
							if (height <= 0 || height > image.size().y - y)
							{
								throw std::runtime_error("Sprite frame attribute 'height' is out of bounds: " + endTag(currentRow));
							}

							const auto bounds = Rectangle<int>::Create(Point<int>{x, y}, Vector{width, height});
							const auto anchorOffset = Vector{anchorx, anchory};
							frameList.emplace_back(AnimationSet::Frame{image, bounds, anchorOffset, delay});
						}
						else
						{
							throw std::runtime_error("Sprite Frame definition references undefined imagesheet: '" + sheetId + "' " + endTag(currentRow));
						}
					}

					return frameList;
				}(); //IIIL

				if (actions[actionName].empty())
				{
					throw std::runtime_error("Sprite Action contains no valid frames: " + actionName);
				}
			}
			return actions;
		}(); //IIIL
	}
	catch (...)
	{
		throw;
	}
}
