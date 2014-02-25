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

#include "TextField.h"


const int MIN_WIDTH = 12;
const int MIN_HEIGHT = 12;

const int MAX_WIDTH = 512;
const int MAX_HEIGHT = 32;

const int REPEAT_DELAY = 500;
const int REPEAT_WAIT = 20;


const int CURSOR_BLINK_DELAY = 250;

/**
 * Constructor
 *
 * \param	name	String defining the name of the TextField.
 * \param	text	String containing the desired initial text.
 * \param	width	Width of the TextField.
 * \param	height	Height of the TextField.
 */
TextField::TextField(const string& name, const string& text, int width, int height, int maxLen):	Control(name, MIN_WIDTH, MIN_HEIGHT),
																									mCursorPosition(0),
																									mCursorX(0),
																									mScrollOffset(0),
																									mMaxScrollOffset(0),
																									mMaxLength(maxLen),
																									mLastCursorTime(0),
																									mLastAction(ACTION_NONE),
																									mActionTick(0),
																									mRepeatTick(0),
																									mEditable(true),
																									mShowCursor(false)
{
	setMaximumSize(MAX_WIDTH, MAX_HEIGHT);
	setSize(width, height);
}


/**
 * Sets the text to be displayed within the TextField.
 *
 * \param	text	String containing the desired text to set the TextField to.
 *
 * \note	Calling this function will overwrite any text that may already be
 *			set in the TextField.
 */
void TextField::setText(const string& text)
{	
	mCaption = text;
	mCursorPosition = 0;
}


void TextField::setText(int number)
{	
	stringstream str;
	str << number;
	mCaption = str.str();
	mCursorPosition = 0;
}


const std::string & TextField::getText() const
{
	return mCaption;
}


void TextField::isEditable(bool editable)
{
	mEditable = editable;
}


bool TextField::isEditable()
{
	return mEditable;
}


/**
 * Ensure that we're automatically setting our height/width parameters.
 */
void TextField::initialize()
{
	setSkinState("TextBox");
	
	if(mFont && mAutosize)
		setSize(mFont->getWidth(mCaption), mFont->getHeight());

	// This should be set by the Skin.
	setMargins(2, 2, 2, 2);

	mLastCursorTime = mTimer.ms();
}


/**
 * Render this control.
 */
void TextField::drawSkin()
{
	Singleton<Renderer>::get().drawImageRect(getAbsoluteX(), getAbsoluteY(), mWidth, mHeight, mCurrentSkinPart->getImageList());
	
	Singleton<Renderer>::get().drawTextClamped(*mFont, mCaption, getAbsoluteX() + getMarginLeft() + 1, getAbsoluteY() + mTop, mScrollOffset, 0, getTextAreaWidth(), mHeight - mBottom, mForegroundColor.red(), mForegroundColor.green(), mForegroundColor.green());
	
	drawCursor();
}


void TextField::drawPlain()
{
	Singleton<Renderer>::get().drawTextClamped(*mFont, mCaption, getAbsoluteX() + getMarginLeft() + 1, getAbsoluteY() + mTop, mScrollOffset, 0, getTextAreaWidth(), mHeight - mBottom, mForegroundColor.red(), mForegroundColor.green(), mForegroundColor.green());
	drawCursor();
}


/**
 * Draws the insertion point cursor.
 */
void TextField::drawCursor()
{
	if(hasFocus())
	{
		if(mShowCursor)
		{
			// updateCursor() should be called only on events relating to the cursor so this is temporary.
			updateCursor();
			Singleton<Renderer>::get().drawLine(mCursorX, getAbsoluteY() + mTop, mCursorX, getAbsoluteY() + mHeight - mBottom, mForegroundColor.red(), mForegroundColor.green(), mForegroundColor.green());
		}
		
		if(mTimer.ms() - mLastCursorTime > CURSOR_BLINK_DELAY)
		{
			mLastCursorTime = mTimer.ms();
			mShowCursor = !mShowCursor;
		}
	}
}


/**
 * Draws a highlight over selected text.
 */
void TextField::drawTextHighlight()
{
	Singleton<Renderer>::get().drawBoxFilled(getAbsoluteX() + mLeft, getAbsoluteY(), mFont->getWidth(mCaption), mHeight, 0, 0, 150, 100);
}


void TextField::selectAll()
{
	isTextHighlighted(true);
	mSelectedText = mCaption;
	drawTextHighlight();
}


void TextField::shiftSelect()
{
	//std::cout << "Shift Select" << endl;
}


void TextField::updateCursor()
{
	int cursorX = mFont->getWidth(mCaption.substr(0, mCursorPosition));

	if(cursorX - mScrollOffset >= getTextAreaWidth())
		mScrollOffset = cursorX - getTextAreaWidth();
	if(cursorX - mScrollOffset <= 0)
		mScrollOffset = cursorX - getTextAreaWidth() / 2;

	if(mScrollOffset < 0)
		mScrollOffset = 0;


	mCursorX = getAbsoluteX() + getMarginLeft() + cursorX - mScrollOffset;
}


