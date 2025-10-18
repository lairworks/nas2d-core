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

#include <algorithm>
#include <utility>
#include <stdexcept>

using namespace NAS2D;


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
	return formatXmlData(mSettings, "configuration", "Automatically generated Configuration file.");
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
