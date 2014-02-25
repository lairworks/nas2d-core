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

#include "Gui.h"

/**
 * C'tor
 */
Gui::Gui():	mSkinManager(new SkinManager()),
mSkin(NULL),
mMouseX(0), mMouseY(0),
mMouseOldX(0), mMouseOldY(0),
mMouseLeftPressed(false),
mMouseMiddlePressed(false),
mMouseRightPressed(false),
mIsActive(true),
mPointer(new Pointer(NULL)),
mCustomPointer(false)
{
	std::cout << "Starting GUI..." << endl;
	scanFonts("fonts");
	
	Singleton<EventHandler>::get().event(SDL_KEYDOWN).connect(this, &Gui::onKeyDown);
	Singleton<EventHandler>::get().event(SDL_KEYUP).connect(this, &Gui::onKeyUp);
	
	Singleton<EventHandler>::get().event(SDL_MOUSEMOTION).connect(this, &Gui::onMouseMove);
	Singleton<EventHandler>::get().event(SDL_MOUSEBUTTONDOWN).connect(this, &Gui::onMouseButtonDown);
	Singleton<EventHandler>::get().event(SDL_MOUSEBUTTONUP).connect(this, &Gui::onMouseButtonUp);
}


/**
 * D'tor
 */
Gui::~Gui()
{
	// Instruct the Controls to clear themselves.
	clear();
    
	// Iterate through all of the Font's in our list of Font's
	// and free the memory allocated for them.
	map<string, Font*>::iterator i = mFontList.begin();
	for(i; i != mFontList.end(); i++)
	{
		delete i->second;
		i->second = NULL;
	}
    
	if(mSkinManager)
	{
		delete mSkinManager;
		mSkinManager = NULL;
	}
    
	if(mPointer)
	{
		delete mPointer;
		mPointer = NULL;
	}
    
	std::cout << "GUI Terminated." << endl;
}


/**
 * Adds a Control to the Gui.
 *
 * \param	control	A pointer to a Control.
 * 
 * \note	Passing a NULL Control will be safely ignored.
 */
void Gui::add(Control *control)
{
	if(control)
	{
		// Avoid Control's with duplicate names.
		ControlList::iterator it = mControlList.begin();
		while(it != mControlList.end())
		{
			if(control->getName() == (*it)->getName())
			{
				std::cout << "WARN: " << "Attempted to add Control with duplicate name: " << control->getName() << endl;
				return;
			}
            
			it++;
		}
        
		// Unfocus all other controls.
		for(ControlList::iterator it = mControlList.begin(); it != mControlList.end(); it++)
			(*it)->hasFocus(false);
        
		// Add Control to the end of the list.
		mControlList.push_front(control);
        
		control->setFontList(mFontList);
		control->setSkin(mSkin);
        
		// Tell the new Control to initialize itself.
		control->initialize();
        
		control->hasFocus(true);
        
		mNumControls = mControlList.size();
		
		KeyDown.connect(control, &Control::onKeyDown);
		KeyUp.connect(control, &Control::onKeyUp);
		
		MouseClick.connect(control, &Control::onClick_b);
		MouseDown.connect(control, &Control::onMouseButtonDown_b);
		MouseUp.connect(control, &Control::onMouseButtonUp_b);
		MouseDrag.connect(control, &Control::onDrag_b);
	}
	else
	{
#if defined(_DEBUG)
		std::cout << "WARN: " << "Attmepting to pass a NULL control pointer to Gui::add()" << endl;
#endif
	}
    
}


/**
 * Removes a Control from the Gui.
 *
 * \param	control		A pointer to a Control.
 * 
 * \note	Passing a NULL Control will be safely ignored.
 */
void Gui::remove(Control *control)
{
	if(control)
	{
		ControlList::iterator it = mControlList.begin();
		while(it != mControlList.end())
		{
			if((*it) == control)
			{
				delete (*it);
				mControlList.erase(it);
				return;
			}
            
			++it;
		}
	}
	else
	{
#if defined(_DEBUG)
		std::cout << "WARN: " << "Attmepting to pass a NULL control pointer to Gui::remove()" << endl;
#endif
	}
}


/**
 * Finds a Control by name.
 *
 * \param	controlName	A string containing the name of the Control. Not case sensitive.
 *
 * \return	A pointer to a Control if found. NULL otherwise.
 * 
 */
Control* Gui::find(const string& controlName) const
{
	ControlList::const_iterator it = mControlList.begin();
    
	while(it != mControlList.end())
	{
		if((*(*it)) == controlName)
			return (*it);
        
		++it;
	}
    
	return NULL;
}


/**
 * Brings the specified Control to the top of the Control stack.
 *
 * \param	control	A pointer to a Control.
 *
 * \note	This function does not set focus.
 * 
 * \note	Passing a NULL Control will be safely ignored.
 */
