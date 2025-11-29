// ==================================================================================
// = NAS2D
// = Copyright © 2008 - 2020 New Age Software
// ==================================================================================
// = NAS2D is distributed under the terms of the zlib license. You are free to copy,
// = modify and distribute the software under the terms of the zlib license.
// =
// = Acknowledgment of your use of NAS2D is appreciated but is not required.
// ==================================================================================

#include "Application.h"

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

#include <stdexcept>
#include <string>
#include <map>


using namespace NAS2D;


namespace
{
	NAS2D::Dictionary defaultConfigGraphics()
	{
		return {{
			{"screenwidth", 1000},
			{"screenheight", 700},
			{"bitdepth", 32},
			{"fullscreen", false},
			{"vsync", true},
		}};
	}

	NAS2D::Dictionary defaultConfigAudio()
	{
		return {{
			{"mixer", "SDL"},
			{"musicvolume", 100},
			{"sfxvolume", 128},
			{"channels", 2},
			{"mixrate", 22050},
			{"bufferlength", 1024},
		}};
	}

	std::map<std::string, NAS2D::Dictionary> defaultConfig()
	{
		return {
			{"graphics", defaultConfigGraphics()},
			{"audio", defaultConfigAudio()},
		};
	}
}


/**
 * Starts the engine by initializing all of the engine sub components and
 * setting up any default values should they be needed.
 *
 * \param	title	The title that should be used for the main window.
 * \param	appName	The name of the app (used to create an app data write path)
 * \param	organizationName	The name of the organization (used to create an app data write path)
 * \param	configPath	Path to the Config file. Defaults to 'config.xml'.
 * \param	dataPath	Initial data path. Defaults to 'data'.
 */
Application::Application(const std::string& title, const std::string& appName, const std::string& organizationName, const std::string& configPath, const std::string& dataPath)
{
	SDL_Init(0);

	auto& fs = Utility<Filesystem>::init<Filesystem>(appName, organizationName);
	fs.mountSoftFail(RealPath{dataPath});
	fs.mountSoftFail(fs.findInParents(RealPath{dataPath}, fs.basePath()));
	fs.mountReadWrite(fs.prefPath());

	Configuration& configuration = Utility<Configuration>::init(defaultConfig());
	configuration.load(configPath);

	Utility<EventHandler>::get();

	const auto rendererOptions = RendererOpenGL::ReadConfigurationOptions(configuration);
	Utility<Renderer>::init<RendererOpenGL>(title, rendererOptions);

	try
	{
		const auto mixerOptions = MixerSDL::InvalidToDefault(MixerSDL::ReadConfigurationOptions(configuration));
		Utility<Mixer>::init<MixerSDL>(mixerOptions);
	}
	catch (std::exception& /*exception*/)
	{
		Utility<Mixer>::init<MixerNull>();
	}
}


/**
 * Stops all subsystems and cleans everything up.
 */
Application::~Application()
{
	// Destroy all of our various components in reverse order.
	Utility<Mixer>::clear();
	Utility<Renderer>::clear();
	Utility<EventHandler>::clear();
	Utility<Configuration>::clear();
	Utility<Filesystem>::clear();

	// Shut down all SDL subsystems.
	SDL_Quit();
}


/**
 * Adds an additional directory to the search path of the Filesystem.
 *
 * \param path	Path to add to the search path.
 */
void Application::mount(const std::string& path)
{
	Utility<Filesystem>::get().mount(RealPath{path});
}


/**
 * Adds an additional directory to the search path of the Filesystem.
 *
 * The path is searched for starting from the `basePath` and searching up.
 *
 * \param path	Path to add to the search path.
 */
void Application::mountFindFromBase(const std::string& path)
{
	auto& filesystem = Utility<Filesystem>::get();
	filesystem.mount(filesystem.findInParents(RealPath{path}, filesystem.basePath()));
}


/**
 * Primes the EventHandler and StateManager and enters the main event loop.
 *
 * \param state	A pointer to a State object.
 *
 * \warning	The State object becomes owned by the StateManager. Do not delete
 *			the State.
 */
void Application::go(State* state)
{
	StateManager stateManager;

	stateManager.setState(state);

	// Application Loop
	while (stateManager.update())
	{
		Utility<Renderer>::get().update();
	}
}


Filesystem& Application::filesystem()
{
	return Utility<Filesystem>::get();
}


Configuration& Application::configuration()
{
	return Utility<Configuration>::get();
}


EventHandler& Application::eventHandler()
{
	return Utility<EventHandler>::get();
}


Renderer& Application::renderer()
{
	return Utility<Renderer>::get();
}


Mixer& Application::mixer()
{
	return Utility<Mixer>::get();
}
