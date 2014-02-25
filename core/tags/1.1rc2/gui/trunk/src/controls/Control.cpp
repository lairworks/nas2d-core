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

#include "Control.h"


const int MAX_WIDTH = 1024;
const int MAX_HEIGHT = 1024;


/**
 * \todo	COMMENT ME!
 */
const string CONTROL_HIDING = "CONTROL_HIDING";


// Default coloration

// FOREGROUND
const Color_4ub DEFAULT_ENABLED_FG(0, 0, 0, 255);
const Color_4ub DEFAULT_DISABLED_FG(155, 155, 155, 255);

// BACKGROUND
const Color_4ub DEFAULT_ENABLED_BG(185, 185, 185, 255);
const Color_4ub DEFAULT_DISABLED_BG(185, 185, 185, 255);

// BORDER
const Color_4ub DEFAULT_ENABLED_BR(0, 0, 0, 255);
const Color_4ub DEFAULT_DISABLED_BR(85, 85, 85, 255);


/**
 * 
 */
Control::Control(const string &controlName, int minWidth, int minHeight):	mParent(NULL),
																			mFont(NULL),
																			mSkin(NULL),
																			mCurrentSkinPart(NULL),
																			mTop(0), mBottom(0), mLeft(0), mRight(0),
																			mWidth(minWidth), mHeight(minHeight),
																			mMinWidth(minWidth), mMinHeight(minHeight),
																			mMaxWidth(MAX_WIDTH), mMaxHeight(MAX_HEIGHT),
																			mChildCounter(-1),
																			mBorderColor(DEFAULT_ENABLED_BR),
																			mBackgroundColor(DEFAULT_ENABLED_BG),
																			mForegroundColor(DEFAULT_ENABLED_FG),
																			mDrawBorder(false),
																			mAnchored(true),
																			mAutosize(false),
																			mEnabled(true),
																			mUsingSkin(false),
																			mName(controlName),
																			mCaption(""),
																			mSkinPartType(""),
																			mTextAlignment(ALIGN_LEFT),
																			mMouseX(0), mMouseY(0),
																			mRelativeX(0), mRelativeY(0),
																			mNumChildren(0),
																			//mParentIndex(0),
																			mHasFocus(false),
																			mIsVisible(true),
																			mIsHidden(false),
																			mIsHighlighted(false),
																			mCanTab(false),
																			mHasSelection(false)
																		
{
}


Control::Control(const Control& src):	mParent(src.mParent),
										mFont(src.mFont),
										mSkin(src.mSkin),
										mCurrentSkinPart(src.mCurrentSkinPart),
										mTop(src.mTop), mBottom(src.mBottom), mLeft(src.mLeft), mRight(src.mRight),
										mWidth(src.mWidth), mHeight(src.mHeight),
										mMinWidth(src.mMinWidth), mMinHeight(src.mMinHeight),
										mMaxWidth(src.mMaxWidth), mMaxHeight(src.mMaxHeight),
										mChildCounter(src.mChildCounter),
										mBorderColor(DEFAULT_ENABLED_BR),
										mBackgroundColor(DEFAULT_ENABLED_BG),
										mForegroundColor(DEFAULT_ENABLED_FG),
										mDrawBorder(src.mDrawBorder),
										mAnchored(src.mAnchored),
										mAutosize(src.mAutosize),
										mEnabled(src.mEnabled),
										mUsingSkin(src.mUsingSkin),
										mChildControls(src.mChildControls),
										mName(src.mName),
										mCaption(src.mCaption),
										mSkinPartType(src.mSkinPartType),
										mTextAlignment(ALIGN_LEFT),
										mMouseX(src.mMouseX), mMouseY(src.mMouseY),
										mRelativeX(src.mRelativeX), mRelativeY(src.mRelativeY),
										mNumChildren(src.mNumChildren),
										//mParentIndex(src.mParentIndex),
										mHasFocus(src.mHasFocus),
										mIsVisible(src.mIsVisible),
										mIsHidden(src.mIsHidden),
										mIsHighlighted(src.mIsHighlighted),
										mFontList(src.mFontList),
										mCanTab(src.mCanTab),
										mHasSelection(src.mHasSelection)
{
	//std::cout << "    *** CONTROL COPY CONSTRUCTOR ***" << endl;
}


