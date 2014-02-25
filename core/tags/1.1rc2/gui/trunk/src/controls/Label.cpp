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

#include "Label.h"

/**
 * Constructor
 */
Label::Label(const string& caption, const string& name, bool autosize, int minWidth, int minHeight):	Control(name, minWidth, minHeight)
{
	setCaption(caption);
	isAutosized(autosize);
}


/**
 * Destructor
 */
Label::~Label()
{}


/**
 * Ensure that we're automatically setting our height/width parameters.
 */
void Label::initialize()
{
	setSkinState("Label");

	if(mFont && mAutosize)
	{
		mHeight = mFont->getHeight();
		mWidth = mFont->getWidth(mCaption);
	}
}


/**
 * Render this control.
 */
void Label::drawSkin()
{
	drawPlain();
}

void Label::drawPlain()
{
	Singleton<Renderer>::get().drawTextClamped(*mFont, mCaption, getAbsoluteX(), getAbsoluteY(), 0, 0, getWidth(), getHeight(), mForegroundColor.red(), mForegroundColor.green(), mForegroundColor.green());
}