// ==================================================================================
// = NAS2D
// = Copyright © 2008 - 2018 New Age Software
// ==================================================================================
// = NAS2D is distributed under the terms of the zlib license. You are free to copy,
// = modify and distribute the software under the terms of the zlib license.
// =
// = Acknowledgement of your use of NAS2D is appriciated but is not required.
// ==================================================================================
// = DELEGATE.H
// ==================================================================================
// = Efficient delegates in C++ that generate only two lines of asm code
// =
// = Modifications include:
// =	- Removed a lot of instructional comments unnecessary for NAS2D.
// =	- Removed support for MSVC compilers < 7 (far too old to still be using)
// =	- Condensed lines for a more compact file
// =
// = Created by Don Clugston. Contributions by Jody Hagins and Patrick Hogan.
// = http://www.codeproject.com/KB/cpp/FastDelegate.aspx
// ==================================================================================
// = LICENSE: PUBLIC DOMAIN
// ==================================================================================

#pragma once

#include <memory>
#include <cstring>

#define FASTDELEGATE_USESTATICFUNCTIONHACK

// Compiler identification. It's not easy to identify Visual C++ because many vendors
// fraudulently define Microsoft's identifiers.
#if defined(_MSC_VER) && !defined(__MWERKS__) && !defined(__VECTOR_C) && !defined(__ICL) && !defined(__BORLANDC__)
#define FASTDLGT_ISMSVC
#define FASTDLGT_MICROSOFT_MFP
#define FASTDLGT_HASINHERITANCE_KEYWORDS
#endif

// Does it allow function declarator syntax? The following compilers are known to work:
#if defined(FASTDLGT_ISMSVC) && (_MSC_VER >= 1310) // VC 7.1
#define FASTDELEGATE_ALLOW_FUNCTION_TYPE_SYNTAX
#endif

// Gcc(2.95+), and versions of Digital Mars, Intel and Comeau in common use.
#if defined (__DMC__) || defined(__GNUC__) || defined(__ICL) || defined(__COMO__)
#define FASTDELEGATE_ALLOW_FUNCTION_TYPE_SYNTAX
#endif

#ifdef __GNUC__ // Workaround GCC bug #8271
#define FASTDELEGATE_GCC_BUG_8271
#endif

namespace NAS2D {

namespace detail {

template <class OutputClass, class InputClass>
union horrible_union { OutputClass out; InputClass in; };

template <class OutputClass, class InputClass>
inline OutputClass horrible_cast(const InputClass input)
{
	horrible_union<OutputClass, InputClass> u;
	static_assert(sizeof(InputClass) != sizeof(u) || sizeof(InputClass) != sizeof(OutputClass), "Can't use horrible cast");
	u.in = input;
	return u.out;
}


// ==================================================================================
// = WORKAROUNDS
// ==================================================================================
typedef void DefaultVoid;

// Translate from 'DefaultVoid' to 'void'.
template <class T>
struct DefaultVoidToVoid { typedef T type; };

template <>
struct DefaultVoidToVoid<DefaultVoid> { typedef void type; };

// Translate from 'void' into 'DefaultVoid'
template <class T>
struct VoidToDefaultVoid { typedef T type; };

template <>
struct VoidToDefaultVoid<void> { typedef DefaultVoid type; };

// GenericClass is a fake class, ONLY used to provide a type. It is vitally important
// that it is never defined.
#ifdef	FASTDLGT_MICROSOFT_MFP

#ifdef FASTDLGT_HASINHERITANCE_KEYWORDS
	class __single_inheritance GenericClass;
#endif
	class GenericClass {};
#else
	class GenericClass;
#endif

const int SINGLE_MEMFUNCPTR_SIZE = sizeof(void (GenericClass::*)());

template <int N>
struct SimplifyMemFunc
{
	template <class X, class XFuncType, class GenericMemFuncType>
	inline static GenericClass *Convert(X *pthis, XFuncType function_to_bind, GenericMemFuncType &bound_func)
	{
		static_assert(N < 100, "Unsupported member function pointer on this compiler");
		return 0;
	}
};

template <>
struct SimplifyMemFunc<SINGLE_MEMFUNCPTR_SIZE>
{
	template <class X, class XFuncType, class GenericMemFuncType>
	inline static GenericClass *Convert(X *pthis, XFuncType function_to_bind, GenericMemFuncType &bound_func)
	{
		#if defined __DMC__
		bound_func = horrible_cast<GenericMemFuncType>(function_to_bind);
		#else
		bound_func = reinterpret_cast<GenericMemFuncType>(function_to_bind);
		#endif
		return reinterpret_cast<GenericClass *>(pthis);
	}
};

#ifdef FASTDLGT_MICROSOFT_MFP

template<>
struct SimplifyMemFunc< SINGLE_MEMFUNCPTR_SIZE + sizeof(int) >
{
	template <class X, class XFuncType, class GenericMemFuncType>
	inline static GenericClass *Convert(X *pthis, XFuncType function_to_bind, GenericMemFuncType &bound_func)
	{
		union
		{
			XFuncType func;
			struct { GenericMemFuncType funcaddress; int delta; } s;
		} u;

		typedef int ERROR_CantUsehorrible_cast[sizeof(function_to_bind) == sizeof(u.s) ? 1 : -1];
		u.func = function_to_bind;
		bound_func = u.s.funcaddress;
		return reinterpret_cast<GenericClass *>(reinterpret_cast<char *>(pthis) + u.s.delta);
	}
};


struct MicrosoftVirtualMFP
{
	void (GenericClass::*codeptr)();
	int delta;
	int vtable_index;
};


struct GenericVirtualClass : virtual public GenericClass
{
	typedef GenericVirtualClass * (GenericVirtualClass::*ProbePtrType)();
	GenericVirtualClass * GetThis() { return this; }
};


template <>
struct SimplifyMemFunc<SINGLE_MEMFUNCPTR_SIZE + 2*sizeof(int) >
{
	template <class X, class XFuncType, class GenericMemFuncType>
	inline static GenericClass *Convert(X *pthis, XFuncType function_to_bind, GenericMemFuncType &bound_func)
	{
		union { XFuncType func; GenericClass* (X::*ProbeFunc)(); MicrosoftVirtualMFP s; } u;
		u.func = function_to_bind;
		bound_func = reinterpret_cast<GenericMemFuncType>(u.s.codeptr);
		union { GenericVirtualClass::ProbePtrType virtfunc; MicrosoftVirtualMFP s; } u2;

		typedef int ERROR_CantUsehorrible_cast[sizeof(function_to_bind) == sizeof(u.s) && sizeof(function_to_bind) == sizeof(u.ProbeFunc) && sizeof(u2.virtfunc) == sizeof(u2.s) ? 1 : -1];

		u2.virtfunc = &GenericVirtualClass::GetThis;
		u.s.codeptr = u2.s.codeptr;
		return (pthis->*u.ProbeFunc)();
	}
};


#if (_MSC_VER <1300)
#error Support for this compiler is no longer provided. Please upgrade.
#else
template <>
struct SimplifyMemFunc<SINGLE_MEMFUNCPTR_SIZE + 3 * sizeof(int)>
{
	template <class X, class XFuncType, class GenericMemFuncType>
	inline static GenericClass *Convert(X *pthis, XFuncType function_to_bind, GenericMemFuncType &bound_func)
	{
		union
		{
			XFuncType func;
			struct
			{
				GenericMemFuncType m_funcaddress;
				int delta;
				int vtordisp;
				int vtable_index;
			} s;
		} u;

		static_assert(sizeof(XFuncType) != sizeof(u.s), "Can't use horrible cast");
		u.func = function_to_bind;
		bound_func = u.s.funcaddress;
		int virtual_delta = 0;
		if (u.s.vtable_index)
		{
			const int * vtable = *reinterpret_cast<const int *const*>(reinterpret_cast<const char *>(pthis) + u.s.vtordisp );
			virtual_delta = u.s.vtordisp + *reinterpret_cast<const int *>(reinterpret_cast<const char *>(vtable) + u.s.vtable_index);
		}
		return reinterpret_cast<GenericClass *>(reinterpret_cast<char *>(pthis) + u.s.delta + virtual_delta);
	};
};
#endif
#endif
}


class DelegateMemento
{
protected:
	typedef void (detail::GenericClass::*GenericMemFuncType)();
	detail::GenericClass *m_pthis;
	GenericMemFuncType m_pFunction;

#if !defined(FASTDELEGATE_USESTATICFUNCTIONHACK)
	typedef void(*GenericFuncPtr)();
	GenericFuncPtr m_pStaticFunction;
#endif

public:
#if !defined(FASTDELEGATE_USESTATICFUNCTIONHACK)
	DelegateMemento() : m_pthis(0), m_pFunction(0), m_pStaticFunction(0) {};
	void clear() { m_pthis = 0; m_pFunction = 0; m_pStaticFunction = 0; }
#else
	DelegateMemento() : m_pthis(0), m_pFunction(0) {};
	void clear() { m_pthis = 0; m_pFunction = 0; }
#endif
public:
#if !defined(FASTDELEGATE_USESTATICFUNCTIONHACK)
	inline bool IsEqual(const DelegateMemento &x) const
	{
		if (m_pFunction != x.m_pFunction) return false;
		if (m_pStaticFunction != x.m_pStaticFunction) return false;
		if (m_pStaticFunction != 0) return m_pthis == x.m_pthis;
		else return true;
	}
#else
	inline bool IsEqual(const DelegateMemento &x) const { return m_pthis == x.m_pthis && m_pFunction == x.m_pFunction; }
#endif
	inline bool IsLess(const DelegateMemento &right) const
	{
		#if !defined(FASTDELEGATE_USESTATICFUNCTIONHACK)
		if (m_pStaticFunction != 0 || right.m_pStaticFunction != 0) return m_pStaticFunction < right.m_pStaticFunction;
		#endif
		if (m_pthis != right.m_pthis) return m_pthis < right.m_pthis;

		return memcmp(&m_pFunction, &right.m_pFunction, sizeof(m_pFunction)) < 0;
	}

