
#include "ParserHelper.h"
#include "StringUtils.h"
#include "ContainerUtils.h"


namespace NAS2D {
	void reportMissingOrUnexpected(const std::vector<std::string>& missing, const std::vector<std::string>& unexpected)
	{
		if (!missing.empty() || !unexpected.empty())
		{
			const auto missingString = !missing.empty() ? "Missing names: {" + join(missing, ", ") +"}" : "";
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
}
