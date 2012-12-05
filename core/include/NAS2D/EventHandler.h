// ==================================================================================
// = NAS2D
// = Copyright © 2008 - 2012 New Age Software
// ==================================================================================
// = NAS2D is distributed under the terms of the zlib license. You are free to copy,
// = modify and distribute the software under the terms of the zlib license.
// = 
// = Acknowledgement of your use of NAS2D is appriciated but is not required.
// ==================================================================================

#ifndef __EVENT_HANDLER__
#define __EVENT_HANDLER__

#include "NAS2D/sigslot.h"

#include "NAS2D/Event/KeyMap.h"
#include "NAS2D/Event/MouseButtons.h"


#include <string>

/**
 * \class	EventHandler
 * \brief	Handles and dispatches low-level events.
 * 
 */
class EventHandler: public sigslot::has_slots<>
{
public:

	/**
	 * \typedef	ActivateEventCallback
	 * \brief	Triggered whenever the application gains or loses focus.
	 * 
	 * The callback function expects one bool paramter.
	 * 
	 * \code
	 * void function(bool gained);
	 * \endcode
	 * 
	 * \arg \c gained	Bool value indicating whether or not the app lost focus.
	 */
	typedef sigslot::signal1<bool>					ActivateEventCallback;

	/**
	 * \typedef	JoystickAxisMotionEventCallback
	 * \brief	Joystic axis events generate three values: Device ID,
	 *			Axis ID and Axis Position.
	 * 
	 * The callback function expects three int paramter.
	 * 
	 * \code
	 * void function(int deviceId, int axisId, int pos);
	 * \endcode
	 * 
	 * \arg \c deviceId	Joystick ID which this event was generated from.
	 * \arg \c axisId	Axis ID. Axis 0 is x, Axis 1 is Y. Some joysticks
						use additional axis as buttons.
	 * \arg \c pos		Current position of the axis.
	 */
	typedef sigslot::signal3<int, int, int>			JoystickAxisMotionEventCallback;

	/**
	 * \typedef	JoystickBallMotionEventCallback
	 * \brief	Joystick trackball events generate four values: Device ID,
	 *			Ball ID, X motion change and Y motion change.
	 * 
	 * The callback function expects four int paramter.
	 * 
	 * \code
	 * void function(int deviceId, int ballId, int xChange, int yChange);
	 * \endcode
	 * 
	 * \arg \c deviceId	Joystick ID which this event was generated from.
	 * \arg \c ballId	Trackball ID.
	 * \arg \c xChange	Change in relative position of the X position.
	 * \arg \c yChange	Change in relative position of the Y position.
	 */
	typedef sigslot::signal4<int, int, int, int>	JoystickBallMotionEventCallback;

	/**
	 * \typedef	JoystickButtonEventCallback
	 * \brief	Joystick button events generate two values: Device ID
	 *			and Button ID. Button events occure when a joystick
	 *			button is pressed or released.
	 * 
	 * The callback function expects two int paramters.
	 * 
	 * \code
	 * void function(int deviceId, int buttonId);
	 * \endcode
	 * 
	 * \arg \c deviceId	Joystick ID which this event was generated from.
	 * \arg \c buttonId	Button ID which the event was generated from.
	 */
	typedef sigslot::signal2<int, int>				JoystickButtonEventCallback;

	/**
	 * \typedef	JoystickHatMotionEventCallback
	 * \brief	Joystick hat motion events generate three values: Device ID,
	 *			Hat ID, and Hat Position.
	 * 
	 * The callback function expects three int paramters.
	 * 
	 * \code
	 * void function(int deviceId, int hatId, int pos);
	 * \endcode
	 * 
	 * \arg \c deviceId	Joystick ID which this event was generated from.
	 * \arg \c hatId	Hat ID.
	 * \arg \c pos		Current position of the hat.
	 */
	typedef sigslot::signal3<int, int, int>			JoystickHatMotionEventCallback;

