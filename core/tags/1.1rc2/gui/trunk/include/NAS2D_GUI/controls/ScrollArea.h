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

#ifndef _LOM_GUI_SCROLLAREA_
#define _LOM_GUI_SCROLLAREA_

#include "Button.h"
#include "Container.h"
#include "ScrollBar.h"



/**
 * \enum ScrollBarPolicy
 *
 * Indicates how to show scroll bars.
 */
enum ScrollBarPolicy
{
	SHOW_ALWAYS,
	SHOW_NEVER,
	SHOW_AUTO
};



/**
 * \class	ScrollArea
 * \brief	Base class for scrolling.
 *
 * \note	When drawing a derived Control, the area must be the
 *			width and height of the scrollarea minus the width
 *			of the vertical scrollbar and the height of the
 *			horizontal scrollbar.
 */
class ScrollArea : public Container
{
public:
	ScrollArea(const std::string& name, int width, int height);
	~ScrollArea();
	
	void setScrollPosition(double position) { mScrollBar->setPosition(position); }

protected:
	void initialize();
	void logic();
	
	ScrollBar	*mScrollBar;

	int		mScrollbarWidth;	/**< Width of the slider. */
	int		mScrollbarHeight;	/**< Height of the slider. */
	int		mScrollAreaWidth;
	int		mScrollAreaHeight;

private:
	void onMouseWheelMotion();

	/**
	 * Used to initialize the control and its properties.
	 * 
	 * This function must be overriden in derived controls, such as
	 * the TextBox.
	 */
	virtual void scrollAreaInit() // = 0;
	{}
	
	virtual void areaChanged()// = 0;
	{
		//std::cout << "I'z updated!" << endl;
	}

	void drawSkin();
	void drawPlain();
};
#endif
