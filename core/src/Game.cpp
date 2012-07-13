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
#include "NAS2D/Renderer/SDL_Renderer.h"


/**
 * Starts the engine by initializing all of the engine sub components and
 * setting up any default values should they be needed.
 *
 * \param	appTitle	The title that should be used for the game window.
 * \param	argv_0		argv[0] from main()'s argument list. Necessary for Linux compatibility.
 * \param	configPath	Path to the Config file. Defaults to 'config.xml'.
 * \param	dataPath	Intitial data path. Defaults to 'data'.
 */
Game::Game(const std::string& appTitle, const std::string& argv_0, const std::string& configPath, const std::string& dataPath)
{
	cout << "Compiled on " << __DATE__ << " at " << __TIME__ << endl << endl;

	cout << "Initializing subsystems:" << endl;	

	Utility<Filesystem>::get().init(argv_0, "data");

	Configuration& cf = Utility<Configuration>::get();
	cf.load(configPath);

	// Instantiate the Renderer object
	if(cf.renderer() == "OGL")
	{
		Utility<Renderer>::instantiateDerived(new OGL_Renderer());

		// We all know how much I *hate* casting but this is one case where it's actually
		// necessary so that we can ensure that LoM stays in a stable state. Also, the cast
		// should never fail but this is one of those 'just in case' deals.
		OGL_Renderer* ogl = dynamic_cast<OGL_Renderer*>(&Utility<Renderer>::get());
		if(!ogl)
		{
			cout << "Unable to create an OpenGL Renderer. Switching to Software Mode instead." << endl;

			Utility<Configuration>::get().renderer("SDL");
			Utility<Configuration>::get().save();
			
			Utility<Renderer>::clean();
			Utility<Renderer>::instantiateDerived(new SDL_Renderer());
		}
	}
	else
		Utility<Renderer>::instantiateDerived(new SDL_Renderer());

	Utility<Renderer>::get().setApplicationTitle(appTitle);

	Utility<Mixer>::instantiateDerived(new SDL_Mixer());
	
	Utility<EventHandler>::instantiateDerived(new EventHandler());

	cout << "Subsystems initialized." << endl << endl;
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

	StateManager	stateManager;

	stateManager.setState(state);

	// Game Loop
	while(stateManager.update())
	{
		Utility<Renderer>::get().update();
		Utility<Mixer>::get().update();
	}
}