	inline bool operator ! () const { return m_pthis == 0 && m_pFunction == 0; }
	inline bool empty() const { return m_pthis == 0 && m_pFunction == 0; }

public:
	DelegateMemento & operator = (const DelegateMemento &right) { SetMementoFrom(right); return *this; }
	inline bool operator <(const DelegateMemento &right) { return IsLess(right); }
	inline bool operator >(const DelegateMemento &right) { return right.IsLess(*this); }
	DelegateMemento(const DelegateMemento &right) : m_pthis(right.m_pthis), m_pFunction(right.m_pFunction)
		#if !defined(FASTDELEGATE_USESTATICFUNCTIONHACK)
		, m_pStaticFunction(right.m_pStaticFunction)
		#endif
	{}

protected:
	void SetMementoFrom(const DelegateMemento &right)
	{
		m_pFunction = right.m_pFunction;
		m_pthis = right.m_pthis;
		#if !defined(FASTDELEGATE_USESTATICFUNCTIONHACK)
		m_pStaticFunction = right.m_pStaticFunction;
		#endif
	}
};


namespace detail
{
template <class GenericMemFunc, class StaticFuncPtr, class UnvoidStaticFuncPtr>
class ClosurePtr : public DelegateMemento
{
public:
	template <class X, class XMemFunc>
	inline void bindmemfunc(X *pthis, XMemFunc function_to_bind)
	{
		m_pthis = SimplifyMemFunc< sizeof(function_to_bind) > ::Convert(pthis, function_to_bind, m_pFunction);
		#if !defined(FASTDELEGATE_USESTATICFUNCTIONHACK)
		m_pStaticFunction = 0;
		#endif
	}

	template <class X, class XMemFunc>
	inline void bindconstmemfunc(const X *pthis, XMemFunc function_to_bind)
	{
		m_pthis = SimplifyMemFunc< sizeof(function_to_bind) > ::Convert(const_cast<X*>(pthis), function_to_bind, m_pFunction);
		#if !defined(FASTDELEGATE_USESTATICFUNCTIONHACK)
		m_pStaticFunction = 0;
		#endif
	}

#ifdef FASTDELEGATE_GCC_BUG_8271	// At present, GCC doesn't recognize constness of MFPs in templates
	template <class X, class XMemFunc>
	inline void bindmemfunc(const X *pthis, XMemFunc function_to_bind)
	{
		bindconstmemfunc(pthis, function_to_bind);
		#if !defined(FASTDELEGATE_USESTATICFUNCTIONHACK)
		m_pStaticFunction = 0;
		#endif
	}
#endif