Control::~Control()
{
	clearChildren();
}


/**
 * Overloaded equality operator.
 */
bool Control::operator==(const Control& control)
{
	if(this == &control)
		return true;

	return false;
}


/**
 * Overloaded equality operator.
 * 
 * Determines if two Control's are the same based on name.
 * 
 * \note	The name comparison is case insensitive.
 */
bool Control::operator==(const std::string& name)
{
	if(toLowercase(getName()) == toLowercase(name))
		return true;

	return false;
}


/**
 * Adds a Control as a child of the parent Control.
 * 
 * \param	child	A pointer to a Control that will become the
 * 					child of the parent Control.
 * 
 * \warning	The child Control passed into this function will
 *			become owned by the parent Control. Do not free
 *			the control pointed at.
 */
void Control::add(Control *child)
{
	if(child)
	{
		// Avoid Control's with duplicate names.
		if(mChildControls.size() > 0)
		{
			for(size_t i = 0; i < mChildControls.size(); i++)
			{
				if(child->getName() == mChildControls[i]->getName())
				{
					std::cout << "WARN: " << "Attempted to add Control with duplicate name: " << child->getName() << endl;
					return;
				}
			}
		}

		// Add Control to the end of the list.
		mChildControls.push_back(child);

		child->mParent = this;
		child->setSkin(mSkin);
		child->setFontList(mFontList);
		child->initialize();
		
		MouseDown.connect(child, &Control::onMouseButtonDown_b);
		MouseUp.connect(child, &Control::onMouseButtonUp_b);
		MouseClick.connect(child, &Control::onClick_b);
		MouseDrag.connect(child, &Control::onDrag_b);

		mNumChildren = mChildControls.size();
	}

}


/**
 * Removes a child Control from a parent Control.
 *
 * \param	child	A pointer to a child Control.
 *
 * \warning	This function frees any memory allocated for the specified Control
 *			so all references or pointers to it will be invalid.
 * 
 * \note	If the given pointer is not a child of the Control then
 *			the call will be ignored.
 */
void Control::remove(Control *child)
{
	vector<Control*>::iterator i = mChildControls.begin();
	while(i != mChildControls.end())
	{
		if((*i) == child)
		{
			delete (*i);
			(*i) = NULL;
			mChildControls.erase(i++);
			mNumChildren--;
			return;
		}
		else
			++i;
	}
}


/**
 * Finds a child Control by name.
 *
 * \param	controlName	A string containing the name of the Control. Not case sensitive.
 *
 * \return	A pointer to a Control if found. NULL otherwise.
 * 
 * \warning	The pointer returned by this function is owned by Control. Do not
 *			delete it or free it manually.
 *
 * \todo	Update Control to contain an STL algorithm compatible function so
 *			we can use find() instead of having to iterate through controls
 *			manually.
 */
Control* Control::getChild(const string& childName) const
{
	if(mChildControls.size() > 0)
		for(size_t i = 0; i < mChildControls.size(); i++)
			if(toLowercase(mChildControls[i]->getName()) == toLowercase(childName))
				return mChildControls[i];

	return NULL;
}


/**
 * Brings the specified control to the front of the list.
 */
void Control::bringToFront(Control *control)
{
	// Stubbed this out because it was a 'hack' to try
	// to get DropDown working. I have another method
	// that I believe will be much more effective and
	// will make this function obsolete.
}


/**
 * Sets the name of the Control.
 */
void Control::setName(const string& name)
{
	mName = name;
}


/**
 * Sets the border color of the Control.
 *
 * \param	r	Red value. Must be between 0 - 255.
 * \param	g	Green value. Must be between 0 - 255.
 * \param	b	Blue value. Must be between 0 - 255.
 */
