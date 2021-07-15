// ==================================================================================
// = NAS2D
// = Copyright © 2008 - 2020 New Age Software
// ==================================================================================
// = NAS2D is distributed under the terms of the zlib license. You are free to copy,
// = modify and distribute the software under the terms of the zlib license.
// =
// = Acknowledgment of your use of NAS2D is appreciated but is not required.
// ==================================================================================

#include "Game.h"
#include "StateManager.h"
#include "Configuration.h"
#include "EventHandler.h"
#include "Filesystem.h"
#include "Utility.h"
#include "Version.h"

#include "Mixer/MixerNull.h"
#include "Mixer/MixerSDL.h"
#include "Renderer/RendererOpenGL.h"
#include "Renderer/RendererNull.h"

#include <SDL2/SDL.h>

#include <iostream>
#include <stdexcept>
#include <string>


using namespace NAS2D;

/**
 * Starts the engine by initializing all of the engine sub components and
 * setting up any default values should they be needed.
 *
 * \param	title		The title that should be used for the game window.
 * \param appName		The name of the app (used to create an app data write path)
 * \param organizationName		The name of the organization (used to create an app data write path)
 * \param	argv_0		argv[0] from main()'s argument list. Necessary for Linux compatibility.
 * \param	configPath	Path to the Config file. Defaults to 'config.xml'.
 * \param	dataPath	Intitial data path. Defaults to 'data'.
 */
Game::Game(const std::string& title, const std::string& appName, const std::string& organizationName, const std::string& argv_0, const std::string& configPath, const std::string& dataPath)
{
	std::cout << "NAS2D BUILD: " << __DATE__ << " | " << __TIME__ << '\n';
	std::cout << "NAS2D VERSION: " << versionString() << "\n\n";
	std::cout << "Initializing subsystems...\n\n";
	std::cout.flush();

	SDL_Init(0);

	auto& fs = Utility<Filesystem>::init<Filesystem>(argv_0, appName, organizationName);
	fs.mountSoftFail(dataPath);
	fs.mountSoftFail(fs.basePath() + dataPath);
	fs.mountReadWrite(fs.prefPath());

	Configuration& cf = Utility<Configuration>::init(
		std::map<std::string, NAS2D::Dictionary>{
			{
				"graphics",
				{{
					{"screenwidth", 1000},
					{"screenheight", 700},
					{"bitdepth", 32},
					{"fullscreen", false},
					{"vsync", true}
				}}
			},
			{
				"audio",
				{{
					{"mixer", "SDL"},
					{"musicvolume", 100},
					{"sfxvolume", 128},
					{"channels", 2},
					{"mixrate", 22050},
					{"bufferlength", 1024}
				}}
			}
		}
	);
	cf.load(configPath);

	try
	{
		Utility<Mixer>::init<MixerSDL>();
	}
	catch (std::exception& e)
	{
		std::cout << "Unable to create SDL Audio Mixer: " << e.what() << ". Setting NULL driver." << std::endl;
		Utility<Mixer>::init<MixerNull>();
	}
	catch (...)
	{
		throw std::runtime_error("Unexpected exception occured while creating a Mixer.");
	}

	std::cout << "Initializing Event Handler... ";
	Utility<EventHandler>::get();
	std::cout << "done.\n\n";
	std::cout.flush();

	try
	{
		Utility<Renderer>::init<RendererOpenGL>(title);
	}
	catch (const std::exception& e)
	{
		std::cerr << "Unable to create OGL Renderer: " << e.what() << ". Setting NULL driver." << std::endl;
		Utility<Renderer>::init<RendererNull>();
	}


	std::cout << "\nSubsystems initialized.\n\n";
	std::cout << "===================================\n\n";
	std::cout.flush();
}


/**
 * Stops all subsystems and cleans everything up.
 */
Game::~Game()
{
	std::cout << "\n===================================\n\n";
	std::cout << "Shutting down..." << std::endl;

	// Destroy all of our various components in reverse order.
	Utility<Renderer>::clear();
	Utility<EventHandler>::clear();
	Utility<Mixer>::clear();
	Utility<Configuration>::clear();
	Utility<Filesystem>::clear();

	// Shut down all SDL subsystems.
	SDL_Quit();

	std::cout << "\nGame object has been terminated." << std::endl;
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
void Game::go(State* state)
{
	std::cout << "** GAME STATE START **\n\n";
	std::cout.flush();

	StateManager stateManager;

	stateManager.setState(state);

	// Game Loop
	while (stateManager.update())
	{
		Utility<Renderer>::get().update();
	}
}