	inline GenericClass *GetClosureThis() const { return m_pthis; }
	inline GenericMemFunc GetClosureMemPtr() const { return reinterpret_cast<GenericMemFunc>(m_pFunction); }

#if !defined(FASTDELEGATE_USESTATICFUNCTIONHACK)

public:
	template< lass DerivedClass>
	inline void CopyFrom(DerivedClass *pParent, const DelegateMemento &x)
	{
		SetMementoFrom(x);
		if (m_pStaticFunction != 0) m_pthis = reinterpret_cast<GenericClass *>(pParent);
	}

	template <class DerivedClass, class ParentInvokerSig>
	inline void bindstaticfunc(DerivedClass *pParent, ParentInvokerSig static_function_invoker, StaticFuncPtr function_to_bind)
	{
		if (function_to_bind == 0) m_pFunction = 0;
		else bindmemfunc(pParent, static_function_invoker);
		m_pStaticFunction = reinterpret_cast<GenericFuncPtr>(function_to_bind);
	}
	inline UnvoidStaticFuncPtr GetStaticFunction() const { return reinterpret_cast<UnvoidStaticFuncPtr>(m_pStaticFunction); }
#else

	template<class DerivedClass>
    inline void CopyFrom(DerivedClass *pParent, const DelegateMemento &right) { (void)pParent; SetMementoFrom(right); }

	template <class DerivedClass, class ParentInvokerSig>
	inline void bindstaticfunc(DerivedClass *pParent, ParentInvokerSig static_function_invoker, StaticFuncPtr function_to_bind)
	{
		if (function_to_bind == 0) m_pFunction = 0;
		else bindmemfunc(pParent, static_function_invoker);
		static_assert(sizeof(GenericClass *) != sizeof(function_to_bind), "Can't use evil method");
		m_pthis = horrible_cast<GenericClass *>(function_to_bind);
	}

	inline UnvoidStaticFuncPtr GetStaticFunction() const
	{
		static_assert(sizeof(UnvoidStaticFuncPtr) != sizeof(this), "Can't use evil method");
		return horrible_cast<UnvoidStaticFuncPtr>(this);
	}
#endif

	inline bool IsEqualToStaticFuncPtr(StaticFuncPtr funcptr)
	{
		if (funcptr == 0) return empty();
		else return funcptr == reinterpret_cast<StaticFuncPtr>(GetStaticFunction());
	}
};

}


template<class RetType = detail::DefaultVoid>
class Delegate0
{
private:
	typedef typename detail::DefaultVoidToVoid<RetType>::type DesiredRetType;
	typedef DesiredRetType(*StaticFunctionPtr)();
	typedef RetType(*UnvoidStaticFunctionPtr)();
	typedef RetType(detail::GenericClass::*GenericMemFn)();
	typedef detail::ClosurePtr<GenericMemFn, StaticFunctionPtr, UnvoidStaticFunctionPtr> ClosureType;
	ClosureType m_Closure;

public:
	typedef Delegate0 type;

	Delegate0() { clear(); }
	Delegate0(const Delegate0 &x) { m_Closure.CopyFrom(this, x.m_Closure); }
	void operator = (const Delegate0 &x) { m_Closure.CopyFrom(this, x.m_Closure); }
	bool operator ==(const Delegate0 &x) const { return m_Closure.IsEqual(x.m_Closure); }
	bool operator !=(const Delegate0 &x) const { return !m_Closure.IsEqual(x.m_Closure); }
	bool operator <(const Delegate0 &x) const { return m_Closure.IsLess(x.m_Closure); }
	bool operator >(const Delegate0 &x) const { return x.m_Closure.IsLess(m_Closure); }

	template < class X, class Y >
	Delegate0(Y *pthis, DesiredRetType(X::* function_to_bind)()) { m_Closure.bindmemfunc(static_cast<X*>(pthis), function_to_bind); }
	template < class X, class Y >
	inline void Bind(Y *pthis, DesiredRetType(X::* function_to_bind)()) { m_Closure.bindmemfunc(static_cast<X*>(pthis), function_to_bind); }

	template < class X, class Y >
	Delegate0(const Y *pthis, DesiredRetType(X::* function_to_bind)() const) { m_Closure.bindconstmemfunc(static_cast<const X*>(pthis), function_to_bind); }
	template < class X, class Y >
	inline void Bind(const Y *pthis, DesiredRetType(X::* function_to_bind)() const) { m_Closure.bindconstmemfunc(static_cast<const X *>(pthis), function_to_bind); }

	Delegate0(DesiredRetType(*function_to_bind)()) { Bind(function_to_bind); }
	void operator = (DesiredRetType(*function_to_bind)()) { Bind(function_to_bind); }
	inline void Bind(DesiredRetType(*function_to_bind)()) { m_Closure.bindstaticfunc(this, &Delegate0::InvokeStaticFunction, function_to_bind); }
	RetType operator() () const { return (m_Closure.GetClosureThis()->*(m_Closure.GetClosureMemPtr()))(); }

private:
	typedef struct SafeBoolStruct { int a_data_pointer_to_this_is_0_on_buggy_compilers; StaticFunctionPtr m_nonzero; } UselessTypedef;
	typedef StaticFunctionPtr SafeBoolStruct::*unspecified_bool_type;

public:
	operator unspecified_bool_type() const { return empty() ? 0 : &SafeBoolStruct::m_nonzero; }

	inline bool operator==(StaticFunctionPtr funcptr) { return m_Closure.IsEqualToStaticFuncPtr(funcptr); }
	inline bool operator!=(StaticFunctionPtr funcptr) { return !m_Closure.IsEqualToStaticFuncPtr(funcptr); }
	inline bool operator ! () const { return !m_Closure; }
	inline bool empty() const { return !m_Closure; }
	void clear() { m_Closure.clear(); }

	const DelegateMemento & GetMemento() { return m_Closure; }
	void SetMemento(const DelegateMemento &any) { m_Closure.CopyFrom(this, any); }

private:
	RetType InvokeStaticFunction() const { return (*(m_Closure.GetStaticFunction()))(); }
};


template<class Param1, class RetType = detail::DefaultVoid>
class Delegate1
{
private:
	typedef typename detail::DefaultVoidToVoid<RetType>::type DesiredRetType;
	typedef DesiredRetType(*StaticFunctionPtr)(Param1 p1);
	typedef RetType(*UnvoidStaticFunctionPtr)(Param1 p1);
	typedef RetType(detail::GenericClass::*GenericMemFn)(Param1 p1);
	typedef detail::ClosurePtr<GenericMemFn, StaticFunctionPtr, UnvoidStaticFunctionPtr> ClosureType;
	ClosureType m_Closure;

public:
	typedef Delegate1 type;

