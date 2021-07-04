#pragma once

#include <string>
#include <utility>

namespace NAS2D
{

	enum class WindowDisplayMode
	{
		Windowed,
		Borderless,
		Borderless_Fullscreen,
	};

	struct WindowDesc
	{
		//The client dimensions
		//Window creation process should automatically add space for borders, menus and title bars
		std::pair<int, int> dimensions{1600,900};
		std::pair<int, int> position{0,0};
		WindowDisplayMode mode{WindowDisplayMode::Windowed};
	};

	class Window
	{
	public:
		virtual ~Window() noexcept {}

		virtual void open() noexcept = 0;
		virtual void close() noexcept = 0;

		virtual void show() noexcept = 0;
		virtual void hide() noexcept = 0;
		virtual void unhide() noexcept = 0;

		[[nodiscard]] virtual bool isOpen() const noexcept = 0;
		[[nodiscard]] virtual bool isClosed() const noexcept = 0;
		[[nodiscard]] virtual bool isFullscreen() const noexcept = 0;
		[[nodiscard]] virtual bool isWindowed() const noexcept = 0;

		/**
		 * @brief Get the underlying OS-specific window handle.
		 * @return void* Handle to the window. static_cast to appropriate OS-specific type. For Windows this would be HWND.
		*/
		[[nodiscard]] virtual void* nativeHandle() const noexcept = 0;

		/**
		 * @brief Set the underlying OS-specific window handle.
		 * @param handle: The handle to the window. For Windows this would be a variable of type HWND.
		 * @return None
		*/
		virtual void nativeHandle(void* handle) noexcept = 0;

		/**
		 * @brief Get the underlying OS-specific window-owned device context.
		 * @return void* handle to the Device Context. static_cast to appropriate OS-specific type. For Windows this would be HDC.
		*/
		[[nodiscard]] virtual void* nativeDeviceContext() const noexcept = 0;

		/**
		 * @brief Set the underlying OS-specific window-owned device context.
		 * @param hDC: The handle to the device context this window will now own.
		 * @comment For Windows this would be a variable of type HDC.
		 * @return None
		*/
		virtual void nativeDeviceContext(void* hDC) noexcept = 0;

		/**
		 * @brief Dimensions of the entire window including any border, menus, or title bars.
		 * @return std::pair<int, int> x/y components of the window dimensions
		*/
		[[nodiscard]] virtual std::pair<int, int> dimensions() const noexcept = 0;

		/**
		 * @brief Dimensions of the renderable area of the window not including any borders, menus, or title bars.
		 * @return std::pair<int, int> x/y components of the client area dimensions
		*/
		[[nodiscard]] virtual std::pair<int, int> clientDimensions() const noexcept = 0;
		virtual void dimensions(std::pair<int, int> newClientDimensions) noexcept = 0;

		virtual void title(std::string newTitle) noexcept = 0;
		[[nodiscard]] virtual std::string title() noexcept = 0;

		virtual void displayMode(WindowDisplayMode newDisplayMode) noexcept = 0;
		[[nodiscard]] virtual WindowDisplayMode displayMode() noexcept = 0;

		[[nodiscard]] virtual std::pair<int, int> position() const noexcept = 0;
		virtual void position(std::pair<int, int> newPosition) noexcept = 0;

		virtual void setDimensionsAndPosition(std::pair<int, int> newClientDimensions, std::pair<int, int> newPosition) noexcept = 0;

		/**
		 * @brief Set the native OS-specific window icon resource.
		 * @param iconResource: Handle to the icon resource.
		 * @return
		*/
		virtual void icon(void* iconResource) noexcept = 0;

		/**
		 * @brief Get the native OS-specific window icon resource.
		 * @return void* handle to the native OS-specific window icon resource.
		*/
		[[nodiscard]] virtual void* icon() const noexcept = 0;
	protected:
	private:

	};

}
