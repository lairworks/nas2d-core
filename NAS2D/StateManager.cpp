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

#include <iostream>

using namespace std;
using namespace NAS2D;

StateManager::StateManager() :
	mActiveState(nullptr),
	mActive(true)
{
	Utility<EventHandler>::get().quit().connect(this, &StateManager::handleQuit);
}


StateManager::~StateManager()
{
	if (mActiveState)
	{
		Utility<EventHandler>::get().disconnectAll();
	}
}

void StateManager::setState(State* state)
{
	if (!state)
	{
		return;
	}

	if (mForceStopAudio) { Utility<Mixer>::get().stopAllAudio(); }

	mActiveState.reset(state);
	mActiveState->initialize();

	mActive = true;
}

bool StateManager::update()
{
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

bool StateManager::active() const
{
	return mActive;
}

void StateManager::forceStopAudio(bool b)
{
	mForceStopAudio = b;
}
