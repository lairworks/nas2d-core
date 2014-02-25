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

#include "Slider.h"

const int DEFAULT_MIN_WIDTH		= 8;
const int DEFAULT_MIN_HEIGHT		= 8;

const int DEFAULT_WIDTH			= 8;
const int DEFAULT_HEIGHT			= 8;

const int THUMB_DEFAULT_SIZE		= 8;
const int THUMB_MIN_HEIGHT			= 8;
const int THUMB_MIN_WIDTH			= 8;


Slider::Slider(const string& name, int end, SliderType type):	Control(name, DEFAULT_MIN_WIDTH, DEFAULT_MIN_HEIGHT),
																	mSliderType(type),
																	mThumb(NULL),
																	mThumbWidth(THUMB_DEFAULT_SIZE),
																	mThumbHeight(THUMB_DEFAULT_SIZE),
																	mThumbPosition(0),
																	mSlideArea(0),
																	mSlideUserArea(end),
																	mMouseX(0), mMouseY(0),
																	mPosition(0.0),
																	mThumbPressed(false)
{}


void Slider::initialize()
{
	/**
	 * \todo	I'm not thrilled with the way the slider determines its
	 *			width. I'd like to go through this code to ensure that
	 *			1) it's correct and 2) it's cleaner than it is.
	 */

	// Turned off skinning for now until we can get the numbers right.
	setSkinState("ScrollBar");
	
	if(!mUsingSkin)
		drawBorder(true);
	else
	{
		mThumb = mSkin->getPart("ScrollBarThumb");
		if(mThumb)
		{
			// If the thumb is skinned, use its minimum width.
			if(mUsingSkin)
			{
				setMinimumSize(mCurrentSkinPart->getMinimumWidth(), mThumb->getMinimumHeight());
				setWidth(mCurrentSkinPart->getMinimumWidth());
				mThumbWidth = getWidth();
			}
			else
			{
				setMinimumSize(mThumb->getMinimumWidth(), mThumb->getMinimumHeight());
				setWidth(mThumb->getMinimumWidth());
			}
		}
	}
}

void Slider::logic()
{
	if(mSliderType == SLIDER_VERTICAL)
		mSlideArea = mHeight - mThumbHeight;
	else
		mSlideArea = mWidth - mThumbWidth;
}


void Slider::drawSkin()
{
	Singleton<Renderer>::get().drawImageRect(getAbsoluteX(), getAbsoluteY(), mWidth, mHeight, mCurrentSkinPart->getImageList());
	//Singleton<Renderer>::get().floodFill(getAbsoluteX(), getAbsoluteY(), mWidth, mHeight, 255, 0, 0);

	if(mThumb)
		if(mSliderType == SLIDER_VERTICAL)
			Singleton<Renderer>::get().drawImageRect(getAbsoluteX(), getAbsoluteY() + mThumbPosition, mThumbWidth, mThumbHeight, mThumb->getImageList());
		else
			Singleton<Renderer>::get().drawImageRect(getAbsoluteX() + mThumbPosition, getAbsoluteY(), mThumbWidth, mThumbHeight, mThumb->getImageList());

	else
	{
		Singleton<Renderer>::get().drawBoxFilled(getAbsoluteX() + mThumbPosition, getAbsoluteY() + mThumbPosition, mThumbWidth, mThumbHeight, 255, 255, 255);
		Singleton<Renderer>::get().drawBox(getAbsoluteX() + mThumbPosition, getAbsoluteY() + mThumbPosition, mThumbWidth, mThumbHeight, mBorderColor.red(), mBorderColor.green(), mBorderColor.blue());
	}
}

void Slider::drawPlain()
{
	Singleton<Renderer>::get().drawBoxFilled(getAbsoluteX(), getAbsoluteY(), mWidth, mHeight, mBackgroundColor.red(), mBackgroundColor.green(), mBackgroundColor.blue());
	
	Singleton<Renderer>::get().drawBoxFilled(getAbsoluteX(), getAbsoluteY() + mThumbPosition, mThumbWidth, mThumbHeight, 255, 255, 255);
	Singleton<Renderer>::get().drawBox(getAbsoluteX(), getAbsoluteY() + mThumbPosition, mThumbWidth, mThumbHeight, mBorderColor.red(), mBorderColor.green(), mBorderColor.blue());

	Singleton<Renderer>::get().drawLine(getAbsoluteX(), getAbsoluteY() + mSlideArea, getAbsoluteX() + mWidth, getAbsoluteY() + mSlideArea, 255, 0, 255);
}

/**
 * Update thumb position
 */
void Slider::setPosition(double position)
{
//	if(mSlideArea == 0)
//	{
//		mPosition = 0.0f;
//		return;
//	}

	mPosition = position;
	
	if(mPosition < 0.0)
		mPosition = 0.0;
	else if(mPosition > 1.0)
		mPosition = 1.0;

	mThumbPosition = (int)((double)mSlideArea * mPosition);
}


