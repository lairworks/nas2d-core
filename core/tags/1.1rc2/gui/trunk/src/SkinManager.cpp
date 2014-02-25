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

#include "SkinManager.h"

#include "NAS2D/Singleton.h"
#include "NAS2D/Filesystem.h"


/**
 * 
 */
SkinManager::SkinManager(const string& skinPath)
{
	std::cout << "Initializing Skin Manager..." << endl;
	
	scanSkins(skinPath);
}


/**
 * 
 */
SkinManager::~SkinManager()
{
	/*
	if(mSkinsFile)
	{
		delete mSkinsFile;
		mSkinsFile = NULL;
	}
	*/

	if(!mSkinList.empty())
	{
		std::map<string, Skin*>::iterator skinIt = mSkinList.begin();

		while(skinIt != mSkinList.end())
		{
			delete skinIt->second;
			skinIt->second = NULL;
			++skinIt;
		}
	}
}


/**
 * Scans a search path and adds skin definitions to the list of available skins.
 */
int SkinManager::scanSkins(const string& searchPath)
{
	StringList tmpLst = Singleton<Filesystem>::get().enumerateDir(searchPath);
	
	// If no skins were found, spit out a warning message and stop processing.
	if(tmpLst.size() == 0)
	{
		std::cout << "\tNo Skins found in '" << Singleton<Filesystem>::get().getDataPath() << searchPath << "." << endl;
		return -1;
	}
	
	// Scan through the list of returned results for directories and attempt
	// to load skins found in said directories.
	for(size_t i = 0; i < tmpLst.size(); i++)
	{
		std::stringstream tmpPath;
		tmpPath << searchPath << "/" << tmpLst[i];
		if(Singleton<Filesystem>::get().isDirectory(tmpPath.str()))
		{
			// Tack on the file we're searching for to the current search path.
			tmpPath << "/" << "skin.xml";
			Skin *tmpSkin = new Skin(tmpPath.str());
			if(tmpSkin->isLoaded())
			{
				mSkinList[tmpSkin->getSkinName()] = tmpSkin;
				//std::cout << Logger::info() << "Skin Name: " << tmpSkin->getName() << endl;
			}
			else
			{
				// Skin didn't load so output any error messages and then free memory allocated
				// for the skin.
				std::cout << "\tCouldn't load '" << tmpPath.str() << "': " << tmpSkin->getErrorMessage() << endl;
				delete tmpSkin;
			}
		}
	}
	
	// If no skins were added to the list, log a warning.
	if(mSkinList.size() < 1)
	{
		std::cout << "\tNo skin files were found in '" << searchPath << "'." << endl;
	}
	// Otherwise, log which skins we've found.
	else
	{
		std::cout << "\tSuccessfully added the following skins:" << endl;
		map<string, Skin*>::iterator i = mSkinList.begin();
		for(i; i != mSkinList.end(); i++)
			std::cout << "\t\t- " << i->first << endl;
	}
	
	return 0;
}


/**
 * Gets a skin by name from the available skins.
 * 
 * \param	name	Name of the Skin to retrieve.
 *
 * \return	A pointer to a Skin Resource if the Skin was found. Otherwise it returns the first
 *			available Skin. If there are no available skins, NULL will be returned.
 * 
 * \warning	The pointer returned is owned by the SkinManager and should not be deleted by the user.
 */
Skin* SkinManager::getSkin(const string& name)
{
	std::map<string, Skin*>::iterator i = mSkinList.find(name);

	if(i != mSkinList.end())
	{
		//std::cout << Logger::info() << "Selected skin '" << name << "'." << endl;
		return i->second;
	}
	else
	{
		std::cout << "WARN: " << "Could not find requested skin '" << name << "'." << endl;
		
		// If the list is empty, return NULL.
		if(mSkinList.size() < 1)
			return NULL;
		else
			return mSkinList.begin()->second;
	}
}