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

#include "ScrollArea.h"

const int MIN_WIDTH = 48;
const int MIN_HEIGHT = 48;

ScrollArea::ScrollArea(const string& name, int width, int height):	Container(name),
																	mScrollBar(NULL),
																	mScrollbarWidth(0),
																	mScrollbarHeight(0),
																	mScrollAreaWidth(0),
																	mScrollAreaHeight(0)
{
	setMinimumSize(MIN_WIDTH, MIN_HEIGHT);
	setSize(width, height);
	//drawBorder(true);
	setBorderColor(0, 0, 255);
	setMargins(2, 2, 2, 2);
}


ScrollArea::~ScrollArea()
{}


void ScrollArea::initialize()
{
	mScrollBar = new ScrollBar("sArea_ScrollBar", mHeight);
	add(mScrollBar);
	int width = mWidth - mScrollBar->getWidth();
	mScrollBar->setRelative(getRelativeX() + mWidth - mScrollBar->getWidth(), getRelativeY());
	mScrollBar->setStepUnits(0);

	setSkinState("ScrollArea", STATE_NORMAL);
	
	// Set scrollable area dimensions.
	mScrollAreaWidth = mWidth - mScrollBar->getWidth() - mLeft - mRight;
	mScrollAreaHeight = mHeight - mTop - mBottom;

	scrollAreaInit();
}


void ScrollArea::logic()
{
}


void ScrollArea::onMouseWheelMotion()
{
//	if(event->getType() == EVENT_MOUSE_WHEEL_UP)
//	{
//		if(isPointInRect(event->getFirst(), event->getSecond(), getAbsoluteX(), getAbsoluteY(), mWidth, mHeight))
//			mScrollBar->setPosition(mScrollBar->getPosition() - mScrollBar->getStepSize());
//	}
//	
//	if(event->getType() == EVENT_MOUSE_WHEEL_DOWN)
//	{
//		if(isPointInRect(event->getFirst(), event->getSecond(), getAbsoluteX(), getAbsoluteY(), mWidth, mHeight))
//			mScrollBar->setPosition(mScrollBar->getPosition() + mScrollBar->getStepSize());
//	}
}

void ScrollArea::drawSkin()
{
	Singleton<Renderer>::get().drawImageRect(getAbsoluteX(), getAbsoluteY(), mScrollAreaWidth, mHeight, mCurrentSkinPart->getImageList());
}

void ScrollArea::drawPlain()
{
	Singleton<Renderer>::get().drawBoxFilled(getAbsoluteX(), getAbsoluteY(), mWidth, mHeight, mBackgroundColor.red(), mBackgroundColor.green(), mBackgroundColor.blue());
	Singleton<Renderer>::get().drawBox(getAbsoluteX(), getAbsoluteY(), mWidth, mHeight, mBorderColor.red(), mBorderColor.green(), mBorderColor.blue());
}
