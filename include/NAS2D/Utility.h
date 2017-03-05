// ==================================================================================
// = NAS2D
// = Copyright © 2008 - 2017 New Age Software
// ==================================================================================
// = NAS2D is distributed under the terms of the zlib license. You are free to copy,
// = modify and distribute the software under the terms of the zlib license.
// = 
// = Acknowledgement of your use of NAS2D is appriciated but is not required.
// ==================================================================================

#pragma once

namespace NAS2D {

/**
 * \class	Utility
 * \brief	A simple singleton type class used specifically for handling single
 *			instances of utility classes (like configuration, renderer, mixer, etc.)
 *			where only one instance should only ever be created for an application.
 *
 * This is a very basic implementation of the single pattern used for getting references
 * to single instance classes like Renderer, Mixer, Configuration, Filesystem, etc.
 *
 * \warning	This class is \a not thread safe. All utilities must be instantiated
 *			from the main thread of any application that uses them.
 *
 * \note	\c Utility offers only basic exception safety. If an exception occurs
 *			during instantiation of its internal type the exception will not be
 *			caught. In general practice using any of the core utility classes, an
 *			exception being thrown indicates a serious error that must be
 *			address by the client.
 *
 * \warning	\c Utility offers a static member method \c Utility::clean(). This
 *			should \a only \a ever be used when nothing is using the instance of
 *			T (e.g., at the end of a program.)
 */
template<typename T>
class Utility
{
public:
	
	/**
	 * Gets an instance of the specified type T. If an instance
	 * hasn't yet been created, one is created.
	 */
	static T& get()
	{
		if(!mInstance)
			mInstance = new T();

		return *mInstance;
	}


	/**
	 * Sets the internal type T to a type that derives from T.
	 *
	 * \note	This function expects that the parameter T is created in
	 *			the function call (e.g.,
	 *			\c Utility<T>::instantiateDerived(new T()).
	 *
	 * \warning	This function is only intended to be used in very special
	 *			cases where the caller needs to instantiate an object that
	 *			derives from type T.
	 *
	 * \warning	If the caller needs a derived type T, this function must
	 *			be called before \c Utility::get();
	 *
	 * \warning	Utility takes ownership of whatever pointer is passed into
	 *			this function. Deleting it outside of the Utility yields
	 *			undefined behavior.
	 */
	static void instantiateDerived(T* t)
	{
		if(mInstance == t)
			return;

		if(mInstance)
		{
			delete mInstance;
			mInstance = 0;
		}
		
		mInstance = t;
	}


	/**
	 * Frees any currently instantiated instances of T.
	 *
	 * \warning	This should \a only \a ever be called when no
	 *			other instances are being used.
	 */
	static void clean()
	{
		if(mInstance)
		{
			delete mInstance;
			mInstance = nullptr;
		}
	}

private:
	Utility<T>() {};							// Explicitly declared private.
	~Utility() {};								// Explicitly declared private.
	Utility<T>(const Utility& s);				// Explicitly declared private.
	Utility<T>& operator=(const Utility& s);	// Explicitly declared private.

	static T* mInstance;	/**< Internal instance of type T. */
};

template<typename T> T* Utility<T>::mInstance = 0;

} // namespace
