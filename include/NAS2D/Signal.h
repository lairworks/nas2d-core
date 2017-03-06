// ==================================================================================
// = NAS2D
// = Copyright © 2008 - 2017 New Age Software
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
 * \class Signal0
 * \brief Signal with no paramters.
 * 
 * See https://github.com/lairworks/nas2d-core/wiki/Signal-&-Slots for usage documentation.
 */
template<class Param0 = void>
class Signal0
{
public:
	typedef Delegate0<void> _Delegate;

private:
	typedef std::set<_Delegate> DelegateList;
	typedef typename DelegateList::const_iterator DelegateIterator;
	DelegateList delegateList;

public:
	void Connect(_Delegate delegate) { delegateList.insert(delegate); }

	template<class X, class Y>
	void Connect(Y * obj, void (X::*func)()) { delegateList.insert(MakeDelegate(obj, func)); }

	template<class X, class Y>
	void Connect(Y * obj, void (X::*func)() const) { delegateList.insert(MakeDelegate(obj, func)); }

	void Disconnect(_Delegate delegate) { delegateList.erase(delegate); }

	template<class X, class Y>
	void Disconnect(Y * obj, void (X::*func)()) { delegateList.erase(MakeDelegate(obj, func)); }

	template<class X, class Y>
	void Disconnect(Y * obj, void (X::*func)() const) { delegateList.erase(MakeDelegate(obj, func)); }

	void Clear() { delegateList.clear(); }
	void Emit() const { for (auto i : delegateList) i(); }
	void operator() () const { Emit(); }
	bool Empty() const { return delegateList.empty(); }
};


/**
 * \class Signal1
 * \brief Signal with one paramter.
 *
 * See https://github.com/lairworks/nas2d-core/wiki/Signal-&-Slots for usage documentation.
 */
template<class Param1>
class Signal1
{
public:
	typedef Delegate1<Param1> _Delegate;

private:
	typedef std::set<_Delegate> DelegateList;
	typedef typename DelegateList::const_iterator DelegateIterator;
	DelegateList delegateList;

public:
	void Connect(_Delegate delegate) { delegateList.insert(delegate); }

	template<class X, class Y>
	void Connect(Y * obj, void (X::*func)(Param1 p1)) { delegateList.insert(MakeDelegate(obj, func)); }

	template<class X, class Y> void Connect(Y * obj, void (X::*func)(Param1 p1) const) { delegateList.insert(MakeDelegate(obj, func)); }

	void Disconnect(_Delegate delegate) { delegateList.erase(delegate); }

	template<class X, class Y>
	void Disconnect(Y * obj, void (X::*func)(Param1 p1)) { delegateList.erase(MakeDelegate(obj, func)); }

	template<class X, class Y>
	void Disconnect(Y * obj, void (X::*func)(Param1 p1) const) { delegateList.erase(MakeDelegate(obj, func)); }
	
	void Clear() { delegateList.clear(); }
	void Emit(Param1 p1) const { for (auto i : delegateList) i(p1); }
	void operator() (Param1 p1) const { Emit(p1); }
	bool Empty() const { return delegateList.empty(); }
};


/**
 * \class Signal2
 * \brief Signal with two paramters.
 *
 * See https://github.com/lairworks/nas2d-core/wiki/Signal-&-Slots for usage documentation.
 */
template<class Param1, class Param2>
class Signal2
{
public:
	typedef Delegate2<Param1, Param2> _Delegate;

private:
	typedef std::set<_Delegate> DelegateList;
	typedef typename DelegateList::const_iterator DelegateIterator;
	DelegateList delegateList;

public:
	void Connect(_Delegate delegate) { delegateList.insert(delegate); }

	template<class X, class Y>
	void Connect(Y * obj, void (X::*func)(Param1 p1, Param2 p2)) { delegateList.insert(MakeDelegate(obj, func)); }

	template<class X, class Y>
	void Connect(Y * obj, void (X::*func)(Param1 p1, Param2 p2) const) { delegateList.insert(MakeDelegate(obj, func)); }

	void Disconnect(_Delegate delegate) { delegateList.erase(delegate); }

	template<class X, class Y>
	void Disconnect(Y * obj, void (X::*func)(Param1 p1, Param2 p2)) { delegateList.erase(MakeDelegate(obj, func)); }

	template<class X, class Y>
	void Disconnect(Y * obj, void (X::*func)(Param1 p1, Param2 p2) const) { delegateList.erase(MakeDelegate(obj, func)); }

	void Clear() { delegateList.clear(); }
	void operator() (Param1 p1, Param2 p2) const { Emit(p1, p2); }
	void Emit(Param1 p1, Param2 p2) const { for (auto i : delegateList) i(p1, p2); }
	bool Empty() const { return delegateList.empty(); }
};


/**
 * \class Signal3
 * \brief Signal with three paramters.
 *
 * See https://github.com/lairworks/nas2d-core/wiki/Signal-&-Slots for usage documentation.
 */
template<class Param1, class Param2, class Param3>
class Signal3
{
public:
	typedef Delegate3<Param1, Param2, Param3> _Delegate;

private:
	typedef std::set<_Delegate> DelegateList;
	typedef typename DelegateList::const_iterator DelegateIterator;
	DelegateList delegateList;

public:
	void Connect(_Delegate delegate) { delegateList.insert(delegate); }