	Delegate1() { clear(); }
	Delegate1(const Delegate1 &x) { m_Closure.CopyFrom(this, x.m_Closure); }
	void operator = (const Delegate1 &x) { m_Closure.CopyFrom(this, x.m_Closure); }
	bool operator ==(const Delegate1 &x) const { return m_Closure.IsEqual(x.m_Closure); }
	bool operator !=(const Delegate1 &x) const { return !m_Closure.IsEqual(x.m_Closure); }
	bool operator <(const Delegate1 &x) const { return m_Closure.IsLess(x.m_Closure); }
	bool operator >(const Delegate1 &x) const { return x.m_Closure.IsLess(m_Closure); }

	template <class X, class Y>
	Delegate1(Y *pthis, DesiredRetType(X::* function_to_bind)(Param1 p1)) { m_Closure.bindmemfunc(static_cast<X*>(pthis), function_to_bind); }
	template <class X, class Y>
	inline void Bind(Y *pthis, DesiredRetType(X::* function_to_bind)(Param1 p1)) { m_Closure.bindmemfunc(static_cast<X*>(pthis), function_to_bind); }

	template <class X, class Y>
	Delegate1(const Y *pthis, DesiredRetType(X::* function_to_bind)(Param1 p1) const) { m_Closure.bindconstmemfunc(static_cast<const X*>(pthis), function_to_bind); }
	template < class X, class Y >
	inline void Bind(const Y *pthis, DesiredRetType(X::* function_to_bind)(Param1 p1) const) { m_Closure.bindconstmemfunc(static_cast<const X *>(pthis), function_to_bind); }

	Delegate1(DesiredRetType(*function_to_bind)(Param1 p1)) { Bind(function_to_bind); }
	void operator = (DesiredRetType(*function_to_bind)(Param1 p1)) { Bind(function_to_bind); }
	inline void Bind(DesiredRetType(*function_to_bind)(Param1 p1)) { m_Closure.bindstaticfunc(this, &Delegate1::InvokeStaticFunction, function_to_bind); }
	RetType operator() (Param1 p1) const { return (m_Closure.GetClosureThis()->*(m_Closure.GetClosureMemPtr()))(p1); }

private:
	typedef struct SafeBoolStruct { int a_data_pointer_to_this_is_0_on_buggy_compilers; StaticFunctionPtr m_nonzero; } UselessTypedef;
	typedef StaticFunctionPtr SafeBoolStruct::*unspecified_bool_type;

public:
	operator unspecified_bool_type() const { return empty() ? 0 : &SafeBoolStruct::m_nonzero; }

	inline bool operator==(StaticFunctionPtr funcptr) { return m_Closure.IsEqualToStaticFuncPtr(funcptr); }
	inline bool operator!=(StaticFunctionPtr funcptr) { return !m_Closure.IsEqualToStaticFuncPtr(funcptr); }
	inline bool operator ! () const { return !m_Closure; }
	inline bool empty() const {return !m_Closure; }
	void clear() { m_Closure.clear(); }

	const DelegateMemento & GetMemento() { return m_Closure; }
	void SetMemento(const DelegateMemento &any) { m_Closure.CopyFrom(this, any); }

private:
	RetType InvokeStaticFunction(Param1 p1) const { return (*(m_Closure.GetStaticFunction()))(p1); }
};


template<class Param1, class Param2, class RetType = detail::DefaultVoid>
class Delegate2
{
private:
	typedef typename detail::DefaultVoidToVoid<RetType>::type DesiredRetType;
	typedef DesiredRetType(*StaticFunctionPtr)(Param1 p1, Param2 p2);
	typedef RetType(*UnvoidStaticFunctionPtr)(Param1 p1, Param2 p2);
	typedef RetType(detail::GenericClass::*GenericMemFn)(Param1 p1, Param2 p2);
	typedef detail::ClosurePtr<GenericMemFn, StaticFunctionPtr, UnvoidStaticFunctionPtr> ClosureType;
	ClosureType m_Closure;

public:
	typedef Delegate2 type;

	Delegate2() { clear(); }
	Delegate2(const Delegate2 &x) { m_Closure.CopyFrom(this, x.m_Closure); }
	void operator = (const Delegate2 &x) { m_Closure.CopyFrom(this, x.m_Closure); }
	bool operator ==(const Delegate2 &x) const { return m_Closure.IsEqual(x.m_Closure); }
	bool operator !=(const Delegate2 &x) const { return !m_Closure.IsEqual(x.m_Closure); }
	bool operator <(const Delegate2 &x) const { return m_Closure.IsLess(x.m_Closure); }
	bool operator >(const Delegate2 &x) const { return x.m_Closure.IsLess(m_Closure); }

	template <class X, class Y>
	Delegate2(Y *pthis, DesiredRetType(X::* function_to_bind)(Param1 p1, Param2 p2)) { m_Closure.bindmemfunc(static_cast<X*>(pthis), function_to_bind); }
	template <class X, class Y>
	inline void Bind(Y *pthis, DesiredRetType(X::* function_to_bind)(Param1 p1, Param2 p2)) { m_Closure.bindmemfunc(static_cast<X*>(pthis), function_to_bind); }

	template <class X, class Y>
	Delegate2(const Y *pthis, DesiredRetType(X::* function_to_bind)(Param1 p1, Param2 p2) const) { m_Closure.bindconstmemfunc(static_cast<const X*>(pthis), function_to_bind); }
	template <class X, class Y>
	inline void Bind(const Y *pthis, DesiredRetType(X::* function_to_bind)(Param1 p1, Param2 p2) const) { m_Closure.bindconstmemfunc(static_cast<const X *>(pthis), function_to_bind); }

