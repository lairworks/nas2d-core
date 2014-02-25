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

#include "SpinBox.h"


SpinBox::SpinBox(const string& name, int width, int height):	Container(name),
																mValue(0)
{
	setSize(width, height);
}


void SpinBox::setValue(int value)
{
	mValue = value;
	mInputField->setText(mValue);
}


int SpinBox::getValue()
{
	return mValue;
}


void SpinBox::initialize()
{
	mIncreaseBtn = new Button("", "btnIncrease", mFont->getHeight() + 4, mFont->getHeight() + 4);
	add(mIncreaseBtn);
	mIncreaseBtn->setRelative(getWidth() - mFont->getHeight() + 4, 0);
	mIncreaseBtn->setSkinPartType("ScrollButtonUp");
	mIncreaseBtn->setSkinState(STATE_NORMAL);
	
	mDecreaseBtn = new Button("", "btnDecrease", mFont->getHeight() + 4, mFont->getHeight() + 4);
	add(mDecreaseBtn);
	mDecreaseBtn->setRelative(getWidth() - mFont->getHeight() + 4, mIncreaseBtn->getWidth());
	mDecreaseBtn->setSkinPartType("ScrollButtonDown");
	mDecreaseBtn->setSkinState(STATE_NORMAL);
	
	mInputField = new TextField("txt_Value", "", getWidth() - mIncreaseBtn->getWidth(), mIncreaseBtn->getHeight() + mDecreaseBtn->getHeight());
	add(mInputField);
	mInputField->setRelative(0, 0);
	mInputField->setText(mValue);
}


void SpinBox::drawSkin()
{
	
}


void SpinBox::drawPlain()
{
	
}


//void SpinBox::push()
//{
//	if(event->getType() == EVENT_GUI_CLICKED)
//	{
//		if(event->getId() == "btnIncrease")
//		{
//			mValue++;
//			mInputField->setText(mValue);
//		}
//		else if(event->getId() == "btnDecrease")
//		{
//			mValue--;
//			mInputField->setText(mValue);
//		}
//	}
//	
//	
//	Control::push(event);
//}