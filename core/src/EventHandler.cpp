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

#include "SDL/sdl.h"


#include <iostream>


using namespace std;


const int MAX_MESSAGE_PROCESSING = 10;	/**< Maximum number of events to process each frame. */

/**
 * 
 */
EventHandler::ActivateEventCallback& EventHandler::activate()
{
	return mActivateEvent;
}


/**
 * 
 */
EventHandler::JoystickAxisMotionEventCallback& EventHandler::joystickAxisMotion()
{
	return mJoystickAxisMotionEvent;
}



/**
 * 
 */
EventHandler::JoystickBallMotionEventCallback& EventHandler::joystickBallMotion()
{
	return mJoystickBallMotionEvent;
}



/**
 * 
 */
EventHandler::JoystickButtonEventCallback& EventHandler::joystickButtonUp()
{
	return mJoystickButtonUpEvent;
}



/**
 * 
 */
EventHandler::JoystickButtonEventCallback& EventHandler::joystickButtonDown()
{
	return mJoystickButtonDownEvent;
}



/**
 * 
 */
EventHandler::JoystickHatMotionEventCallback& EventHandler::joystickHatMotion()
{
	return mJoystickHatMotionEvent;
}


/**
 * 
 */
EventHandler::KeyEventCallback& EventHandler::keyUp()
{
	return mKeyUpEvent;
}


/**
 * 
 */
EventHandler::KeyEventCallback& EventHandler::keyDown()
{
	return mKeyDownEvent;
}


/**
 * 
 */
EventHandler::MouseButtonEventCallback& EventHandler::mouseButtonUp()
{
	return mMouseButtonUpEvent;
}


/**
 * 
 */
EventHandler::MouseButtonEventCallback& EventHandler::mouseButtonDown()
{
	return mMouseButtonDownEvent;
}


/**
 * 
 */
EventHandler::MouseMotionEventCallback& EventHandler::mouseMotion()
{
	return mMouseMotionEvent; 
}


/**
 * 
 */
EventHandler::QuitEventCallback& EventHandler::quit()
{
	return mQuitEvent;
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
				mKeyDownEvent(static_cast<KeyCode>(event.key.keysym.sym), static_cast<KeyModifier>(event.key.keysym.mod));
				break;

			case SDL_KEYUP:
				mKeyDownEvent(static_cast<KeyCode>(event.key.keysym.sym), static_cast<KeyModifier>(event.key.keysym.mod));
				break;

			case SDL_MOUSEBUTTONDOWN:
				// Hack to get wheel mouse motions to behave with SDL input. This will produce weird behavior for
				// mice with real buttons for id's 4 and 5.
				if(event.button.button == SDL_BUTTON_WHEELDOWN)
					mMouseButtonDownEvent(BUTTON_WHEELDOWN, event.button.x, event.button.y);
				else if(event.button.button == SDL_BUTTON_WHEELUP)
				{}
				else
					mMouseButtonDownEvent(static_cast<MouseButton>(event.button.button), event.button.x, event.button.y);
				break;

			case SDL_MOUSEBUTTONUP:
				// Hack to get wheel mouse motions to behave with SDL input. This will produce weird behavior for
				// mice with real buttons for id's 4 and 5.
				if(event.button.button == SDL_BUTTON_WHEELDOWN)
				{}
				else if(event.button.button == SDL_BUTTON_WHEELUP)
					mMouseButtonDownEvent(BUTTON_WHEELUP, event.button.x, event.button.y);
				else
					mMouseButtonDownEvent(static_cast<MouseButton>(event.button.button), event.button.x, event.button.y);
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

			case SDL_ACTIVEEVENT:
				mActivateEvent(event.active.gain != 0); // SDL states that gain will be either 1 (true) or 0 (false).
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