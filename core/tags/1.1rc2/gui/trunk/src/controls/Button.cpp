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

#include "Button.h"

const int MIN_WIDTH = 12;
const int MIN_HEIGHT = 12;

/**
 * Constructs a basic Button.
 *
 * \param	caption	The text to display on the button.
 * \param	name	Name of the Button.
 * \param	width	Width of the Button.
 * \param	height	Height of the button.
 */
Button::Button(const string& caption, const string& name, int width, int height, ButtonType type):	Control(name, MIN_WIDTH, MIN_HEIGHT),
																									mType(type),
																									mImage(NULL),
																									mDisabledImage(NULL),
																									mImgX(0),
																									mImgY(0),
																									mClampedWidth(0),
																									mTextCenterOffset(0),
																									mIsToggled(false)
{
	setCaption(caption);
	setSize(width, height);
}


/**
 * Constructs a Button with an Image instead of text.
 *
 * \param	image	A pointer to an Image. The Image object passed is now owned by the Button.
 * \param	name	Name of the Button.
 * \param	width	Width of the Button.
 * \param	height	Height of the button.
 *
 * \note	Images are clamped to the drawable area of the button. They are not centered.
 * \warning	Button takes ownership of any Image passed to it.
 */
Button::Button(Image *image, const string& name, int width, int height, ButtonType type):	Control(name, MIN_WIDTH, MIN_HEIGHT),
																							mType(type),
																							mImage(image),
																							mDisabledImage(NULL),
																							mImgX(0),
																							mImgY(0),
																							mClampedWidth(0),
																							mTextCenterOffset(0),
																							mIsToggled(false)
{
	setSize(width, height);
}


/**
 *	Destructor.
 *
 * Frees Button::mImage if allocated.
 */
Button::~Button()
{
	if(mImage)
	{
		delete mImage;
		mImage = NULL;
	}
	if(mDisabledImage)
	{
		delete mDisabledImage;
		mDisabledImage = NULL;
	}
}


/**
 * Initializes the Button.
 */
void Button::initialize()
{
	setSkinState("Button");

	if(mAutosize)
		setSize( mFont->getWidth(mCaption) + mLeft + mRight, mFont->getHeight() + mTop + mBottom);

	// This should be determined by the Skin
	setMargins(2, 2, 2, 2);

	setDrawValues();

	// Figure out the offset of the image to draw it centered in the button
	// based on the dimensions of the button taking margins into account.
	if(mImage)
	{
		// Resize the image if it's too large to fit in the available area.
		int maxWidth = mWidth - mLeft - mRight, maxHeight = mHeight - mTop - mBottom;
		if((mImage->getWidth() > mWidth) || (mImage->getHeight() > mHeight))
		{
			Image *tmpImage = new Image(mImage, 0, 0, mWidth - mLeft - mRight, mHeight - mTop - mBottom);
			if(tmpImage && tmpImage->isLoaded())
			{
				delete mImage;
				mImage = tmpImage;
			}
			else
				delete tmpImage;
		}

		mImgX = (mWidth / 2) - (mImage->getWidth() / 2);
		mImgY = (mHeight / 2) - (mImage->getHeight() / 2);

		mDisabledImage = new Image(*mImage);
		Singleton<Renderer>::get().imageDesaturate(*mDisabledImage);
	}
}


/**
 * Called during initialization to set draw values for image type buttons.
 */
void Button::setDrawValues()
{
	if(mUsingSkin && mCurrentSkinPart->getImageList().size() > 1)
	{
		// Subtract the left and right image widths from the width area.
		mClampedWidth = mWidth - mCurrentSkinPart->getImageList()[0].getWidth() - mCurrentSkinPart->getImageList()[2].getWidth();
		mTextCenterOffset = mCurrentSkinPart->getImageList()[0].getWidth();
	}
	else
	{
		mClampedWidth = mWidth;
		mTextCenterOffset = 0;
	}
}


/**
 *
 */
void Button::logic()
{}


/**
 * Button's onClick handler.
 *
 * Notifies a click Listener and pushes a Click event to it.
 *
 * \warning	The event pushed into the click Listener is owned
 *			by the Button and as such is destroyed by Button.
 *			Do not store a direct pointer to the Event.
 */
void Button::onClick()
{
	//std::cout << "* Button::onClick(): " << mName << " " << listener << endl;

	if(mType != Button::TOGGLE)
	{
		setSkinState("Button");
		setDrawValues();
	}
}


