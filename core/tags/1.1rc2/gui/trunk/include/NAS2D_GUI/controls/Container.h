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

#ifndef _LOM_GUI_CONTAINER_
#define _LOM_GUI_CONTAINER_

#include "Control.h"

/**
 * \class	Container
 * \brief	Basic class for containing other controls.
 * 
 * The Container is purely a container for other Controls.
 */
class Container: public Control
{
public:
	Container(const std::string& name);
	~Container();
	
	const ControlList& getChildControlList() const;
	
private:
	void initialize();
	
	void handleTab();

	virtual void containerInit() 
	{}
};

#endif