void Control::setBorderColor(int r, int g, int b, int a)
{
	mBorderColor = Color_4ub(r, g, b, a);
}


/**
 * Sets the border color of the Control.
 *
 * \param	color	A reference to a Color_4ub.
 */
void Control::setBorderColor(const Color_4ub& color)
{
	mBorderColor = color;
}


/**
 * Sets the background color of the Control.
 *
 * \param	r	Red value. Must be between 0 - 255.
 * \param	g	Green value. Must be between 0 - 255.
 * \param	b	Blue value. Must be between 0 - 255.
 */
void Control::setBackgroundColor(int r, int g, int b, int a)
{
	mBackgroundColor = Color_4ub(r, g, b, a);
}


/**
 * Sets the background color of the Control.
 *
 * \param	color	A reference to a Color_4ub.
 */
void Control::setBackgroundColor(const Color_4ub& color)
{
	mBackgroundColor = color;
}


/**
 * Sets the foreground color of the Control.
 *
 * \param	r	Red value. Must be between 0 - 255.
 * \param	g	Green value. Must be between 0 - 255.
 * \param	b	Blue value. Must be between 0 - 255.
 */
void Control::setForegroundColor(int r, int g, int b, int a)
{
	mForegroundColor = Color_4ub(r, g, b, a);
}


/**
 * Sets the foreground color of the Control.
 *
 * \param	color	A reference to a Color_4ub.
 */
void Control::setForegroundColor(const Color_4ub& color)
{
	mForegroundColor = color;
}


/**
 * Sets the alignment of the caption text for the Control.
 *
 * \param	alignment	Takes a valid TextAlignment value.
 * 
 * \note	Alignment is handled by derived controls and so
 *			is sometimes ignored. See documentation for
 *			individual derived controls.
 */
void Control::setTextAlignment(TextAlignment alignment)
{
	mTextAlignment = alignment;
	onTextAlign();
}


/**
 * Sets the caption for the Control.
 *
 * \param	caption		A string containing the text for the caption.
 */
void Control::setCaption(const string& caption)
{
    mCaption = caption;

	if(mAutosize)
		setSize(mFont->getWidth(caption), mHeight);

	onChanged();
}


/**
 * \todo	This should handle setting local copies of images and coordinates
 */
void Control::setSkin(Skin* skin)
{
	if(!skin)
	{
		mUsingSkin = false;
		mSkin = NULL;
		mFont = new Font();
		std::cout << "WARN: " << "Attempted to set a NULL Skin." << endl;
		std::cout << "WARN: " << "Default font being used." << endl;
	}
	else if(!skin->isLoaded())
	{
		mUsingSkin = false;
		mSkin = NULL;
		mFont = new Font();
		std::cout << "WARN: " << "Requested skin has not properly loaded: " << mSkin->getErrorMessage() << endl;
		std::cout << "WARN: " << "Default font being used." << endl;
	}
	else
	{
		mSkin = skin;
		mFont = mSkin->getSkinFont();

		for(size_t i = 0; i < mChildControls.size(); i++)
		{
			mChildControls[i]->setSkin(mSkin);
		}
	}	
}


/**
 * Attempts to set the skin graphics to this Control to the requested type and state.
 *
 * \param	type	A string representing a control type.
 * \param	state	A state for the requested control type. Must be of type \c enum \c ControlState.
 *
 * \note	Paramter \c state is optional and defaults to STATE_NORMAL.
 */