	/**
	 * \typedef	KeyEventCallback
	 * \brief	Keyboard key eventsgenerate two values: Keyboard
	 *			scan code and key modifier.
	 * 
	 * The callback function expects one KeyCode paramter and one KeyModifier paramter.
	 * 
	 * \code
	 * void function(KeyCode key, KeyModifier mod);
	 * \endcode
	 * 
	 * \arg \c key	KeyCode representing a key on the keyboard.
	 * \arg \c mod	explanation
	 */
	typedef sigslot::signal2<KeyCode, KeyModifier>	KeyEventCallback;

	/**
	 * \typedef	MouseButtonEventCallback
	 * \brief	Mouse button events generate three values: MouseButton, x
	 *			and y.
	 * 
	 * The callback function expects one MouseButton and two int parameters.
	 * 
	 * \code
	 * void function(MouseButton button, int x, int y);
	 * \endcode
	 * 
	 * \arg \c button:	MouseButton value indicating which button is pressed.
	 * \arg	\c x:		X position of the mouse button event.
	 * \arg	\c y:		Y position of the mouse button event.
	 */
	typedef sigslot::signal3<MouseButton, int, int>	MouseButtonEventCallback;

	/**
	 * \typedef	MouseMotionEventCallback
	 * \brief	Mouse motion events generate four values: x, y, relative x
	 *			and relative y values.
	 * 
	 * The callback function expects four int paramters.
	 * 
	 * \code
	 * void function(int x, int y, int relX, int relY);
	 * \endcode
	 * 
	 * \arg	\c x:		Absolute x position of the mouse.
	 * \arg	\c y:		Absolute y position of the mouse.
	 * \arg	\c relX:	X position of the mouse relative to its last position.
	 * \arg	\c relY;	Y position of the mouse relative to its last position.
	 */
	typedef sigslot::signal4<int, int, int, int>	MouseMotionEventCallback;

	/**
	 * \typedef	QuitEventCallback
	 * \brief	Triggered whenever a Quit message is posted.
	 * 
	 * A Quit callback function requires no paramters.
	 * 
	 * \code
	 * void function(void);
	 * \endcode
	 */
	typedef sigslot::signal0<>						QuitEventCallback;

	/**
	 * Default c'tor.
	 */
	EventHandler()
	{}

	ActivateEventCallback&				activate();

	JoystickAxisMotionEventCallback&	joystickAxisMotion();
	JoystickBallMotionEventCallback&	joystickBallMotion();
	JoystickButtonEventCallback&		joystickButtonUp();
	JoystickButtonEventCallback&		joystickButtonDown();
	JoystickHatMotionEventCallback&		joystickHatMotion();

	KeyEventCallback&					keyUp();
	KeyEventCallback&					keyDown();

	MouseButtonEventCallback&			mouseButtonUp();
	MouseButtonEventCallback&			mouseButtonDown();
	
	MouseMotionEventCallback&			mouseMotion();

	QuitEventCallback&					quit();

	void grabMouse();
	void releaseMouse();
	void warpMouse(int x, int y);
	void mouseRelativeMode(bool rel);

	void pump();

private:

	ActivateEventCallback				mActivateEvent;
	JoystickAxisMotionEventCallback		mJoystickAxisMotionEvent;
	JoystickBallMotionEventCallback		mJoystickBallMotionEvent;
	JoystickButtonEventCallback			mJoystickButtonUpEvent;
	JoystickButtonEventCallback			mJoystickButtonDownEvent;
	JoystickHatMotionEventCallback		mJoystickHatMotionEvent;
	KeyEventCallback					mKeyUpEvent;
	KeyEventCallback					mKeyDownEvent;
	MouseButtonEventCallback			mMouseButtonUpEvent;
	MouseButtonEventCallback			mMouseButtonDownEvent;
	MouseMotionEventCallback			mMouseMotionEvent;
	QuitEventCallback					mQuitEvent;

};

void postQuitEvent();

#endif