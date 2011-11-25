// ==================================================================================
// = NAS2D
// = Copyright © 2008 - 2011 New Age Software
// ==================================================================================
// = NAS2D is distributed under the terms of the zlib license. You are free to copy,
// = modify and distribute the software under the terms of the zlib license.
// = 
// = Acknowledgement of your use of NAS2D is appriciated but is not required.
// ==================================================================================

#ifndef _NAS_STATE_
#define _NAS_STATE_

#include "SDL/sdl.h"

#include "NAS2D/EventHandler.h"
#include "NAS2D/sigslot.h"

/**
 * \class	State
 * \brief	The State class operates within a StateManager as a separate unit of
 *			logic.
 *
 * \note	The State class has twp functions ('initialize()', 'upate()') that all
 *			derived State objects must override.
 */
class State: public sigslot::has_slots<>
{
public:
	State() {}

	virtual ~State() {}

protected:
	friend class StateManager;

	virtual void initialize() = 0;
	virtual State* update() = 0;
};


#endif