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

#include "Pointer.h"

Pointer::Pointer(Image *image, int hotspotX, int hotspotY) :	mPointerImage(image),
																mMouseX(0),
																mMouseY(0),
																mHotspotX(hotspotX),
																mHotspotY(hotspotY)
{
}


Pointer::~Pointer()
{
	if(mPointerImage)
	{
		delete mPointerImage;
		mPointerImage = NULL;
	}
}


void Pointer::update()
{
	SDL_GetMouseState(&mMouseX, &mMouseY);
}


/**
 * Returns a pointer to the internally stored Pointer Image.
 */
Image *Pointer::getPointerImage() const
{
	return mPointerImage;
}


/**
 * Sets the internal pointer Image.
 *
 * \note	Pointer creates a copy of the Image Resources
 *			it is given.
 */
void Pointer::setPointerImage(Image *image)
{
	if(!image)
	{
		SDL_ShowCursor(true);
		return;
	}
	
	if(mPointerImage)
	{
		delete mPointerImage;
		mPointerImage = NULL;
	}

	SDL_ShowCursor(false);
	mPointerImage = new Image(*image);
}