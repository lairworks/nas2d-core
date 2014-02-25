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

#ifndef _LOM_GUI_WINDOW_
#define _LOM_GUI_WINDOW_

#include "Button.h"
#include "Container.h"
#include "Control.h"


/**
 * \class CloseButton
 * \brief A Button on Windows that "Hides" the window and its children.
 *
 * 
 */
class CloseButton : public Button
{
public:
	CloseButton() :	Button(" ", "guisys_btnClose", 12, 12) // Magic numbers: Default width/height for close button.
	{}

	~CloseButton()
	{}

protected:

	void initialize()
	{
		setSkinState("WindowCloseButton");

		// Make sure we're using a skin before we try to access it.
		if(mUsingSkin)
		{
			setMinimumSize(mCurrentSkinPart->getMinimumWidth(), mCurrentSkinPart->getMinimumHeight());
			setMaximumSize(mCurrentSkinPart->getMinimumWidth(), mCurrentSkinPart->getMinimumHeight());
			setSize(mCurrentSkinPart->getMinimumWidth(), mCurrentSkinPart->getMinimumHeight());
		}
	}

	void drawSkin()
	{
		Singleton<Renderer>::get().drawImage(mCurrentSkinPart->getImageList()[0], getAbsoluteX(), getAbsoluteY());
	}

	virtual void drawPlain()
	{
		Singleton<Renderer>::get().drawBoxFilled(getAbsoluteX(), getAbsoluteY(), mWidth, mHeight, mBackgroundColor.red(), mBackgroundColor.green(), mBackgroundColor.blue());
		Singleton<Renderer>::get().drawBox(getAbsoluteX(), getAbsoluteY(), mWidth, mHeight, mBorderColor.red(), mBorderColor.green(), mBorderColor.blue());

		Singleton<Renderer>::get().drawLine(getAbsoluteX() + 2, getAbsoluteY() + 2, getAbsoluteX() + mWidth - 2, getAbsoluteY() + mHeight - 2, mForegroundColor.red(), mForegroundColor.green(), mForegroundColor.green());
		Singleton<Renderer>::get().drawLine(getAbsoluteX() + 2, getAbsoluteY() + mHeight - 2, getAbsoluteX() + mWidth - 2, getAbsoluteY() + 2, mForegroundColor.red(), mForegroundColor.green(), mForegroundColor.green());
	}

private:
	void onClick(SDL_Event& event) 
	{ 
		if(isVisible())
		{
			if(mParent)
				mParent->hide();

			setSkinState("WindowCloseButton");
		}
	}

	void onMouseButtonDown() { setSkinState("WindowCloseButton", STATE_PRESSED); }
	void onMouseButtonUp() { setSkinState("WindowCloseButton"); }
};

/**
 * \class Window
 * \brief A Basic Window.
 *
 * The Window Control is effectively a container Control with a TitleBar
 * that can be resized with a mouse. Window should generally not be used
 * on its own and istead should be used as a base upon which to derive new
 * Window objects.
 */
class Window : public Control
{
public:
	Window(const std::string& titlebar, int x, int y, int width, int height);
	~Window();
	
	void add(Control *control);
	void remove(Control *control);

	virtual Control* getChild(string childName) const;

	/**
	 * Sets whether or not the Close Button is displayed.
	 */
	void hideCloseButton(bool hidden)
	{
		btnClose->isHidden(hidden);
	}

protected:
	Container		*mContainer;

private:
	void initialize();
	void determineTitleBarArea();
	
	void logic();

	/**
	 * \brief	Instructs derived Window objects to initialize themselves.
	 * \note	This function must be overridden in derived objects.
	 * 
	 * When deriving a Window Control, windowInit() must be overridden. This
	 * is the function where one would create and add new Control objects
	 * as well as specify various attributes such as color, size, position,
	 * etc.
	 */
	virtual void windowInit() {};

	/**
	 * \brief	Instructs derived Window objects to perform their logic.
	 * \note	This function must be overridden in derived objects.
	 *
	 * During every GUI update cycle, windowLogic() is called to allow
	 * derived Window objects to perform logic operations. This is the
	 * function where one would respond to events, perform calculations,
	 * etc.
	 */
	virtual void windowLogic() {};

	void drawSkin();
	void drawPlain();

	void onDrag(SDL_Event& event);

	void onFocus() { setSkinState("Window"); /*mContainer->hasFocus(true); btnClose->hasFocus(false);*/ }
	void onLostFocus()  { setSkinState("Window", STATE_NOFOCUS); }
	void onResized() { if(mIsInited) determineTitleBarArea(); }

	// Ensure that when a Window gains focus via a mouse click that the container
	// also gets focus so that field tabbing and activation events are captured
	// properly.
	//void onMouseButtonDown() { mContainer->hasFocus(true); }

	CloseButton		*btnClose;			/**< Close button. */

	Font			*mTitleFont;		/**< Windows draw with a bold font. */

	int				mTitleOffset;
	Rectangle_2d	mTitleBarArea;

	bool			mIsInited;			/**< This is a bit of a hack. Not thrilled with it. */
};
#endif