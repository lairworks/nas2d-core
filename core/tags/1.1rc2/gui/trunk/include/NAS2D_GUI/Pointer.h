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

#ifndef _LOM_GUI_POINTER_
#define _LOM_GUI_POINTER_

#include "NAS2D/Resources/Image.h"

#include "SDL/SDL.h"

class Pointer
{
public:
	Pointer(Image *image, int hotspotX = 0, int hotspotY = 0);
	~Pointer();
	
	int getX() const { return mMouseX; }
	int getY() const { return mMouseY; }

	int getHotspotX() const { return mHotspotX; }
	int getHotspotY() const { return mHotspotY; }

	void setHotspot(int x, int y) { mHotspotX = x; mHotspotY = y; }
	
	void update();

	void setPointerImage(Image *image);
	Image *getPointerImage() const;
	
private:
	Image		*mPointerImage;

	int			mMouseX;
	int			mMouseY;

	int			mHotspotX;
	int			mHotspotY;
};
#endif

