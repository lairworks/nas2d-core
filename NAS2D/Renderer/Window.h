#pragma once

#include "../Math/Vector.h"

#include <string>
#include <vector>
#include <map>


struct SDL_Cursor;


namespace NAS2D
{
	struct DisplayDesc;


	class Window
	{
	public:
		Window();
		Window(const std::string& appTitle);
		virtual ~Window() = default;

		virtual std::vector<DisplayDesc> getDisplayModes() const;
		virtual DisplayDesc getClosestMatchingDisplayMode(const DisplayDesc& preferredDisplayDesc) const;

		const std::string& title() const;
		void title(const std::string& title);

		virtual void window_icon(const std::string& path);

		virtual void showSystemPointer(bool);
		virtual void addCursor(const std::string& filePath, int cursorId, int offx, int offy);
		virtual void setCursor(int cursorId);

		virtual void fullscreen(bool fs, bool maintain = false);
		virtual bool fullscreen() const;

		virtual void resizeable(bool _r);
		virtual bool resizeable() const;

		virtual void minimumSize(Vector<int> newSize);

		virtual Vector<int> size() const;
		virtual void size(Vector<int> newSize);
		void setResolution(Vector<int> newResolution);

		virtual Vector<int> getWindowClientArea() const noexcept;

	protected:
		virtual void onResize(Vector<int> newSize);

	protected:
		Vector<int> mResolution{1600, 900};
		std::string mTitle;
		std::map<int, SDL_Cursor*> cursors{};
	};
}
