// ==================================================================================
// = NAS2D
// = Copyright © 2008 - 2013 New Age Software
// ==================================================================================
// = NAS2D is distributed under the terms of the zlib license. You are free to copy,
// = modify and distribute the software under the terms of the zlib license.
// = 
// = Acknowledgement of your use of NAS2D is appriciated but is not required.
// ==================================================================================

#ifndef _NAS_GAME_
#define _NAS_GAME_

#include "StateManager.h"

#include <string>

/**
 * \class Game
 * \brief The Engine Core.
 *
 * The Game class is what handles everything from initializing the video subsystems
 * to handling timing and logic issues. All core facilities are instantiated when a
 * Game object is created.
 */
class Game
{
public:
	Game(const std::string& appTitle, const std::string& argv_0, const std::string& configPath = "config.xml", const std::string& dataPath = "data");
	~Game();

	void mount(const std::string& path);

	void go(State *state);
};

#endif
