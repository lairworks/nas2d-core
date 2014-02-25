// ==================================================================================
// = NAS2D
// = Copyright © 2008 - 2014 New Age Software
// ==================================================================================
// = NAS2D is distributed under the terms of the zlib license. You are free to copy,
// = modify and distribute the software under the terms of the zlib license.
// = 
// = Acknowledgement of your use of NAS2D is appriciated but is not required.
// ==================================================================================

#include "NAS2D/StateManager.h"
#include "NAS2D/Mixer/Mixer.h"

#include <iostream>

using namespace std;
using namespace NAS2D;

/**
 * C'tor
 */
StateManager::StateManager():	mActiveState(NULL),
								mActive(true)
{
	// Ensure that all quit messages are handled in some way even if a State object doesn't.
	Utility<EventHandler>::get().quit().Connect(this, &StateManager::handleQuit);
}


/**
 * \note	The active State is destroyed when the StateManager is destroyed.
 */
StateManager::~StateManager()
{
	if(mActiveState)
	{
		Utility<EventHandler>::get().disconnectAll();
		delete mActiveState;
	}
}


/**
 * \brief	Sets the StateManager to run specified State.
 *
 * \param	state	A pointer to a State.
 * 
 * \note	Passing a NULL pointer to this function will effectively
 *			terminate the application.
 * 
 * \warning	The pointer given to the StateManager becomes owned by
 *			the StateManager.
 */
void StateManager::setState(State* state)
{
	if(!state)
		return;

	Utility<Mixer>::get().stopAllAudio();

	if(mActiveState != NULL)
	{
		Utility<EventHandler>::get().disconnectAll();
		delete mActiveState;
	}

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
		State* nextState = mActiveState->update();

		if(!nextState)
		{
			mActive = false;	
		}
		else if(nextState != mActiveState)
		{
			setState(nextState);
		}

		Utility<EventHandler>::get().pump();
	}
	else
	{
		mActive = false;
	}

	return mActive;
}


void StateManager::handleQuit()
{
	mActive = false;
}