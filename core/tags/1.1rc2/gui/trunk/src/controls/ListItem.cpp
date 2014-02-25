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

#include "ListItem.h"

const int MIN_WIDTH			= 8;
const int MIN_HEIGHT		= 8;

const int DEFAULT_BG_RED	= 100;
const int DEFAULT_BG_GREEN	= 100;
const int DEFAULT_BG_BLUE	= 255;

ListItem::ListItem(const string& name, Image *icon, ListItemType type):	Control(name, MIN_WIDTH, MIN_HEIGHT),
																		mIsSelected(false),
																		mIcon(NULL),
																		mType(type)
{
	//setSize(width, height);
	if(icon)
		mIcon = new Image(*icon);
}


ListItem::ListItem(const ListItem &src):	Control(src),
											mIsSelected(src.mIsSelected),
											mIcon(new Image(*src.mIcon)),
											mType(src.mType)
{
}


ListItem::~ListItem()
{
	if(mIcon)
	{
		delete mIcon;
		mIcon = NULL;
	}
}

void ListItem::initialize()
{
	setMargins(1, 0, 1, 0);
	setBackgroundColor(DEFAULT_BG_RED, DEFAULT_BG_GREEN, DEFAULT_BG_BLUE);

	if(mFont)
		setSize(mWidth, mFont->getHeight() + mTop + mBottom);

	if(mIcon)
		setSize(mWidth, mIcon->getHeight());

	setSkinState("ListItem");
}


void ListItem::isSelected(bool selected)
{
	mIsSelected = selected;

	mIsSelected ? setSkinState("ListItem", STATE_SELECTED) : setSkinState("ListItem");
}


bool ListItem::isSelected() const
{
	return mIsSelected;
}


void ListItem::drawSkin()
{
	drawPlain();
}


void ListItem::drawPlain()
{
	// Here for now just to show where the box is.
	//Singleton<Renderer>::get().drawBox(getAbsoluteX(), getAbsoluteY(), mWidth, mHeight, 200, 200, 200);

	if(mIsSelected)
		Singleton<Renderer>::get().drawBoxFilled(getAbsoluteX(), getAbsoluteY(), mWidth, mHeight, mBackgroundColor.red(), mBackgroundColor.green(), mBackgroundColor.blue());

	switch(mType)
	{
		case LIST_TEXT_ONLY:
			Singleton<Renderer>::get().drawTextClamped(*mFont, mName, getAbsoluteX() + mLeft, getAbsoluteY() + mTop, 0, 0, mWidth - mLeft - mRight, mHeight - mTop - mBottom, mForegroundColor.red(), mForegroundColor.green(), mForegroundColor.green());
			break;
		case LIST_ICON_ONLY:
			Singleton<Renderer>::get().drawSubImage(*mIcon, getAbsoluteX(), getAbsoluteY(), 0, 0, getWidth(), getHeight());
			break;
		case LIST_TEXT_ICON:
			Singleton<Renderer>::get().drawSubImage(*mIcon, getAbsoluteX(), getAbsoluteY(), 0, 0, getWidth(), getHeight());
			Singleton<Renderer>::get().drawTextClamped(*mFont, mName, getAbsoluteX() + mLeft + mIcon->getWidth() + 1, getAbsoluteY() + (mIcon->getHeight() >> 1) - (mFont->getHeight() >> 1), 0, 0, mWidth - mLeft - mRight - mIcon->getWidth() - 1, mHeight - mTop - mBottom, mForegroundColor.red(), mForegroundColor.green(), mForegroundColor.green());
			break;
		default:
			stringstream errMsg;
			errMsg << "An invalid ListItem type was specified: " << __FILE__;
			throw Exception(1, "Invalid List Item", errMsg.str());
	}
}