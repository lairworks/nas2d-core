#pragma once

#include <string>
#include <utility>

enum class WindowDisplayMode {
	Windowed
	,Borderless
	,Borderless_Fullscreen
};

struct WindowDesc {
	//The client dimensions
	//Window creation process should automatically add space for borders, menus and title bars
	std::pair<int, int> dimensions{1600,600};
	std::pair<int, int> position{0,0};
	WindowDisplayMode mode{WindowDisplayMode::Windowed};
};

class Window {
public:
	virtual ~Window() noexcept {}

	virtual void Open() noexcept = 0;
	virtual void Close() noexcept = 0;

	virtual void Show() noexcept = 0;
	virtual void Hide() noexcept = 0;
	virtual void Unhide() noexcept = 0;

	[[nodiscard]] virtual bool IsOpen() const noexcept = 0;
	[[nodiscard]] virtual bool IsClosed() const noexcept = 0;
	[[nodiscard]] virtual bool IsFullscreen() const noexcept = 0;
	[[nodiscard]] virtual bool IsWindowed() const noexcept = 0;

	/**
	 * @brief Get the underlying OS-specific window handle.
	 * @return void* Handle to the window. static_cast to appropriate OS-specific type. For Windows this would be HWND.
	*/
	[[nodiscard]] virtual void* NativeHandle() const noexcept = 0;

	/**
	 * @brief Set the underlying OS-specific window handle.
	 * @param handle: The handle to the window. For Windows this would be a variable of type HWND.
	 * @return None
	*/
	virtual void NativeHandle(void* handle) noexcept = 0;

	/**
	 * @brief Get the underlying OS-specific window-owned device context.
	 * @return void* handle to the Device Context. static_cast to appropriate OS-specific type. For Windows this would be HDC.
	*/
	[[nodiscard]] virtual void* NativeDeviceContext() const noexcept = 0;

	/**
	 * @brief Set the underlying OS-specific window-owned device context.
	 * @param hDC: The handle to the device context this window will now own.
	 * @comment For Windows this would be a variable of type HDC.
	 * @return None
	*/
	virtual void NativeDeviceContext(void* hDC) noexcept = 0;

	/**
	 * @brief Dimensions of the entire window including any border, menus, or title bars.
	 * @return std::pair<int, int> x/y components of the window dimensions
	*/
	[[nodiscard]] virtual std::pair<int, int> GetDimensions() const noexcept = 0;
	
	/**
	 * @brief Dimensions of the renderable area of the window not including any borders, menus, or title bars.
	 * @return std::pair<int, int> x/y components of the client area dimensions
	*/
	[[nodiscard]] virtual std::pair<int, int> GetClientDimensions() const noexcept = 0;

	virtual void Dimensions(std::pair<int, int> newClientDimensions) noexcept = 0;

	virtual void Title(std::string title) noexcept = 0;
	[[nodiscard]] virtual std::string Title() noexcept = 0;

	virtual void DisplayMode(WindowDisplayMode mode) noexcept = 0;
	[[nodiscard]] virtual WindowDisplayMode DisplayMode() noexcept = 0;

	[[nodiscard]] virtual std::pair<int, int> Position() const noexcept = 0;
	virtual void Position(std::pair<int, int> newPosition) noexcept = 0;

	virtual void SetDimensionsAndPosition(std::pair<int, int> newClientDimensions, std::pair<int, int> newPosition) noexcept = 0;

	/**
	 * @brief Set the native OS-specific window icon resource.
	 * @param iconResource: Handle to the icon resource.
	 * @return 
	*/
	virtual void Icon(void* iconResource) noexcept = 0;

	/**
	 * @brief Get the native OS-specific window icon resource.
	 * @return void* handle to the native OS-specific window icon resource.
	*/
	[[nodiscard]] virtual void* Icon() const noexcept = 0;
protected:
private:
	
};