void Gui::sendToFront(Control *control)
{
	if(!control)
		return;
    
	ControlList::iterator it = mControlList.begin();
	while(it != mControlList.end())
	{
		if((*it) == control)
		{
			// I'm not convinced this is correct but it does prevent the iterator
			// invalidation error and it does appear to work.
			Control *tmpControl = mControlList.front();
			mControlList.front() = control;
			*it = tmpControl;
			return;
		}
        
		++it;
	}
}


/**
 * Returns a list of Font names currently loaded into the Gui system.
 *
 * \return	StringList containing the names of fonts. If there are
 *			no fonts in the system an empty StringList is returned.
 */
StringList Gui::getFontList() const
{
	StringList returnList;
    
	FontList::const_iterator it = mFontList.begin();
	while(it != mFontList.end())
	{
		returnList.push_back(it->first);
		++it;
	}
    
	return returnList;
}


/**
 * Gets a Font by name.
 *
 * \param	fontName	Name of the font to get.
 *
 * \return	A pointer to a Font if found. NULL if Font was not found.
 *
 * \note	Names are case sensitive so "MyFont", "myfont" and "mYfONt"
 *			are not the same identifiers.
 */
Font* Gui::getFont(const string& fontName) const
{
	FontList::const_iterator it = mFontList.begin();
    
	while(it != mFontList.end())
	{
		if(toLowercase(fontName) == toLowercase(it->second->getFontName()))
			return it->second;
        
		++it;
	}
    
	return NULL;
}


/**
 * Updated the GUI state.
 *
 * \return	Returns \c false if the GUI received an EVENT_SYS_QUIT message.
 */
bool Gui::update()
{
	//handler().pump();
	
	ControlList::reverse_iterator it = mControlList.rbegin();
    
	while(it != mControlList.rend())
	{
		(*it)->update();
		it++;
	}
    
	it = mControlList.rbegin();
    
	while(it != mControlList.rend())
	{
		(*it)->draw();
		it++;
	}
	
	if(mCustomPointer)
	{
		mPointer->update();
		drawPointer();
	}
    
	return mIsActive;
}


/**
 * Cleans up all GUI elements and refreshes the GUI
 * to a clean state with no Controls.
 */
void Gui::clear()
{
	ControlList::iterator it = mControlList.begin();
    
	while(it != mControlList.end())
	{
		(*it)->clearChildren();
		delete (*it);
		(*it) = NULL;
		mControlList.erase(it++);
	}
    
	setFocus(false);
}


/**
 * Parses a specified directory within the main data path for TrueType font files.
 * 
 * \note	'.fon' files have not been tested and may not work properly if at all.
 * 
 */
void Gui::scanFonts(const string& searchPath)
{
	std::cout << "\tSearching for fonts... ";
    
	StringList ttfList = Singleton<Filesystem>::get().enumerateDir(searchPath, "ttf");
	StringList otfList = Singleton<Filesystem>::get().enumerateDir(searchPath, "otf");
    
	vector<StringList> fontList;
	fontList.push_back(ttfList);
	fontList.push_back(otfList);
    
	// If no fonts were found, spit out a warning message and stop processing.
	if(ttfList.empty() && otfList.empty())
	{
		std::cout << endl << "\tNo font's found in '" << Singleton<Filesystem>::get().getDataPath() << "fonts'." << endl << "\tProviding a default font." << endl;
		
		Font *tmpFont = new Font();
		if(tmpFont)
			mFontList[tmpFont->getFontName()] = tmpFont;
        
		return;
	}
    
	
	for(size_t i = 0; i < fontList.size(); i++)
	{
		if(!fontList[i].empty())
		{
			for(size_t j = 0; j < fontList[i].size(); j++)
			{
				std::stringstream tmpPath;
				tmpPath << searchPath << "/" << fontList[i][j];	
				Font *tmpFont = new Font(tmpPath.str(), 12);
				if(tmpFont->isLoaded())
					mFontList[tmpFont->getFontName()] = tmpFont;
				else
					delete tmpFont;
			}
		}
	}
    
	std::cout << "Done!" << endl;
	std::cout << "\tSuccessfully added the following fonts:" << endl;
	map<string, Font*>::iterator i = mFontList.begin();
	for(i; i != mFontList.end(); i++)
		std::cout << "\t\t- " << i->first << endl;
}


/**
 * Sets whether or not the GUI has focus.
 * 
 * \param	focus	Boolean value indicating GUI focus.
 */
void Gui::setFocus(bool focus)
{
	if(focus && !mControlList.empty())
		(*mControlList.begin())->hasFocus(true);
	else
	{
		for(ControlList::iterator it = mControlList.begin(); it != mControlList.end(); it++)
			(*it)->hasFocus(false);
	}
}


/**
 * Sets a GUI Theme by theme name.
 * 
 * \param	theme	A string representing the name of the theme to use.
 * 
 * \note	If the named Skin isn't found in the list of skins
 *			scanned at startup, a default skin will be provided.
 *			If no default skin was found, the GUI will revert to
 *			using primitives to draw itself.
 */
void Gui::setSkinTheme(const std::string& theme)
{
	mSkin = mSkinManager->getSkin(theme);
    
	ControlList::iterator it = mControlList.begin();
    
	while(it != mControlList.end())
	{
		(*it)->setSkin(mSkin);
	}
	
}


