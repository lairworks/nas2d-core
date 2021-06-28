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

#include <iostream>
#include <algorithm>
#include <utility>

using namespace NAS2D;


Configuration::Configuration(std::map<std::string, Dictionary> defaults) :
	mDefaults{std::move(defaults)},
	mSettings{mDefaults}
{
}

void Configuration::loadData(const std::string& fileData)
{
	mLoadedSettings = parseXmlFileData<decltype(mLoadedSettings)>(fileData, "configuration");
	mSettings = mergeByKey(mDefaults, mLoadedSettings);
}

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
			auto xmlFile = Utility<Filesystem>::get().open(filePath);
			loadData(xmlFile.raw_bytes());
			std::cout << "done." << std::endl;
		}
		catch (const std::runtime_error& e) {
			std::cout << "unable to process '" << filePath << "'. Using default options. Error: " << e.what() << std::endl;
		}
	}
}

std::string Configuration::saveData() const
{
	return formatXmlData(mSettings, "configuration", "Automatically generated Configuration file.");
}

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
