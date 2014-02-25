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

#ifndef _LOM_GUI_CHECKBOX_
#define _LOM_GUI_CHECKBOX_

#include "Control.h"


/**
 * \class CheckBox
 * \brief A CheckBox can be toggled on/off.
 */
class CheckBox : public Control
{
public:
	CheckBox(const std::string& caption, int w = 12, int h = 12);
	//~CheckBox() {}

	void isToggled(bool toggled);
	bool isToggled() { return mIsToggled; }

private:
	void initialize();

	void drawSkin();
	void drawPlain();

	void onClick();
	void onEnabled();

	bool mIsToggled;
};

#endif