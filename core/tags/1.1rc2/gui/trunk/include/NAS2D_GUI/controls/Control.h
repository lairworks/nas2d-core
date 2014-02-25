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

#ifndef _LOM_GUI_CONTROL_
#define _LOM_GUI_CONTROL_

#include "Skin.h"

#include "NAS2D/Mixer/Mixer.h"
#include "NAS2D/Renderer/Renderer.h"
#include "NAS2D/Resources/Font.h"
#include "NAS2D/Resources/Image.h"
#include "NAS2D/Common.h"
#include "NAS2D/sigslot.h"

#include <vector>
#include <string>


// Note that the trivial accessor functions provided for Control's interface
// are not defined in the header in an effort to keep the header interface as
// readable as possible. The definitions have been moved to the source file
// so that documentation comments can be stored there.


/**
 * \class Control
 * \brief Basic Control upon which all other Control's are derived.
 *
 * The Control class provides all basic functions that are common to
 * all Control objects within a GUI.
 *
 * In order to have a derived Control appear differently than other
 * Control objects, the Control::draw() function must be overridden.
 *
 * \note	Any derived Control that needs to render fonts differently
 *			than the base font must store a separate Font Resource.
 */
class Control : public sigslot::has_slots<>
{
public:

	/**
	 * \enum	Alignment of text within the Control.
	 */
	enum TextAlignment
	{
		ALIGN_LEFT,
		ALIGN_MIDDLE,
		ALIGN_RIGHT
	};

	Control(const std::string &controlName, int minWidth = 16, int minHeight = 16);
	Control(const Control& src);
	~Control();

	bool operator==(const Control& control);
	bool operator==(const std::string& name);

	virtual void add(Control *child);
	virtual void remove(Control *child);

	virtual Control* getChild(const std::string& childName) const;

	void setFont(Font *font);
	void setFont(const std::string& fontName);

	void setSkin(Skin *skin);

	void setAbsolute(int x, int y);
	void setRelative(int x, int y);

	void setMinimumSize(int w, int h);
	void setMaximumSize(int w, int h);

	void setWidth(int w);
	void setHeight(int h);
	void setSize(int w, int h);
    void setMargins(int top, int bottom, int left, int right);

	void setBorderColor(int r, int g, int b, int a = 255);
	void setBorderColor(const Color_4ub& color);

	void setBackgroundColor(int r, int g, int b, int a = 255);
	void setBackgroundColor(const Color_4ub& color);

	void setForegroundColor(int r, int g, int b, int a = 255);
	void setForegroundColor(const Color_4ub& color);

	void setTextAlignment(TextAlignment alignment);

	int getAbsoluteX() const;
	int getAbsoluteY() const;
	int getRelativeX() const;
	int getRelativeY() const;

	int getWidth() const;
	int getHeight() const;
	int getMinWidth() const;
	int getMinHeight() const;
	int getMaxWidth() const;
	int getMaxHeight() const;

    int getMarginTop() const;
    int getMarginBottom() const;
    int getMarginLeft() const;
    int getMarginRight() const;

	int getNumChildren() const;

	void drawBorder(bool);

	void hasFocus(bool focus);
	bool hasFocus() const;

	void setName(const std::string& name);
	std::string getName() const;
	void setCaption(const std::string& caption);

	void isTabable(bool tabable);
	bool isTabable() const;

	void isVisible(bool visible);
	bool isVisible() const;

	void isHidden(bool hidden);
	bool isHidden() const;

	void isHighlighted(bool highlighted);
	bool isHighlighted() const;

	void isAutosized(bool autosize);
	bool isAutosized() const;

	void isAnchored(bool anchored);
	bool isAnchored() const;

	void isEnabled(bool enabled);
	bool isEnabled() const;
	
	void isTextHighlighted(bool textHighlight);
	bool isTextHighlighted() const;

	void show();
	void hide();
	
	void update();
	void clearChildren();

