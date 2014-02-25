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

#include "ListBox.h"


/**
 * Constructor
 *
 * \param	name	String defining the name of the TextField.
 * \param	text	String containing the desired initial text.
 * \param	width	Width of the TextField.
 * \param	height	Height of the TextField.
 */
ListBox::ListBox(const string& name, int width, int height, ListBoxType type):	ScrollArea(name, width, height),
																					mLineSelection(0),
																					mMaxNumItems(0),
																					mCurrentLine(0),
																					mPreviousLine(0),
																					mType(type),
																					mCurrentSelection(0),
																					mCtrlPressed(false)
{
	setSize(width, height);
}


/**
 * Ensure that we're automatically setting our height/width parameters.
 */
void ListBox::scrollAreaInit()
{
	mMaxNumItems = mHeight / mFont->getHeight();

	setSkinState("TextBox");
	
	if(mFont && mAutosize)
		setSize(mFont->getWidth(mTextList.back()) + mScrollBar->getWidth(), mHeight);
	
	// This should be set by the Skin.
	setMargins(2, 2, 2, 2);
}


/**
 * Adds a ListItem to the ListBox.
 *
 * \param	item	A pointer to a ListItem box.
 *
 * \note	Any ListItem objects that are added to the ListBox are then
 *			owned by the ListBox and should not be deleted.
 */
void ListBox::addItem(ListItem *item)
{
	if(!item)
	{
		std::cout << "WARN: " << "Attempting to add a NULL item to ListBox: " << mName << endl;
		return;
	}

	item->setWidth(mWidth - mScrollBar->getWidth() - mLeft - mRight);

	
	// Save the number of children for future reference.
	int listSize = mChildControls.size();

	// Attempt to add it.
	add(item);

	// Check that the list item was successfully added. If not
	// return and ignore the add request.
	if(listSize == mChildControls.size())
		return;

	mItemList.push_back(item);

	int height = mItemList[0]->getHeight();

	item->setRelative(mLeft, mTop + (height * (mItemList.size() - 1)));

	item->setSize(mWidth - mLeft - mRight - mScrollBar->getWidth(), 0);
	
	int scrollUnits = mItemList.size() - mMaxNumItems;
	(scrollUnits > 0) ? mScrollBar->setStepUnits(scrollUnits) : mScrollBar->setStepUnits(0);

	mMaxNumItems = mHeight / item->getHeight();
}


/**
 * Adds a single new text-only item to the list.
 */
void ListBox::addItem(const std::string& item)
{
	if(item.empty())
		return;

	addItem(new ListItem(item));
}


/**
 * Removes a ListItem from the ListBox by index.
 * 
 * \param	index	Selection index value.
 * 
 * \note	Specifying an index out of range will be
 *			ignored.
 */
void ListBox::removeItem(int index)
{
	if(index >= mItemList.size())
		return;

	remove(mItemList[index]);
	mItemList.erase(mItemList.begin() + index);
}


/**
 * Removes a ListItem from the ListBox by name.
 * 
 * \param	item	A string containing the name of the ListItem.
 * 
 * \note	If /c item is not found, this call will be ignored.
 */
void ListBox::removeItem(const std::string& item)
{
	for(size_t i = 0; i < mItemList.size(); i++)
	{
		if(mItemList[i]->getName() == item)
			removeItem(i);
	}
}


/**
 * Sets the selection using a \c std::string reference.
 *
 * \param	ref		A reference to a \c std::string.
 */
void ListBox::setSelection(const std::string &ref)
{
	for(size_t i = 0; i < mItemList.size(); i++)
	{
		if(mItemList[i]->getName() == ref)
		{
			mItemList[i]->isSelected(true);
			return;
		}
	}
	clearSelection();
}


/**
 * Sets the selection using an index.
 *
 * \param index	Index of the list item to set to selected.
 *
 * \note	Clears all other selections.
 * \note	If the index is out of range, the request will be ignored.
 * 
 */
void ListBox::setSelection(int index)
{
	if(static_cast<size_t>(index) >= mItemList.size())
		return;

	clearSelection();

	mItemList[index]->isSelected(true);
}


/**
 * Adds a set of text-only items based on a string list.
 *
 * \param	strList	A reference to a StringList.
 */
void ListBox::addItem(const StringList& strList)
{
	// Convert each string here into a list item and then push each
	// of those into the list box using the add(ListItem*) function.

	if(strList.empty())
		return;

	for(size_t i = 0; i < strList.size(); i++)
		addItem(new ListItem(strList[i]));
}


/**
 * Returns a list of all items in the ListBox.
 */
StringList ListBox::getList() const
{
	StringList tempList;
	for(size_t i = 0; i < mItemList.size(); i++)
			tempList.push_back(mItemList[i]->getName());

	return tempList;
}


/**
 * Gets a list of the currently selected items.
 */
