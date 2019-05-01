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

#include <utility>
#include <type_traits>
#include <stdexcept>


namespace NAS2D {

/**
 * \class	Utility
 * \brief	Provides a simple interface to classes that should only be instantiated
 *			once globally for a NAS2D application.
 *
 * \c Utility is a simple templated class designed to provide easy access to 'global'
 * objects that should only ever be instantiated once (for instance the Renderer,
 * Filesystem and Mixer classes).
 *
 * Most often usage cases are a simple matter of asking for a reference to a specific
 * type that is used several times within a section of code. For instance, if you
 * wanted to get a reference to a Renderer:
 *
 * \code{.cpp}
 * // Keep a reference to use multiple times in a code block.
 * Renderer& r Utility<Renderer>::get();
 *
 * r.drawBox(0, 0, 10, 10, 255, 255, 255, 255);
 * r.drawLine( 10, 10, 400, 200, 255, 0, 0, 255, 4);
 * // and so on
 * \endcode
 *
 * If you only need to use the reference for one or two calls, it may make more
 * sense to just use the provided reference:
 *
 * \code{.cpp}
 * // Only need to make a single call.
 * Utility<Mixer>::get().stopAllAudio();
 * \endcode
 *
 * \sa <a href="https://github.com/lairworks/nas2d-core/wiki/Using-Derived-Types-with-Utility">Using Utility<T> with Derived Types</a>
 *
 * \note	\c Utility does not catch exceptions. If any instantiated object
 *			throws the client will be responsible for handling it.
 */
template<typename T>
class Utility
{
public:

	/**
	 * Gets a reference to a global instance of the specified type
	 * \c T. If an instance doesn't exist, one is created.
	 * Type \c T must be default constructible to use this overload.
	 */
	template<typename Type = T>
	static
	std::enable_if_t<std::is_default_constructible<Type>::value, T&>
	get()
	{
		if (!mInstance)
		{
			mInstance = new T();
		}

		return *mInstance;
	}


	/**
	 * Gets a reference to a global instance of the specified type
	 * \c T. If an instance doesn't exist, an exception is thrown.
	 * Type \c T is not default constructible for this overload.
	 */
	template<typename Type = T>
	static
	std::enable_if_t<!std::is_default_constructible<Type>::value, T&>
	get()
	{
		if (!mInstance)
		{
			throw std::runtime_error("Type must be default constructible or initialized with `init`");
		}

		return *mInstance;
	}


	/**
	 * Creates a \c Utility<T> interface for a newly created object of type
	 * \c Type. \c Type must be derived from type \c T specified in the
	 * template class argument list, and defaults to \c T if not explicitly
	 * specified.
	 *
	 * \c Type should be explicitly specified when instantiating a derived type
	 * and polymorphic behavior is desired.
	 *
	 * \code{.cpp}
	 * // Instantiate Mixer (default Type is Mixer)
	 * Utility<Mixer>::init();
	 * \endcode
	 *
	 * \code{.cpp}
	 * // Instantiate derived type Mixer_SDL, of base type Mixer
	 * Utility<Mixer>::init<Mixer_SDL>();
	 * \endcode
	 *
	 * \code{.cpp}
	 * // Instantiate derived type with constructor arguments, as base type reference
	 * Utility<Base>::init<Derived>(arg1, arg2, arg3);
	 * \endcode
	 *
	 * \param	args	A list of arguments to be forwarded to the \c Type objects's constructor
	 *
	 * \return Reference to the newly created object as \c Type.
	 * This allows further method calls to complete initialization or setup
	 * of the object, using the full interface of the derived type.
	 *
	 * \note	This method should be called before <tt>Utility::get()</tt>.
	 */
	template<typename Type = T, typename... Args>
	static Type& init(Args&&... args)
	{
		// Instantiate a new object with forwarded constructor arguments
		auto newInstance = new Type(std::forward<Args>(args)...);

		delete mInstance;

		mInstance = newInstance;
		return *newInstance;
	}


	/**
	 * Creates a \c Utility<T> interface using a derived type \c T* for
	 * derived implementation. This is useful when you need polymorphic
	 * behavior in an object that you need global access to.
	 *
	 * \param	t	A pointer to a new derived object of type \c T.
	 *
	 * \note	This method should be called before <tt>Utility::get()</tt>.
	 *
	 * \warning	This method expects a pointer to a derived type that is
	 *			created with \c new. It then takes ownership of the pointer
	 *			and manages deallocation. Passing the address of a locally
	 *			created object or deallocating the object will yield
	 *			undefined behavior.
	 */
	[[deprecated("Consider using Utility::init instead")]]
	static void instantiateDerived(T* t)
	{
		if (mInstance == t)
			return;

		delete mInstance;

		mInstance = t;
	}


	/**
	 * Frees any currently instantiated instances of \c T.
	 *
	 * \warning	Any remaining references to the object \c T that
	 *			the \c Utility manages will be invalidated.
	 */
	static void clear()
	{
		delete mInstance;
		mInstance = nullptr;
	}

private:
	Utility<T>() {};							// Explicitly declared private.
	~Utility() {};								// Explicitly declared private.
	Utility<T>(const Utility& s);				// Explicitly declared private.
	Utility<T>& operator=(const Utility& s);	// Explicitly declared private.

	static T* mInstance;						/**< Internal instance of type \c T. */
};

template<typename T> T* Utility<T>::mInstance = nullptr;

} // namespace