	bool isSkinned() const;
	void setSkinState(ControlState state = STATE_NORMAL);
	void setSkinPartType(const std::string& type);

	// DEPRECATED
	void bringToFront(Control *control);

protected:
    friend class Gui;
	typedef std::vector<Control*> ControlList;
	typedef std::map<string, Font*> FontList;
	
	void setFontList(FontList fontList);

	Control *getParent();

	void setSkinState(const std::string& type, ControlState state = STATE_NORMAL);
	
	sigslot::signal0<> MouseMotion;
	sigslot::signal1<SDL_Event&> MouseDown;
	sigslot::signal1<SDL_Event&> MouseUp;
	sigslot::signal1<SDL_Event&> Pressed;
	sigslot::signal1<SDL_Event&> MouseClick;
	sigslot::signal1<SDL_Event&> MouseDrag;

	Control			*mParent;						/**< Reference to a Parent Control. */

	Font			*mFont;							/**< Internal Font used to render text. */
	Skin			*mSkin;							/**< Internal Skin used to render controls. */
	SkinPart		*mCurrentSkinPart;				/**< The current graphics to use for rendering. */

    int				mTop;							/**< Top margin in pixels. */
	int				mBottom;						/**< Bottom margin in pixels. */
	int				mLeft;							/**< Left margin in pixels. */
	int				mRight;							/**< Right margin in pixels. */

	int				mWidth;							/**< Absolute Width of the Control. */
	int				mHeight;						/**< Absolute Height of the Control. */

	int				mMinWidth;						/**< Minimum Width of the Control. */
	int				mMinHeight;						/**< Minimum Height of the Control. */

	int				mMaxWidth;						/**< Maximum Width of the Control. */
	int				mMaxHeight;						/**< Maximum Height of the Control. */

	int				mChildCounter;					/**< Counter to keep track of currently focused child. */

	Color_4ub		mBorderColor;					/**< Border color of the Control. */
	Color_4ub		mBackgroundColor;				/**< Background color of the Control. */
	Color_4ub		mForegroundColor;				/**< Foreground color of the Control. */
	
	bool			mDrawBorder;					/**< Flag to tell Control whether or not to draw a border around itself. */
	bool			mAnchored;						/**< Flag to determine whether or not this control can be dragged. Child controls that are anchored will still move with their parent. */
	bool			mAutosize;						/**< Flag used to determine if this control should automatically resize itself based on its content. */
	bool			mEnabled;						/**< Internal flag indicating whether or not this Control is enabled. */
	bool			mUsingSkin;						/**< Flag used to indicate whether or not to render with a UI skin. */

    ControlList		mChildControls;					/**< Internal list of Child Control's */

	std::string		mName;							/**< Name of the Control. */
	std::string		mCaption;						/**< Caption Text */
	std::string		mSkinPartType;					/**< Type to look for when requesting a skin part. */

	TextAlignment	mTextAlignment;					/**< Alignment of the text. */


	/**
	 * Called whenever a mouse button was clicked over a Control.
	 * This is purely for interaction with GUI to ensure the event is only called if within the control.
	 */
	void onClick_b(SDL_Event& event)
	{
		for(size_t i = 0; i < mChildControls.size(); ++i)
		{
			if(isPointInRect(event.motion.x, event.motion.y, mChildControls[i]->getAbsoluteX(), mChildControls[i]->getAbsoluteY(),
							 mChildControls[i]->getWidth(), mChildControls[i]->getHeight()) && mChildControls[i]->isVisible())
			{
				mChildControls[i]->onClick_b(event);
				return;
			}
		}
		
		onClick(event);
	}
	