void Control::setSkinState(const string& type, ControlState state)
{
	string partToGet = type;

//	if(!mSkinPartType.empty())
//		partToGet = mSkinPartType;

	if(mSkin)
	{
		// Check that the skin has the desired part.
		if(!mSkin->hasPart(partToGet))
		{
			std::cout << "WARN: " << "Requested skin part '" << type << "' is not defined." << endl;
			mUsingSkin = false;
			return;
		}

		// Attempt to get the requested part.
		mCurrentSkinPart = mSkin->getPart(partToGet, state);
		if(!mCurrentSkinPart)
		{
			std::cout << "WARN: " << "Requested skin part '" << type << "' is not defined." << endl;
			mUsingSkin = false;
			return;
		}
		else
		{
			mUsingSkin = true;

			// Set Control colors.
			Color_4ub def = mCurrentSkinPart->getForeground();
			setForegroundColor(def.red(), def.green(), def.blue());
			def = mCurrentSkinPart->getBackground();
			setBackgroundColor(def.red(), def.green(), def.blue());
			def = mCurrentSkinPart->getBorder();
			setBorderColor(def.red(), def.green(), def.blue());

			setMinimumSize(mCurrentSkinPart->getMinimumWidth(), mCurrentSkinPart->getMinimumHeight());

			if(mCurrentSkinPart->getPartType() == PART_MONAD)
				setSize(mCurrentSkinPart->getMinimumWidth(), mCurrentSkinPart->getMinimumHeight());

			return;
		}

		// If the requested state is unavailable attempt to get the requested part with a "Normal" state.
		mCurrentSkinPart = mSkin->getPart(partToGet);
		if(!mCurrentSkinPart)
		{
			std::cout << "WARN: " << "Requested skin part '" << type << "' doesn't have a 'Normal' state defined." << endl;
			mUsingSkin = false;
			return;
		}
		else
			setMinimumSize(mCurrentSkinPart->getMinimumWidth(), mCurrentSkinPart->getMinimumHeight());
	}
	else
		mUsingSkin = false;
}


/**
 * Attempts to set the skin graphics to this Control to the requested state.
 *
 * \param	state	A state for the requested control type. Must be of type \c enum
 *					\c ControlState.
 *
 * \note	Paramter \c state is optional and defaults to STATE_NORMAL.
 *
 * \note	This function internally calls Control::setSkinState(string, ControlState)
 *			with the part type defined by setSkinPartType().
 */
void Control::setSkinState(ControlState state)
{
	setSkinState(mSkinPartType, state);
}


/**
 * Sets the internal skin part type to use.
 */
void Control::setSkinPartType(const string& type)
{
	mSkinPartType = type;
}


/**
 * Gets whether or not this Control is skinned.
 */
bool Control::isSkinned() const
{
	return mUsingSkin;
}


/**
 * Gets the number of children the Control is a parent of.
 */
int Control::getNumChildren() const
{
	return mNumChildren;
}


/**
 * Sets the margins of the Control.
 *
 * Child Control's added to this Control can only be positioned within
 * the margins set by this function.
 */
void Control::setMargins(int top, int bottom, int left, int right)
{
    mTop = top;
    mBottom = bottom;
    mLeft = left;
    mRight = right;    
}


/**
 * Sets the minimum width and height of the Control.
 * 
 * \note	Width and Height can never be less than one so
 *			passing in values less than 1 will set the value
 *			to 1.
 */
void Control::setMinimumSize(int w, int h)
{
	(w < 1) ? mMinWidth = 1 : mMinWidth = w;
	(h < 1) ? mMinHeight = 1 : mMinHeight = h;
}


/**
 * Sets the maximum width and height of the Control.
 */
void Control::setMaximumSize(int w, int h)
{
	(w < mMinWidth) ? mMaxWidth = mMinWidth : mMaxWidth = w;
	(h < mMinHeight) ? mMaxHeight = mMinHeight : mMaxHeight = h;
}


/**
 * Sets the size of the Control.
 *
 * \note	Controls cannot be set smaller than their defined
 *			minimum width/height.
 *
 * \note	Controls cannot be set larger than their defined
 *			maximum width/height.
 */
void Control::setSize(int w, int h)
{
	setWidth(w);
	setHeight(h);
}


/**
 * Sets the width of the Control.
 *
 * \note	Controls cannot be set smaller than their defined
 *			minimum width.
 *
 * \note	Controls cannot be set larger than their defined
 *			maximum width.
 *
 * \todo	Doesn't take into account a parent's margins.
 */
