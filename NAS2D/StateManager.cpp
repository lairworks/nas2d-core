// ==================================================================================
// = NAS2D
// = Copyright © 2008 - 2020 New Age Software
// ==================================================================================
// = NAS2D is distributed under the terms of the zlib license. You are free to copy,
// = modify and distribute the software under the terms of the zlib license.
// =
// = Acknowledgment of your use of NAS2D is appreciated but is not required.
// ==================================================================================

#include "StateManager.h"
#include "EventHandler.h"
#include "Utility.h"
#include "State.h"
#include "Mixer/Mixer.h"

using namespace NAS2D;

StateManager::StateManager() :
	mActiveState(nullptr),
	mActive(true)
{
	// Ensure that all quit messages are handled in some way even if a State object doesn't.
	Utility<EventHandler>::get().quit().connect({this, &StateManager::handleQuit});
}


StateManager::~StateManager()
{
	if (mActiveState)
	{
		Utility<EventHandler>::get().disconnectAll();
	}
}


/**
 * Sets the StateManager to run specified State.
 *
 * \param	state	A pointer to a State.
 *
 * \note	Passing a nullptr to this function will terminate the
 * 			application.
 *
 * \warning	The pointer given to the StateManager becomes owned by
 *			the StateManager.
 */
void StateManager::setState(State* state)
{
	if (!state)
	{
		return;
	}

	if (mForceStopAudio) { Utility<Mixer>::get().stopAllAudio(); }

	// Initialize the new one
	mActiveState.reset(state);
	mActiveState->initialize();

	mActive = true;
}


/**
 * Updates the current state and makes any necessary changes to
 * the state manager.
 */
bool StateManager::update()
{
	Utility<EventHandler>::get().pump();

	if (mActiveState)
	{
		State* nextState = mActiveState->update();

		if (!nextState)
		{
			mActive = false;
		}
		else if (nextState != mActiveState.get())
		{
			setState(nextState);
		}
	}
	else
	{
		mActive = false;
	}

	return mActive;
}


/**
 * Called when a quit event is raised.
 *
 * Used to ensure that quit events are responded to
 * if a State doesn't.
 */
void StateManager::handleQuit()
{
	mActive = false;
}


/**
 * Indicates that the StateManger is active.
 */
bool StateManager::active() const
{
	return mActive;
}


/**
 * Sets whether or not the StateManager force stops all audio
 * between state changes. Default is true.
 */
void StateManager::forceStopAudio(bool b)
{
	mForceStopAudio = b;
}
