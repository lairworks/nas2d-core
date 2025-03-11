// ==================================================================================
// = NAS2D
// = Copyright © 2008 - 2020 New Age Software
// ==================================================================================
// = NAS2D is distributed under the terms of the zlib license. You are free to copy,
// = modify and distribute the software under the terms of the zlib license.
// =
// = Acknowledgment of your use of NAS2D is appreciated but is not required.
// ==================================================================================

#pragma once

#include <memory>


namespace NAS2D
{

	class State;


	/**
	 * Implements a Finite State Machine model that switches between
	 *			State objects.
	 *
	 * \note	StateManager handles distribution of input and system events to
	 *			both State objects and to the Gui.
	 */
	class StateManager
	{
	public:
		StateManager();
		~StateManager();

		void setState(State* state);
		bool update();

		bool active() const;

		void forceStopAudio(bool);

	private:
		void handleQuit();

		std::unique_ptr<State> mActiveState;
		bool mActive;
		bool mForceStopAudio = true;
	};

} // namespace
