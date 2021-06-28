// ==================================================================================
// = NAS2D
// = Copyright © 2008 - 2020 New Age Software
// ==================================================================================
// = NAS2D is distributed under the terms of the zlib license. You are free to copy,
// = modify and distribute the software under the terms of the zlib license.
// =
// = Acknowledgment of your use of NAS2D is appreciated but is not required.
// ==================================================================================
// = SIGNAL.H
// ==================================================================================
// = This file is provided under the terms of the MIT license and is included as part
// = of NAS2D's library interface.
// =
// = Modifications include:
// =	- Used variadic templates for arbitrary parameter lists
// =	- Condensed lines for a more compact file
// =	- Replaced for loops using C++11 range-based loops
// =	- Split client methods into SignalSource.h
// =
// = Created by Patrick Hogan on 5/18/09.
// = https://github.com/pbhogan/Signals
// ==================================================================================
// = LICENSE: MIT (http://www.opensource.org/licenses/mit-license.php)
// ==================================================================================
#pragma once

#include "SignalSource.h"


namespace NAS2D {

template<typename ... Params>
class Signal : public SignalSource<Params...>
{
public:
	void emit(Params...params) const { for (auto& delegate : this->delegateList) { delegate(params...); } }
	void operator() (Params...params) const { emit(params...); }
};


} // namespace
