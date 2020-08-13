// ==================================================================================
// = NAS2D
// = Copyright © 2008 - 2019 New Age Software
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


	Xml::XmlElement* DictionaryToXmlElementOptions(const std::string& tagName, const Dictionary& dictionary)
	{
		auto* element = new Xml::XmlElement(tagName.c_str());

		for (const auto& key : dictionary.keys())
		{
			auto* option = new Xml::XmlElement("option");
			option->attribute("name", key);
			option->attribute("value", dictionary.get(key));
			element->linkEndChild(option);
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
	ReportProblemNames(getKeys(mSettings), {"graphics", "audio", "options"});
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

	const auto& graphics = mSettings.at("graphics");
	const auto& audio = mSettings.at("audio");

	auto* root = SectionsToXmlElement("configuration", std::map<std::string, Dictionary>{{"graphics", graphics}, {"audio", audio}});
	root->linkEndChild(DictionaryToXmlElementOptions("options", mSettings.at("options")));
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


/**
 * Sets a program option.
 *
 * \param	option		Name of the option.
 * \param	value		Value of the option.
 * \param	overwrite	Overwrites an existing option's value. Default is \c true.
 *
 * \note	Option values are stored and read as strings. How the program
 *			interprets the data contained in value is up to the programmer.
 */
void Configuration::option(const std::string& option, const std::string& value, bool overwrite)
{
	if (!overwrite && mSettings.at("options").has(option))
	{
		return;
	}

	mSettings.at("options").set(option, value);
}


/**
 * Gets an option's value.
 *
 * \note	If the option was not defined when loading the configuration
 *			file, calling this function will create the option with an
 *			empty value.
 *
 * \note	Option values are stored and read as strings. How the program
 *			interprets the data contained in value is up to the programmer.
 */
std::string Configuration::option(const std::string& key)
{
	if (!mSettings.at("options").has(key))
	{
		mSettings.at("options").set(key, std::string{});
	}

	return mSettings.at("options").get(key);
}


/**
 * Erases an option.
 *
 * \param	option	Name of the option to erase.
 *
 * \note	It is safe to call this function if the named
 *			option has not been defined.
 */
void Configuration::deleteOption(const std::string& option)
{
	if (mSettings.at("options").has(option))
	{
		mSettings.at("options").erase(option);
	}
}