void Button::onMouseButtonDown()
{
	setSkinState("Button", STATE_PRESSED);
	setDrawValues();

	//std::cout << "* Button::onMouseButtonDown(): " << mName << " " << listener << endl;

	// Set toggle state
	if(mType == Button::TOGGLE)
		mIsToggled ? mIsToggled = false : mIsToggled = true;
}


void Button::onMouseButtonUp()
{
	if(mType == Button::TOGGLE)
	{

		if(mIsToggled)
		{
			setSkinState("Button", STATE_PRESSED);
			return;
		}
	}

	setSkinState("Button");
	setDrawValues();
}


void Button::onEnabled()
{
	if(mEnabled)
		setSkinState("Button");
	else
		setSkinState("Button", STATE_DISABLED);

	setDrawValues();
}


/**
 * Draws a button skinned.
 *
 * \todo	This method only supports type \c SkinPartType::PART_ENNEAD. This
 *			should also support \c SkinPartType::PART_TRIAD.
 */
void Button::drawSkin()
{
	// Determine what kind of part type we're using and use the appropriate drawing method.
	switch(mCurrentSkinPart->getPartType())
	{
		case PART_ENNEAD:
			Singleton<Renderer>::get().drawImageRect(getAbsoluteX(), getAbsoluteY(), mWidth, mHeight, mCurrentSkinPart->getImageList());
			break;
		case PART_MONAD:
			Singleton<Renderer>::get().drawImage(mCurrentSkinPart->getImageList()[0], getAbsoluteX(), getAbsoluteY());
			break;
		case PART_TRIAD:
			// Not supported yet.
			break;
		default:
			Singleton<Renderer>::get().drawBoxFilled(getAbsoluteX(), getAbsoluteY(), mWidth, mHeight, mBackgroundColor.red(), mBackgroundColor.green(), mBackgroundColor.blue());
			Singleton<Renderer>::get().drawBox(getAbsoluteX(), getAbsoluteY(), mWidth, mHeight, mBorderColor.red(), mBorderColor.green(), mBorderColor.blue());
			break;
	}
	
	if(mImage && mEnabled)
		Singleton<Renderer>::get().drawImage(*mImage, getAbsoluteX() + mImgX, getAbsoluteY() + mImgY);
	else if(mDisabledImage && !mEnabled)
		Singleton<Renderer>::get().drawImage(*mDisabledImage, getAbsoluteX() + mImgX, getAbsoluteY() + mImgY);
	else
		drawText();
}


void Button::drawPlain()
{
	Singleton<Renderer>::get().drawBoxFilled(getAbsoluteX(), getAbsoluteY(), mWidth, mHeight, mBackgroundColor.red(), mBackgroundColor.green(), mBackgroundColor.blue());
	Singleton<Renderer>::get().drawBox(getAbsoluteX(), getAbsoluteY(), mWidth, mHeight, mBorderColor.red(), mBorderColor.green(), mBorderColor.blue());

	if(mImage && mEnabled)
		Singleton<Renderer>::get().drawImage(*mImage, getAbsoluteX() + mImgX, getAbsoluteY() + mImgY);
	else if(mDisabledImage && !mEnabled)
		Singleton<Renderer>::get().drawImage(*mDisabledImage, getAbsoluteX() + mImgX, getAbsoluteY() + mImgY);
	else
		drawText();
}


void Button::drawHighlight()
{
	Singleton<Renderer>::get().drawBox(getAbsoluteX() - 1, getAbsoluteY() - 1, getWidth() + 1, getHeight() + 1, mBorderColor.red(), mBorderColor.green(), mBorderColor.blue());
}


void Button::drawText()
{
	int xPos = getAbsoluteX() + (mClampedWidth >> 1) - (mFont->getWidth(mCaption) >> 1);
	int yPos = getAbsoluteY() + (mHeight >> 1) - (mFont->getHeight() >> 1);

	if(xPos < getAbsoluteX())
		xPos = getAbsoluteX();
	if(yPos < getAbsoluteY())
		yPos = getAbsoluteY();

	Singleton<Renderer>::get().drawTextClamped(*mFont, mCaption, xPos + mTextCenterOffset, yPos, 0, 0, mClampedWidth, mHeight, mForegroundColor.red(), mForegroundColor.green(), mForegroundColor.green());
}


void Button::onKeyDown()
{
//	if(event->getFirst() == KEY_RETURN || event->getFirst() == KEY_SPACE)
//		onMouseButtonDown();
}


void Button::onKeyUp()
{
//	if(event->getFirst() == KEY_RETURN || event->getFirst() == KEY_SPACE)
//	{
//		//onMouseButtonUp(NULL);
//		onClick();
//	}
}