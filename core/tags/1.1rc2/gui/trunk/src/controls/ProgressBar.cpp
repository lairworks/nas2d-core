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

#include "ProgressBar.h"

ProgressBar::ProgressBar(const string& name, int minWidth, int minHeight):	Control(name, minWidth, minHeight),
																			mBarWidth(0),
																			mBarRed(0),
																			mBarGreen(0),
																			mBarBlue(0),
																			mProgress(0.0),
																			mShowPercent(false)
{
	setCaption(name);
	setMargins(1, 1, 1, 1);
	drawBorder(true);

	setBackgroundColor(255, 255, 255);
	setForegroundColor(255, 255, 255);
	setBarColor(0, 0, 255);
}


void ProgressBar::setBarColor(int r, int g, int b)
{
	mBarRed = r;
	mBarGreen = g;
	mBarBlue = b;
}

void ProgressBar::logic()
{
}


void ProgressBar::drawSkin()
{
	drawPlain();
}


void ProgressBar::drawPlain()
{
	Singleton<Renderer>::get().drawBoxFilled(getAbsoluteX(), getAbsoluteY(), mWidth, mHeight, mBackgroundColor.red(), mBackgroundColor.green(), mBackgroundColor.blue());
	Singleton<Renderer>::get().drawBoxFilled(getAbsoluteX(), getAbsoluteY(), mBarWidth, mHeight, mBarRed, mBarGreen, mBarBlue);

	if(mShowPercent)
	{
		std::stringstream progressString;
		progressString << setprecision(3) << mProgress * 100 << "%";
		//progressString << mProgress * 100 << "%";
		Singleton<Renderer>::get().drawText(*mFont, progressString.str(), getAbsoluteX() + mWidth / 2 - (mFont->getWidth(progressString.str()) / 2),
							getAbsoluteY() + (mHeight / 2) - (mFont->getHeight() / 2), mForegroundColor.red(), mForegroundColor.green(), mForegroundColor.green());
	}
}


/**
 * Sets/updates the progress value.
 * 
 * \note	Needs to be a double.
 */
void ProgressBar::setProgress(double progress)
{
	if(progress < 0.0)
		mProgress = 0.0;
	else if(progress > 1.0) 
		mProgress = 1.0;

	mProgress = progress;

	mBarWidth = (int)(mWidth * mProgress);
}


/**
 * Sets the ProgressBar to display a percentage text.
 *
 * \param	Can be \c true or \c false.
 */
void ProgressBar::showPercent(bool toggle)
{
	mShowPercent = toggle;
}