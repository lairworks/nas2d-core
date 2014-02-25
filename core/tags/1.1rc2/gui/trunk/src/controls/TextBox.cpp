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

#include "TextBox.h"

#include <sstream>
#include <istream>

/**
 * 
 */
TextBox::TextBox(const string& name, int width, int height):	ScrollArea(name, width, height),
																mTotalLines(0),
																mCurrentLine(0),
																mCursorPositionX(0),
																mCursorPositionY(0),
																mWrapped(true),
																mEditable(true)

{
	setSize(width, height);
}


void TextBox::scrollAreaInit()
{
	setSkinState("TextBox");
	
	// TODO:
	// These are values that should be defined by the Skin.
	setMargins(2, 2, 2, 2);
}


/**
 * Sets the text we wish to be displayed within the textbox.
 *
 * \note	This function calls the formatText().
 */
void TextBox::setText(const string& text)
{	
	mText = text;

	mFormattedText.clear();
	mTokens.clear();
	mLines.clear();

	mTokens = Tokenize(text, " ");
	formatText();

	// Ensure that we properly set the number of text units available whenever
	// anything changes.
	setTotalLines();

	int scrollUnits = mFormattedText.size() - mTotalLines;
	(scrollUnits > 0) ? mScrollBar->setStepUnits(scrollUnits) : mScrollBar->setStepUnits(0);
}


void TextBox::appendText(const std::string& text)
{
	string newText = mText + text;
	setText(newText);
}


/**
 * Returns the text contained in the TextBox.
 */
std::string TextBox::getText() const
{
	return mText;
}


/**
 * Breaks our strings to new lines based on textbox size.
 *
 * \note	Needs to be tested against resized boxes.
 * 
 * The string is broken down by words so as to properly display each
 * line of text to wrap properly.
 */
void TextBox::setWrapped(bool wrapped)
{       
	mWrapped = wrapped;
}


void TextBox::isEditable(bool editable)
{
	mEditable = editable;
}


bool TextBox::isEditable()
{
	return mEditable;
}


/**
 * Sets the maximum lines of text to display within the box.
 * 
 * Calculation is done based on the height of the textbox and the font
 * height.
 */
void TextBox::setTotalLines()
{
	mTotalLines = mHeight / mFont->getHeight();
}


/**
 * Builds a new string with formatting characters.
 */
void TextBox::formatText()
{
	string tmpStr = "";
	
	int tokenCounter = 0;//, tokenMaxCount = mTokens.size();
	bool working = true;
	
	if(mWrapped)
	{
		int textSpace = mScrollAreaWidth;
		int spaceLen = mFont->getWidth(" ");
	
		while(tokenCounter < mTokens.size())
		{
			if (mFont->getWidth(mTokens[tokenCounter]) > textSpace)
			{
				if(tmpStr.empty())
				{
					string brokenToken = mTokens[tokenCounter];
					string remainingBrokenToken = "";
					int charCount = 0;
					
					while(mFont->getWidth(brokenToken) > textSpace)
					{
                        remainingBrokenToken.insert(remainingBrokenToken.size() - charCount, brokenToken, brokenToken.size() - 1, 1);
                        brokenToken.erase(brokenToken.size() - 1, 1);
                        charCount++;
					}
					
					mTokens.erase(mTokens.begin() + tokenCounter);
					mTokens.insert(mTokens.begin() + tokenCounter, brokenToken);
					mTokens.insert(mTokens.begin() + tokenCounter + 1, remainingBrokenToken);
					
					tmpStr += mTokens[tokenCounter];
					
					mFormattedText.push_back(tmpStr);
					tmpStr = "";
				}
				else
				{
					mFormattedText.push_back(tmpStr);
					tmpStr = "";
					tokenCounter--;
				}
			}
			else
			{
				if (mFont->getWidth(tmpStr) + spaceLen + mFont->getWidth(mTokens[tokenCounter]) <= textSpace)
				{
					if(tmpStr.empty())
						tmpStr += mTokens[tokenCounter];
					else
						tmpStr += " " + mTokens[tokenCounter];
				}
				else if (mFont->getWidth(tmpStr) + spaceLen + mFont->getWidth(mTokens[tokenCounter]) > textSpace)
				{
					mFormattedText.push_back(tmpStr);
					tmpStr.clear();
					
					if(tmpStr.empty())
						tmpStr += mTokens[tokenCounter];
					else
						tmpStr += " " + mTokens[tokenCounter];
				}
			}
			tokenCounter++;
		}
		
		mFormattedText.push_back(tmpStr);
		tmpStr = "";
	}
	else
	{
		while(working)
		{
			while(mTokens[tokenCounter].size() != 0)
			{
				tmpStr += mTokens[tokenCounter];
				tmpStr += " ";
				
				tokenCounter++;
				if(tokenCounter >= mTokens.size())
				{
					working = false;
					break;
				}
			}
			
			mFormattedText.push_back(tmpStr);
			tmpStr.clear();
		}
	}
}


