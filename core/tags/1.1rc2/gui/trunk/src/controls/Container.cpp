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

#include "Container.h"

const int MIN_WIDTH		= 16;
const int MIN_HEIGHT		= 16;

/**
 * Constructor
 */
Container::Container(const string& name):	Control(name, MIN_WIDTH, MIN_HEIGHT)
{}


/**
 * Destructor
 */
Container::~Container()
{}


/**
 * Gets a list of the Container's children.
 *
 * \return	A references to a const ControlList. Note that the list
 *			should not be changed and absolutely should not have its
 *			constness cast away.
 */
const Control::ControlList& Container::getChildControlList() const
{
	return mChildControls;
}


/**
 * Initialize all internal variables, and any child controls needed.
 */
void Container::initialize()
{
	isAnchored(true);
	//setSkinState("BaseContainer");

	containerInit();
}


/**
 * Internal function that handles a key-down event for the Tab key.
 */
void Container::handleTab()
{
	for(size_t i = 0; i < mChildControls.size(); i++)
	{
		if(mChildControls[i]->isTabable() && mChildControls[i]->hasFocus())
		{
			mChildCounter = i + 1;
			break;
		}
		else if(mChildControls[i]->isTabable())
		{
			mChildControls[i]->hasFocus(true);
			mChildControls[i]->isHighlighted(true);
		}
	}
	
	for(mChildCounter; static_cast<size_t>(mChildCounter) < mChildControls.size(); ++mChildCounter)
	{
		if(mChildControls[mChildCounter]->isTabable())
		{
			mChildControls[mChildCounter]->hasFocus(true);
			mChildControls[mChildCounter]->isHighlighted(true);
			break;
		}
	}
	
	if(static_cast<size_t>(mChildCounter) >= mChildControls.size())
	{
		mChildCounter = 0;
		mChildControls[mChildCounter]->hasFocus(true);
		mChildControls[mChildCounter]->isHighlighted(true);
	}
}