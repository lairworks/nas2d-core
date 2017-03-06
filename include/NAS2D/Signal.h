// ==================================================================================
// = SIGNAL.H
// ==================================================================================
// =  A lightweight signals and slots implementation using fast delegates
// = 
// = Created by Patrick Hogan on 5/18/09.
// = https://github.com/pbhogan/Signals
// ==================================================================================

// ==================================================================================
// = LICENSE
// ==================================================================================
// = MIT http://www.opensource.org/licenses/mit-license.php
// ==================================================================================
#pragma once

#include "Delegate.h"
#include <set>

namespace NAS2D {

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
	void Emit() const { for (DelegateIterator i = delegateList.begin(); i != delegateList.end(); ) (*(i++))(); }
	void operator() () const { Emit(); }
	bool Empty() const { return delegateList.empty(); }
};


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
	void Emit(Param1 p1) const { for (DelegateIterator i = delegateList.begin(); i != delegateList.end(); ) (*(i++))(p1); }
	void operator() (Param1 p1) const { Emit(p1); }
	bool Empty() const { return delegateList.empty(); }
};


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
	void Emit(Param1 p1, Param2 p2) const { for (DelegateIterator i = delegateList.begin(); i != delegateList.end(); ) (*(i++))(p1, p2); }
	bool Empty() const { return delegateList.empty(); }
};


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
	void Emit(Param1 p1, Param2 p2, Param3 p3) const { for (DelegateIterator i = delegateList.begin(); i != delegateList.end(); ) (*(i++))(p1, p2, p3); }
	void operator() (Param1 p1, Param2 p2, Param3 p3) const { Emit(p1, p2, p3); }
	bool Empty() const { return delegateList.empty(); }
};


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
	void Emit(Param1 p1, Param2 p2, Param3 p3, Param4 p4) const { for (DelegateIterator i = delegateList.begin(); i != delegateList.end(); ) (*(i++))(p1, p2, p3, p4); }
	void operator() (Param1 p1, Param2 p2, Param3 p3, Param4 p4) const { Emit(p1, p2, p3, p4); }
	bool Empty() const { return delegateList.empty(); }
};


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
	void Emit(Param1 p1, Param2 p2, Param3 p3, Param4 p4, Param5 p5) const { for (DelegateIterator i = delegateList.begin(); i != delegateList.end(); ) (*(i++))(p1, p2, p3, p4, p5); }
	void operator() (Param1 p1, Param2 p2, Param3 p3, Param4 p4, Param5 p5) const { Emit(p1, p2, p3, p4, p5); }
	bool Empty() const { return delegateList.empty(); }
};

} // namespace
