// ==================================================================================
// = NAS2D
// = Copyright © 2008 - 2018 New Age Software
// ==================================================================================
// = NAS2D is distributed under the terms of the zlib license. You are free to copy,
// = modify and distribute the software under the terms of the zlib license.
// = 
// = Acknowledgement of your use of NAS2D is appriciated but is not required.
// ==================================================================================

#include "NAS2D/Configuration.h"
#include "NAS2D/EventHandler.h"
#include "NAS2D/Exception.h"
#include "NAS2D/Filesystem.h"
#include "NAS2D/Game.h"
#include "NAS2D/Utility.h"

#include "NAS2D/Mixer/Mixer_SDL.h"
#include "NAS2D/Renderer/OGL_Renderer.h"

#include <SDL2/SDL.h>

#include <iostream>


using namespace NAS2D;

/**
 * Starts the engine by initializing all of the engine sub components and
 * setting up any default values should they be needed.
 *
 * \param	title		The title that should be used for the game window.
 * \param appName		The name of the app (used to create an app data write path)
 * \param organiationName		The name of the organization (used to create an app data write path)
 * \param	argv_0		argv[0] from main()'s argument list. Necessary for Linux compatibility.
 * \param	configPath	Path to the Config file. Defaults to 'config.xml'.
 * \param	dataPath	Intitial data path. Defaults to 'data'.
 */
Game::Game(const std::string& title, const std::string& appName, const std::string& organizationName, const std::string& argv_0, const std::string& configPath, const std::string& dataPath)
{
	std::cout << "NAS2D BUILD: " << __DATE__ << " | " << __TIME__ << std::endl;
	std::cout << "NAS2D VERSION: " << NAS2D::versionString() << std::endl << std::endl;

	std::cout << "Initializing subsystems..." << std::endl << std::endl;

	Utility<Filesystem>::get().init(argv_0, appName, organizationName, dataPath);

	Configuration& cf = Utility<Configuration>::get();
	cf.load(configPath);

	try
	{
		Utility<Mixer>::init<Mixer_SDL>();
	}
	catch (std::exception& e)
	{
		std::cout << "Unable to create SDL Audio Mixer: " << e.what() << ". Setting NULL driver." << std::endl;
		Utility<Mixer>::init();
	}
	catch (...)
	{
		throw std::runtime_error("Unexpected exception occured while creating a Mixer.");
	}

	std::cout << "Initializing Event Handler... ";
	Utility<EventHandler>::get();
	std::cout << "done." << std::endl << std::endl;

	Utility<Renderer>::init<OGL_Renderer>(title);

	std::cout << std::endl << "Subsystems initialized." << std::endl << std::endl;
	std::cout << "===================================" << std::endl << std::endl;
}


/**
 * Stops all subsystems and cleans everything up.
 */
Game::~Game()
{
	std::cout << std::endl << "===================================" << std::endl << std::endl;
	std::cout << "Shutting down..." << std::endl;

	// Destroy all of our various components in reverse order.
	Utility<Renderer>::clear();
	Utility<EventHandler>::clear();
	Utility<Mixer>::clear();
	Utility<Configuration>::clear();
	Utility<Filesystem>::clear();

	std::cout << std::endl << "Game object has been terminated." << std::endl;

	// Shut down all SDL subsystems.
	SDL_Quit();
}


/**
 * Adds an additional directory or archive to the search path
 * of the Filesystem.
 * 
 * \param path	Path to add to the search path.
 */
void Game::mount(const std::string& path)
{
	Utility<Filesystem>::get().mount(path);
}


/**
 * Primes the EventHandler and StateManager and enters the main game loop.
 *
 * \param state	A pointer to a State object.
 * 
 * \warning	The State object becomes owned by the StateManager. Do not delete
 *			the State.
 */
void Game::go(State *state)
{
	std::cout << "** GAME STATE START **" << std::endl << std::endl;

	StateManager stateManager;

	stateManager.setState(state);

	// Game Loop
	while (stateManager.update())
	{
		Utility<Renderer>::get().update();
	}
}