void Control::setWidth(int w)
{
	if(w <= 0)
		return;

	if(mParent)
	{
		if(getRelativeX() + w > getParent()->getWidth())
			mWidth = getParent()->getWidth() - getMarginLeft() - (getRelativeX() * 2);
		else
			mWidth = w;
	}
	else if(w < mMinWidth)
		mWidth = mMinWidth;
	else
		mWidth = w;

	onResized();
}


/**
 * Sets the height of the Control.
 *
 * \note	Controls cannot be set smaller than their defined
 *			minimum height.
 *
 * \note	Controls cannot be set larger than their defined
 *			maximum height.
 *
 * \todo	Doesn't take into account a parent's margins.
 */
void Control::setHeight(int h)
{
	if(h <= 0)
		return;

	if(mParent)
	{
		if(getRelativeY() + h > mParent->getHeight())
		{
			//cout << mName << ": " << getRelativeY() << ", " << h << ", " << mParent->getHeight() << ", " << (getRelativeY() + h) - mParent->getHeight() << endl;
			mHeight = h - ((getRelativeY() + h) - mParent->getHeight());

			// Clamp height to the minimum.
			if(mHeight < mMinHeight)
				mHeight = mMinHeight;

			// Controls must always stay within the parent so if the Control with the clamped height
			// is still outside of the parent adjust its position.
			if((getRelativeY() + mHeight) > mParent->getHeight())
				setRelative(getRelativeX(), getRelativeY() - ((getRelativeY() + mHeight) - mParent->getHeight()));
		}
		else
			mHeight = h;
	}
	else if(h < mMinHeight)
		mHeight = mMinHeight;
	else if(h > mMaxHeight)
		mHeight = mMaxHeight;
	else
		mHeight = h;

	onResized();
}


/**
 * Gets the current width of the Control.
 */
int Control::getWidth() const
{
	return mWidth;
}


/**
 * Gets the current height of the Control.
 */
int Control::getHeight() const
{
	return mHeight;
}


/**
 * Gets the minimum valid width of the Control.
 */
int Control::getMinWidth() const
{
	return mMinWidth;
}


/**
 * Gets the minimum valid height of the Control.
 */
int Control::getMinHeight() const
{
	return mMinHeight;
}


/**
 * Gets the maximum valid width of the Control.
 */
int Control::getMaxWidth() const
{
	return mMaxWidth;
}


/**
 * Gets the maximum valid height of the Control.
 */
int Control::getMaxHeight() const
{
	return mMaxHeight;
}


/**
 * Sets relative position relative to this Control's parent Control.
 *
 * \todo        This function does not check for constraints within the Parent Control.
 */
void Control::setRelative(int x, int y)
{
	mRelativeX = x;
	mRelativeY = y;
}

/**
 * Gets the X-Coordinate of the Control relative to its parent.
 */
int Control::getRelativeX() const
{
	return mRelativeX;
}


/**
 * Gets the Y-Coordinate of the Control relative to its parent.
 */
int Control::getRelativeY() const
{
	return mRelativeY;
}


/**
 * Sets absolute position and size attributes of this Control.
 *
 * \note	This function will set the absolute coordinate values
 *			of a Control if it doesn't have a parent. Otherwise it
 *			will set coordinates relative to its parent.
 */
void Control::setAbsolute(int x, int y)
{
	mRelativeX = x - (mParent ? mParent->getAbsoluteX() : 0);
	mRelativeY = y - (mParent ? mParent->getAbsoluteY() : 0);
}


/**
 * Gets the absolute X-Coordinate of the Control on screen.
 */
int Control::getAbsoluteX() const
{
	return (mParent ? mParent->getAbsoluteX() : 0) + mRelativeX;
}


/**
 * Gets the absolute Y-Coordinate of the Control on screen.
 */
int Control::getAbsoluteY() const
{
	return (mParent ? mParent->getAbsoluteY() : 0) + mRelativeY;
}


/**
 * Gets the top margin of the Control.
 */
