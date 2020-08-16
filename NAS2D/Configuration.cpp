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
#include "Dictionary.h"
#include "ContainerUtils.h"
#include "Filesystem.h"
#include "Utility.h"
#include "Xml/Xml.h"

#include <iostream>
#include <algorithm>
#include <utility>

using namespace NAS2D;


namespace {
	std::map<std::string, Dictionary> merge(const std::map<std::string, Dictionary>& defaults, const std::map<std::string, Dictionary>& priorityValues)
	{
		std::map<std::string, Dictionary> results = defaults;
		for (const auto& [key, dictionary] : priorityValues)
		{
			results[key] += dictionary;
		}
		return results;
	}


	void ReportProblemNames(
		const std::vector<std::string>& names,
		const std::vector<std::string>& required,
		const std::vector<std::string>& optional = {}
	) {
		using namespace ContainerOperators;

		const auto expected = required + optional;

		const auto missing = names - required;
		const auto unexpected = names - expected;

		if (!missing.empty() || !unexpected.empty())
		{
			throw std::runtime_error("Missing required names: {" + join(missing, ", ") +"}, unexpected names: {" + join(unexpected, ", ") + "}");
		}
	}


	Dictionary ParseXmlElementAttributesToDictionary(const Xml::XmlElement& element)
	{
		Dictionary dictionary;

		for (const auto* attribute = element.firstAttribute(); attribute; attribute = attribute->next())
		{
			dictionary.set(attribute->name(), attribute->value());
		}

		return dictionary;
	}


	// This method should be scheduled for deprecation
	// It should remain for a short while to allow configuration options to be resaved
	// Resaving will convert data to a consistent format for all sections
	// We will no longer need to be able to parse the special format of the "options" section
	Dictionary ParseOptionsToDictionary(const Xml::XmlElement& element)
	{
		Dictionary dictionary;

		for (const auto* setting = element.firstChildElement(); setting; setting = setting->nextSiblingElement())
		{
			if (setting->value() != "option")
			{
				throw std::runtime_error("Unexpected tag found in configuration on row: " + std::to_string(setting->row()) + "  <" + setting->value() + ">");
			}

			const auto optionDictionary = ParseXmlElementAttributesToDictionary(*setting);
			ReportProblemNames(optionDictionary.keys(), {"name", "value"});

			const auto name = optionDictionary.get("name");
			const auto value = optionDictionary.get("value");

			if (name.empty() || value.empty())
			{
				throw std::runtime_error("Invalid name/value pair in <option> tag in configuration file on row: " + std::to_string(setting->row()));
			}

			dictionary.set(name, value);
		}

		return dictionary;
	}


	std::map<std::string, Dictionary> ParseXmlSections(const Xml::XmlElement& element)
	{
		std::map<std::string, Dictionary> sections;

		for (auto childElement = element.firstChildElement(); childElement; childElement = childElement->nextSiblingElement())
		{
			if (childElement->type() == Xml::XmlNode::NodeType::XML_COMMENT) { continue; } // Ignore comments

			sections[childElement->value()] = ParseXmlElementAttributesToDictionary(*childElement) + ParseOptionsToDictionary(*childElement);
		}

		return sections;
	}


	std::map<std::string, Dictionary> ParseXmlSections(const std::string& xmlString, const std::string& sectionName)
	{
		Xml::XmlDocument xmlDocument;
		xmlDocument.parse(xmlString.c_str());

		if (xmlDocument.error())
		{
			throw std::runtime_error("Error parsing XML file on (Row " + std::to_string(xmlDocument.errorRow()) + ", Column " + std::to_string(xmlDocument.errorCol()) + "): " + xmlDocument.errorDesc());
		}

		auto* root = xmlDocument.firstChildElement(sectionName);
		if (!root)
		{
			throw std::runtime_error("XML file does not contain tag: " + sectionName);
		}

		return ParseXmlSections(*root);
	}


	Xml::XmlElement* DictionaryToXmlElementAttributes(const std::string& tagName, const Dictionary& dictionary)
	{
		auto* element = new Xml::XmlElement(tagName.c_str());

		for (const auto& key : dictionary.keys())
		{
			element->attribute(key, dictionary.get(key));
		}

		return element;
	}


	Xml::XmlElement* SectionsToXmlElement(const std::string& tagName, const std::map<std::string, Dictionary>& sections)
	{
		auto* element = new Xml::XmlElement(tagName);

		for (const auto& [key, dictionary] : sections)
		{
			element->linkEndChild(DictionaryToXmlElementAttributes(key, dictionary));
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
	mLoadedSettings = ParseXmlSections(fileData, "configuration");
	mSettings = merge(mDefaults, mLoadedSettings);
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
			File xmlFile = Utility<Filesystem>::get().open(filePath);
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

	auto* root = SectionsToXmlElement("configuration", mSettings);
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
