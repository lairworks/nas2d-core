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

#ifndef _LOM_GUI_SKIN_MANAGER_
#define _LOM_GUI_SKIN_MANAGER_

#include <vector>
#include <map>

#include "NAS2D/Common.h"
#include "NAS2D/Exception.h"
#include "NAS2D/Filesystem.h"

#include "NAS2D/tinyxml/tinyxml.h"

#include "Skin.h"


/**
 * \class SkinManager
 * \brief Scans Skins folder.
 *
 * Scans for skin directories and stores valid directories to a list.
 * Manages Skin resources.
 */
class SkinManager
{
public:
	SkinManager(const std::string& skinPath = "gui");
	~SkinManager();
	
	Skin* getSkin(const string& name);
	
private:
	int scanSkins(const string& searchPath);
	
	void load();
	void parse();
	
	std::map<string, Skin*>	mSkinList;		/**< List of fonts found in the specified fonts directory. */
};

#endif