	/**
	 * Called whenever a mouse button is released over a Control.
	 * 
	 * \note	This function is called regardless of whether or not
	 *			a mouse button was depressed over a Control.
	 */
	void onMouseButtonUp_b(SDL_Event& event)
	{
		for(size_t i = 0; i < mChildControls.size(); ++i)
		{
			if(isPointInRect(event.motion.x, event.motion.y, mChildControls[i]->getAbsoluteX(), mChildControls[i]->getAbsoluteY(),
							 mChildControls[i]->getWidth(), mChildControls[i]->getHeight()) && mChildControls[i]->isVisible())
			{
				mChildControls[i]->onMouseButtonUp_b(event);
				return;
			}
		}
		
		onMouseButtonUp(event);
	}
	
	/**
	 * Called whenever a mouse button is depressed over a Control.
	 */
	void onMouseButtonDown_b(SDL_Event& event)
	{
		for(size_t i = 0; i < mChildControls.size(); ++i)
		{
			if(isPointInRect(event.motion.x, event.motion.y, mChildControls[i]->getAbsoluteX(), mChildControls[i]->getAbsoluteY(),
							 mChildControls[i]->getWidth(), mChildControls[i]->getHeight()) && mChildControls[i]->isVisible())
			{
				mChildControls[i]->onMouseButtonDown_b(event);
				return;
			}
		}
		
		onMouseButtonDown(event);
	}
	
	/**
	 * Called whenever a mouse pointer is within a Control and changes position.
	 */
	void onMouseMove_b(SDL_Event& event)
	{
		onMouseMove(event);
		
		for(size_t i = 0; i < mChildControls.size(); i++)
		{
			mChildControls[i]->onMouseMove(event);
		}
	}
	
	/**
	 *	Called whenever a MOUSE_WHEEL_UP or MOUSE_WHEEL_DOWN are fired.
	 */
	void onMouseWheelMotion_b(SDL_Event& event)
	{
		for(size_t i = 0; i < mChildControls.size(); ++i)
		{
			if(isPointInRect(event.motion.x, event.motion.y, mChildControls[i]->getAbsoluteX(), mChildControls[i]->getAbsoluteY(),
							 mChildControls[i]->getWidth(), mChildControls[i]->getHeight()) && mChildControls[i]->isVisible())
			{
				mChildControls[i]->onMouseWheelMotion_b(event);
				return;
			}
		}
		
		onMouseWheelMotion(event);
	}
	
	/**
	 * Called whenever a Drag event occurs within a Control.
	 */
	void onDrag_b(SDL_Event& event)
	{
		for(size_t i = 0; i < mChildControls.size(); ++i)
		{
			if(isPointInRect(event.motion.x, event.motion.y, mChildControls[i]->getAbsoluteX(), mChildControls[i]->getAbsoluteY(),
							 mChildControls[i]->getWidth(), mChildControls[i]->getHeight()) && mChildControls[i]->isVisible())
			{
				mChildControls[i]->onDrag_b(event);
				return;
			}
		}
		
		onDrag(event);
	}
	
	/**
	 * Called whenever a mouse button was clicked over a Control.
	 */
	virtual void onClick(SDL_Event& event)
	{}
	
	/**
	 * Called whenever a mouse button was pressed over a Control.
	 */
	virtual void onPressed(SDL_Event& event)
	{}

	/**
	 * Called whenever text alignment for the Control is changed.
	 */
	virtual void onTextAlign()
	{}

	/**
	 * Called whenever a mouse button is released over a Control.
	 * 
	 * \note	This function is called regardless of whether or not
	 *			a mouse button was depressed over a Control.
	 */
	virtual void onMouseButtonUp(SDL_Event& event)
	{}

	/**
	 * Called whenever a mouse button is depressed over a Control.
	 */
	virtual void onMouseButtonDown(SDL_Event& event)
	{}

	/**
	 * Called whenever a mouse pointer is within a Control and changes position.
	 */
	virtual void onMouseMove(SDL_Event& event)
	{}

	/**
	 *	Called whenever a MOUSE_WHEEL_UP or MOUSE_WHEEL_DOWN are fired.
	 */
	virtual void onMouseWheelMotion(SDL_Event& event)
	{}

