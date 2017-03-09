// ==================================================================================
// = NAS2D
// = Copyright © 2008 - 2017 New Age Software
// ==================================================================================
// = NAS2D is distributed under the terms of the zlib license. You are free to copy,
// = modify and distribute the software under the terms of the zlib license.
// = 
// = Acknowledgement of your use of NAS2D is appriciated but is not required.
// ==================================================================================
#pragma once

#include "NAS2D/Signal.h"

#include "NAS2D/Event/KeyMap.h"

namespace NAS2D {

/**
 * \class	EventHandler
 * \brief	Handles and dispatches low-level events.
 * 
 */
class EventHandler
{
public:
	/**
	* \enum		MouseButton
	* \brief	Mouse button identifiers.
	*/
	enum MouseButton
	{
		BUTTON_NONE,
		BUTTON_LEFT,
		BUTTON_MIDDLE,
		BUTTON_RIGHT
	};
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
	typedef NAS2D::Signals::Signal1<bool>				ActivateEventCallback;

	/**
	 * \typedef	WindowHiddenEventCallback
	 * \brief	Triggered whenever the application's window is hidden or exposed.
	 * 
	 * The callback function expects one bool paramter.
	 * 
	 * \code
	 * void function(bool hidden);
	 * \endcode
	 * 
	 * \arg \c gained	Bool value indicating whether or not the app lost focus.
	 */
	typedef NAS2D::Signals::Signal1<bool>				WindowHiddenEventCallback;


	/**
	 * \typedef	WindowExposedEventCallback
	 * \brief	Triggered whenever the application's window is exposed.
	 * 
	 * \code
	 * void function(void);
	 * \endcode
	 * 
	 * \arg \c gained	Bool value indicating whether or not the app lost focus.
	 */
	typedef NAS2D::Signals::Signal0<void>				WindowExposedEventCallback;

	/**
	 * \typedef	WindowMinimizedEventCallback
	 * \brief	Triggered whenever the application's window is minimized.
	 * 
	 * \code
	 * void function(void);
	 * \endcode
	 * 
	 * \arg \c gained	Bool value indicating whether or not the app lost focus.
	 */
	typedef NAS2D::Signals::Signal0<void>				WindowMinimizedEventCallback;

	/**
	 * \typedef	WindowMaximizedEventCallback
	 * \brief	Triggered whenever the application's window is maximized.
	 * 
	 * \code
	 * void function(void);
	 * \endcode
	 * 
	 * \arg \c gained	Bool value indicating whether or not the app lost focus.
	 */
	typedef NAS2D::Signals::Signal0<void>				WindowMaximizedEventCallback;

	/**
	 * \typedef	WindowRestoredEventCallback
	 * \brief	Triggered whenever the application's window is restored to its original size and location.
	 * 
	 * \code
	 * void function(void);
	 * \endcode
	 * 
	 * \arg \c gained	Bool value indicating whether or not the app lost focus.
	 */
	typedef NAS2D::Signals::Signal0<void>				WindowRestoredEventCallback;

	/**
	 * \typedef	WindowMouseEnterEventCallback
	 * \brief	Triggered whenever the mouse enters the application's window.
	 * 
	 * \code
	 * void function(void);
	 * \endcode
	 * 
	 * \arg \c gained	Bool value indicating whether or not the app lost focus.
	 */
	typedef NAS2D::Signals::Signal0<void>				WindowMouseEnterEventCallback;

	/**
	 * \typedef	WindowMouseLeaveEventCallback
	 * \brief	Triggered whenever the mouse leaves the application's window.
	 * 
	 * \code
	 * void function(void);
	 * \endcode
	 * 
	 * \arg \c gained	Bool value indicating whether or not the app lost focus.
	 */
	typedef NAS2D::Signals::Signal0<void>				WindowMouseLeaveEventCallback;

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
	typedef NAS2D::Signals::Signal3<int, int, int>		JoystickAxisMotionEventCallback;

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
	typedef NAS2D::Signals::Signal4<int, int, int, int>	JoystickBallMotionEventCallback;

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
	typedef NAS2D::Signals::Signal2<int, int>			JoystickButtonEventCallback;

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
	typedef NAS2D::Signals::Signal3<int, int, int>		JoystickHatMotionEventCallback;

	/**
	 * \typedef	KeyDownEventCallback
	 * \brief	Keyboard key down events generate three values: Keyboard
	 *			scan code, key modifier and a repeat flag.
	 * 
	 * The callback function expects one KeyCode paramter, one KeyModifier paramter
	 * and one bool parameter.
	 * 
	 * \code
	 * void function(KeyCode key, KeyModifier mod, bool repeat);
	 * \endcode
	 * 
	 * \arg \c key		KeyCode representing a key on the keyboard.
	 * \arg \c mod		Keyboard modifier.
	 * \arg \c repeat	Indicates that this event is a repeat and not an initial key event.
	 */
	typedef NAS2D::Signals::Signal3<KeyCode, KeyModifier, bool>	KeyDownEventCallback;

