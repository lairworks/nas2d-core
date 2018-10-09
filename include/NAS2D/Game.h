// ==================================================================================
// = NAS2D
// = Copyright © 2008 - 2018 New Age Software
// ==================================================================================
// = NAS2D is distributed under the terms of the zlib license. You are free to copy,
// = modify and distribute the software under the terms of the zlib license.
// = 
// = Acknowledgement of your use of NAS2D is appriciated but is not required.
// ==================================================================================

#pragma once

#include "StateManager.h"

#include <string>

namespace NAS2D {

/**
 * \class Game
 * \brief	A simple way to start a NAS2D application.
 *
 * The Game class is a simple way to quickly delve right into developing a NAS2D
 * application without having to worry about initializing the core utilities. It
 * also provides a method of mounting directories and/or archives to the file
 * system and provides a simple entry point that handles the main application
 * loop.
 * 
 * Most NAS2D applications that use the Game object will look something like this:
 * 
 * \code{.cpp}
 * #include "NAS2D/NAS2D.h"
 * #include "MyState.h"
 * 
 * #include <iostream>
 * 
 * int main(int argc, char *argv[])
 * {
 *	try
 *	{
 *		Game game("My NAS2D Application", argv[0]);
 *		game.mount("gfx.zip");
 *		game.go(new MyState());
 *	}
 *	catch (const std::exception& e)
 *	{
 *		std::cout << "Error: " << e.what() << std::endl;
 *	}
 *	catch (...)
 *	{
 *	}
 *	return 0;
 * }
 * \endcode
 */
class Game
{
public:
	Game(const std::string& appTitle, const std::string& argv_0, const std::string& configPath = "config.xml", const std::string& dataPath = "data");
	~Game();

	void mount(const std::string& path);

	void go(State *state);
};

} // namespace
