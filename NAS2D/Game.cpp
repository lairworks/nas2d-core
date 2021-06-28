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
#include "Exception.h"
#include "Filesystem.h"
#include "Utility.h"
#include "Version.h"

#include "Mixer/MixerNull.h"
#include "Mixer/MixerSDL.h"
#include "Renderer/RendererOpenGL.h"
#include "Renderer/RendererNull.h"

#include <SDL2/SDL.h>

#include <iostream>


using namespace NAS2D;

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

	Configuration& cf = Utility<Configuration>::get();
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

Game::~Game()
{
	std::cout << "\n===================================\n\n";
	std::cout << "Shutting down..." << std::endl;

	Utility<Renderer>::clear();
	Utility<EventHandler>::clear();
	Utility<Mixer>::clear();
	Utility<Configuration>::clear();
	Utility<Filesystem>::clear();

	SDL_Quit();

	std::cout << "\nGame object has been terminated." << std::endl;
}

void Game::mount(const std::string& path)
{
	Utility<Filesystem>::get().mount(path);
}

void Game::go(State *state)
{
	std::cout << "** GAME STATE START **\n\n";
	std::cout.flush();

	StateManager stateManager;

	stateManager.setState(state);

	while (stateManager.update())
	{
		Utility<Renderer>::get().update();
	}
}