	/**
	 * \typedef	KeyUpEventCallback
	 * \brief	Keyboard key up events generate two values: Keyboard
	 *			scan code and key modifier.
	 * 
	 * The callback function expects one KeyCode paramter and one KeyModifier paramter.
	 * 
	 * \code
	 * void function(KeyCode key, KeyModifier mod, bool repeat);
	 * \endcode
	 * 
	 * \arg \c key		KeyCode representing a key on the keyboard.
	 * \arg \c mod		Keyboard modifier.
	 * \arg \c repeat	Indicates that this event is a repeat and not an initial key event.
	 */
	typedef NAS2D::Signals::Signal2<KeyCode, KeyModifier>	KeyUpEventCallback;

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
	typedef NAS2D::Signals::Signal3<MouseButton, int, int>	MouseButtonEventCallback;

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
	typedef NAS2D::Signals::Signal4<int, int, int, int>	MouseMotionEventCallback;

	/**
	 * \typedef	MouseWheelEventCallback
	 * \brief	Mouse wheel events generate two values: x and y
	 *			motion values.
	 * 
	 * The callback function expects two int parameters.
	 * 
	 * \code
	 * void function(int x, int y);
	 * \endcode
	 * 
	 * \arg	\c x:		Change along the X-Axis.
	 * \arg	\c y:		Change along the Y-Axis.
	 * 
	 * \note	The value given in the axis parameters is the value
	 * 			of a single 'click' of the mouse wheel. This may be
	 * 			more than one (on Windows this value is typical 120,
	 * 			not 1).
	 */
	typedef NAS2D::Signals::Signal2<int, int>	MouseWheelEventCallback;

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
	typedef NAS2D::Signals::Signal0<>					QuitEventCallback;
	
public:
	EventHandler();
	~EventHandler();

	ActivateEventCallback&				activate();

	WindowHiddenEventCallback&			windowHidden();
	WindowExposedEventCallback&			windowExposed();

	WindowMinimizedEventCallback&		windowMinimized();
	WindowMaximizedEventCallback&		windowMaximized();
	WindowRestoredEventCallback	&		windowRestored();

	WindowMouseEnterEventCallback&		windowMouseEnter();
	WindowMouseLeaveEventCallback&		windowMouseLeave();

	JoystickAxisMotionEventCallback&	joystickAxisMotion();
	JoystickBallMotionEventCallback&	joystickBallMotion();
	JoystickButtonEventCallback&		joystickButtonUp();
	JoystickButtonEventCallback&		joystickButtonDown();
	JoystickHatMotionEventCallback&		joystickHatMotion();

	KeyUpEventCallback&					keyUp();
	KeyDownEventCallback&				keyDown();

	MouseButtonEventCallback&			mouseButtonUp();
	MouseButtonEventCallback&			mouseButtonDown();
	MouseButtonEventCallback&			mouseDoubleClick();
	MouseMotionEventCallback&			mouseMotion();
	MouseWheelEventCallback&			mouseWheel();

	QuitEventCallback&					quit();

	void grabMouse();
	void releaseMouse();
	void warpMouse(int x, int y);
	void mouseRelativeMode(bool rel);

	void pump();

	void disconnectAll();

private:
	ActivateEventCallback				mActivateEvent;

	WindowHiddenEventCallback			mWindowHiddenEventCallback;
	WindowExposedEventCallback			mWindowExposedEventCallback;
	WindowMinimizedEventCallback		mWindowMinimizedEventCallback;
	WindowMaximizedEventCallback		mWindowMaximizedEventCallback;
	WindowRestoredEventCallback			mWindowRestoredEventCallback;
	WindowMouseEnterEventCallback		mWindowMouseEnterEventCallback;
	WindowMouseLeaveEventCallback		mWindowMouseLeaveEventCallback;

	JoystickAxisMotionEventCallback		mJoystickAxisMotionEvent;
	JoystickBallMotionEventCallback		mJoystickBallMotionEvent;
	JoystickButtonEventCallback			mJoystickButtonUpEvent;
	JoystickButtonEventCallback			mJoystickButtonDownEvent;
	JoystickHatMotionEventCallback		mJoystickHatMotionEvent;

	KeyUpEventCallback					mKeyUpEvent;
	KeyDownEventCallback				mKeyDownEvent;

	MouseButtonEventCallback			mMouseButtonUpEvent;
	MouseButtonEventCallback			mMouseButtonDownEvent;
	MouseButtonEventCallback			mMouseDoubleClick;
	MouseMotionEventCallback			mMouseMotionEvent;
	MouseWheelEventCallback				mMouseWheelEvent;

	QuitEventCallback					mQuitEvent;
};

void postQuitEvent();

} // namespace
