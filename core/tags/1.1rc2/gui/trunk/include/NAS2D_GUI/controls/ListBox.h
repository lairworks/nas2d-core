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

#ifndef _LOM_GUI_LISTBOX_
#define _LOM_GUI_LISTBOX_

#include "ScrollArea.h"
#include "ListItem.h"

enum ListBoxType
{
	LIST_SINGLE,
	LIST_MULTI
};

/**
 * \class ListBox
 * \brief A Basic ListBox.
 *
 * The ListBox contains a list of ListItem objects that the user
 * can select from.
 * 
 * \note	One of ListBox's limitations is that no two items can
 *			have the same name.
 */
class ListBox: public ScrollArea
{
public:
	ListBox(const std::string& name, int width, int height, ListBoxType type = LIST_SINGLE);
	
	~ListBox()
	{}
	
	StringList getList() const;
	StringList getSelection() const;

	int getSelectionIndex() const { return mCurrentSelection; }

	void addItem(ListItem *item);
	void addItem(const std::string& item);
	void addItem(const StringList& strList);

	void removeItem(const std::string& item);
	void removeItem(int index);

	void setSelection(const std::string& ref);
	void setSelection(int index);

	void clearSelection();
	void clear();
	
private:
	typedef vector<ListItem*>	ItemList;
	
	void scrollAreaInit();
	void areaChanged();

	void drawSkin();
	void drawPlain();
	void drawList();
	
	void onClick();
	void onKeyDown();
	void onKeyUp();
	
	
	ItemList	mItemList;			/**< List of ListItem objects. */
	StringList	mTextList;			/**< Vector of text to list. */
	ListBoxType	mType;				/**< Type of ListBox. */
	
	int		mLineSelection;		/**< Position of the Insertion Cursor. */
	int		mMaxNumItems;		/**< COMMENT ME! */
	int		mCurrentLine;		/**< Current line based on scrollbar's position. */
	int		mPreviousLine;		/**< Previous line based on scrollbar's position. */

	int		mCurrentSelection;	/**< Index of the currently selected item. */
	
	bool		mCtrlPressed;
};

#endif