	Delegate2(DesiredRetType(*function_to_bind)(Param1 p1, Param2 p2)) { Bind(function_to_bind); }
	void operator = (DesiredRetType(*function_to_bind)(Param1 p1, Param2 p2)) { Bind(function_to_bind); }
	inline void Bind(DesiredRetType(*function_to_bind)(Param1 p1, Param2 p2)) { m_Closure.bindstaticfunc(this, &Delegate2::InvokeStaticFunction, function_to_bind); }
	RetType operator() (Param1 p1, Param2 p2) const { return (m_Closure.GetClosureThis()->*(m_Closure.GetClosureMemPtr()))(p1, p2); }

private:
	typedef struct SafeBoolStruct { int a_data_pointer_to_this_is_0_on_buggy_compilers; StaticFunctionPtr m_nonzero; } UselessTypedef;
	typedef StaticFunctionPtr SafeBoolStruct::*unspecified_bool_type;

public:
	operator unspecified_bool_type() const { return empty() ? 0 : &SafeBoolStruct::m_nonzero; }

	inline bool operator==(StaticFunctionPtr funcptr) { return m_Closure.IsEqualToStaticFuncPtr(funcptr); }
	inline bool operator!=(StaticFunctionPtr funcptr) { return !m_Closure.IsEqualToStaticFuncPtr(funcptr); }
	inline bool operator ! () const { return !m_Closure; }
	inline bool empty() const { return !m_Closure; }
	void clear() { m_Closure.clear(); }
	const DelegateMemento & GetMemento() { return m_Closure; }
	void SetMemento(const DelegateMemento &any) { m_Closure.CopyFrom(this, any); }

private:
	RetType InvokeStaticFunction(Param1 p1, Param2 p2) const { return (*(m_Closure.GetStaticFunction()))(p1, p2); }
};


template<class Param1, class Param2, class Param3, class RetType = detail::DefaultVoid>
class Delegate3
{
private:
	typedef typename detail::DefaultVoidToVoid<RetType>::type DesiredRetType;
	typedef DesiredRetType(*StaticFunctionPtr)(Param1 p1, Param2 p2, Param3 p3);
	typedef RetType(*UnvoidStaticFunctionPtr)(Param1 p1, Param2 p2, Param3 p3);
	typedef RetType(detail::GenericClass::*GenericMemFn)(Param1 p1, Param2 p2, Param3 p3);
	typedef detail::ClosurePtr<GenericMemFn, StaticFunctionPtr, UnvoidStaticFunctionPtr> ClosureType;
	ClosureType m_Closure;

public:
	typedef Delegate3 type;

	Delegate3() { clear(); }
	Delegate3(const Delegate3 &x) { m_Closure.CopyFrom(this, x.m_Closure); }
	void operator = (const Delegate3 &x) { m_Closure.CopyFrom(this, x.m_Closure); }
	bool operator ==(const Delegate3 &x) const { return m_Closure.IsEqual(x.m_Closure); }
	bool operator !=(const Delegate3 &x) const { return !m_Closure.IsEqual(x.m_Closure); }
	bool operator <(const Delegate3 &x) const { return m_Closure.IsLess(x.m_Closure); }
	bool operator >(const Delegate3 &x) const { return x.m_Closure.IsLess(m_Closure); }

	template <class X, class Y>
	Delegate3(Y *pthis, DesiredRetType(X::* function_to_bind)(Param1 p1, Param2 p2, Param3 p3)) { m_Closure.bindmemfunc(static_cast<X*>(pthis), function_to_bind); }
	template <class X, class Y>
	inline void Bind(Y *pthis, DesiredRetType(X::* function_to_bind)(Param1 p1, Param2 p2, Param3 p3)) { m_Closure.bindmemfunc(static_cast<X*>(pthis), function_to_bind); }

	template <class X, class Y>
	Delegate3(const Y *pthis, DesiredRetType(X::* function_to_bind)(Param1 p1, Param2 p2, Param3 p3) const) { m_Closure.bindconstmemfunc(static_cast<const X*>(pthis), function_to_bind); }
	template <class X, class Y>
	inline void Bind(const Y *pthis, DesiredRetType(X::* function_to_bind)(Param1 p1, Param2 p2, Param3 p3) const) { m_Closure.bindconstmemfunc(static_cast<const X *>(pthis), function_to_bind); }

	Delegate3(DesiredRetType(*function_to_bind)(Param1 p1, Param2 p2, Param3 p3)) { Bind(function_to_bind); }
	void operator = (DesiredRetType(*function_to_bind)(Param1 p1, Param2 p2, Param3 p3)) { Bind(function_to_bind); }
	inline void Bind(DesiredRetType(*function_to_bind)(Param1 p1, Param2 p2, Param3 p3)) { m_Closure.bindstaticfunc(this, &Delegate3::InvokeStaticFunction, function_to_bind); }
	RetType operator() (Param1 p1, Param2 p2, Param3 p3) const { return (m_Closure.GetClosureThis()->*(m_Closure.GetClosureMemPtr()))(p1, p2, p3); }

private:
	typedef struct SafeBoolStruct { int a_data_pointer_to_this_is_0_on_buggy_compilers; StaticFunctionPtr m_nonzero; } UselessTypedef;
	typedef StaticFunctionPtr SafeBoolStruct::*unspecified_bool_type;

public:
	operator unspecified_bool_type() const { return empty() ? 0 : &SafeBoolStruct::m_nonzero; }

	inline bool operator==(StaticFunctionPtr funcptr) { return m_Closure.IsEqualToStaticFuncPtr(funcptr); }
	inline bool operator!=(StaticFunctionPtr funcptr) { return !m_Closure.IsEqualToStaticFuncPtr(funcptr); }
	inline bool operator ! () const { return !m_Closure; }
	inline bool empty() const { return !m_Closure; }
	void clear() { m_Closure.clear(); }
	const DelegateMemento & GetMemento() { return m_Closure; }
	void SetMemento(const DelegateMemento &any) { m_Closure.CopyFrom(this, any); }

private:
	RetType InvokeStaticFunction(Param1 p1, Param2 p2, Param3 p3) const { return (*(m_Closure.GetStaticFunction()))(p1, p2, p3); }
};


template<class Param1, class Param2, class Param3, class Param4, class RetType=detail::DefaultVoid>
class Delegate4
{
private:
	typedef typename detail::DefaultVoidToVoid<RetType>::type DesiredRetType;
	typedef DesiredRetType(*StaticFunctionPtr)(Param1 p1, Param2 p2, Param3 p3, Param4 p4);
	typedef RetType(*UnvoidStaticFunctionPtr)(Param1 p1, Param2 p2, Param3 p3, Param4 p4);
	typedef RetType(detail::GenericClass::*GenericMemFn)(Param1 p1, Param2 p2, Param3 p3, Param4 p4);
	typedef detail::ClosurePtr<GenericMemFn, StaticFunctionPtr, UnvoidStaticFunctionPtr> ClosureType;
	ClosureType m_Closure;

public:
	typedef Delegate4 type;

