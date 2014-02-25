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

#ifndef _LOM_GUI_SCROLLBAR_
#define _LOM_GUI_SCROLLBAR_

#include "Button.h"
#include "Container.h"
#include "Slider.h"


/** \class	ScrollBar
 *	\brief	A scroll bar.
 *
 *	Implementation of a ScrollBar.
 *
 *	\todo	Currently only supports Vertical scroll bars.
 */
class ScrollBar : public Container
{
public:
	ScrollBar(const std::string& name, int height, SliderType type = SLIDER_VERTICAL);
	~ScrollBar()
	{}

	double getPosition() const;

	void setPosition(double position);
	
	double getStepSize() const;

	void setStepUnits(int units);

protected:
	void initialize();
	void logic();

	void push();

private:
	void onChanged();

	Timer	mTimer;				/**< Internal timer used to slow down the progress of the thumb. */

	Slider	*mSlider;			/**< Internal Slider. */

	Button	*mDecreaseButton;	/**< Decrease Slider Position Button. */
	Button	*mIncreaseButton;	/**< Increase Slider Position Button. */

	int	mPressedTick;		/**< Internal time when pressed note. */
	int	mScrolledTick;		/**< Internal time when last changed slider position. */

	int	mNumStepUnits;		/**< Number of step units for this scroll bar. */
	
	double	mStepSize;			/**< Size of the step to take per scroll unit. */

	bool	mIsDecreasing;		/**< Flag indicating that we're decreasing. */
	bool	mIsInreasing;		/**< Flag indicating that we're inreasing. */
	bool	mIsScrolling;		/**< Flad indicating that we're past the initial wait delay. */
};


#endif