// ==================================================================================
// = NAS2D
// = Copyright © 2008 - 2019 New Age Software
// ==================================================================================
// = NAS2D is distributed under the terms of the zlib license. You are free to copy,
// = modify and distribute the software under the terms of the zlib license.
// =
// = Acknowledgement of your use of NAS2D is appriciated but is not required.
// ==================================================================================
// = SIGNAL.H
// ==================================================================================
// = This file is provided under the terms of the MIT license and is included as part
// = of NAS2D's library interface.
// =
// = Modifications include:
// =	- Condensed lines for a more compact file
// =	- Removed templates for parameter lists 6 - 8
// =	- Replaced for loops using C++11 range-based loops
// =	- Added doxygen comments for automatic generation of documentation.
// =
// = Created by Patrick Hogan on 5/18/09.
// = https://github.com/pbhogan/Signals
// ==================================================================================
// = LICENSE: MIT (http://www.opensource.org/licenses/mit-license.php)
// ==================================================================================
#pragma once

#include "Delegate.h"
#include <set>

namespace NAS2D {
namespace Signals {

/**
 * \class Signal
 * \brief Signal with preset number of parameters
 *
 * See https://github.com/lairworks/nas2d-core/wiki/Signal-&-Slots for usage documentation.
 */
template<typename ... Params>
class Signal
{
public:
	using _Delegate = DelegateX<void, Params...>;

public:
	void connect(_Delegate delegate) { delegateList.insert(delegate); }

	template<class X, class Y>
	void connect(Y * obj, void (X::*func)(Params...)) { delegateList.insert(MakeDelegate(obj, func)); }

	template<class X, class Y>
	void connect(Y * obj, void (X::*func)(Params...) const) { delegateList.insert(MakeDelegate(obj, func)); }

	void disconnect(_Delegate delegate) { delegateList.erase(delegate); }

	template<class X, class Y>
	void disconnect(Y * obj, void (X::*func)(Params...)) { delegateList.erase(MakeDelegate(obj, func)); }

	template<class X, class Y>
	void disconnect(Y * obj, void (X::*func)(Params...) const) { delegateList.erase(MakeDelegate(obj, func)); }

	void clear() { delegateList.clear(); }
	void emit(Params...params) const { for (DelegateIterator i = delegateList.begin(); i != delegateList.end(); ++i) (*i)(params...); }
	void operator() (Params...params) const { emit(params...); }
	bool empty() const { return delegateList.empty(); }

private:
	using DelegateList = std::set<_Delegate>;
	using DelegateIterator = typename DelegateList::const_iterator;
	DelegateList delegateList;
};


template<class Param0 = void>
using Signal0 = Signal<>;

template<class Param1>
using Signal1 = Signal<Param1>;

template<class Param1, class Param2>
using Signal2 = Signal<Param1, Param2>;

template<class Param1, class Param2, class Param3>
using Signal3 = Signal<Param1, Param2, Param3>;

template<class Param1, class Param2, class Param3, class Param4>
using Signal4 = Signal<Param1, Param2, Param3, Param4>;

template<class Param1, class Param2, class Param3, class Param4, class Param5>
using Signal5 = Signal<Param1, Param2, Param3, Param4, Param5>;


} // namespace
} // namespace