int Control::getMarginTop() const
{
    return mTop;
}


/**
 * Gets the bottom margin of the Control.
 */
int Control::getMarginBottom() const
{
    return mBottom;
}


/**
 * Gets the left margin of the Control.
 */
int Control::getMarginLeft() const
{
    return mLeft;
}


/**
 * Gets the right margin of the Control.
 */
int Control::getMarginRight() const
{
    return mRight;
}


/**
 * Gets the parent of the Control.
 */
Control* Control::getParent()
{
	if (mParent != NULL)
		return mParent;
	else
		return NULL;
}


/**
 * Determines if we can draw with skin or if we must default.
 */
void Control::draw()
{
	if(mIsVisible && !mIsHidden)
	{
		if(mUsingSkin)
			drawSkin();
		else
			drawPlain();

		if(mChildControls.size() > 0)
			for(size_t i = 0; i < mChildControls.size(); i++)
				mChildControls[i]->draw();

		drawBorder();
		
		if(mHasSelection)
			drawTextHighlight();
		
		if(mHasFocus && mIsHighlighted)
			drawHighlight();
	}

}


/**
 * Clears the children from the Control.
 * 
 * \warning	Any references to children will be invalidated
 *			after calling this function.
 */
void Control::clearChildren()
{
	// Iterate through all children Control's and call their clearChildern()
	// function. Note that we a direct index access instead of the at()
	// function so this is a potential point of failure.
	for(size_t i = 0; i < mChildControls.size(); i++)
	{
		mChildControls[i]->clearChildren();
		delete mChildControls[i];
		mChildControls[i] = NULL;
	}
	mChildControls.clear();
}


/**
 * Returns whether the control has Focus or not.
 */
bool Control::hasFocus() const
{
	return mHasFocus;
}


/**
 * Sets whether the control has Focus or not.
 */
void Control::hasFocus(bool focus)
{
	mHasFocus = focus;

	if(mParent && mHasFocus)
		mParent->bringToFront(this);

	// Debug info -- remove or comment when no longer needed.
	//if(focus)
	//	std::cout << Logger::debug() << "" << mName << " gained focus." << endl;

	// Iterate through all children and set their focus.
	if(!focus)
	{
		onLostFocus();

		if(mNumChildren > 0)
		{
			for(int i = 0; i < mNumChildren; i++)
				mChildControls[i]->hasFocus(false);
		}
	}
	else
		onFocus();
}


/**
 * Gets whether or not the Control can gain focus by being tabbed to.
 */
bool Control::isTabable() const
{
	return mCanTab;
}


/**
 * Sets whether or not the Control can gain focus by being tabbed to.
 */
void Control::isTabable(bool tabable)
{
	mCanTab = tabable;
}


/**
 * Sets whether or not the Control autosizes itself to fit its contents.
 */
void Control::isAutosized(bool autosize)
{
	mAutosize = autosize;
}


/**
 * Gets whether or not the Control autosizes itself to fit its contents.
 */
bool Control::isAutosized() const
{
	return mAutosize;
}


/**
 * Sets whether or not the Control is anchored (cannot be dragged).
 */
void Control::isAnchored(bool anchored)
{
	mAnchored = anchored;
}


/**
 * Gets whether or not the Control is anchored (cannot be dragged).
 */
bool Control::isAnchored() const
{
	return mAnchored;
}


/**
 * Sets the font from a pointer to a Font object.
 *
 * \param	font	A pointer to a Font. Must not be NULL.
 *
 * \note	There is no checking for NULL or invalid Font objects.
 *			This was a design decision to reduce the number of tests
 *			during Control rendering.
 *
 * \warning	Passing in a NULL or invalid pointer to a Font object will
 *			result in undefined behavior.
 */
void Control::setFont(Font *font)
{
	if(font)
		mFont = font;
}


