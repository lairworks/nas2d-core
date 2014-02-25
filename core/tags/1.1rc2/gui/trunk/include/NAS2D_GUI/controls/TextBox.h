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

#ifndef _LOM_GUI_TEXTBOX_
#define _LOM_GUI_TEXTBOX_

#include "ScrollArea.h"

#include "NAS2D/Common.h"


/**
 * \class	TextBox
 * \brief	A box of text that can be wrapped to its dimensions.
 *
 * \note	TextBoxes include scrolling capability.
 */
class TextBox : public ScrollArea
{
public:
	TextBox(const std::string& name, int width, int height);

	void setText(const std::string& text);
	std::string getText() const;

	void appendText(const std::string& text);

	void setWrapped(bool wrapped);
	
	void isEditable(bool editable);
	bool isEditable();
	
	StringList Tokenize(const std::string& str, const std::string& delimiter = " ");

private:
	void drawSkin();
	void drawPlain();
	void drawCursor();
	void drawText();
	
	void scrollAreaInit();
	void areaChanged();
	
	void insertCharacter(const char* character);
	
	void formatText();
	
	void setTotalLines();
	
	void onKeyDown();
	
	StringList	mFormattedText;		/**< Vector containing the formatted strings of text. */
	StringList	mTokens;			/**< \todo Clarify this comment: Vector containing each individual word. */
	StringList	mLines;				/**< \todo Clarify this comment: Vector containing broken lines based on where \n was found. */

	string		mText;				/**< Text contained within this Control. */

	int			mTotalLines;		/**< Total amount of lines we can render. */
	int			mCurrentLine;		/**< Current line based on scrollbar's position. */
	int			mCursorPositionX;	/**< Position of the Insertion Cursor. */
	int			mCursorPositionY;	/**< Position of the Insertion Cursor. */

	bool		mWrapped;			/**< Flag indicating whether the text should wrap within a set region. */
	bool		mEditable;			/**< Toggle editing of the field. */
	
	
};

#endif
