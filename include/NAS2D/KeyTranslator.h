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

#include "NAS2D/Event/KeyMap.h"

namespace NAS2D {

/**
 * \class KeyTranslator
 * \brief Translates key events into a single alpha-numeric character.
 * 
 * The KeyTranslator class is a quick and easy way to translate keyboard
 * events into alphanumeric characters. This is usefull in cases where you
 * need text input without needing large if/then chains or a large switch
 * statement.
 * 
 * \note	KeyTranslator is a static class and therefore should never
 *			be instantiated by itself.
 * 
 * Using the KeyTranslator is simple. Simply call its static methods with
 * the parameters from your KeyDown event handler:
 * 
 * \code{.cpp}
 * void onKeyDown(KeyCode key, KeyModifer mod, bool repeat)
 * {
 *	std::string character = KeyTranslator::translate(key, mod);
 *	std::cout << character << std::endl;
 * }
 *  \endcode
 */
class KeyTranslator
{
public:

	static const char* translate(KeyCode key, KeyModifier mod);

	static bool shift(KeyModifier mod);
	static bool numlock(KeyModifier mod);
	static bool control(KeyModifier mod);

private:
	KeyTranslator(); // explicitly declared private and undefined.

};

} // namespace
