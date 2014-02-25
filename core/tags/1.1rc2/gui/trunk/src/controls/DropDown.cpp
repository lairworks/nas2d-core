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

#include "DropDown.h"


DropDown::DropDown(const string& name, int width, int height):	Control(name)
{
	setSize(width, height);
}


void DropDown::initialize()
{
	setSkinState("DropDown");
	setHeight(325);

	mListButton = new Button("", "btn_ListShow", mFont->getHeight() + 4, mFont->getHeight() + 4);
	add(mListButton);
	mListButton->setRelative(getWidth() - mListButton->getWidth(), 0);
	mListButton->setSkinPartType("ScrollButtonDown");
	mListButton->setSkinState(STATE_NORMAL);
	
	mTextField = new TextField("txt_Default", "", getWidth() - mListButton->getWidth(), mFont->getHeight() + 4);
	add(mTextField);
	mTextField->setRelative(0, 0);
	mTextField->isEditable(false);
	
	mOptionList = new ListBox("lst_DropDown", getWidth(), getHeight() - mTextField->getHeight());
	add(mOptionList);
	mOptionList->setRelative(0, mTextField->getHeight() + 4);
	mOptionList->isHidden(true);

	
	drawBorder(true);
}


void DropDown::logic()
{}


void DropDown::addItem(ListItem *item)
{
	if(item != NULL)
		mOptionList->addItem(item);
}



void DropDown::addItem(StringList &strList)
{
	if(!strList.empty())
		mOptionList->addItem(strList);
}



void DropDown::drawSkin()
{}


void DropDown::drawPlain()
{}