
#include "Window.h"

#include "DisplayDesc.h"
#include "../Utility.h"
#include "../Filesystem.h"
#include "../EventHandler.h"
#include "../Math/Point.h"

#include <SDL2/SDL.h>

#if defined(__XCODE_BUILD__)
#include <SDL2_image/SDL_image.h>
#else
#include <SDL2/SDL_image.h>
#endif

#if defined(_WIN32)
	#include <windows.h>
	#include <SDL2/SDL_syswm.h>
#endif

#include <array>


using namespace NAS2D;


namespace
{
	bool isAnyWindowFlagSet(SDL_Window* window, Uint32 testFlags)
	{
		return (SDL_GetWindowFlags(window) & testFlags) != 0;
	}


#if defined(_WIN32)
	/**
	 * Gets a Windows API HWND handle to the application window.
	 */
	HWND getWin32Handle(SDL_Window* sdlWindow)
	{
		SDL_SysWMinfo systemInfo;
		SDL_VERSION(&systemInfo.version);

		if (SDL_GetWindowWMInfo(sdlWindow, &systemInfo) == 1)
		{
			return systemInfo.info.win.window;
		}

		return nullptr;
	}
#endif


	/**
	 * Shows a message dialog box.
	 */
	void doModalError(const std::string& title, const std::string& message, SDL_Window* sdlWindow)
	{
#if defined(_WIN32)
		MessageBoxA(getWin32Handle(sdlWindow), message.c_str(), title.c_str(), MB_OK | MB_ICONERROR | MB_TASKMODAL);
#else
		SDL_ShowSimpleMessageBox(SDL_MESSAGEBOX_ERROR, title.c_str(), message.c_str(), sdlWindow);
#endif
	}


	/**
	 * Shows a message dialog box with no icon.
	 */
	void doModalAlert(const std::string& title, const std::string& message, SDL_Window* sdlWindow)
	{
#if defined(_WIN32)
		MessageBoxA(getWin32Handle(sdlWindow), message.c_str(), title.c_str(), MB_OK | MB_TASKMODAL);
#else
		SDL_ShowSimpleMessageBox(SDL_MESSAGEBOX_WARNING, title.c_str(), message.c_str(), sdlWindow);
#endif
	}


	/**
	 * Shows a message dialog box with Yes and No buttons.
	 */
	bool doModalYesNo(const std::string& title, const std::string& message, SDL_Window* sdlWindow)
	{
		bool isYes = false;
#if defined(_WIN32)
		isYes = (MessageBoxA(getWin32Handle(sdlWindow), message.c_str(), title.c_str(), MB_YESNO | MB_ICONINFORMATION | MB_TASKMODAL) == IDYES);
#else
		constexpr std::array<SDL_MessageBoxButtonData, 2> buttons = {{
			{SDL_MESSAGEBOX_BUTTON_RETURNKEY_DEFAULT, 1, "Yes"},
			{SDL_MESSAGEBOX_BUTTON_ESCAPEKEY_DEFAULT, 0, "No"},
		}};

		const SDL_MessageBoxData messageBoxData = {
			SDL_MESSAGEBOX_INFORMATION,
			sdlWindow,
			title.c_str(),
			message.c_str(),
			buttons.size(),
			buttons.data(),
			nullptr,
		};

		int buttonId = 0;
		SDL_ShowMessageBox(&messageBoxData, &buttonId);
		isYes = (buttonId == 1);
#endif

		return isYes;
	}
}


Window::Window() :
	Window{"Default Application"}
{
}


Window::Window(const std::string& appTitle) :
	mTitle{appTitle}
{
	Utility<EventHandler>::get().windowResized().connect({this, &Window::onResize});
}


Window::~Window()
{
	for (auto& [key, cursor] : cursors)
	{
		SDL_FreeCursor(cursor);
	}
}


const std::string& Window::title() const
{
	return mTitle;
}


