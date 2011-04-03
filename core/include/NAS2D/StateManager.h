// ==================================================================================
// = NAS2D
// = Copyright © 2008 New Age Software
// ==================================================================================
// = NAS2D is distributed under the terms of the zlib license. You are free to copy,
// = modify and distribute the software as long under the terms of the zlib license.
// = 
// = Acknowledgement of your use of NAS2D is appriciated but is not required.
// ==================================================================================

#ifndef _NAS_STATE_MANAGER_
#define _NAS_STATE_MANAGER_

#include "State.h"
#include "NAS2D/Singleton.h"

/**
 * \class	StateManager
 * \brief	Implements a Finite State Machine model that switches between
 *			State objects.
 *
 * \note	StateManager handles distribution of input and system events to
 *			both State objects and to the Gui.
 */
class StateManager: public sigslot::has_slots<>
{
public:
	// Constructor & destructor
	StateManager();
	~StateManager();

	void setState(State* state);	
	bool update();

private:
	void handleQuit(SDL_Event& event);

	State			*mActiveState;
	bool			mActive;
};

#endif