// ==================================================================================
// = NAS2D
// = Copyright © 2008 - 2014 New Age Software
// ==================================================================================
// = NAS2D is distributed under the terms of the zlib license. You are free to copy,
// = modify and distribute the software under the terms of the zlib license.
// = 
// = Acknowledgement of your use of NAS2D is appriciated but is not required.
// ==================================================================================

#ifndef __KEY_TRANSLATOR__
#define __KEY_TRANSLATOR__

#include "NAS2D/Event/KeyMap.h"

namespace NAS2D {

/**
 * \class KeyTranslator
 * \brief Translates key events into a single alpha-numeric character.
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

#endif