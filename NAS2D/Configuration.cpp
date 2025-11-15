// ==================================================================================
// = NAS2D
// = Copyright © 2008 - 2020 New Age Software
// ==================================================================================
// = NAS2D is distributed under the terms of the zlib license. You are free to copy,
// = modify and distribute the software under the terms of the zlib license.
// =
// = Acknowledgment of your use of NAS2D is appreciated but is not required.
// ==================================================================================

#include "Configuration.h"
#include "ContainerUtils.h"
#include "ParserHelper.h"
#include "Filesystem.h"
#include "Utility.h"

#include "Xml/XmlDocument.h"
#include "Xml/XmlMemoryBuffer.h"
#include "Xml/XmlElement.h"

#include <algorithm>
#include <utility>
#include <stdexcept>


using namespace NAS2D;


namespace
{
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

	std::string formatXmlData(const std::map<std::string, Dictionary>& data, const std::string& tagName)
	{
		Xml::XmlDocument doc;

		doc.linkEndChild(dictionaryMapToElement(tagName, data));

		// Write out the XML file.
		Xml::XmlMemoryBuffer buff;
		doc.accept(&buff);

		return buff.buffer();
	}
}


Configuration::Configuration(std::map<std::string, Dictionary> defaults) :
	mDefaults{std::move(defaults)},
	mSettings{mDefaults}
{
}


const Dictionary& Configuration::operator[](const std::string& key) const
{
	return mSettings.at(key);
}


Dictionary& Configuration::operator[](const std::string& key)
{
	return mSettings.at(key);
}


bool Configuration::anyLoadedConfig() const
{
	return !mLoadedSettings.empty();
}


bool Configuration::anyNonDefaultConfig() const
{
	return mSettings != mDefaults;
}


/**
 * Parses XML configuration data.
 */
void Configuration::loadData(const std::string& fileData)
{
	// Start parsing through the Config.xml file.
	mLoadedSettings = parseXmlFileData<decltype(mLoadedSettings)>(fileData, "configuration");
	mSettings = mergeByKey(mDefaults, mLoadedSettings);
}


/**
 * Loads a configuration file.
 */
void Configuration::load(const std::string& filePath)
{
	const auto& filesystem = Utility<Filesystem>::get();
	if (filesystem.exists(VirtualPath{filePath}))
	{
		try
		{
			// Read in the Config File.
			auto xmlData = filesystem.readFile(VirtualPath{filePath});
			loadData(xmlData);
		}
		catch (const std::runtime_error& e)
		{
			throw std::runtime_error("Error loading configuration file: '" + filePath + "' : " + e.what());
		}
	}
}


/**
 * Saves the Configuration to an XML string
 */
std::string Configuration::saveData() const
{
	return formatXmlData(mSettings, "configuration");
}


/**
 * Saves the Configuration to an XML file.
 */
void Configuration::save(const std::string& filePath) const
{
	try
	{
		Utility<Filesystem>::get().writeFile(VirtualPath{filePath}, saveData());
	}
	catch (const std::runtime_error& e)
	{
		throw std::runtime_error("Error saving configuration file: '" + filePath + "' : " + e.what());
	}
}
