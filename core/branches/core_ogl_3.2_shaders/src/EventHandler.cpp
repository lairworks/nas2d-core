// ==================================================================================
// = NAS2D
// = Copyright © 2008 - 2012 New Age Software
// ==================================================================================
// = NAS2D is distributed under the terms of the zlib license. You are free to copy,
// = modify and distribute the software under the terms of the zlib license.
// = 
// = Acknowledgement of your use of NAS2D is appriciated but is not required.
// ==================================================================================

#include "NAS2D/EventHandler.h"

#ifdef __APPLE__
#include <SDL2/SDL.h>
#else
#include "SDL/SDL.h"
#endif

// UGLY ASS HACK for mouse window grabbing
#include "NAS2D/Renderer/OGL_Renderer.h"
#include "NAS2D/Renderer/OGL_Renderer_3_2.h"

#include <iostream>


using namespace std;


const int MAX_MESSAGE_PROCESSING = 10;	/**
										 * Maximum number of events to process each frame.
										 * 
										 * \todo	Make this configurable?
										 */

/**
 * \brief Gets the handler slot associated with this event.
 * 
 * To connect an event handler to this event, call the 'connect()'
 * function as follows:
 * 
 * \code
 * connect(this, &Object::function);
 * \endcode
 * 
 * See EventHandler::ActivateEventCallback for paramter listing.
 */
EventHandler::ActivateEventCallback& EventHandler::activate()
{
	return mActivateEvent;
}


/**
 * \brief Gets the handler slot associated with this event.
 * 
 * To connect an event handler to this event, call the 'connect()'
 * function as follows:
 * 
 * \code
 * connect(this, &Object::function);
 * \endcode
 * 
 * See EventHandler::JoystickAxisMotionEventCallback for paramter listing.
 */
EventHandler::JoystickAxisMotionEventCallback& EventHandler::joystickAxisMotion()
{
	return mJoystickAxisMotionEvent;
}



/**
 * \brief Gets the handler slot associated with this event.
 * 
 * To connect an event handler to this event, call the 'connect()'
 * function as follows:
 * 
 * \code
 * connect(this, &Object::function);
 * \endcode
 * 
 * See EventHandler::JoystickBallMotionEventCallback for paramter listing.
 */
EventHandler::JoystickBallMotionEventCallback& EventHandler::joystickBallMotion()
{
	return mJoystickBallMotionEvent;
}



/**
 * \brief Gets the handler slot associated with this event.
 * 
 * To connect an event handler to this event, call the 'connect()'
 * function as follows:
 * 
 * \code
 * connect(this, &Object::function);
 * \endcode
 * 
 * See EventHandler::JoystickButtonEventCallback for paramter listing.
 */
EventHandler::JoystickButtonEventCallback& EventHandler::joystickButtonUp()
{
	return mJoystickButtonUpEvent;
}



/**
 * \brief Gets the handler slot associated with this event.
 * 
 * To connect an event handler to this event, call the 'connect()'
 * function as follows:
 * 
 * \code
 * connect(this, &Object::function);
 * \endcode
 * 
 * See EventHandler::JoystickButtonEventCallback for paramter listing.
 */
EventHandler::JoystickButtonEventCallback& EventHandler::joystickButtonDown()
{
	return mJoystickButtonDownEvent;
}



/**
 * \brief Gets the handler slot associated with this event.
 * 
 * To connect an event handler to this event, call the 'connect()'
 * function as follows:
 * 
 * \code
 * connect(this, &Object::function);
 * \endcode
 * 
 * See EventHandler::JoystickHatMotionEventCallback for paramter listing.
 */
EventHandler::JoystickHatMotionEventCallback& EventHandler::joystickHatMotion()
{
	return mJoystickHatMotionEvent;
}


/**
 * \brief Gets the handler slot associated with this event.
 * 
 * To connect an event handler to this event, call the 'connect()'
 * function as follows:
 * 
 * \code
 * connect(this, &Object::function);
 * \endcode
 * 
 * See EventHandler::KeyEventCallback for paramter listing.
 */
EventHandler::KeyUpEventCallback& EventHandler::keyUp()
{
	return mKeyUpEvent;
}


/**
 * \brief Gets the handler slot associated with this event.
 * 
 * To connect an event handler to this event, call the 'connect()'
 * function as follows:
 * 
 * \code
 * connect(this, &Object::function);
 * \endcode
 * 
 * See EventHandler::KeyEventCallback for paramter listing.
 */
EventHandler::KeyDownEventCallback& EventHandler::keyDown()
{
	return mKeyDownEvent;
}


/**
 * \brief Gets the handler slot associated with this event.
 * 
 * To connect an event handler to this event, call the 'connect()'
 * function as follows:
 * 
 * \code
 * connect(this, &Object::function);
 * \endcode
 * 
 * See EventHandler::MouseButtonEventCallback for paramter listing.
 */
EventHandler::MouseButtonEventCallback& EventHandler::mouseButtonUp()
{
	return mMouseButtonUpEvent;
}


/**
 * \brief Gets the handler slot associated with this event.
 * 
 * To connect an event handler to this event, call the 'connect()'
 * function as follows:
 * 
 * \code
 * connect(this, &Object::function);
 * \endcode
 * 
 * See EventHandler::MouseButtonEventCallback for paramter listing.
 */
EventHandler::MouseButtonEventCallback& EventHandler::mouseButtonDown()
{
	return mMouseButtonDownEvent;
}


