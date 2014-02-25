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

#include "Window.h"

const int WINDOW_MARGIN = 5;
const int WINDOW_MIN_WIDTH = 64;
const int WINDOW_MIN_HEIGHT = 64;

// Titlebar Height will be this value if no skin is set for the Window.
const int WINDOW_TITLEBAR_DEFAULT_HEIGHT = 18;


Window::Window(const string& title, int x, int y, int w, int h):	Control(title, WINDOW_MIN_WIDTH, WINDOW_MIN_HEIGHT),
																	mContainer(NULL),
																	btnClose(NULL),
																	mTitleFont(NULL),
																	mTitleOffset(0),
																	mIsInited(false)
{
	//std::cout << "(GUI) Window '" << title << "'." << endl;

	setRelative(x, y);
	setSize(w, h);
	setCaption(title);
	isAnchored(false);
}


Window::~Window()
{
	if(mTitleFont)
		delete mTitleFont;
}


void Window::initialize()
{	
	// Set Window Skin
	hasFocus() ? setSkinState("Window") : setSkinState("Window", STATE_NOFOCUS);

	btnClose = new CloseButton();
	Control::add(btnClose);

	/**
	 * \todo	The positioning of the close button should be defined within the skin
	 *			file and not hard coded here.
	 */
	btnClose->setRelative(mWidth - btnClose->getMinWidth() - 3, 3);
	
	determineTitleBarArea();

	setMargins(mTitleBarArea.h, WINDOW_MARGIN, WINDOW_MARGIN, WINDOW_MARGIN);

	mContainer = new Container("Container");
	Control::add(mContainer);
	mContainer->setRelative(mLeft, mTop);
	mContainer->setWidth(mWidth);
	mContainer->setHeight(mHeight - mTop - mBottom);
	mContainer->setBorderColor(255, 0, 0);

	// This is a hack to get windows that automatically resize themselved during initialization
	// to set their title bar area widths properly.
	mIsInited = true;

	// Make sure derived Window's can be properly initialized.
	windowInit();

}


void Window::determineTitleBarArea()
{
	//assert(mFont);
	mTitleFont = new Font();
	mTitleFont->setStyle(STYLE_BOLD);

	// Determine Title Bar Area
	if(mUsingSkin)
	{
		if(!mCurrentSkinPart->getCaptionOffsets().blank)
		{
			TextOffsets tO = mCurrentSkinPart->getCaptionOffsets();
			mTitleBarArea = Rectangle_2d(tO.left, tO.top, mWidth - tO.left - tO.right, tO.min_height);
		}
		else
		{
			// The assumption here is that the top right corner of the window graphics
			// (ImageList[0]) will give us the height of the title bar.
			mTitleBarArea = Rectangle_2d(mLeft, 0, mWidth - mLeft - mRight - btnClose->getWidth(), mCurrentSkinPart->getImageList()[0].getHeight());
		}
	}
	else
		mTitleBarArea = Rectangle_2d(mLeft, mTop, mWidth - mLeft - mRight - btnClose->getWidth(), WINDOW_TITLEBAR_DEFAULT_HEIGHT);


	// Do our best to center the given font horizontally in our title bar area.
	mTitleOffset = (mTitleBarArea.h / 2) - (mTitleFont->getHeight() / 2);

	//Singleton<Renderer>::get().drawTextClamped(mTitleFont, mCaption, getAbsoluteX() + mLeft, getAbsoluteY() + mTitleOffset, mWidth - mLeft - mRight - btnClose->getWidth(), mTitlebarHeight - 2, mForegroundColor.red(), mForegroundColor.green(), mForegroundColor.green());
}


void Window::add(Control *control)
{
	mContainer->add(control);
}


void Window::remove(Control *control)
{
	mContainer->remove(control);
}


void Window::drawSkin()
{    
	Singleton<Renderer>::get().drawImageRect(getAbsoluteX(), getAbsoluteY(), mWidth, mHeight, mCurrentSkinPart->getImageList());
	Singleton<Renderer>::get().drawTextClamped(*mTitleFont, mCaption, getAbsoluteX() + mTitleBarArea.x, getAbsoluteY() + mTitleBarArea.y, 0, 0, mTitleBarArea.w, mTitleBarArea.h, mForegroundColor.red(), mForegroundColor.green(), mForegroundColor.green());
}

void Window::drawPlain()
{
	Singleton<Renderer>::get().drawBoxFilled(getAbsoluteX(), getAbsoluteY(), mWidth, mHeight, mBackgroundColor.red(), mBackgroundColor.green(), mBackgroundColor.blue());
    Singleton<Renderer>::get().drawBox(getAbsoluteX(), getAbsoluteY(), mWidth, mHeight, mBorderColor.red(), mBorderColor.green(), mBorderColor.blue());

	Singleton<Renderer>::get().drawLine(getAbsoluteX() + 5, getAbsoluteY() + mTop - 1, getAbsoluteX() + mWidth - 5, getAbsoluteY() + mTop - 1, mBorderColor.red(), mBorderColor.green(), mBorderColor.blue(), 255);

	Singleton<Renderer>::get().drawTextClamped(*mTitleFont, mCaption, getAbsoluteX() + mTitleBarArea.x, getAbsoluteY() + mTitleBarArea.y, 0, 0, mTitleBarArea.w, mTitleBarArea.h, mForegroundColor.red(), mForegroundColor.green(), mForegroundColor.green());
}


void Window::logic()
{
	windowLogic();
}


void Window::onDrag(SDL_Event& event)
{
	if(mChildControls.size() > 0)
		for(size_t i = 0; i < mChildControls.size(); i++)
			if(mChildControls[i]->hasFocus())
				return;

	if(hasFocus() && !isAnchored())
		setAbsolute(getAbsoluteX() + event.motion.xrel, getAbsoluteY() + event.motion.yrel);
}


/**
 * This is here as a special case for Windows as we are adding controls to
 * a Container, not directly to the Window.
 */
Control* Window::getChild(string childName) const
{
	return mContainer->getChild(childName);
}