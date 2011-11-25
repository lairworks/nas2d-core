// ==================================================================================
// = NAS2D
// = Copyright © 2008 - 2011 New Age Software
// ==================================================================================
// = NAS2D is distributed under the terms of the zlib license. You are free to copy,
// = modify and distribute the software under the terms of the zlib license.
// = 
// = Acknowledgement of your use of NAS2D is appriciated but is not required.
// ==================================================================================

#include "NAS2D/EventHandler.h"

#include <iostream>

using namespace std;


const int MAX_MESSAGE_PROCESSING = 10;	/**< Maximum number of events to process each frame. */

EventHandler::Callback& EventHandler::event(SDL_EventType type)
{
	return mCallbacks[type];
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
		mCallbacks[static_cast<SDL_EventType>(event.type)](event);
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