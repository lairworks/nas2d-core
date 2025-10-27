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


namespace NAS2D
{
	/**
	 * The State class operates within a StateManager as a separate unit of
	 *			logic.
	 *
	 * States make it easy to break certain parts of your program into separate and
	 * distinct modes of operation. Using them is pretty straightforward although
	 * there are a few requirements.
	 *
	 * First and foremost, your custom state will derive from State. State has two
	 * pure virtual functions that you need to override:
	 *
	 * \code
	 * void initialize() and
	 * State* update
	 * \endcode
	 *
	 * \section state-init Initializing the State
	 *
	 * After the State is constructed, the \c initialize() function is called. This only
	 * happens once and is usually where event hooks are setup. Also, any GUI layouts
	 * and other initialization that is inappropriate to do in a constructor are done
	 * in this function.
	 *
	 * \section state-update State Updates
	 *
	 * Updates to the State are done once every frame via the \c update() function. Updates
	 * include logic, drawing, event handling, state changes, etc.
	 *
	 * \section state-template Base Template
	 *
	 * The following is a template State object. Use this as a starting point for building
	 * your own States. This is about as much boiler plate code as you'll need to start
	 * a new State object:
	 *
	 * \code
	 * class MyState: public State
	 * {
	 * public:
	 * 	MyState() {}
	 * 	~MyState() {}
	 *
	 * protected:
	 *
	 * 	void initialize() {}
	 * 	State* update() {}
	 *
	 * private:
	 *
	 * 	State* mReturnState;
	 * };
	 * \endcode
	 *
	 * Note that the pointer to a State object as a member of the template class
	 * is not required but recommended as a best practice for handling state switches.
	 *
	 * \note	The State class has twp functions ('initialize()', 'update()') that all
	 *			derived State objects must override.
	 */
	class State
	{
	public:
		State() = default;

		virtual ~State() = default;

	protected:
		friend class StateManager;

		/**
		 * Called immediately after the State is constructed.
		 *
		 * All derived State objects must override this function.
		 *
		 * The initialize() function is called immediately after construction
		 * of the State object. Use it for one-time initialization of any
		 * members, event hooks, etc.
		 */
		virtual void initialize();


		/**
		 * Called once every frame.
		 *
		 * All derived states must override this function.
		 *
		 * As this function is called every frame, this is generally
		 * where you will be doing most of your logic updates, drawing,
		 * etc.
		 *
		 * \return	A pointer to a State object. NULL to terminate the
		 *			NAS2D application.
		 */
		virtual State* update() = 0;
	};
}
