// ==================================================================================
// = NAS2D
// = Copyright © 2008 - 2020 New Age Software
// ==================================================================================
// = NAS2D is distributed under the terms of the zlib license. You are free to copy,
// = modify and distribute the software under the terms of the zlib license.
// =
// = Acknowledgment of your use of NAS2D is appreciated but is not required.
// ==================================================================================


#include "ParserHelper.h"
#include "StringUtils.h"
#include "ContainerUtils.h"

#include "Xml/XmlNode.h"
#include "Xml/XmlElement.h"

#include <stdexcept>


namespace NAS2D
{
	void reportMissingOrUnexpected(const std::vector<std::string>& missing, const std::vector<std::string>& unexpected)
	{
		if (!missing.empty() || !unexpected.empty())
		{
			const auto missingString = !missing.empty() ? "Missing names: {" + join(missing, ", ") + "}" : "";
			const auto unexpectedString = !unexpected.empty() ? "Unexpected names: {" + join(unexpected, ", ") + "}" : "";
			const auto joinString = (!missingString.empty() && !unexpectedString.empty()) ? "\n" : "";
			throw std::runtime_error(missingString + joinString + unexpectedString);
		}
	}

	void reportMissingOrUnexpected(const std::vector<std::string>& names, const std::vector<std::string>& required, const std::vector<std::string>& optional)
	{
		const auto missing = missingValues(names, required);
		const auto unexpected = unexpectedValues(names, required, optional);
		reportMissingOrUnexpected(missing, unexpected);
	}


	Dictionary attributesToDictionary(const Xml::XmlElement& element)
	{
		Dictionary dictionary;
		for (const auto* attribute = element.firstAttribute(); attribute; attribute = attribute->next())
		{
			dictionary.set(attribute->name(), attribute->value());
		}
		return dictionary;
	}


	std::map<std::string, Dictionary> subTagsToDictionaryMap(const Xml::XmlElement& element)
	{
		std::map<std::string, Dictionary> sections;
		for (auto childElement = element.firstChildElement(); childElement; childElement = childElement->nextSiblingElement())
		{
			if (childElement->type() != Xml::XmlNode::NodeType::XML_COMMENT)
			{
				sections[childElement->value()] = attributesToDictionary(*childElement);
			}
		}
		return sections;
	}


	Xml::XmlElement* dictionaryToAttributes(const std::string& tagName, const Dictionary& dictionary)
	{
		auto* element = new Xml::XmlElement(tagName);
		for (const auto& key : dictionary.keys())
		{
			element->attribute(key, dictionary.get(key));
		}
		return element;
	}

	Xml::XmlElement* dictionaryMapToElement(const std::string& tagName, const std::map<std::string, Dictionary>& sections)
	{
		auto* element = new Xml::XmlElement(tagName);
		for (const auto& [key, dictionary] : sections)
		{
			element->linkEndChild(dictionaryToAttributes(key, dictionary));
		}
		return element;
	}
}
