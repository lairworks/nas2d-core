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

#ifndef _LOM_GUI_TOOLTIP_
#define _LOM_GUI_TOOLTIP_

#include "Control.h"
#include "TextBox.h"

/**
 * \class Tooltip
 * \brief A Tooltip object derived from base class Control.
 *
 * The Tooltip class provides all basic functions of a tooltip.
 *
 * The Tooltip should respond to mouseover events for controls
 * it is attached to.
 *
 */

class Tooltip : public Control
{
public:
	Tooltip(const std::string& caption, const std::string& name, int width, int height);
	~Tooltip();
	
private:
	void initialize();
	void logic();
	
	void drawSkin();
	void drawPlain();
};

#endif
