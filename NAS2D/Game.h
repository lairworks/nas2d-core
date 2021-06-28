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

#include <string>


namespace NAS2D {

class State;

class Game
{
public:
	Game(const std::string& title, const std::string& appName, const std::string& organizationName, const std::string& argv_0, const std::string& configPath = "config.xml", const std::string& dataPath = "data");
	~Game();

	void mount(const std::string& path);

	void go(State* state);
};

} // namespace