StringList TextBox::Tokenize(const string& str, const string& delimiter)
{
	StringList strList;

	// Skip delimiters at beginning.
	string::size_type lastPos = str.find_first_not_of(delimiter, 0);
    
	// Find first "non-delimiter".
	string::size_type pos = str.find_first_of(delimiter, lastPos);

    while((string::npos != pos) || (string::npos != lastPos))
    {
        strList.push_back(str.substr(lastPos, pos - lastPos));

		lastPos = str.find_first_not_of(delimiter, pos);
        pos = str.find_first_of(delimiter, lastPos);
    }

	return strList;
}


void TextBox::areaChanged()
{
	//cout << "textbox changed" << endl;
	mCurrentLine = static_cast<int>((mFormattedText.size() - mTotalLines) * mScrollBar->getPosition());
}


void TextBox::drawSkin()
{
	Singleton<Renderer>::get().drawImageRect(getAbsoluteX(), getAbsoluteY(), mWidth - mScrollBar->getWidth(), mHeight, mCurrentSkinPart->getImageList());
	
	// Used to debug the mScrollAreaWidth being larger then it should.
	//Singleton<Renderer>::get().drawBoxFilled(getAbsoluteX() + mLeft, getAbsoluteY() + mTop, mScrollAreaWidth, mScrollAreaHeight, 255, 0, 0);
	drawText();
	drawCursor();
}


void TextBox::drawPlain()
{
	Singleton<Renderer>::get().drawBoxFilled(getAbsoluteX(), getAbsoluteY(), mWidth, mHeight, mBackgroundColor.red(), mBackgroundColor.green(), mBackgroundColor.blue());
	Singleton<Renderer>::get().drawBox(getAbsoluteX(), getAbsoluteY(), mWidth, mHeight, mBorderColor.red(), mBorderColor.green(), mBorderColor.blue());

	drawText();
	drawCursor();
}


/**
 * Draws the insertion point cursor.
 */
void TextBox::drawCursor()
{
	if(hasFocus())
	{
		int cursorX = getAbsoluteX() + mLeft + mFont->getWidth(mText.substr(0, mCursorPositionX));
		int cursorY = getAbsoluteY() + mTop + mFont->getWidth(mText.substr(0, mCursorPositionY));
		Singleton<Renderer>::get().drawLine(cursorX, getAbsoluteY(), cursorX, getAbsoluteY(), 0, 0, 0);
	}
}


/**
 * Draws the formatted text of this Control.
 */
void TextBox::drawText()
{
	int numLines = mFormattedText.size();
	
	if(mCurrentLine > numLines - mTotalLines)
		mCurrentLine = numLines - mTotalLines;

	for(size_t i = 0; i < mTotalLines; ++i)
	{
		
		//std::cout << "Slider Position: " << mSlider->getPosition() << "\tTotal Lines: " << mTotalLines << "\tNum Lines: " << numLines << "\tCurrent Line: " << mCurrentLine << "\tI: " << i << endl;
		
		// If we exceed the total lines we can display, return.
		if(i + mCurrentLine >= numLines)
			return;
		else
			Singleton<Renderer>::get().drawText(*mFont, mFormattedText[i + mCurrentLine], getAbsoluteX() + mLeft, getAbsoluteY() + (i * mFont->getHeight()) + mTop, mForegroundColor.red(), mForegroundColor.green(), mForegroundColor.green());
	}
}


void TextBox::onKeyDown()
{
//	if(hasFocus() && isEditable())
//	{
//		switch(event->getFirst())
//		{
//				// COMMAND KEYS
//			case KEY_BACKSPACE:
//				if(!mText.empty())
//				{
//					if(mCursorPositionX > 0)
//					{
//						mCursorPositionX--;
//						// STL '.end()' iterators are the end of the container, not the last element.
//						mText.erase(mCursorPositionX, 1);
//					}
//				}
//				break;
//			case KEY_HOME:
//				mCursorPositionX = 0;
//				break;
//			case KEY_END:
//				mCursorPositionX = mText.length();
//				break;
//			case KEY_DELETE:
//				if(mText.length() > 0)
//					mText = mText.erase(mCursorPositionX, 1);
//				break;
//				
//				// ARROW KEYS
//			case KEY_LEFT:
//				if(mCursorPositionX > 0)
//					mCursorPositionX--;
//				break;
//			case KEY_RIGHT:
//				if(mCursorPositionX < mText.length())
//					mCursorPositionX++;
//				break;
//				
//				// KEYPAD ARROWS
//			case KEY_KP4:
//				if((mCursorPositionX > 0) && !(event->getSecond() & KEY_MOD_NUM))
//					mCursorPositionX--;
//				else
//					// Insert this manually otherwise it gets ignored.
//					insertCharacter(translateKey(*event));
//				break;
//			case KEY_KP6:
//				if((mCursorPositionX < mText.length()) && !(event->getSecond() & KEY_MOD_NUM))
//					mCursorPositionX++;
//				else
//					// Insert this manually otherwise it gets ignored.
//					insertCharacter(translateKey(*event));
//				break;
//				
//				// IGNORE ENTER/RETURN KEY
//			case KEY_RETURN:
//			case KEY_KP_ENTER:
//				break;
//				// REGULAR KEYS
//			default:
//				insertCharacter(translateKey(*event));
//				break;
//		}
//	}
}

void TextBox::insertCharacter(const char* character)
{
	int prvLen = mText.length();
	mText = mText.insert(mCursorPositionX, character);
	
	if(mText.length() - prvLen)
		mCursorPositionX++;
}