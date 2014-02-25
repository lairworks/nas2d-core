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

#include "CheckBox.h"

/**
 * Space between the check box and the caption text.
 */
const int SPACER = 5;

CheckBox::CheckBox(const string& caption, int w, int h):	Control(caption, w, h),
															mIsToggled(false)
{
	setCaption(caption);
}


void CheckBox::initialize()
{
	if(!mIsToggled)
		setSkinState("CheckBox");
	else
		setSkinState("CheckBox", STATE_SELECTED);

	setWidth(mHeight + mFont->getWidth(mCaption) + SPACER);
}


void CheckBox::isToggled(bool toggled)
{
	mIsToggled = toggled;

	if(mIsToggled)
		setSkinState("CheckBox", STATE_SELECTED);
	else
		setSkinState("CheckBox", STATE_NORMAL);

}


void CheckBox::drawSkin()
{
	Singleton<Renderer>::get().drawImage(mCurrentSkinPart->getImageList()[0], getAbsoluteX(), getAbsoluteY());
	
	// Note, I'm using bitshifts in place of a division by two.
	Singleton<Renderer>::get().drawText(*mFont, mCaption, getAbsoluteX() + mHeight + SPACER, getAbsoluteY() - (mFont->getHeight() >> 1) + (mHeight >> 1), mForegroundColor.red(), mForegroundColor.green(), mForegroundColor.green());
}


void CheckBox::drawPlain()
{
	if(mIsToggled)
	{
		Singleton<Renderer>::get().drawBoxFilled(getAbsoluteX(), getAbsoluteY(), mHeight, mHeight, mBackgroundColor.red(), mBackgroundColor.green(), mBackgroundColor.blue());
		Singleton<Renderer>::get().drawBox(getAbsoluteX(), getAbsoluteY(), mHeight, mHeight, mBorderColor.red(), mBorderColor.green(), mBorderColor.blue());
		
		Singleton<Renderer>::get().drawLine(getAbsoluteX(), getAbsoluteY(), getAbsoluteX() + mHeight, getAbsoluteY() + mHeight, mBorderColor.red(), mBorderColor.green(), mBorderColor.blue());
		Singleton<Renderer>::get().drawLine(getAbsoluteX(), getAbsoluteY() + mHeight, getAbsoluteX() + mHeight, getAbsoluteY(), mBorderColor.red(), mBorderColor.green(), mBorderColor.blue());
	}
	else if(!mIsToggled)
	{
		Singleton<Renderer>::get().drawBoxFilled(getAbsoluteX(), getAbsoluteY(), mHeight, mHeight, mBackgroundColor.red(), mBackgroundColor.green(), mBackgroundColor.blue());
		Singleton<Renderer>::get().drawBox(getAbsoluteX(), getAbsoluteY(), mHeight, mHeight, mBorderColor.red(), mBorderColor.green(), mBorderColor.blue());
	}

	// Note, I'm using bitshifts in place of a division by two.
	Singleton<Renderer>::get().drawText(*mFont, mCaption, getAbsoluteX() + mHeight + SPACER, getAbsoluteY() - (mFont->getHeight() >> 1) + (mHeight >> 1), mForegroundColor.red(), mForegroundColor.green(), mForegroundColor.green());
}


void CheckBox::onClick()
{
	std::cout << "CheckBox has been clicked!" << endl;
	if(mIsToggled)
	{
		mIsToggled = false;
		setSkinState("CheckBox");
	}
	else
	{
		mIsToggled = true;
		setSkinState("CheckBox", STATE_SELECTED);
	}
}


void CheckBox::onEnabled()
{
	// Set the skin state
	if(mEnabled)
		setSkinState("CheckBox");
	else
		setSkinState("CheckBox", STATE_DISABLED);
}