/**
 * \brief Gets the handler slot associated with this event.
 * 
 * To connect an event handler to this event, call the 'connect()'
 * function as follows:
 * 
 * \code
 * connect(this, &Object::function);
 * \endcode
 * 
 * See EventHandler::MouseMotionEventCallback for paramter listing.
 */
EventHandler::MouseMotionEventCallback& EventHandler::mouseMotion()
{
	return mMouseMotionEvent; 
}


/**
 * \brief Gets the handler slot associated with this event.
 * 
 * To connect an event handler to this event, call the 'connect()'
 * function as follows:
 * 
 * \code
 * connect(this, &Object::function);
 * \endcode
 * 
 * See EventHandler::MouseWheelEventCallback for paramter listing.
 */
EventHandler::MouseWheelEventCallback& EventHandler::mouseWheel()
{
	return mMouseWheelEvent; 
}


/**
 * \brief Gets the handler slot associated with this event.
 * 
 * To connect an event handler to this event, call the 'connect()'
 * function as follows:
 * 
 * \code
 * connect(this, &Object::function);
 * \endcode
 * 
 * See EventHandler::QuitEventCallback for paramter listing.
 */
EventHandler::QuitEventCallback& EventHandler::quit()
{
	return mQuitEvent;
}


/**
 * Grabs exclusive mouse input.
 * 
 * \param	update	True if mouse position should be updated while
 *					in exclusive grab, otherwise don't update position.
 *					Default true.
 * 
 * \todo	Decide of the EventHandler is an appropriate place
 *			for this series of functions.
 */
void EventHandler::grabMouse()
{
	if(_window)
		SDL_SetWindowGrab(_window, SDL_TRUE);
}


/**
 * Releases exclusive mouse input.
 * 
 * \todo	Decide of the EventHandler is an appropriate place
 *			for this series of functions.
 */
void EventHandler::releaseMouse()
{
	if(_window)
		SDL_SetWindowGrab(_window, SDL_FALSE);
}


/**
 * Sets the mouse pointer to a specified location within the application window.
 * 
 * \todo	Decide of the EventHandler is an appropriate place
 *			for this series of functions.
 */
void EventHandler::warpMouse(int x, int y)
{
	if(_window)
		SDL_WarpMouseInWindow(_window, x, y);
}


/**
 * Sets mouse updates to relative or absolute mostion.
 * 
 * \param	rel		True for relative mode. False for absolute mode.
 * 
 * \todo	Decide of the EventHandler is an appropriate place
 *			for this series of functions.
 */
void EventHandler::mouseRelativeMode(bool rel)
{
	rel ? SDL_SetRelativeMouseMode(SDL_TRUE) : SDL_SetRelativeMouseMode(SDL_FALSE);
}


/**
 * Updates the event pump and calls any associated callbacks.
 */
void EventHandler::pump()
{
	SDL_Event event;

	int count = 0;
	while((SDL_PollEvent(&event) != 0) && (count < MAX_MESSAGE_PROCESSING))
	{
		switch(event.type)
		{
			case SDL_MOUSEMOTION:
				mMouseMotionEvent(event.motion.x, event.motion.y, event.motion.xrel, event.motion.yrel);
				break;

			case SDL_KEYDOWN:
				mKeyDownEvent(static_cast<KeyCode>(event.key.keysym.sym), static_cast<KeyModifier>(event.key.keysym.mod), event.key.repeat != 0 ? true : false);
				break;

			case SDL_KEYUP:
				mKeyUpEvent(static_cast<KeyCode>(event.key.keysym.sym), static_cast<KeyModifier>(event.key.keysym.mod));
				break;

			case SDL_MOUSEBUTTONDOWN:
				mMouseButtonDownEvent(static_cast<MouseButton>(event.button.button), event.button.x, event.button.y);
				break;

			case SDL_MOUSEBUTTONUP:
				mMouseButtonUpEvent(static_cast<MouseButton>(event.button.button), event.button.x, event.button.y);
				break;

			case SDL_MOUSEWHEEL:
				mMouseWheelEvent(event.wheel.x, event.wheel.y);
				break;

			case SDL_JOYAXISMOTION:
				mJoystickAxisMotionEvent(event.jaxis.which, event.jaxis.axis, event.jaxis.value);
				break;

			case SDL_JOYBALLMOTION:
				mJoystickBallMotionEvent(event.jball.which, event.jball.ball, event.jball.xrel, event.jball.yrel);
				break;

			case SDL_JOYHATMOTION:
				mJoystickHatMotionEvent(event.jhat.which, event.jhat.hat, event.jhat.value);
				break;

			case SDL_JOYBUTTONDOWN:
				mJoystickButtonDownEvent(event.jbutton.which, event.jbutton.button);
				break;

			case SDL_JOYBUTTONUP:
				mJoystickButtonUpEvent(event.jbutton.which, event.jbutton.button);
				break;

			case SDL_WINDOWEVENT:
				/// \todo	This is very incomplete. Update to include the other window events.
				if(event.window.event == SDL_WINDOWEVENT_FOCUS_GAINED)
					mActivateEvent(true);
				else
					mActivateEvent(false);

				break;

			case SDL_QUIT:
				mQuitEvent();
				break;

			default:
				// Ignore any cases not handled.
				break;
		}
		count++;
	}
}


/**
 * Posts a quit event to the event system.
 */
void postQuitEvent()
{
	SDL_Event event;
	event.type = SDL_QUIT;
	SDL_PushEvent(&event);
}