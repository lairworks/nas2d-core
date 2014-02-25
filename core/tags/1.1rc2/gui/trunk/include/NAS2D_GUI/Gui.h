// ==================================================================================
// = The Legend of Mazzeroth
// = Copyright © 2008 - 2010, New Age Software
// ==================================================================================
// = This file is part of The Legend of Mazzeroth.
// = 
// = The Legend of Mazzeroth is free software; you can redistribute it and/or modify
// = it under the terms of the GNU General Public License version 3 as published by
// = the Free Software Foundation.
// = 
// = The Legend of Mazzeroth is distributed in the hope that it will be useful,
// = but WITHOUT ANY WARRANTY; without even the implied warranty of
// = MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
// = GNU General Public License for more details.
// = 
// = You should have received a copy of the GNU General Public License
// = along with The Legend of Mazzeroth; if not, write to the Free Software
// = Foundation, Inc., 59 Temple Place, Suite 330, Boston, MA  02111-1307  USA
// ==================================================================================

#ifndef _LOM_GUI_
#define _LOM_GUI_

#include <list>
#include <map>

#include "Controls.h"
#include "Pointer.h"
#include "SkinManager.h"

#include "NAS2D/Renderer/Renderer.h"
#include "NAS2D/Resources/Image.h"
#include "NAS2D/Common.h"
#include "NAS2D/Filesystem.h"
#include "NAS2D/Exception.h"
#include "NAS2D/EventHandler.h"
#include "NAS2D/sigslot.h"


/**
 * \class Gui
 * \brief A GUI object.
 * 
 * 
 * 
 * \note	The GUI does not use bitmap fonts of any sort, only TrueType fonts.
 */
class Gui : public sigslot::has_slots<>
{
public:
	Gui();
	~Gui();
	
	void add(Control *control);
	void remove(Control *control);
	void sendToFront(Control *control);
    
	Control* find(const std::string& controlName) const;
    
	StringList getFontList() const;
	Font* getFont(const std::string& fontName) const;
	
	bool update();
	void clear();
    
	int getMouseX() const { return mMouseX; }
	int getMouseY() const { return mMouseY; }
    
	void setFocus(bool focus);
	
	void setPointer(MousePointerState state = MOUSE_DEFAULT);
    
	void setSkinTheme(const std::string& theme);
	
private:
	typedef std::list<Control*> ControlList;
	typedef std::map<string, Font*> FontList;
    
	Gui(const Gui&);			// Intentionally not defined.
	Gui& operator=(const Gui&);	// Intentionally not defined.
    
	void scanFonts(const std::string& searchPath);
    
	bool controlHasFocus();
	void testForFocus();
	
	void drawPointer();
	
	void onKeyDown(SDL_Event& event);
	void onKeyUp(SDL_Event& event);
	
	void onMouseMove(SDL_Event& event);
	void onMouseButtonDown(SDL_Event& event);
	void onMouseButtonUp(SDL_Event& event);
	
	sigslot::signal1<SDL_Event&> KeyDown;
	sigslot::signal1<SDL_Event&> KeyUp;
	
	sigslot::signal1<SDL_Event&> MouseMove;
	sigslot::signal1<SDL_Event&> MouseDown;
	sigslot::signal1<SDL_Event&> MouseUp;
	sigslot::signal1<SDL_Event&> MouseClick;
	sigslot::signal1<SDL_Event&> MouseDrag;
    
	SkinManager		*mSkinManager;			/**< A pointer to a SkinManager object. */
	Skin			*mSkin;					/**< Internal Skin used to render controls. */
    
	FontList		mFontList;				/**< List of fonts found in the specified fonts directory. */
	ControlList		mControlList;			/**< Internal list of Child Control's */
    
	int				mMouseX, mMouseY;		/**< Current mouse coordinates. */
	int				mMouseOldX, mMouseOldY;	/**< Previous mouse coordinates. */
	
	int				mNumControls;			/**< Number of Control objects being managed by the GUI. */
    
	bool			mMouseLeftPressed;		/**< Flag indicating that the left mouse button is pressed. */
	bool			mMouseRightPressed;		/**< Flag indicating that the left mouse button is pressed. */
	bool			mMouseMiddlePressed;	/**< Flag indicating that the left mouse button is pressed. */
    
	bool			mIsActive;				/**< Flag indicating that the GUI is still in an active state. */
    
	StringList		mErrorMessages;			/**< Stores a message should any errors occur. */
	
	Pointer			*mPointer;				/**< Internal mouse pointer. */
	bool			mCustomPointer;			/**< Toggles drawing of system pointer or custom set pointer. */
};
#endif
