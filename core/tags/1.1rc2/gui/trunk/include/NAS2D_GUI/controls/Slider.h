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

#ifndef _LOM_GUI_SLIDER_
#define _LOM_GUI_SLIDER_

#include "Button.h"
#include "Control.h"

enum SliderType
{
	SLIDER_VERTICAL,
	SLIDER_HORIZONTAL
};


/**
 * \class	Slider
 * \brief	Base class for all sliding controls.
 *
 * \todo	Currently only Veritcal type sliders are supported. Horizontal should be pretty straight
 *			forward.
 * 
 * \todo	There's a bit of odd behavior when the mouse is dragged passed the slide area. This
 *			should be fairly straight forward to fix using the mMouseX and mMouseY values although
 *			it requires that this Control respond to onMouseMove() events.
 */
class Slider : public Control
{
public:
	Slider(const std::string& name, int height, SliderType type = SLIDER_VERTICAL);

	void setPosition(double position);
	void changePosition(double change);
	double getPosition() const;

	double getStepSize(int units = 1) const;

	int getSlideLength() const;

	void setThumbSize(int height);
	
protected:
	
private:
	void initialize();
	void drawSkin();
	void drawPlain();
	void logic();

	void onClick(SDL_Event& event);
	void onDrag(SDL_Event& event);
	void onMouseButtonDown(SDL_Event& event);
	void onChanged();
	void onResized();

	void constrainThumb();

	SliderType	mSliderType;		/**< Type of the Slider. */

	SkinPart	*mThumb;			/**< Slider's Thumb graphics. */

	int			mThumbWidth;		/**< Width of the Thumb. */
	int			mThumbHeight;		/**< Height of the Thumb. */
	
	int			mThumbPosition;	/**< Current location of the Thumb. */

	int			mSlideArea;			/**< Area in which the slider is allowed to move. Avoids unnecessary on-the-fly calculations. */
	int			mSlideUserArea;

	int			mMouseX;			/**< Mouse X coordinate. Used to prevent odd behavior in the thumb. */
	int			mMouseY;			/**< Mouse X coordinate. Used to prevent odd behavior in the thumb. */

	double		mPosition;			/**< Thumbnail position. */

	bool		mThumbPressed;		/**< Flag to indicate if this control is pressed. */
};

#endif