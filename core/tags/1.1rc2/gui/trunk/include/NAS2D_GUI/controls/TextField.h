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

#ifndef _LOM_GUI_TEXTFIELD_
#define _LOM_GUI_TEXTFIELD_

#include "Control.h"


/**
 * \class TextField
 * \brief A Basic Text Field.
 *
 * The Field Control is just a set of rendered text a user can enter.
 */

class TextField: public Control
{
public:
	TextField(const std::string& name, const std::string& text, int width, int height, int maxLen = 0);
	
	~TextField()
	{}
	
	void setText(const std::string& text);
	void setText(int value);
	const std::string & getText() const;
	
	void isEditable(bool editable);
	bool isEditable();

	bool empty() const { return mCaption.empty(); }
	
private:

	enum ActionType
	{
		ACTION_NONE,
		ACTION_INSERT,
		ACTION_BACKSPACE,
		ACTION_DELETE,
		ACTION_MOVE_LEFT,
		ACTION_MOVE_RIGHT
	};

	void initialize();
	void drawSkin();
	void drawPlain();
	void drawCursor();
	void drawTextHighlight();

	int getTextAreaWidth() const { return getWidth() - getMarginLeft() - getMarginRight() - 2; }

	void insertChar();
	void insertCharacter(const char* character);
	
	void onMouseButtonDown();
	void onKeyDown();
	void onKeyUp();
	void onChanged();
	void onLostFocus();
	
	void updateCursor();

	void logic();
	
	void selectAll();
	void shiftSelect();

	void setAction(ActionType aType);
	
	Timer		mTimer;				/**< Internal timer. */

	int 		mCursorPosition;	/**< Position of the Insertion Cursor. */
	int 		mCursorX;			/**< Pixel position of the Cursor. */
	int 		mScrollOffset;		/**< Scroller offset. */
	int			mMaxScrollOffset;	/**< Maximum allowable offset. */

	int		mMaxLength;			/**< Maximum number of characters that can be stored in the TextField. */
	int		mLastCursorTime;

	ActionType	mLastAction;		/**< Last action involving insertion, deletion and moving the cursor. */

	int		mActionTick;		/**< Tick that the last action occurred on. */
	int		mRepeatTick;		/**< Tick since the last repeat of the last action. */

	string		mInsertChar;		/**< Character to insert when repeating character insertion. */
	string		mSelectedText;      /**< Text that is currently selected. */

	bool		mEditable;			/**< Toggle editing of the field. */
	bool		mShowCursor;		/**< Flag indicating whether or not to draw the cursor. */

};

#endif