	/**
	 * Called whenever a Drag event occurs within a Control.
	 */
	virtual void onDrag(SDL_Event& event)
	{
		if(!mAnchored)
			setRelative(event.motion.xrel, event.motion.yrel);
	}

	/**
	 * Called whenever a Control changes its size.
	 */
	virtual void onResized()
	{}

	/**
	 * Called whenever a Control's hide() function is called.
	 */
	virtual void onHide()
	{}

	/**
	 * Called whenever a Control's show() function is called.
	 */
	virtual void onShow()
	{}

	/**
	 * Called whenever a Control gains focus.
	 *
	 * \note	Default implementation sets focus. Overridden versions
	 *			should either call the setFocus() function or explicitly
	 *			call the default implementation.
	 */
	virtual void onFocus()
	{
		std::cout << mName << " gained focus!" << endl;
	}

	/**
	 * Called whenever a Control loses focus.
	 *
	 * \note	Default implementation sets focus. Overridden versions
	 *			should either call the setFocus() function or explicitly
	 *			call the default implementation.
	 */
	virtual void onLostFocus()
	{}

	/**
	 * Called whenever a Control's data changes.
	 */
	virtual void onChanged()
	{}

	/**
	 * Sends an Event to all of the Event Listeners.
	 */
	virtual void onNotify()
    {}

	/**
	 * Called whenever a Control's setEnabled() function is called.
	 */
	virtual void onEnabled()
	{}

	/**
	 * Called whenever an EVENT_KEYDOWN event is fired.
	 */
	virtual void onKeyDown(SDL_Event& event)
	{
		//cout << translateKey(*event);
	}

	/**
	 * Called whenever an EVENT_KEYUP event is fired.
	 */
	virtual void onKeyUp(SDL_Event& event)
	{
		//cout << mName << " KeyUp: " << event->getFirst() << endl;
	}

private:
	Control();							// Do not allow default constructors.
	Control& operator=(const Control&); // Do not allow assignments.
	
	void draw();
	void drawBorder();

	/**
	 * Draws the Control using a Skin.
	 * 
	 * \note	Derived controls will need to override this function
	 *			in order to be visible.
	 */
	virtual void drawSkin()
	{}

	/**
	 * Draws the Control without a Skin.
	 * 
	 * \note	Derived controls will need to override this function
	 *			in order to be visible.
	 */
	virtual void drawPlain()
	{}

	/**
	 * Draws highlight.
	 */
	virtual void drawHighlight()
	{}
	
	/**
	 * Draws text highlight.
	 */
	virtual void drawTextHighlight()
	{}

	/**
	 * Called during each update loop.
	 * 
	 * Control::logic() is called every frame to give the Control
	 * an opportunity to perform internal logic.
	 */
	virtual void logic()
	{}

	/**
	 * Called after the constructor of Control and the Derived Control
	 * to give the Control an opportunity to set up its internal values.
	 */
	virtual void initialize()
	{}

	int			mMouseX;					/**< Internal mouse pointer X-Coordinate. */
	int			mMouseY;					/**< Internal mouse pointer Y-Coordinate. */
	int			mRelativeX;					/**< Control's x-position relative to its Parent. */
	int			mRelativeY;					/**< Control's y-position relative to its Parent. */
	int			mNumChildren;				/**< Number of Child Control's directly contained in this Control.*/

	bool		mHasFocus;					/**< Internal flag indicating whether or not this Control has focus. */
	bool		mIsVisible;					/**< Internal flag indicating whether or not this Control is visible. */
	bool		mIsHidden;					/**< Internal flag indicating that the Control is indefinitely hidden and should not be made visible by Control::show(). */
	bool		mIsHighlighted;				/**< Indicates that the Control is highlighted. */
	bool		mCanTab;					/**< Flag used to indicate if the control can be tabbed to. */
	bool		mHasSelection;				/**< Internal flag indicating whether or not this control has text selected. */

	FontList	mFontList;
};

extern const string CONTROL_HIDING;

#endif