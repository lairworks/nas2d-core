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

#include "Dictionary.h"
#include "Xml/XmlDocument.h"
#include "Xml/XmlMemoryBuffer.h"
#include "Xml/XmlComment.h"
#include "Xml/XmlElement.h"

#include <string>
#include <vector>
#include <map>
#include <stdexcept>


namespace NAS2D
{
	void reportMissingOrUnexpected(const std::vector<std::string>& missing, const std::vector<std::string>& unexpected);
	void reportMissingOrUnexpected(const std::vector<std::string>& names, const std::vector<std::string>& required, const std::vector<std::string>& optional);

	Dictionary attributesToDictionary(const Xml::XmlElement& element);
	std::map<std::string, Dictionary> subTagsToDictionaryMap(const Xml::XmlElement& element);

	Xml::XmlElement* dictionaryToAttributes(const std::string& tagName, const Dictionary& dictionary);
	Xml::XmlElement* dictionaryMapToElement(const std::string& tagName, const std::map<std::string, Dictionary>& sections);

	template <typename ReturnType, ReturnType fileFormatParser(const Xml::XmlElement& element) = subTagsToDictionaryMap>
	auto parseXmlFileData(const std::string& xmlString, const std::string& sectionName = "", const std::string& requiredVersion = "")
	{
		Xml::XmlDocument xmlDocument;
		xmlDocument.parse(xmlString.c_str());

		if (xmlDocument.error())
		{
			throw std::runtime_error("Error parsing XML file on (Row " + std::to_string(xmlDocument.errorRow()) + ", Column " + std::to_string(xmlDocument.errorCol()) + "): " + xmlDocument.errorDesc());
		}

		auto* root = !sectionName.empty() ? xmlDocument.firstChildElement(sectionName) : xmlDocument.rootElement();
		if (!root)
		{
			throw std::runtime_error("XML file does not contain tag: " + (!sectionName.empty() ? sectionName : "(root element)"));
		}

		if (!requiredVersion.empty())
		{
			const auto actualVersion = root->attribute("version");
			if (actualVersion != requiredVersion)
			{
				throw std::runtime_error("Version mismatch. Expected: " + std::string{requiredVersion} + " Actual: " + actualVersion);
			}
		}

		return fileFormatParser(*root);
	}

	template <typename Data, Xml::XmlElement* dataFormatter(const std::string& tagName, const Data& data) = dictionaryMapToElement>
	std::string formatXmlData(const Data& data, const std::string& tagName, const std::string& comment = "")
	{
		Xml::XmlDocument doc;

		if (!comment.empty())
		{
			doc.linkEndChild(new Xml::XmlComment(comment));
		}
		doc.linkEndChild(dataFormatter(tagName, data));

		// Write out the XML file.
		Xml::XmlMemoryBuffer buff;
		doc.accept(&buff);

		return buff.buffer();
	}
}
