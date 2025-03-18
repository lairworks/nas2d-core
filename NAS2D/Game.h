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


namespace NAS2D
{

	class State;


	/**
	 * A simple way to start a NAS2D application.
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
	 * #include "NAS2D.h"
	 * #include "MyState.h"
	 *
	 * int main(int argc, char* argv[])
	 * {
	 *	try
	 *	{
	 *		Game game("My NAS2D Application", "ApplicationName", "OrganizationName");
	 *		game.mount("gfx.zip");
	 *		game.go(new MyState());
	 *	}
	 *	catch (const std::exception& exception)
	 *	{
	 *		// Report error message in `exception.what()`
	 *		// ...
	 *	}
	 *	return 0;
	 * }
	 * \endcode
	 */
	class Game
	{
	public:
		Game(const std::string& title, const std::string& appName, const std::string& organizationName, const std::string& configPath = "config.xml", const std::string& dataPath = "data");
		~Game();

		void mount(const std::string& path);
		void mountFindFromBase(const std::string& path);

		void go(State* state);
	};

} // namespace
