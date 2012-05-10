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

#include "SDL/sdl.h"
#include <string>
#include <map>

/**
 * \class	EventHandler
 * \brief	Handles and dispatches low-level events.
 * 
 */
class EventHandler: public sigslot::has_slots<>
{
public:
	typedef sigslot::signal1<SDL_Event&> Callback;

	EventHandler()
	{}

	Callback& event(SDL_EventType type);

	void pump();

private:
	typedef std::map<SDL_EventType, Callback>	CallbackMap;

	CallbackMap		mCallbacks;
};

void postQuitEvent();

#endif