	template<class X, class Y>
	void Connect(Y * obj, void (X::*func)(Param1 p1, Param2 p2, Param3 p3)) { delegateList.insert(MakeDelegate(obj, func)); }

	template<class X, class Y>
	void Connect(Y * obj, void (X::*func)(Param1 p1, Param2 p2, Param3 p3) const) { delegateList.insert(MakeDelegate(obj, func)); }

	void Disconnect(_Delegate delegate) { delegateList.erase(delegate); }

	template<class X, class Y>
	void Disconnect(Y * obj, void (X::*func)(Param1 p1, Param2 p2, Param3 p3)) { delegateList.erase(MakeDelegate(obj, func)); }

	template<class X, class Y>
	void Disconnect(Y * obj, void (X::*func)(Param1 p1, Param2 p2, Param3 p3) const) { delegateList.erase(MakeDelegate(obj, func)); }

	void Clear() { delegateList.clear(); }
	void Emit(Param1 p1, Param2 p2, Param3 p3) const { for (auto i : delegateList) i(p1, p2, p3); }
	void operator() (Param1 p1, Param2 p2, Param3 p3) const { Emit(p1, p2, p3); }
	bool Empty() const { return delegateList.empty(); }
};


/**
 * \class Signal4
 * \brief Signal with four paramters.
 *
 * See https://github.com/lairworks/nas2d-core/wiki/Signal-&-Slots for usage documentation.
 */
template<class Param1, class Param2, class Param3, class Param4>
class Signal4
{
public:
	typedef Delegate4<Param1, Param2, Param3, Param4> _Delegate;

private:
	typedef std::set<_Delegate> DelegateList;
	typedef typename DelegateList::const_iterator DelegateIterator;
	DelegateList delegateList;

public:
	void Connect(_Delegate delegate) { delegateList.insert(delegate); }

	template<class X, class Y>
	void Connect(Y * obj, void (X::*func)(Param1 p1, Param2 p2, Param3 p3, Param4 p4)) { delegateList.insert(MakeDelegate(obj, func)); }

	template<class X, class Y>
	void Connect(Y * obj, void (X::*func)(Param1 p1, Param2 p2, Param3 p3, Param4 p4) const) { delegateList.insert(MakeDelegate(obj, func)); }

	void Disconnect(_Delegate delegate) { delegateList.erase(delegate); }

	template<class X, class Y>
	void Disconnect(Y * obj, void (X::*func)(Param1 p1, Param2 p2, Param3 p3, Param4 p4)) { delegateList.erase(MakeDelegate(obj, func)); }

	template<class X, class Y>
	void Disconnect(Y * obj, void (X::*func)(Param1 p1, Param2 p2, Param3 p3, Param4 p4) const) { delegateList.erase(MakeDelegate(obj, func)); }

	void Clear() { delegateList.clear(); }
	void Emit(Param1 p1, Param2 p2, Param3 p3, Param4 p4) const { for (auto i : delegateList) i(p1, p2, p3, p4); }
	void operator() (Param1 p1, Param2 p2, Param3 p3, Param4 p4) const { Emit(p1, p2, p3, p4); }
	bool Empty() const { return delegateList.empty(); }
};


/**
 * \class Signal5
 * \brief Signal with five paramters.
 *
 * See https://github.com/lairworks/nas2d-core/wiki/Signal-&-Slots for usage documentation.
 */
template<class Param1, class Param2, class Param3, class Param4, class Param5>
class Signal5
{
public:
	typedef Delegate5<Param1, Param2, Param3, Param4, Param5> _Delegate;

private:
	typedef std::set<_Delegate> DelegateList;
	typedef typename DelegateList::const_iterator DelegateIterator;
	DelegateList delegateList;

public:
	void Connect(_Delegate delegate) { delegateList.insert(delegate); }

	template<class X, class Y>
	void Connect(Y * obj, void (X::*func)(Param1 p1, Param2 p2, Param3 p3, Param4 p4, Param5 p5)) { delegateList.insert(MakeDelegate(obj, func)); }

	template<class X, class Y>
	void Connect(Y * obj, void (X::*func)(Param1 p1, Param2 p2, Param3 p3, Param4 p4, Param5 p5) const) { delegateList.insert(MakeDelegate(obj, func)); }

	void Disconnect(_Delegate delegate) { delegateList.erase(delegate); }

	template<class X, class Y>
	void Disconnect(Y * obj, void (X::*func)(Param1 p1, Param2 p2, Param3 p3, Param4 p4, Param5 p5)) { delegateList.erase(MakeDelegate(obj, func)); }

	template<class X, class Y>
	void Disconnect(Y * obj, void (X::*func)(Param1 p1, Param2 p2, Param3 p3, Param4 p4, Param5 p5) const) { delegateList.erase(MakeDelegate(obj, func)); }

	void Clear() { delegateList.clear(); }
	void Emit(Param1 p1, Param2 p2, Param3 p3, Param4 p4, Param5 p5) const { for (auto i : delegateList) i(p1, p2, p3, p4, p5); }
	void operator() (Param1 p1, Param2 p2, Param3 p3, Param4 p4, Param5 p5) const { Emit(p1, p2, p3, p4, p5); }
	bool Empty() const { return delegateList.empty(); }
};

} // namespace
} // namespace
