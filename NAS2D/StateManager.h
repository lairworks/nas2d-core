// ==================================================================================
// = NAS2D
// = Copyright © 2008 - 2019 New Age Software
// ==================================================================================
// = NAS2D is distributed under the terms of the zlib license. You are free to copy,
// = modify and distribute the software under the terms of the zlib license.
// =
// = Acknowledgement of your use of NAS2D is appriciated but is not required.
// ==================================================================================

#pragma once

#include "State.h"
#include "Utility.h"

namespace NAS2D {

/**
 * \class	StateManager
 * \brief	Implements a Finite State Machine model that switches between
 *			State objects.
 *
 * \note	StateManager handles distribution of input and system events to
 *			both State objects and to the Gui.
 */
class StateManager
{
public:
	// Constructor & destructor
	StateManager();
	~StateManager();

	void setState(State* state);
	bool update();

	bool active() const;

	void forceStopAudio(bool);

private:
	void handleQuit();

	State *mActiveState;
	bool mActive;
	bool mForceStopAudio = true;
};

} // namespace
