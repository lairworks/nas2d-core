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

#ifndef _LOM_GUI_BUTTON_
#define _LOM_GUI_BUTTON_

#include "Control.h"

/**
 * \class Button
 * \brief A Button object derived from base class Control.
 *
 * The Button class provides all basic functions of a button.
 *
 * The Button should respond to all relevant input events coming
 * downstream and send the appropriate response back upstream.
 *
 * \todo Finish implementing broadcasting of events both
 * upstream and downstream so that the button can respond
 * appropriately.
 */

class Button : public Control
{
public:

	/**
	 * \enum ButtonType
	 * 
	 * Used by Button to determine behavior.
	 */
	enum ButtonType
	{
		NORMAL,
		TOGGLE
	};

	Button(const std::string& caption, const std::string& name, int width, int height, ButtonType type = NORMAL);
	Button(Image *image, const std::string& name, int width, int height, ButtonType type = NORMAL);
	~Button();

	bool isToggled() const { return mIsToggled; }
	void isToggled(bool toggled) { mIsToggled = toggled; onMouseButtonUp(); }

private:
	void logic();
	void drawSkin();
	void drawPlain();
	void drawHighlight();
	void initialize();

	void drawText();

	void onClick();
	void onMouseButtonDown();
	void onMouseButtonUp();
	void onEnabled();

	void onKeyDown();
	void onKeyUp();

	void setDrawValues();

	ButtonType	mType;				/**< Internal button behavior indicator. */
	
	Image		*mImage;			/**< Image displayed on an image type button. */
	Image		*mDisabledImage;	/**< Dimmed version of a button icon if the image is defined. */

	int			mImgX;				/**< Internal image Y-position. */
	int			mImgY;				/**< Internal image Y-position. */

	int			mClampedWidth;		/**< Actual area that text can be drawn in with side graphics accounted for. */
	int			mTextCenterOffset;	/**< Center offset taking into account the clamped width area. */

	bool		mIsToggled;			/**< Flag indicating whether or not the button is toggled. Only used when button is of type ButtonType::TOGGLE. */
};

#endif

