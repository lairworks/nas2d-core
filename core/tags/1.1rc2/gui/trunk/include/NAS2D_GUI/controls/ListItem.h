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

#ifndef _LOM_GUI_LISTITEM_
#define _LOM_GUI_LISTITEM_

#include "Control.h"


enum ListItemType
{
	LIST_TEXT_ONLY,	/**< Text only list item. */
	LIST_ICON_ONLY,	/**< Icon only list item. */
	LIST_TEXT_ICON	/**< Text & Icon list item. */
};


class ListItem : public Control
{
public:
	ListItem(const std::string& name, Image *icon = NULL, ListItemType type = LIST_TEXT_ONLY);
	ListItem(const ListItem &src);
	~ListItem();

	void isSelected(bool selected);
	bool isSelected() const;
	
private:
	friend class ListBox;

	void initialize();
	void drawSkin();
	void drawPlain();
	
	bool			mIsSelected;
	
	Image			*mIcon;
	
	ListItemType	mType;

};

#endif