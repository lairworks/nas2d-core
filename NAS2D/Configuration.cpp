// ==================================================================================
// = NAS2D
// = Copyright © 2008 - 2020 New Age Software
// ==================================================================================
// = NAS2D is distributed under the terms of the zlib license. You are free to copy,
// = modify and distribute the software under the terms of the zlib license.
// =
// = Acknowledgement of your use of NAS2D is appriciated but is not required.
// ==================================================================================

#include "Configuration.h"
#include "ContainerUtils.h"
#include "ParserHelper.h"
#include "Filesystem.h"
#include "Utility.h"
#include "Xml/Xml.h"

#include <iostream>
#include <algorithm>
#include <utility>

using namespace NAS2D;


namespace {
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


	template <auto fileFormatParser(const Xml::XmlElement& element) = subTagsToDictionaryMap>
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


	Xml::XmlElement* dictionaryToAttributes(const std::string& tagName, const Dictionary& dictionary)
	{
		auto* element = new Xml::XmlElement(tagName.c_str());
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


Configuration::Configuration(std::map<std::string, Dictionary> defaults) :
	mDefaults{std::move(defaults)},
	mSettings{mDefaults}
{
}


/**
 * Reads a given XML configuration file.
 *
 * \param fileData	Name of an XML Configuration file to be read.
 */
void Configuration::loadData(const std::string& fileData)
{
	// Start parsing through the Config.xml file.
	mLoadedSettings = parseXmlFileData(fileData, "configuration");
	mSettings = mergeByKey(mDefaults, mLoadedSettings);
}


/**
 * Loads a configuration file.
 *
 * \param	filePath	A string indicating the file to load and process for configuration.
 */
void Configuration::load(const std::string& filePath)
{
	std::cout << "Initializing Configuration... ";

	if (!Utility<Filesystem>::get().exists(filePath))
	{
		std::cout << "configuration file '" << filePath << "' does not exist. Using default options." << std::endl;
	}
	else
	{
		try {
			// Read in the Config File.
			auto xmlFile = Utility<Filesystem>::get().open(filePath);
			loadData(xmlFile.raw_bytes());
			std::cout << "done." << std::endl;
		}
		catch (const std::runtime_error& e) {
			std::cout << "unable to process '" << filePath << "'. Using default options. Error: " << e.what() << std::endl;
		}
	}
}


/**
 * Saves the Configuration to an XML string
 */
std::string Configuration::saveData() const
{
	Xml::XmlDocument doc;

	auto* comment = new Xml::XmlComment("Automatically generated Configuration file.");
	doc.linkEndChild(comment);

	auto* root = dictionaryMapToElement("configuration", mSettings);
	doc.linkEndChild(root);

	// Write out the XML file.
	Xml::XmlMemoryBuffer buff;
	doc.accept(&buff);

	return buff.buffer();
}


/**
 * Saves the Configuration to an XML file.
 */
void Configuration::save(const std::string& filePath) const
{
	try
	{
		Utility<Filesystem>::get().write(File(saveData(), filePath));
	}
	catch (const std::runtime_error& e)
	{
		throw std::runtime_error("Error saving configuration file: '" + filePath + "'  Error: " + e.what());
	}
}
