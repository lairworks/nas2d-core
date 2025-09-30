#include "AnimationFile.h"

#include "../Utility.h"
#include "../Filesystem.h"
#include "../ParserHelper.h"
#include "../Xml/XmlNode.h"
#include "../Xml/XmlElement.h"
#include "../Xml/XmlDocument.h"

#include <utility>
#include <stdexcept>


using namespace NAS2D;


namespace
{
	constexpr std::string_view SpriteVersion{"0.99"};


	[[noreturn]] void throwLoadError(std::string_view message, const Xml::XmlNode* node);
	AnimationFile readAnimationFile(std::string_view filePath);
	AnimationFileData readAnimationFileData(std::string_view fileData);
	std::vector<AnimationImageSheetReference> readImageSheetReferences(const Xml::XmlElement* element);
	std::vector<AnimationAction> readActions(const Xml::XmlElement* element);
	std::vector<AnimationFrameData> readFrames(const Xml::XmlElement* element);


	void throwLoadError(std::string_view message, const Xml::XmlNode* node)
	{
		throw std::runtime_error(std::string{message} + " (Line: " + std::to_string(node->row()) + ")");
	}


	AnimationFile readAnimationFile(std::string_view filePath)
	{
		auto& filesystem = Utility<Filesystem>::get();
		return {
			Filesystem::parentPath(filePath),
			readAnimationFileData(filesystem.readFile(VirtualPath{filePath})),
		};
	}


	AnimationFileData readAnimationFileData(std::string_view fileData)
	{
		Xml::XmlDocument xmlDoc;
		xmlDoc.parse(fileData.data());

		if (xmlDoc.error())
		{
			throw std::runtime_error("Malformed XML: Line: " + std::to_string(xmlDoc.errorRow()) + " Column: " + std::to_string(xmlDoc.errorCol()) + " : " + xmlDoc.errorDesc());
		}

		const auto* spriteElement = xmlDoc.firstChildElement("sprite");
		if (!spriteElement)
		{
			throw std::runtime_error("Missing required <sprite> tag");
		}

		const auto version = spriteElement->attribute("version");
		if (version.empty())
		{
			throwLoadError("No version specified", spriteElement);
		}
		if (version != SpriteVersion)
		{
			throwLoadError("Unsupported version: Expected: " + std::string{SpriteVersion} + " Actual: " + version, spriteElement);
		}

		return {
			readImageSheetReferences(spriteElement),
			readActions(spriteElement),
		};
	}


	std::vector<AnimationImageSheetReference> readImageSheetReferences(const Xml::XmlElement* element)
	{
		std::vector<AnimationImageSheetReference> imageSheetReferences;
		for (const auto* node = element->firstChildElement("imagesheet"); node; node = node->nextSiblingElement("imagesheet"))
		{
			const auto dictionary = attributesToDictionary(*node);
			const auto& imageSheetReference = imageSheetReferences.emplace_back(
				dictionary.get("id"),
				dictionary.get("src")
			);

			if (imageSheetReference.id.empty())
			{
				throwLoadError("Image sheet definition has `id` of length zero", node);
			}

			if (imageSheetReference.filePath.empty())
			{
				throwLoadError("Image sheet definition has `src` of length zero", node);
			}
		}
		return imageSheetReferences;
	}


	std::vector<AnimationAction> readActions(const Xml::XmlElement* element)
	{
		std::vector<AnimationAction> actionDefinitions;
		for (const auto* action = element->firstChildElement("action"); action; action = action->nextSiblingElement("action"))
		{
			const auto dictionary = attributesToDictionary(*action);
			const auto& actionDefinition = actionDefinitions.emplace_back(
				dictionary.get("name"),
				readFrames(action)
			);

			if (actionDefinition.name.empty())
			{
				throwLoadError("Action definition has 'name' of length zero", action);
			}
		}
		return actionDefinitions;
	}


	std::vector<AnimationFrameData> readFrames(const Xml::XmlElement* element)
	{
		std::vector<AnimationFrameData> frameDefinitions;
		for (const auto* frame = element->firstChildElement("frame"); frame; frame = frame->nextSiblingElement("frame"))
		{
			const auto dictionary = attributesToDictionary(*frame);
			reportMissingOrUnexpected(dictionary.keys(), {"sheetid", "x", "y", "width", "height", "anchorx", "anchory"}, {"delay"});

			const auto& animationFrameData = frameDefinitions.emplace_back(
				dictionary.get("sheetid"),
				Rectangle{
					Point{
						dictionary.get<int>("x"),
						dictionary.get<int>("y"),
					},
					Vector{
						dictionary.get<int>("width"),
						dictionary.get<int>("height"),
					}
				},
				Vector{
					dictionary.get<int>("anchorx"),
					dictionary.get<int>("anchory"),
				},
				Duration{dictionary.get<unsigned int>("delay", 0)}
			);

			if (animationFrameData.id.empty())
			{
				throwLoadError("Frame definition has 'sheetid' of length zero", frame);
			}
		}
		return frameDefinitions;
	}
}


AnimationFile::AnimationFile(std::string_view filePath) :
	AnimationFile{readAnimationFile(filePath)}
{
}


AnimationFile::AnimationFile(std::string basePath, AnimationFileData animationFileData) :
	mBasePath{std::move(basePath)},
	mImageSheetReferences{std::move(animationFileData.imageSheetReferences)},
	mActions{std::move(animationFileData.actions)}
{
}


AnimationFile::~AnimationFile()
{
}


const std::string& AnimationFile::basePath() const
{
	return mBasePath;
}


const std::vector<AnimationImageSheetReference>& AnimationFile::imageSheetReferences() const
{
	return mImageSheetReferences;
}


const std::vector<AnimationAction>& AnimationFile::actions() const
{
	return mActions;
}


namespace NAS2D
{
	AnimationFile loadAnimationFile(std::string_view filePath)
	{
		return readAnimationFile(filePath);
	}
}
