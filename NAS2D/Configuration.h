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
#include <string>
#include <map>


namespace NAS2D {

class Configuration
{
public:
	Configuration() = default;
	Configuration(std::map<std::string, Dictionary> defaults);
	Configuration(const Configuration&) = delete;
	Configuration& operator=(const Configuration&) = delete;
	Configuration(Configuration&&) = delete;
	Configuration& operator=(Configuration&&) = delete;
	~Configuration() = default;

	const Dictionary& operator[](const std::string& key) const { return mSettings.at(key); }
	Dictionary& operator[](const std::string& key) { return mSettings.at(key); }

	void loadData(const std::string& fileData);
	void load(const std::string& filePath);
	std::string saveData() const;
	void save(const std::string& filePath) const;

protected:
private:
	const std::map<std::string, Dictionary> mDefaults{};
	std::map<std::string, Dictionary> mLoadedSettings{};
	std::map<std::string, Dictionary> mSettings{};
};

} // namespace
