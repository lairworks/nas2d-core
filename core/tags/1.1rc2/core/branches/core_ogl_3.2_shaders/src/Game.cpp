// ==================================================================================
// = NAS2D
// = Copyright © 2008 - 2012 New Age Software
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

#include "NAS2D/Mixer/SDL_Mixer.h"
#include "NAS2D/Renderer/OGL_Renderer.h"
#include "NAS2D/Renderer/OGL_Renderer_3_2.h"


/**
 * Starts the engine by initializing all of the engine sub components and
 * setting up any default values should they be needed.
 *
 * \param	title		The title that should be used for the game window.
 * \param	argv_0		argv[0] from main()'s argument list. Necessary for Linux compatibility.
 * \param	configPath	Path to the Config file. Defaults to 'config.xml'.
 * \param	dataPath	Intitial data path. Defaults to 'data'.
 */
Game::Game(const std::string& title, const std::string& argv_0, const std::string& configPath, const std::string& dataPath)
{
	cout << "NAS2D BUILD: " << __DATE__ << " | " << __TIME__ << endl << endl;

	cout << "Initializing subsystems..." << endl << endl;	

	Utility<Filesystem>::get().init(argv_0, dataPath);

	Configuration& cf = Utility<Configuration>::get();
	cf.load(configPath);

	Utility<Mixer>::instantiateDerived(new SDL_Mixer());
	
	cout << "Initializing Event Handler... ";
	Utility<EventHandler>::get();
	cout << "done." << endl << endl;

	try
	{
		Utility<Renderer>::instantiateDerived(new OGL_Renderer_3_2(title));
	}
	catch(Exception e)
	{
		// Attempt to create a 3.2 context failed, so fall back on a 2.1 context.
		try
		{
			Utility<Renderer>::clean();
			Utility<Renderer>::instantiateDerived(new OGL_Renderer(title));
		}
		catch(Exception e)
		{
			throw Exception(0, "OpenGL Renderer", "Unable to create a Renderer:\n\n" + e.getDescription());
		}
	}
	catch(...)
	{
		throw Exception(0, "OpenGL Renderer", "Unhandled exception occured while creating a Renderer.");
	}

	cout << endl << "Subsystems initialized." << endl << endl;
	cout << "===================================" << endl << endl;
}


/**
 * Stops all subsystems and cleans everything up.
 */
Game::~Game()
{
	cout << endl << "===================================" << endl << endl;
	cout << "Shutting down..." << endl;

	// Destroy all of our various components in reverse order.
	Utility<EventHandler>::clean();
	Utility<Mixer>::clean();
	Utility<Renderer>::clean();
	Utility<Configuration>::clean();
	Utility<Filesystem>::clean();

	cout << endl << "Game object has been terminated." << endl;

	// Shut down all SDL subsystems.
	SDL_Quit();
}


/**
 * Adds an additional directory or archive to the search path
 * of the Filesystem.
 * 
 * \param	path	Path to add to the search path.
 */
void Game::mount(const std::string& path)
{
	Utility<Filesystem>::get().addToSearchPath(path);
}


/**
 * Primes the EventHandler and StateManager and enteres the main game loop.
 *
 * \param	state	A pointer to a State object.
 * 
 * \warning	The State object becomes owned by the StateManager. Do not delete
 *			the State.
 */
void Game::go(State *state)
{
	cout << "** GAME STATE START **" << endl << endl;

	StateManager stateManager;

	stateManager.setState(state);

	// Game Loop
	while(stateManager.update())
	{
		Utility<Renderer>::get().update();
	}
}