void TextField::logic()
{
	if((mLastAction == ACTION_NONE) || (mTimer.ms() - mActionTick < REPEAT_DELAY))
		return;

	int currentTick = mTimer.ms();

	if(currentTick - mRepeatTick > REPEAT_WAIT)
	{
		mRepeatTick = currentTick;
		switch(mLastAction)
		{
			case ACTION_INSERT:
				insertCharacter(mInsertChar.c_str());
				break;
			case ACTION_BACKSPACE:
				if(!mCaption.empty() && mCursorPosition > 0)
				{
					mCursorPosition--;
					// STL '.end()' iterators are the end of the container, not the last element.
					mCaption.erase(mCursorPosition, 1);
				}
				break;
			case ACTION_DELETE:
				if(mCaption.length() > 0)
					mCaption = mCaption.erase(mCursorPosition, 1);
				break;
			case ACTION_MOVE_LEFT:
				if(mCursorPosition > 0)
					mCursorPosition--;
				break;
			case ACTION_MOVE_RIGHT:
				if(static_cast<size_t>(mCursorPosition) < mCaption.length())
					mCursorPosition++;
				break;
			default:
				break;
		}
	}
}


void TextField::onKeyDown()
{
//	if(hasFocus() && isEditable())
//	{
//		switch(event->getFirst())
//		{	
//			// SELECTION
//			case KEY_a:
//				if(event->getSecond() == KEY_MOD_LCTRL)
//					selectAll();
//				else
//					insertChar(event);
//				break;
//			case KEY_LSHIFT:
//				shiftSelect();
//				break;
//				
//			// COMMAND KEYS
//			case KEY_BACKSPACE:
//				if(!mCaption.empty() && mCursorPosition > 0)
//				{
//					mCursorPosition--;
//					// STL '.end()' iterators are the end of the container, not the last element.
//					mCaption.erase(mCursorPosition, 1);
//					setAction(ACTION_BACKSPACE);
//				}
//				break;
//			case KEY_HOME:
//				mCursorPosition = 0;
//				break;
//			case KEY_END:
//				mCursorPosition = mCaption.length();
//				break;
//			case KEY_DELETE:
//				if(mCaption.length() > 0)
//				{
//					mCaption = mCaption.erase(mCursorPosition, 1);
//					setAction(ACTION_DELETE);
//				}
//				break;
//
//			// ARROW KEYS
//			case KEY_LEFT:
//				if(mCursorPosition > 0)
//				{
//					isTextHighlighted(false);
//					mCursorPosition--;
//					setAction(ACTION_MOVE_LEFT);
//				}
//				break;
//			case KEY_RIGHT:
//				if(static_cast<size_t>(mCursorPosition) < mCaption.length())
//				{
//					isTextHighlighted(false);
//					mCursorPosition++;
//					setAction(ACTION_MOVE_RIGHT);
//				}
//				break;
//
//			// KEYPAD ARROWS
//			case KEY_KP4:
//				if((mCursorPosition > 0) && !(event->getSecond() & KEY_MOD_NUM))
//				{
//					isTextHighlighted(false);
//					mCursorPosition--;
//					setAction(ACTION_MOVE_LEFT);
//				}
//				else
//					insertChar(event);
//				break;
//			case KEY_KP6:
//				if((static_cast<size_t>(mCursorPosition) < mCaption.length()) && !(event->getSecond() & KEY_MOD_NUM))
//				{
//					isTextHighlighted(false);
//					mCursorPosition++;
//					setAction(ACTION_MOVE_RIGHT);
//				}
//				else
//					insertChar(event);
//				break;
//
//			// IGNORE ENTER/RETURN KEY
//			case KEY_RETURN:
//			case KEY_KP_ENTER:
//				break;
//			// REGULAR KEYS
//			default:
//				insertChar(event);
//				break;
//		}
//	}
}


void TextField::onKeyUp()
{
	mLastAction = ACTION_NONE;
}


void TextField::insertChar()
{
//	mInsertChar = translateKey(*event);
//	insertCharacter(mInsertChar.c_str());
//	setAction(ACTION_INSERT);
}


void TextField::insertCharacter(const char* character)
{
	int prvLen = mCaption.length();
	mCaption = mCaption.insert(mCursorPosition, character);

	if(mCaption.length() - prvLen)
		mCursorPosition++;
}


void TextField::setAction(ActionType aType)
{
	mLastAction = aType;
	mActionTick = mTimer.ms();
}


void TextField::onMouseButtonDown()
{
//	int relativePosition = event->getFirst() - getAbsoluteX();
//
//	// If the click occured past the width of the text, we can immediatly
//	// set the position to the end and move on.
//	if(mFont->getWidth(mCaption.c_str()) < static_cast<int>(relativePosition))
//	{
//		isTextHighlighted(false);
//		mCursorPosition = mCaption.size();
//		return;
//	}
//	else if(event->getSecond() != KEY_MOD_LCTRL)
//		isTextHighlighted(false);
//
//
//	// Figure out where the click occured within the visible string.
//	int i = 0;
//	while(i <= mCaption.size() - mScrollOffset)
//	{
//		string cmpStr = mCaption.substr(mScrollOffset, i);
//		int strLen = mFont->getWidth(cmpStr.c_str());
//		if(strLen > relativePosition)
//		{
//			mCursorPosition = i - 1;
//			break;
//		}
//
//		i++;
//	}
}


void TextField::onChanged()
{
}


void TextField::onLostFocus()
{
	setAction(ACTION_NONE);
}