// ==================================================================================
// = NAS2D
// = Copyright © 2008 - 2012 New Age Software
// ==================================================================================
// = NAS2D is distributed under the terms of the zlib license. You are free to copy,
// = modify and distribute the software under the terms of the zlib license.
// = 
// = Acknowledgement of your use of NAS2D is appriciated but is not required.
// ==================================================================================

#include "NAS2D/StateManager.h"

#include <iostream>

using namespace std;

/**
 * C'tor
 */
StateManager::StateManager():	mActiveState(NULL),
								mActive(true)
{
	// Ensure that all quit messages are handled in some way even if a State object doesn't.
	Utility<EventHandler>::get().quit().connect(this, &StateManager::handleQuit);
}


/**
 * \note	The active State is destroyed when the StateManager is destroyed.
 */
StateManager::~StateManager()
{
	if(mActiveState)
		delete mActiveState;
}


/**
 * \brief	Sets the StateManager to run specified State.
 *
 * \note	State objects are destroyed when setting a new state so cross-state
 *			values will need to be saved upon destruction in the overloaded
 *			finalize() function.
 *
 * \param	state	Pointer to a new State.
 * 
 */
void StateManager::setState(State* state)
{
	if(!state)
		return;

	// Destroy the current state
	if(mActiveState != NULL)
		delete mActiveState;
	
	// Initialize the new one
	mActiveState = state;
	mActiveState->initialize();
}

/**
 * Updates the current state and makes any necessary changes to
 * the state manager.
 */
bool StateManager::update()
{
	if(mActiveState)
	{
		State *nextState = mActiveState->update();

		if(!nextState)
			mActive = false;	
		else if(nextState != mActiveState)
			setState(nextState);

		Utility<EventHandler>::get().pump();
	}

	return mActive;
}


void StateManager::handleQuit()
{
	mActive = false;
}