/**
 * Changes the state of the mouse pointer.
 * 
 * \param	state	A MousePointerState to set the mouse pointer to.
 */
void Gui::setPointer(MousePointerState state)
{
	if(mSkin)
	{
		mCustomPointer = true;
		mPointer->setPointerImage(&mSkin->getMousePointer(state));
	}
	else
	{
		mCustomPointer = false;
	}
}


/**
 * Draws the mouse pointer.
 */
void Gui::drawPointer()
{
	Singleton<Renderer>::get().drawImage(*mPointer->getPointerImage(), getMouseX(), getMouseY());
}


/**
 * Grabs mouse motion events and sets internal mouse pointer coords.
 */
void Gui::onKeyDown(SDL_Event& event)
{	
	ControlList::iterator it = mControlList.begin();
	while(it != mControlList.end())
	{
		testForFocus();
		
		if(!controlHasFocus())
			return;
		
		KeyDown.emit(event);
		++it;
	}
}


/**
 * Grabs mouse motion events and sets internal mouse pointer coords.
 */
void Gui::onKeyUp(SDL_Event& event)
{	
	ControlList::iterator it = mControlList.begin();
	while(it != mControlList.end())
	{
		testForFocus();
		
		if(!controlHasFocus())
			return;
		
		KeyUp.emit(event);
		++it;
	}
}


/**
 * Grabs mouse motion events and sets internal mouse pointer coords.
 */
void Gui::onMouseMove(SDL_Event& event)
{
	mMouseOldX = mMouseX; mMouseOldY = mMouseY;
	mMouseX = event.motion.x;
	mMouseY = event.motion.y;
	
	if(mMouseLeftPressed)
		MouseDrag.emit(event);
	else
		MouseMove.emit(event);
}


/**
 * Grabs mouse motion events and sets internal mouse pointer coords.
 */
void Gui::onMouseButtonUp(SDL_Event& event)
{
	if(event.button.button == SDL_BUTTON_LEFT)
	{
		mMouseLeftPressed = false;
	}
	else if(event.button.button == SDL_BUTTON_RIGHT)
	{
		mMouseRightPressed = false;
	}
	
	ControlList::iterator it = mControlList.begin();
	while(it != mControlList.end())
	{
		if(isPointInRect(event.motion.x, event.motion.y, (*it)->getAbsoluteX(), (*it)->getAbsoluteY(), (*it)->getWidth(), (*it)->getHeight()) && (*it)->isVisible() && !(*it)->isHidden())
		{
			std::cout << (*it)->getName() << " received a click event..." << endl;
			MouseClick.emit(event);
		}
		else
			MouseUp.emit(event);
		
		++it;
	}
}


/**
 * Grabs mouse motion events and sets internal mouse pointer coords.
 */
void Gui::onMouseButtonDown(SDL_Event& event)
{	
	if(event.button.button == SDL_BUTTON_LEFT || event.button.button == SDL_BUTTON_RIGHT)
		testForFocus();
	
	if(!controlHasFocus())
		return;
	
	if(event.button.button == SDL_BUTTON_LEFT)
		mMouseLeftPressed = true;
	else if(event.button.button == SDL_BUTTON_RIGHT)
		mMouseRightPressed = true;
	
	ControlList::iterator it = mControlList.begin();
	while(it != mControlList.end())
	{
		if(isPointInRect(mMouseX, mMouseY, (*it)->getAbsoluteX(), (*it)->getAbsoluteY(), (*it)->getWidth(), (*it)->getHeight()) && (*it)->isVisible() && !(*it)->isHidden())
		{
			MouseDown.emit(event);
		}
		
		++it;
	}
}

/**
 * Scans through all Control's to check for focus.
 *
 * \return	\c true if any Controls have focus, otherwise \c false.
 *
 * \todo	I hate the name of this function. We need to come up with
 *			a better name.
 */
bool Gui::controlHasFocus()
{
	ControlList::iterator it = mControlList.begin();
	while(it != mControlList.end())
	{
		if((*it)->hasFocus())
			return true;
        
		++it;
	}
    
	return false;
}


/**
 * Tests to see if a mouse click occured within a Control in the
 * Control List and, if yes, sets focus for that Control.
 */
void Gui::testForFocus()
{
	ControlList::iterator it = mControlList.begin();
	while(it != mControlList.end())
	{
		if(isPointInRect(mMouseX, mMouseY, (*it)->getAbsoluteX(), (*it)->getAbsoluteY(), (*it)->getWidth(), (*it)->getHeight()) && (*it)->isVisible() && !(*it)->isHidden())
		{
			(*it)->hasFocus(true);
			mControlList.push_front((*it));
			mControlList.erase(it++);
			// Ignore the rest of the testing, set the rest of the control's to unfocused and move on.
			while(it != mControlList.end())
			{
				(*it)->hasFocus(false);
				++it;
			}
		}
		else
		{
			(*it)->hasFocus(false);
			++it;
		}
	}
}
