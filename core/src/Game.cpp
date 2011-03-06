// ==================================================================================
// = NAS2D
// = Copyright © 2008 New Age Software
// ==================================================================================
// = NAS2D is distributed under the terms of the zlib license. You are free to copy,
// = modify and distribute the software as long under the terms of the zlib license.
// = 
// = Acknowledgement of your use of NAS2D is appriciated but is not required.
// ==================================================================================

#include "NAS2D/Configuration.h"
#include "NAS2D/EventHandler.h"
#include "NAS2D/Exception.h"
#include "NAS2D/Filesystem.h"
#include "NAS2D/Game.h"
#include "NAS2D/Singleton.h"

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
Game::Game(const string& appTitle, const string& argv_0, const string& configPath, const string& dataPath)
{
	cout << "Compiled on " << __DATE__ << " at " << __TIME__ << endl << endl;

	cout << "Initializing subsystems:" << endl;	

	Singleton<Filesystem>::get().init(argv_0, "data");

	Configuration& cf = Singleton<Configuration>::get();
	cf.loadConfig(configPath);

	// Instantiate the Renderer object
	if(cf.getRenderer() == "OGL")
	{
		Singleton<Renderer>::instantiateDerived(new OGL_Renderer());

		// We all know how much I *hate* casting but this is one case where it's actually
		// necessary so that we can ensure that LoM stays in a stable state. Also, the cast
		// should never fail but this is one of those 'just in case' deals.
		OGL_Renderer* ogl = dynamic_cast<OGL_Renderer*>(&Singleton<Renderer>::get());
		if(!ogl || !ogl->valid())
		{
			cout << "Unable to create an OpenGL Renderer. Switching to Software Mode instead." << endl;

			Singleton<Configuration>::get().setRenderer("SDL");
			Singleton<Configuration>::get().saveConfig();
			
			Singleton<Renderer>::clean();
			Singleton<Renderer>::instantiateDerived(new SDL_Renderer());
		}
	}
	else
		Singleton<Renderer>::instantiateDerived(new SDL_Renderer());

	Singleton<Renderer>::get().setApplicationTitle(appTitle);

	Singleton<Mixer>::instantiateDerived(new SDL_Mixer());

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
	Singleton<Mixer>::clean();
	Singleton<Renderer>::clean();
	Singleton<Configuration>::clean();
	Singleton<Filesystem>::clean();

	cout << endl << "Game Object has been Terminated." << endl;

	// Shut down all SDL subsystems.
	/** \todo	Find a same place for this. */
	SDL_Quit();
}


/**
 * Adds an additional directory or archive to the search path
 * of the Filesystem.
 * 
 * \param	path	Path to add to the search path.
 */
void Game::mount(const string& path)
{
	Singleton<Filesystem>::get().addToSearchPath(path);
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
		Singleton<Renderer>::get().update();
		Singleton<Mixer>::get().update();
	}
}