/**
 * Adds the change amount to the current position.
 *
 * \param	change	Amount to change in percent to change the
 *					slider's position. Must be between 0.0
 *					1.0.
 */
void Slider::changePosition(double change)
{
	setPosition(mPosition + change);
}


/**
 * Gets the current position of the slider as
 * percentage of the length of the slider.
 */
double Slider::getPosition() const
{
	return mPosition;
}


/**
 * Gets the size of a step based on a unit size.
 *
 * \param	unit	Size of a step as a measure of pixels.
 *					Default unit size is 1 pixel.
 */
double Slider::getStepSize(int unit) const
{
	if(unit == 0)
		return 0;
	else
		return (double)unit / (double)mSlideArea;
}


/**
 * Returns the number of pixels the slider can move along.
 */
int Slider::getSlideLength() const
{
	return mSlideArea;
}


/**
 * Reset mouse pressed.
 */
void Slider::onClick(SDL_Event& event)
{
	mThumbPressed = false;
}

/**
 * If the mouse is still pressed on the thumbnail and moves,
 * update the thumbnail position.
 */
void Slider::onDrag(SDL_Event& event)
{
	if(mThumbPressed)
	{
		if(mSliderType == SLIDER_VERTICAL)
			mThumbPosition += event.motion.yrel;
		else
			mThumbPosition += event.motion.xrel;
			
		constrainThumb();
		
		// Note:	Values need to be cast to a double type otherwise it always evaluates
		//			to either 0.0 or 1.0
		if(mSlideArea > 0)
		{
			setPosition((double)mThumbPosition / (double)mSlideArea);
		}
		else
			setPosition(0);
	}
}


void Slider::onMouseButtonDown(SDL_Event& event)
{
	mMouseX = event.motion.x, mMouseY = event.motion.y;
	
	if(mSliderType == SLIDER_VERTICAL)
	{
		if(isPointInRect(event.motion.x, event.motion.y, getAbsoluteX(), getAbsoluteY() + mThumbPosition, mThumbWidth, mThumbHeight))
			mThumbPressed = true;
		else if(isPointInRect(event.motion.x, event.motion.y, getAbsoluteX(), getAbsoluteY(), getWidth(), getHeight()))
		{
			// Haven't decided yet if I like sliders following mouse drags if we didn't
			// actually click inside the thumb.
			//mThumbPressed = true;
			mThumbPressed = false;
			mThumbPosition = mMouseY - getAbsoluteY() - (mThumbHeight / 2);
			constrainThumb();
			setPosition((float)mThumbPosition / (float)mSlideArea);
		}
	}
	else
	{
		if(isPointInRect(event.motion.x, event.motion.y, getAbsoluteX() + mThumbPosition, getAbsoluteY(), mThumbWidth, mThumbHeight))
			mThumbPressed = true;
		else if(isPointInRect(event.motion.x, event.motion.y, getAbsoluteX(), getAbsoluteY(), getWidth(), getHeight()))
		{
			// Haven't decided yet if I like sliders following mouse drags if we didn't
			// actually click inside the thumb.
			//mThumbPressed = true;
			mThumbPressed = false;
			mThumbPosition = mMouseX - getAbsoluteX() - (mThumbWidth / 2);
			constrainThumb();
			setPosition((float)mThumbPosition / (float)mSlideArea);
		}
	}
}


void Slider::onChanged()
{}


void Slider::onResized()
{
	if(mSliderType == SLIDER_VERTICAL)
		mThumbWidth = getWidth();
	else
		mThumbHeight = getHeight();
}


void Slider::constrainThumb()
{
	if(mThumbPosition < 0)
		mThumbPosition = 0;
	else if(mThumbPosition > mSlideArea)
		mThumbPosition = mSlideArea;
}


/**
 * Sets the thumb size in pixels.
 *
 * \param	size	Number of pixels to set the size of the Thumb.
 *
 * \note	If slider type is \c SLIDER_VERTICAL than \c size will modify height.
 *			Otherwise \c size will modify width.
 */
void Slider::setThumbSize(int size)
{
	if(mSliderType == SLIDER_VERTICAL)
	{
		if(size <= mHeight)
		{
			(size >= THUMB_MIN_HEIGHT) ? (mThumbHeight = size) : (mThumbHeight = THUMB_MIN_HEIGHT);
			mSlideArea = mHeight - mThumbHeight;
		}
		else
		{
			mThumbHeight = mHeight;
			mSlideArea = 0;
		}
	}
	else
	{
		if(size <= mWidth)
		{
			(size >= THUMB_MIN_WIDTH) ? (mThumbWidth = size) : (mThumbWidth = THUMB_MIN_WIDTH);
			mSlideArea = mWidth - mThumbWidth;
		}
		else
		{
			mThumbWidth = mWidth;
			mSlideArea = 0;
		}
	}
}