void Window::title(const std::string& title)
{
	mTitle = title;
}


void Window::setResolution(Vector<int> newResolution)
{
	if (!fullscreen())
	{
		mResolution = newResolution;
	}
}


std::vector<DisplayDesc> Window::getDisplayModes() const
{
	const auto displayIndex = SDL_GetWindowDisplayIndex(underlyingWindow);
	const auto numResolutions = SDL_GetNumDisplayModes(displayIndex);
	if (numResolutions < 0)
	{
		throw std::runtime_error("Error getting number of display modes for display index: " + std::to_string(displayIndex) + " : " + SDL_GetError());
	}

	std::vector<DisplayDesc> result{};
	result.reserve(static_cast<std::size_t>(numResolutions));
	for (int i = 0; i < numResolutions; ++i)
	{
		SDL_DisplayMode currentMode{};
		SDL_GetDisplayMode(displayIndex, i, &currentMode);
		result.push_back({currentMode.w, currentMode.h, currentMode.refresh_rate});
	}
	return result;
}


DisplayDesc Window::getClosestMatchingDisplayMode(const DisplayDesc& preferredDisplayDesc) const
{
	const auto displayIndex = SDL_GetWindowDisplayIndex(underlyingWindow);
	SDL_DisplayMode preferredMode{};
	preferredMode.w = preferredDisplayDesc.width;
	preferredMode.h = preferredDisplayDesc.height;
	preferredMode.refresh_rate = preferredDisplayDesc.refreshHz;

	SDL_DisplayMode closestMode{};
	if (SDL_GetClosestDisplayMode(displayIndex, &preferredMode, &closestMode))
	{
		return {closestMode.w, closestMode.h, closestMode.refresh_rate};
	}
	throw std::runtime_error("No matching display mode for " + std::string{preferredDisplayDesc});
}


void Window::window_icon(const std::string& path)
{
	auto iconData = Utility<Filesystem>::get().readFile(path);
	SDL_Surface* icon = IMG_Load_RW(SDL_RWFromConstMem(iconData.c_str(), static_cast<int>(iconData.size())), 1);
	if (!icon)
	{
		throw std::runtime_error("Failed to set window icon: " + path + " : " + SDL_GetError());
	}

	SDL_SetWindowIcon(underlyingWindow, icon);
	SDL_FreeSurface(icon);
}


void Window::showSystemPointer(bool _b)
{
	SDL_ShowCursor(static_cast<int>(_b));
}


void Window::addCursor(CursorId cursorId, const std::string& filePath, Vector<int> hotOffset)
{
	auto imageData = Utility<Filesystem>::get().readFile(filePath);
	if (imageData.size() == 0)
	{
		throw std::runtime_error("Cursor file is empty: " + filePath);
	}

	SDL_Surface* surface = IMG_Load_RW(SDL_RWFromConstMem(imageData.c_str(), static_cast<int>(imageData.size())), 1);
	if (!surface)
	{
		throw std::runtime_error("Failed to load cursor: " + filePath + " : " + SDL_GetError());
	}

	SDL_Cursor* cursor = SDL_CreateColorCursor(surface, hotOffset.x, hotOffset.y);
	SDL_FreeSurface(surface);
	if (!cursor)
	{
		throw std::runtime_error("Failed to create color cursor: " + filePath + " : " + SDL_GetError());
	}

	if (cursors.contains(cursorId.id))
	{
		SDL_FreeCursor(cursors[cursorId.id]);
	}

	cursors[cursorId.id] = cursor;

	if (cursors.size() == 1)
	{
		setCursor(cursorId);
	}
}


void Window::setCursor(CursorId cursorId)
{
	SDL_SetCursor(cursors[cursorId.id]);
}