	Delegate4() { clear(); }
	Delegate4(const Delegate4 &x) { m_Closure.CopyFrom(this, x.m_Closure); }
	void operator = (const Delegate4 &x) { m_Closure.CopyFrom(this, x.m_Closure); }
	bool operator ==(const Delegate4 &x) const { return m_Closure.IsEqual(x.m_Closure); }
	bool operator !=(const Delegate4 &x) const { return !m_Closure.IsEqual(x.m_Closure); }
	bool operator <(const Delegate4 &x) const { return m_Closure.IsLess(x.m_Closure); }
	bool operator >(const Delegate4 &x) const { return x.m_Closure.IsLess(m_Closure); }

	template <class X, class Y>
	Delegate4(Y *pthis, DesiredRetType(X::* function_to_bind)(Param1 p1, Param2 p2, Param3 p3, Param4 p4)) { m_Closure.bindmemfunc(static_cast<X*>(pthis), function_to_bind); }
	template <class X, class Y>
	inline void Bind(Y *pthis, DesiredRetType(X::* function_to_bind)(Param1 p1, Param2 p2, Param3 p3, Param4 p4)) { m_Closure.bindmemfunc(static_cast<X*>(pthis), function_to_bind); }

	template <class X, class Y>
	Delegate4(const Y *pthis, DesiredRetType(X::* function_to_bind)(Param1 p1, Param2 p2, Param3 p3, Param4 p4) const) { m_Closure.bindconstmemfunc(static_cast<const X*>(pthis), function_to_bind); }
	template <class X, class Y>

	inline void Bind(const Y *pthis, DesiredRetType(X::* function_to_bind)(Param1 p1, Param2 p2, Param3 p3, Param4 p4) const) { m_Closure.bindconstmemfunc(static_cast<const X *>(pthis), function_to_bind); }
	Delegate4(DesiredRetType(*function_to_bind)(Param1 p1, Param2 p2, Param3 p3, Param4 p4)) { Bind(function_to_bind); }
	void operator = (DesiredRetType(*function_to_bind)(Param1 p1, Param2 p2, Param3 p3, Param4 p4)) { Bind(function_to_bind); }
	inline void Bind(DesiredRetType(*function_to_bind)(Param1 p1, Param2 p2, Param3 p3, Param4 p4)) { m_Closure.bindstaticfunc(this, &Delegate4::InvokeStaticFunction, function_to_bind); }
	RetType operator() (Param1 p1, Param2 p2, Param3 p3, Param4 p4) const { return (m_Closure.GetClosureThis()->*(m_Closure.GetClosureMemPtr()))(p1, p2, p3, p4); }

private:
	typedef struct SafeBoolStruct { int a_data_pointer_to_this_is_0_on_buggy_compilers; StaticFunctionPtr m_nonzero; } UselessTypedef;
	typedef StaticFunctionPtr SafeBoolStruct::*unspecified_bool_type;

public:
	operator unspecified_bool_type() const { return empty() ? 0 : &SafeBoolStruct::m_nonzero; }
	inline bool operator==(StaticFunctionPtr funcptr) { return m_Closure.IsEqualToStaticFuncPtr(funcptr); }
	inline bool operator!=(StaticFunctionPtr funcptr) { return !m_Closure.IsEqualToStaticFuncPtr(funcptr); }
	inline bool operator ! () const { return !m_Closure; }
	inline bool empty() const { return !m_Closure; }
	void clear() { m_Closure.clear(); }
	const DelegateMemento & GetMemento() { return m_Closure; }
	void SetMemento(const DelegateMemento &any) { m_Closure.CopyFrom(this, any); }

private:
	RetType InvokeStaticFunction(Param1 p1, Param2 p2, Param3 p3, Param4 p4) const { return (*(m_Closure.GetStaticFunction()))(p1, p2, p3, p4); }
};


template<class Param1, class Param2, class Param3, class Param4, class Param5, class RetType=detail::DefaultVoid>
class Delegate5
{
private:
	typedef typename detail::DefaultVoidToVoid<RetType>::type DesiredRetType;
	typedef DesiredRetType(*StaticFunctionPtr)(Param1 p1, Param2 p2, Param3 p3, Param4 p4, Param5 p5);
	typedef RetType(*UnvoidStaticFunctionPtr)(Param1 p1, Param2 p2, Param3 p3, Param4 p4, Param5 p5);
	typedef RetType(detail::GenericClass::*GenericMemFn)(Param1 p1, Param2 p2, Param3 p3, Param4 p4, Param5 p5);
	typedef detail::ClosurePtr<GenericMemFn, StaticFunctionPtr, UnvoidStaticFunctionPtr> ClosureType;
	ClosureType m_Closure;

public:
	typedef Delegate5 type;

	Delegate5() { clear(); }
	Delegate5(const Delegate5 &x) { m_Closure.CopyFrom(this, x.m_Closure); }
	void operator = (const Delegate5 &x) { m_Closure.CopyFrom(this, x.m_Closure); }
	bool operator ==(const Delegate5 &x) const { return m_Closure.IsEqual(x.m_Closure); }
	bool operator !=(const Delegate5 &x) const { return !m_Closure.IsEqual(x.m_Closure); }
	bool operator <(const Delegate5 &x) const { return m_Closure.IsLess(x.m_Closure); }
	bool operator >(const Delegate5 &x) const { return x.m_Closure.IsLess(m_Closure); }

	template <class X, class Y>
	Delegate5(Y *pthis, DesiredRetType(X::* function_to_bind)(Param1 p1, Param2 p2, Param3 p3, Param4 p4, Param5 p5)) { m_Closure.bindmemfunc(static_cast<X*>(pthis), function_to_bind); }
	template <class X, class Y>
	inline void Bind(Y *pthis, DesiredRetType(X::* function_to_bind)(Param1 p1, Param2 p2, Param3 p3, Param4 p4, Param5 p5)) { m_Closure.bindmemfunc(static_cast<X*>(pthis), function_to_bind); }