StringList ListBox::getSelection() const
{
	StringList tempList;
	for(size_t i = 0; i < mItemList.size(); i++)
	{
		if(mItemList[i]->isSelected())
			tempList.push_back(mItemList[i]->getName());
	}
	return tempList;
}


/**
 * Clears the current selection.
 */
void ListBox::clearSelection()
{
	if(mItemList.empty())
		return;

	for(size_t i = 0; i < mItemList.size(); i++)
		mItemList[i]->isSelected(false);
}


/**
 * Clears the ListBox of all items.
 *
 * \warning	Clearing the ListBox also invalidates any pointers or references
 *			to any of the stored ListItem objects.
 */
void ListBox::clear()
{
	clearSelection();

	if(mItemList.empty())
		return;

	for(size_t i = 0; i < mItemList.size(); i++)
		remove(mItemList[i]);

	mItemList.clear();

	mScrollBar->setPosition(0.0);
}


/**
 * \todo	Comment me.
 */
void ListBox::areaChanged()
{
	mPreviousLine = mCurrentLine;
	mCurrentLine = (mItemList.size() - mMaxNumItems) * mScrollBar->getPosition();
	
	for(size_t i = 0; i < mItemList.size(); i++)
	{
		if(mPreviousLine < mCurrentLine)
		{
			//std::cout << "mPreviousLine: " << mPreviousLine << "\tmCurrentLine: " << mCurrentLine << endl;
			mItemList[i]->setRelative(mItemList[i]->getRelativeX(), mItemList[i]->getRelativeY() - ((mCurrentLine - mPreviousLine) * mItemList[i]->getHeight()));
		}
		else if(mPreviousLine > mCurrentLine)
		{
			//std::cout << "mPreviousLine: " << mPreviousLine << "\tmCurrentLine: " << mCurrentLine << endl;
			mItemList[i]->setRelative(mItemList[i]->getRelativeX(), mItemList[i]->getRelativeY() + ((mPreviousLine - mCurrentLine) * mItemList[i]->getHeight()));
		}
	}
}


/**
 * Render this control.
 */
void ListBox::drawSkin()
{
	Singleton<Renderer>::get().drawImageRect(getAbsoluteX(), getAbsoluteY(), mWidth - mScrollBar->getWidth(), mHeight, mCurrentSkinPart->getImageList());

	drawList();
}


void ListBox::drawPlain()
{
	Singleton<Renderer>::get().drawBoxFilled(getAbsoluteX(), getAbsoluteY(), mWidth - mScrollBar->getWidth(), mHeight, mBackgroundColor.red(), mBackgroundColor.green(), mBackgroundColor.blue());
	Singleton<Renderer>::get().drawBox(getAbsoluteX(), getAbsoluteY(), mWidth - mScrollBar->getWidth(), mHeight, mBorderColor.red(), mBorderColor.green(), mBorderColor.blue());
	
	drawList();
}


/**
 * Draws the list.
 */
void ListBox::drawList()
{
	int numLines = mItemList.size();
	
	if(mCurrentLine > numLines - mMaxNumItems)
		mCurrentLine = numLines - mMaxNumItems;
	
	for(size_t i = 0; i < numLines; ++i)
	{
		if (i - mCurrentLine < mMaxNumItems) 
			mItemList[i]->isVisible(true);
		else
			mItemList[i]->isVisible(false);
	}	
}


void ListBox::onClick()
{
//	int mouseX = event->getFirst();
//	int mouseY = event->getSecond();
//	
//	for(size_t i = 0; i < mItemList.size(); i++)
//	{
//		if (isPointInRect(mouseX, mouseY, mItemList[i]->getAbsoluteX(), mItemList[i]->getAbsoluteY(), mItemList[i]->getWidth(), mItemList[i]->getHeight()))
//		{
//			mItemList[i]->isSelected(true);
//			mCurrentSelection = i;
//
//			// Inform any listeners that a selection was made
//			Event* tmpEvent = new Event(EVENT_GUI_SELECTED, 0, 0, mName);
//			onNotify(tmpEvent);
//		}
//		else if(!isPointInRect(mouseX, mouseY, mScrollBar->getAbsoluteX(), mScrollBar->getAbsoluteY(), mScrollBar->getWidth(), mScrollBar->getHeight()))
//		{
//			if(mType == LIST_MULTI)
//			{
//				if(!mCtrlPressed)
//					mItemList[i]->isSelected(false);
//			}
//			else
//				mItemList[i]->isSelected(false);
//			
//			event->consume();
//		}
//	}
}


void ListBox::onKeyDown()
{
//	if(hasFocus() && (mType == LIST_MULTI))
//	{
//		if((event->getFirst() == KEY_LCTRL) || (event->getFirst() == KEY_RCTRL))
//			mCtrlPressed = true;
//	}
}


void ListBox::onKeyUp()
{
//	if((mType == LIST_MULTI) && (event->getFirst() == KEY_LCTRL || event->getFirst() == KEY_RCTRL))
//		mCtrlPressed = false;
}