/**
 * Sets the Font for the Control to use from a FontList.
 * 
 * \param	fontName	Name of the Font to use for the Control.
 * 
 * \note	If the named Font is not in the Font list, a default Font will
 *			be created for the Control.
 * 
 * \note	The Font set for the Control will be assigned to any children
 *			added to the Control after the Font was set. It will not affect
 *			any children that were added before Control::setFont() is called.
 */
void Control::setFont(const string& fontName)
{
	if(mFontList[fontName])
		mFont = mFontList[fontName];
	else
		mFont = new Font();
}


/**
 * \todo	COMMENT ME!
 */
void Control::setFontList(FontList fontList)
{
	if(fontList.empty())
	{
		std::cout << "WARN: " << "Attempting to set an empty FontList in Control." << endl;
		return;
	}

	if(mFontList.empty())
		mFontList = fontList;
	else
	{
		FontList::iterator fontIt = mFontList.begin();

		while(fontIt != mFontList.end())
		{
			delete fontIt->second;
			fontIt->second = NULL;
			fontIt++;
		}

		mFontList.clear();
		mFontList = fontList;
	}
}


/**
 * Sets whether or not the Control is enabled.
 */
void Control::isEnabled(bool enabled)
{
	mEnabled = enabled;

	onEnabled();
}


/**
 * Gets whether or not the Control is enabled.
 */
bool Control::isEnabled() const
{
	return mEnabled;
}


/**
 * Sets whether or not the Control is enabled.
 */
void Control::isTextHighlighted(bool textHighlight)
{
	mHasSelection = textHighlight;
}


/**
 * Gets whether or not the Control is enabled.
 */
bool Control::isTextHighlighted() const
{
	return mHasSelection;
}
	


/**
 * Tells the Control whether or not to draw a border around itself.
 *
 * \param	bool	\c True to draw border, \c False otherwise.
 */
void Control::drawBorder(bool draw)
{
	mDrawBorder = draw;
}


/**
 * Draws a border around the Control using the supplied Border Color.
 */
void Control::drawBorder()
{
	if(mDrawBorder)
		Singleton<Renderer>::get().drawBox(getAbsoluteX(), getAbsoluteY(), mWidth, mHeight, mBorderColor.red(), mBorderColor.green(), mBorderColor.blue());
}





/**
 * Sets whether or not the Control is visible.
 *
 * \param	visible		Can be \c true or \c false.
 */
void Control::isVisible(bool visible)
{
	mIsVisible = visible;
}


/**
 * Gets whether or not the Control is visible.
 */
bool Control::isVisible() const
{
	return mIsVisible;
}


/**
 * Sets whether the Control is hidden or not.
 *
 * \note	A Control that's hidden should not be confused with a Control
 *			that's not visible that become visible when Control::show() is
 *			versus hidden controls which are not shown when Control::show()
*			is called.
 */
void Control::isHidden(bool hidden)
{
	mIsHidden = hidden;

	if(hidden)
		mIsVisible = false;
	else
		mIsVisible = true;
}


/**
 * Returns if this Control is hidden.
 *
 * \note	Controls that are hidden should not be confused with Controls
 *			that are simply not visible. Hidden Controls are not made
 *			visible when Control::show() is called.
 */
bool Control::isHidden() const
{
	return mIsHidden;
}


/**
 * Sets whether or not the Control is highlighted.
 */
void Control::isHighlighted(bool highlighted)
{
	mIsHighlighted = highlighted;
}


/**
 * Sets whether or not the Control is highlighted.
 */
bool Control::isHighlighted() const
{
	return mIsHighlighted;
}


/**
 * Show's a Control and all if its children except for those that are hidden.
 *
 * \note	Showing a control also gives it input focus.
 */
void Control::show()
{
	if(isHidden())
		return;

	onShow();
	
	mIsVisible = true;
	//mHasFocus = true;
	for(size_t i = 0; i < mChildControls.size(); i++)
		mChildControls[i]->show();
}


/**
 * Hide's a Control and all if its children.
 *
 * \note	This function should not be mistaken with the isHidden()
 *			functions.
 */