	template <class X, class Y>
	Delegate5(const Y *pthis, DesiredRetType(X::* function_to_bind)(Param1 p1, Param2 p2, Param3 p3, Param4 p4, Param5 p5) const) { m_Closure.bindconstmemfunc(static_cast<const X*>(pthis), function_to_bind); }
	template <class X, class Y>
	inline void Bind(const Y *pthis, DesiredRetType(X::* function_to_bind)(Param1 p1, Param2 p2, Param3 p3, Param4 p4, Param5 p5) const) { m_Closure.bindconstmemfunc(static_cast<const X *>(pthis), function_to_bind); }

	Delegate5(DesiredRetType(*function_to_bind)(Param1 p1, Param2 p2, Param3 p3, Param4 p4, Param5 p5)) { Bind(function_to_bind); }
	void operator = (DesiredRetType(*function_to_bind)(Param1 p1, Param2 p2, Param3 p3, Param4 p4, Param5 p5)) { Bind(function_to_bind); }
	inline void Bind(DesiredRetType(*function_to_bind)(Param1 p1, Param2 p2, Param3 p3, Param4 p4, Param5 p5)) { m_Closure.bindstaticfunc(this, &Delegate5::InvokeStaticFunction, function_to_bind); }
	RetType operator() (Param1 p1, Param2 p2, Param3 p3, Param4 p4, Param5 p5) const { return (m_Closure.GetClosureThis()->*(m_Closure.GetClosureMemPtr()))(p1, p2, p3, p4, p5); }

private:
	typedef struct SafeBoolStruct { int a_data_pointer_to_this_is_0_on_buggy_compilers; StaticFunctionPtr m_nonzero; } UselessTypedef;
	typedef StaticFunctionPtr SafeBoolStruct::*unspecified_bool_type;

public:
	operator unspecified_bool_type() const { return empty() ? 0 : &SafeBoolStruct::m_nonzero; }
	inline bool operator==(StaticFunctionPtr funcptr) { return m_Closure.IsEqualToStaticFuncPtr(funcptr); }
	inline bool operator!=(StaticFunctionPtr funcptr) { return !m_Closure.IsEqualToStaticFuncPtr(funcptr); }
	inline bool operator ! () const { return !m_Closure; }
	inline bool empty() const { return !m_Closure; }
	void clear() { m_Closure.clear(); }
	const DelegateMemento & GetMemento() { return m_Closure; }
	void SetMemento(const DelegateMemento &any) { m_Closure.CopyFrom(this, any); }

private:
	RetType InvokeStaticFunction(Param1 p1, Param2 p2, Param3 p3, Param4 p4, Param5 p5) const { return (*(m_Closure.GetStaticFunction()))(p1, p2, p3, p4, p5); }
};

#ifdef FASTDELEGATE_ALLOW_FUNCTION_TYPE_SYNTAX

template <typename Signature>
class Delegate;

template<typename R>
class Delegate<R()> : public Delegate0 <R>
{
public:
	typedef Delegate0<R> BaseType;
	typedef Delegate SelfType;

	Delegate() : BaseType() {}

	template <class X, class Y>
	Delegate(Y * pthis, R(X::* function_to_bind)()) : BaseType(pthis, function_to_bind) {}

	template <class X, class Y>
	Delegate(const Y *pthis, R(X::* function_to_bind)() const) : BaseType(pthis, function_to_bind) {}

	Delegate(R(*function_to_bind)()) : BaseType(function_to_bind) {}
	void operator = (const BaseType &x) { *static_cast<BaseType*>(this) = x; }
};


template<typename R, class Param1>
class Delegate<R (Param1)> : public Delegate1 <Param1, R>
{
public:
	typedef Delegate1 < Param1, R > BaseType;
	typedef Delegate SelfType;

	Delegate() : BaseType() {}

	template <class X, class Y>
	Delegate(Y * pthis, R(X::* function_to_bind)(Param1 p1)) : BaseType(pthis, function_to_bind) {}

	template <class X, class Y>
	Delegate(const Y *pthis, R(X::* function_to_bind)(Param1 p1) const) : BaseType(pthis, function_to_bind) {}

	Delegate(R(*function_to_bind)(Param1 p1)) : BaseType(function_to_bind) {}
	void operator = (const BaseType &x) { *static_cast<BaseType*>(this) = x; }
};


template<typename R, class Param1, class Param2>
class Delegate<R (Param1, Param2)> : public Delegate2 <Param1, Param2, R>
{
public:
	typedef Delegate2 < Param1, Param2, R > BaseType;
	typedef Delegate SelfType;

	Delegate() : BaseType() {}

	template <class X, class Y>
	Delegate(Y * pthis, R(X::* function_to_bind)(Param1 p1, Param2 p2)) : BaseType(pthis, function_to_bind) {}

	template <class X, class Y>
	Delegate(const Y *pthis, R(X::* function_to_bind)(Param1 p1, Param2 p2) const) : BaseType(pthis, function_to_bind) {}

	Delegate(R(*function_to_bind)(Param1 p1, Param2 p2)) : BaseType(function_to_bind) {}
	void operator = (const BaseType &x) { *static_cast<BaseType*>(this) = x; }
};


template<typename R, class Param1, class Param2, class Param3>
class Delegate<R ( Param1, Param2, Param3 )> : public Delegate3 <Param1, Param2, Param3, R>
{
public:
	typedef Delegate3 <Param1, Param2, Param3, R> BaseType;
	typedef Delegate SelfType;

	Delegate() : BaseType() {}

	template <class X, class Y>
	Delegate(Y * pthis, R(X::* function_to_bind)(Param1 p1, Param2 p2, Param3 p3)) : BaseType(pthis, function_to_bind) {}

	template <class X, class Y>
	Delegate(const Y *pthis, R(X::* function_to_bind)(Param1 p1, Param2 p2, Param3 p3) const) : BaseType(pthis, function_to_bind) {}

	Delegate(R(*function_to_bind)(Param1 p1, Param2 p2, Param3 p3)) : BaseType(function_to_bind) {}
	void operator = (const BaseType &x) { *static_cast<BaseType*>(this) = x; }
};


template<typename R, class Param1, class Param2, class Param3, class Param4>
class Delegate<R (Param1, Param2, Param3, Param4)> : public Delegate4 <Param1, Param2, Param3, Param4, R>
{
public:
	typedef Delegate4 <Param1, Param2, Param3, Param4, R> BaseType;
	typedef Delegate SelfType;