void Window::fullscreen(bool fullscreen, bool maintain)
{
	if (fullscreen)
	{
		const auto windowFlags = maintain ? SDL_WINDOW_FULLSCREEN : SDL_WINDOW_FULLSCREEN_DESKTOP;
		SDL_SetWindowFullscreen(underlyingWindow, windowFlags);
		SDL_SetWindowResizable(underlyingWindow, SDL_FALSE);
	}
	else
	{
		SDL_SetWindowFullscreen(underlyingWindow, 0);
		const auto windowSize = size();
		SDL_SetWindowSize(underlyingWindow, windowSize.x, windowSize.y);
		onResize(windowSize);
		SDL_SetWindowPosition(underlyingWindow, SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED);
	}
}


bool Window::fullscreen() const
{
	return isAnyWindowFlagSet(underlyingWindow, SDL_WINDOW_FULLSCREEN | SDL_WINDOW_FULLSCREEN_DESKTOP);
}


void Window::maximize()
{
	SDL_MaximizeWindow(underlyingWindow);
}


bool Window::isMaximized() const
{
	const auto flags = SDL_GetWindowFlags(underlyingWindow);
	return (flags & SDL_WINDOW_MAXIMIZED);
}


void Window::resizeable(bool resizable)
{
	if (fullscreen())
	{
		return;
	}

#if defined(_MSC_VER)
	#pragma warning(suppress : 26812) // C26812 Warns to use enum class (C++), but SDL is a C library
#endif
	SDL_SetWindowResizable(underlyingWindow, resizable ? SDL_TRUE : SDL_FALSE);
}


bool Window::resizeable() const
{
	return isAnyWindowFlagSet(underlyingWindow, SDL_WINDOW_RESIZABLE);
}


void Window::minimumSize(Vector<int> newSize)
{
	SDL_SetWindowMinimumSize(underlyingWindow, newSize.x, newSize.y);

	// Read back the window size, in case it was changed
	// Window may need to have been enlarged to the minimum size
	SDL_GetWindowSize(underlyingWindow, &newSize.x, &newSize.y);
	onResize(newSize);
}


Vector<int> Window::size() const
{
	if (isAnyWindowFlagSet(underlyingWindow, SDL_WINDOW_FULLSCREEN_DESKTOP))
	{
		SDL_DisplayMode dm;
		if (SDL_GetDesktopDisplayMode(0, &dm) != 0)
		{
			throw std::runtime_error("Unable to get desktop display mode: " + std::string{SDL_GetError()});
		}

		return {dm.w, dm.h};
	}

	return mResolution;
}


void Window::size(Vector<int> newSize)
{
	SDL_SetWindowSize(underlyingWindow, newSize.x, newSize.y);
	onResize(newSize);
	SDL_SetWindowPosition(underlyingWindow, SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED);
}


void Window::onResize(Vector<int> /*newSize*/)
{
}


Vector<int> Window::getWindowClientArea() const noexcept
{
	Vector<int> size;
	SDL_GetWindowSize(underlyingWindow, &size.x, &size.y);
	return size;
}


void Window::captureMouse()
{
	SDL_SetWindowGrab(underlyingWindow, SDL_TRUE);
}


void Window::releaseMouse()
{
	SDL_SetWindowGrab(underlyingWindow, SDL_FALSE);
}


/**
 * Sets the mouse pointer to a specified location within the application window.
 *
 * \note Coordinates will be clamped to the window's dimensions.
 */
void Window::warpMouse(Point<int> mousePositionInWindow)
{
	SDL_WarpMouseInWindow(underlyingWindow, mousePositionInWindow.x, mousePositionInWindow.y);
}


void Window::doModalError(const std::string& title, const std::string& message) const
{
	::doModalError(title, message, underlyingWindow);
}


void Window::doModalAlert(const std::string& title, const std::string& message) const
{
	::doModalAlert(title, message, underlyingWindow);
}


bool Window::doModalYesNo(const std::string& title, const std::string& message) const
{
	return ::doModalYesNo(title, message, underlyingWindow);
}
