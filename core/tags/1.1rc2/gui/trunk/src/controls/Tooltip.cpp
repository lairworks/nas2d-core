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

#include "Tooltip.h"

Tooltip::Tooltip(const string& caption, const string& name, int width, int height):	Control(name, width, height)
{
	setCaption(caption);
}


Tooltip::~Tooltip()
{}


void Tooltip::initialize()
{}


void Tooltip::logic()
{}


void Tooltip::drawSkin()
{
	Singleton<Renderer>::get().drawImage(mCurrentSkinPart->getImageList()[0], getAbsoluteX(), getAbsoluteY());
	
	// Note, I'm using bitshifts in place of a division by two.
	Singleton<Renderer>::get().drawText(*mFont, mCaption, getAbsoluteX() + mHeight, getAbsoluteY() - (mFont->getHeight() >> 1) + (mHeight >> 1), mForegroundColor.red(), mForegroundColor.green(), mForegroundColor.blue());
}


void Tooltip::drawPlain()
{
	Singleton<Renderer>::get().drawBoxFilled(getAbsoluteX(), getAbsoluteY(), mHeight, mHeight, mBackgroundColor.red(), mBackgroundColor.green(), mBackgroundColor.blue());
	Singleton<Renderer>::get().drawBox(getAbsoluteX(), getAbsoluteY(), mHeight, mHeight, mBackgroundColor.red(), mBackgroundColor.green(), mBackgroundColor.blue());
	
	// Note, I'm using bitshifts in place of a division by two.
	Singleton<Renderer>::get().drawText(*mFont, mCaption, getAbsoluteX() + mHeight, getAbsoluteY() - (mFont->getHeight() >> 1) + (mHeight >> 1), mForegroundColor.red(), mForegroundColor.green(), mForegroundColor.blue());
}