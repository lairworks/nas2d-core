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

#include "Signal/Forward.h"

#include <memory>
#include <type_traits>
#include <stdexcept>


namespace NAS2D
{
	/**
	 * Provides a simple interface to classes that should only be instantiated
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
	 * Renderer& renderer = Utility<Renderer>::get();
	 *
	 * renderer.drawBox(0, 0, 10, 10, 255, 255, 255, 255);
	 * renderer.drawLine( 10, 10, 400, 200, 255, 0, 0, 255, 4);
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
	template <typename T>
	class Utility
	{
	public:
		Utility() = delete;
		~Utility() = delete;
		Utility(const Utility& s) = delete;
		Utility& operator=(const Utility& s) = delete;

		/**
		 * Gets a reference to a global instance of the specified type
		 * \c T. If an instance doesn't exist, one is created.
		 * Type \c T must be default constructible to use this overload.
		 */
		template <typename Type = T>
		static std::enable_if_t<std::is_default_constructible<Type>::value, T&>
		get()
		{
			if (!mInstance)
			{
				mInstance = std::make_unique<T>();
			}

			return *mInstance;
		}


		/**
		 * Gets a reference to a global instance of the specified type
		 * \c T. If an instance doesn't exist, an exception is thrown.
		 * Type \c T is not default constructible for this overload.
		 */
		template <typename Type = T>
		static std::enable_if_t<!std::is_default_constructible<Type>::value, T&>
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
		 * // Instantiate derived type MixerSDL, of base type Mixer
		 * Utility<Mixer>::init<MixerSDL>();
		 * \endcode
		 *
		 * \code{.cpp}
		 * // Instantiate derived type with constructor arguments, as base type reference
		 * Utility<Base>::init<Derived>(arg1, arg2, arg3);
		 * \endcode
		 *
		 * \param	args	A list of arguments to be forwarded to the \c Type object's constructor
		 *
		 * \return Reference to the newly created object as \c Type.
		 * This allows further method calls to complete initialization or setup
		 * of the object, using the full interface of the derived type.
		 *
		 * \note	This method should be called before <tt>Utility::get()</tt>.
		 */
		template <typename Type = T, typename... Args>
		static Type& init(Args&&... args)
		{
			auto newInstance = std::make_unique<Type>(NAS2D::forward<Args>(args)...);
			// The new instance may be a sub-type of T, so return as sub-type
			auto typedNewInstance = newInstance.release();
			mInstance.reset(typedNewInstance);
			return *typedNewInstance;
		}


		/**
		 * Frees any currently instantiated instances of \c T.
		 *
		 * \warning	Any remaining references to the object \c T that
		 *			the \c Utility manages will be invalidated.
		 */
		static void clear()
		{
			mInstance.reset();
		}

	private:
		static std::unique_ptr<T> mInstance;
	};


	template <typename T>
	std::unique_ptr<T> Utility<T>::mInstance{};
}