void Control::hide()
{
	if(isHidden())
		return;

	onHide();

	mIsVisible = false;
	hasFocus(false);

	for(size_t i = 0; i < mChildControls.size(); i++)
	{
		mChildControls[i]->hide();
		
		// Debug
		//std::cout << mChildControls[i]->getName() << " is now hidden." << endl;
	}
}


/**
 * Gets the name of the Control.
 *
 * \note	Control's name is not the same as its
 *			caption although the two may be set to
 *			the same value.
 */
std::string Control::getName() const
{
	return mName;
}


/**
 * Pumps events, runs logic and instructs all children to update.
 */
void Control::update()
{
	logic();

	// Iterate through all children Control's and call their update()
	// function. Note that we use a direct index access instead of the
	// at() function so this is a potential point of failure.
	if(mChildControls.size() > 0)
		for(size_t i = 0; i < mChildControls.size(); i++)
			mChildControls[i]->update();
}


//void Control::push()
//{
//	// If incoming event is a MOUSE_DOWN, iterate through children and check that
//	// event occurred within child. If it did, set focus. Otherwise ignore.
//	if(event->getType() == EVENT_MOUSE_DOWN_LEFT)
//	{
//		if(mChildControls.size() > 0)
//		{
//			for(size_t i = 0; i < mChildControls.size(); i++)
//			{
//				Control *control = mChildControls[i];
//				if(isPointInRect(event->getFirst(), event->getSecond(), control->getAbsoluteX(), control->getAbsoluteY(), control->getWidth(), control->getHeight()) && isVisible())
//				{
//					control->hasFocus(true);
//				}
//				else
//					control->hasFocus(false);
//			}
//		}
//	}
//
//	// If Control has focus, check through events and process them.
//	if(mHasFocus && mEnabled)
//	{
//		switch(event->getType())
//		{
//			case EVENT_MOUSE_CLICK_LEFT:
//				// Only submit the click if the click occured within the confines of this Control.
//				if(isPointInRect(event->getFirst(), event->getSecond(), getAbsoluteX(), getAbsoluteY(), mWidth, mHeight) && isVisible())
//				{
//					addEvent(new Event(*event));
//					onClick(event);
//					event->consume();
//				}
//				break;
//
//			case EVENT_MOUSE_DOWN_LEFT:
//				addEvent(new Event(*event));
//				onMouseButtonDown(event);
//				event->consume();
//				break;
//
//			case EVENT_MOUSE_BUTTON_UP:
//				addEvent(new Event(*event));
//				onMouseButtonUp(event);
//				event->consume();
//				break;
//
//			case EVENT_MOUSE_WHEEL_UP:
//			case EVENT_MOUSE_WHEEL_DOWN:
//				addEvent(new Event(*event));
//				onMouseWheelMotion(event);
//				event->consume();
//				break;
//
//			case EVENT_GUI_DRAG:
//				addEvent(new Event(*event));
//				onDrag(event);
//				event->consume();
//				break;
//
//			case EVENT_MOUSE_MOVE:
//				onMouseMove(event);
//				mMouseX = event->getFirst();
//				mMouseY = event->getSecond();
//				event->consume();
//				break;
//
//			case EVENT_GUI_CHANGED:
//				onChanged(event);
//				event->consume();
//				break;
//
//			case EVENT_KEYDOWN:
//				addEvent(new Event(*event));
//				onKeyDown(event);
//				event->consume();
//				break;
//
//			case EVENT_KEYUP:
//				addEvent(new Event(*event));
//				onKeyUp(event);
//				event->consume();
//				break;
//
//			default:
//				break;
//		}
//	}
//	else
//	{
//		// CHANGED events are special and always need to be responed to regardless of
//		// focus or whether or not the control is Enabled. There may be other cases in
//		// the future of events that need to be responded regardless of status. Such
//		// cases should be handled here.
//	
//		switch(event->getType())
//		{
//			case EVENT_GUI_CHANGED:
//				onChanged(event);
//				event->consume();
//				break;
//		}
//	}
//}
