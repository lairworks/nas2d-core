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

#include "ScrollBar.h"

const int SLIDE_SCROLL_WAIT = 500;
const int SLIDE_DELAY = 20;

const int MIN_VERTICAL_WIDTH = 16;
const int MIN_VERTICAL_HEIGHT = 32;

const int MIN_HORIZONTAL_WIDTH = 32;
const int MIN_HORIZONTAL_HEIGHT = 16;

/**
 *	Constructor.
 *
 *	\param	name	Internal name of the ScrollBar.
 *	\param	x		X-Position of the ScrollBar relative to its parent Control.
 *	\param	y		Y-Position of the ScrollBar relative to its parent Control.
 *	\param	width	Width of the ScrollBar.
 *	\param	height	Width of the ScrollBar.
 *	\param	type	Value of type \c SliderType. Can either be SLIDER_HORIZONTAL or SLIDER_VERTICAL. Defaults to SLIDER_HORIZONTAL.
 */
ScrollBar::ScrollBar(const string& name, int height, SliderType type):	Container(name),
																		mTimer(Timer()),
																		mSlider(NULL),
																		mDecreaseButton(NULL),
																		mIncreaseButton(NULL),
																		mPressedTick(0),
																		mScrolledTick(0),
																		mNumStepUnits(0),
																		mStepSize(0),
																		mIsDecreasing(false),
																		mIsInreasing(false),
																		mIsScrolling(false)
{
	(type == SLIDER_VERTICAL) ? setMinimumSize(MIN_VERTICAL_WIDTH, MIN_VERTICAL_HEIGHT) : setMinimumSize(MIN_HORIZONTAL_WIDTH, MIN_HORIZONTAL_HEIGHT);

	setHeight(height);
}


void ScrollBar::initialize()
{
	/**
	 * \todo	I would like to go through all of the code here and
	 *			clean it up as best as possible. It's a bit cludgy
	 *			and I'm sure it can be done a little better.
	 */

	// Create a new Decrease button and set it at the bottom of the ScrollBar
	mDecreaseButton = new Button("", "scrl_BtnDecr", mWidth, mWidth);
	mDecreaseButton->setSkinPartType("ScrollButtonUp");
	mDecreaseButton->setSkinState(STATE_NORMAL);
	mDecreaseButton->setRelative(0, 0);
	add(mDecreaseButton);


	// Create a new Increase button and set it at the top of the ScrollBar
	mIncreaseButton = new Button("", "scrl_BtnIncr", mWidth, mWidth);
	mIncreaseButton->setSkinPartType("ScrollButtonDown");
	mIncreaseButton->setSkinState(STATE_NORMAL);
	mIncreaseButton->setRelative(0, mHeight - mDecreaseButton->getHeight());
	add(mIncreaseButton);

	
	// If the buttons were properly skinned set the minimum and
	// maximum width to the minimum width of the increase button.
	if(mDecreaseButton->isSkinned() && mIncreaseButton->isSkinned())
	{
		setMinimumSize(mIncreaseButton->getWidth(), mHeight);
		setSize(mIncreaseButton->getWidth(), mHeight);
	}
	else
	{
		setMinimumSize(MIN_VERTICAL_WIDTH, mHeight);
		setMaximumSize(MIN_VERTICAL_WIDTH, mHeight);
		setSize(MIN_VERTICAL_WIDTH, mHeight);
	}

	// Create a new Slider and set it immediatly underneath the Decrease button.
	int sliderHeight = mHeight - mDecreaseButton->getHeight() - mIncreaseButton->getHeight();
	mSlider = new Slider("scrl_Slider", sliderHeight, SLIDER_VERTICAL);
	mSlider->setRelative(getRelativeX(), getRelativeY() + mIncreaseButton->getHeight());

	add(mSlider);

	// This is here to ensure that sizes are set properly.
	setSize(mWidth, mHeight);
	mSlider->setWidth(mIncreaseButton->getWidth());
	mSlider->setHeight(sliderHeight);

	mStepSize = mSlider->getStepSize();
}


/**
 * Not sure I can really comment this any more than the code already documents it.
 */
void ScrollBar::logic()
{
	int currentTick = mTimer.ms();

	if(!mIsScrolling && (mIsDecreasing || mIsInreasing))
		if(currentTick - mPressedTick >= SLIDE_SCROLL_WAIT)
			mIsScrolling = true;

	if(mIsScrolling)
	{
		if(currentTick - mScrolledTick >= SLIDE_DELAY)
		{
			mScrolledTick = currentTick;

			if(mIsDecreasing)
				mSlider->changePosition(-(mStepSize));
			else
				mSlider->changePosition(mStepSize);
		}
	}
}


void ScrollBar::onChanged()
{}


//void ScrollBar::push()
//{
//
//	switch(event->getType())
//	{
//		case EVENT_GUI_PRESSED:
//			mPressedTick = mTimer.ms();
//			if(event->getId() == "scrl_BtnDecr")
//			{
//				mSlider->changePosition(-(mStepSize));
//				mIsDecreasing = true;
//			}
//			else if(event->getId() == "scrl_BtnIncr")
//			{
//				mSlider->changePosition(mStepSize);
//				mIsInreasing = true;
//			}
//			break;
//
//		case EVENT_GUI_CLICKED:
//			if(event->getId() == "scrl_BtnDecr")
//				mIsDecreasing = false;
//			else if(event->getId() == "scrl_BtnIncr")
//				mIsInreasing = false;
//
//			// Hackish but works because CLICKED is only fired when the mouse button goes up.
//			mIsScrolling = false;
//			break;
//
//		default:
//			// Finished looking at events so push it down to the base
//			// Control so it can still respond to any other events.
//			Control::push(event);
//			break;
//	}
//	Control::push(event);
//}


/**
 * Gets the current position of the ScrollBar.
 */
double ScrollBar::getPosition() const
{
	return mSlider->getPosition();
}


double ScrollBar::getStepSize() const
{
	return mStepSize;
}


void ScrollBar::setPosition(double position)
{
	mSlider->setPosition(position);
}


/**
 * Sets the number of step units for the ScrollBar.
 *
 * \param	units	Number of step units.
 */
void ScrollBar::setStepUnits(int units)
{
	if(units > 0)
	{
		mNumStepUnits = units;
		
		// TODO:
		// Step units and scrolling with them is not accurate. When there's only one step it
		// doesn't scroll properly. With many sometimes a step unit skips a line. This has
		// to do with the conversion from floating-point percentage values and absolute
		// integer pixel values.
		mSlider->setThumbSize(mSlider->getHeight() / mNumStepUnits);
		mStepSize = mSlider->getStepSize(mSlider->getSlideLength() / mNumStepUnits);
	}
	else
	{
		mNumStepUnits = 0;
		mStepSize = 0;
		mSlider->setThumbSize(mSlider->getHeight());
	}
}