	Delegate() : BaseType() {}

	template <class X, class Y>
	Delegate(Y * pthis, R(X::* function_to_bind)(Param1 p1, Param2 p2, Param3 p3, Param4 p4)) : BaseType(pthis, function_to_bind) {}

	template <class X, class Y>
	Delegate(const Y *pthis, R(X::* function_to_bind)(Param1 p1, Param2 p2, Param3 p3, Param4 p4) const) : BaseType(pthis, function_to_bind) {}

	Delegate(R(*function_to_bind)(Param1 p1, Param2 p2, Param3 p3, Param4 p4)) : BaseType(function_to_bind) {}
	void operator = (const BaseType &x) { *static_cast<BaseType*>(this) = x; }
};


template<typename R, class Param1, class Param2, class Param3, class Param4, class Param5>
class Delegate<R(Param1, Param2, Param3, Param4, Param5)> : public Delegate5 <Param1, Param2, Param3, Param4, Param5, R>
{
public:
	typedef Delegate5 <Param1, Param2, Param3, Param4, Param5, R> BaseType;
	typedef Delegate SelfType;

	Delegate() : BaseType() {}

	template <class X, class Y>
	Delegate(Y * pthis, R(X::* function_to_bind)(Param1 p1, Param2 p2, Param3 p3, Param4 p4, Param5 p5)) : BaseType(pthis, function_to_bind) {}

	template <class X, class Y>
	Delegate(const Y *pthis, R(X::* function_to_bind)(Param1 p1, Param2 p2, Param3 p3, Param4 p4, Param5 p5) const) : BaseType(pthis, function_to_bind) {}

	Delegate(R(*function_to_bind)(Param1 p1, Param2 p2, Param3 p3, Param4 p4, Param5 p5)) : BaseType(function_to_bind) {}
	void operator = (const BaseType &x) { *static_cast<BaseType*>(this) = x; }
};
#endif

#define FASTDLGT_RETTYPE RetType

template <class X, class Y, class RetType>
Delegate0<FASTDLGT_RETTYPE> MakeDelegate(Y* x, RetType(X::*func)()) { return Delegate0<FASTDLGT_RETTYPE>(x, func); }
template <class X, class Y, class RetType>
Delegate0<FASTDLGT_RETTYPE> MakeDelegate(Y* x, RetType(X::*func)() const) { return Delegate0<FASTDLGT_RETTYPE>(x, func); }

template <class X, class Y, class Param1, class RetType>
Delegate1<Param1, FASTDLGT_RETTYPE> MakeDelegate(Y* x, RetType(X::*func)(Param1 p1)) { return Delegate1<Param1, FASTDLGT_RETTYPE>(x, func); }
template <class X, class Y, class Param1, class RetType>
Delegate1<Param1, FASTDLGT_RETTYPE> MakeDelegate(Y* x, RetType(X::*func)(Param1 p1) const) { return Delegate1<Param1, FASTDLGT_RETTYPE>(x, func); }

template <class X, class Y, class Param1, class Param2, class RetType>
Delegate2<Param1, Param2, FASTDLGT_RETTYPE> MakeDelegate(Y* x, RetType(X::*func)(Param1 p1, Param2 p2)) { return Delegate2<Param1, Param2, FASTDLGT_RETTYPE>(x, func); }
template <class X, class Y, class Param1, class Param2, class RetType>
Delegate2<Param1, Param2, FASTDLGT_RETTYPE> MakeDelegate(Y* x, RetType(X::*func)(Param1 p1, Param2 p2) const) { return Delegate2<Param1, Param2, FASTDLGT_RETTYPE>(x, func); }

template <class X, class Y, class Param1, class Param2, class Param3, class RetType>
Delegate3<Param1, Param2, Param3, FASTDLGT_RETTYPE> MakeDelegate(Y* x, RetType(X::*func)(Param1 p1, Param2 p2, Param3 p3)) { return Delegate3<Param1, Param2, Param3, FASTDLGT_RETTYPE>(x, func); }
template <class X, class Y, class Param1, class Param2, class Param3, class RetType>
Delegate3<Param1, Param2, Param3, FASTDLGT_RETTYPE> MakeDelegate(Y* x, RetType(X::*func)(Param1 p1, Param2 p2, Param3 p3) const) { return Delegate3<Param1, Param2, Param3, FASTDLGT_RETTYPE>(x, func); }

template <class X, class Y, class Param1, class Param2, class Param3, class Param4, class RetType>
Delegate4<Param1, Param2, Param3, Param4, FASTDLGT_RETTYPE> MakeDelegate(Y* x, RetType(X::*func)(Param1 p1, Param2 p2, Param3 p3, Param4 p4)) { return Delegate4<Param1, Param2, Param3, Param4, FASTDLGT_RETTYPE>(x, func); }
template <class X, class Y, class Param1, class Param2, class Param3, class Param4, class RetType>
Delegate4<Param1, Param2, Param3, Param4, FASTDLGT_RETTYPE> MakeDelegate(Y* x, RetType(X::*func)(Param1 p1, Param2 p2, Param3 p3, Param4 p4) const) { return Delegate4<Param1, Param2, Param3, Param4, FASTDLGT_RETTYPE>(x, func); }

template <class X, class Y, class Param1, class Param2, class Param3, class Param4, class Param5, class RetType>
Delegate5<Param1, Param2, Param3, Param4, Param5, FASTDLGT_RETTYPE> MakeDelegate(Y* x, RetType(X::*func)(Param1 p1, Param2 p2, Param3 p3, Param4 p4, Param5 p5)) { return Delegate5<Param1, Param2, Param3, Param4, Param5, FASTDLGT_RETTYPE>(x, func); }
template <class X, class Y, class Param1, class Param2, class Param3, class Param4, class Param5, class RetType>
Delegate5<Param1, Param2, Param3, Param4, Param5, FASTDLGT_RETTYPE> MakeDelegate(Y* x, RetType(X::*func)(Param1 p1, Param2 p2, Param3 p3, Param4 p4, Param5 p5) const) { return Delegate5<Param1, Param2, Param3, Param4, Param5, FASTDLGT_RETTYPE>(x, func); }

#undef FASTDLGT_RETTYPE

}
