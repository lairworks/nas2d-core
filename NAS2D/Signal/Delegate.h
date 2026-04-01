// ==================================================================================
// = NAS2D
// = Copyright © 2008 - 2020 New Age Software
// ==================================================================================
// = NAS2D is distributed under the terms of the zlib license. You are free to copy,
// = modify and distribute the software under the terms of the zlib license.
// =
// = Acknowledgment of your use of NAS2D is appreciated but is not required.
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

#include "Forward.h"

#include <cstring>


// Compiler identification. It's not easy to identify Visual C++ because many vendors
// fraudulently define Microsoft's identifiers.
#if defined(_MSC_VER) && !defined(__MWERKS__) && !defined(__VECTOR_C) && !defined(__ICL) && !defined(__BORLANDC__)
	#define FASTDLGT_MICROSOFT_MFP
#endif


namespace NAS2D
{
	namespace detail
	{
		template <typename OutputClass, typename InputClass>
		union horrible_union
		{
			OutputClass out;
			InputClass in;
		};

		template <typename OutputClass, typename InputClass>
		inline OutputClass horrible_cast(const InputClass input)
		{
			horrible_union<OutputClass, InputClass> u;
			static_assert(sizeof(InputClass) == sizeof(u) && sizeof(InputClass) == sizeof(OutputClass), "Can't use horrible cast");
			u.in = input;
			return u.out;
		}


		template <typename GenericMemFuncType, typename XFuncType>
		GenericMemFuncType CastMemFuncPtr(XFuncType function_to_bind)
		{
#if __GNUC__ >= 8
	#pragma GCC diagnostic push
	#pragma GCC diagnostic ignored "-Wcast-function-type"
#endif
			return reinterpret_cast<GenericMemFuncType>(function_to_bind);
#if __GNUC__ >= 8
	#pragma GCC diagnostic pop
#endif
		}

// GenericClass is a fake class, ONLY used to provide a type. It is vitally important
// that it is never defined.
#ifdef FASTDLGT_MICROSOFT_MFP
		class __single_inheritance GenericClass;
		class GenericClass
		{};
#else
		class GenericClass;
#endif

		constexpr int memberFunctionPointerSize = sizeof(void(GenericClass::*)());

		template <int N>
		struct SimplifyMemFunc
		{
			template <typename X, typename XFuncType, typename GenericMemFuncType>
			inline static GenericClass* Convert(X* /*pthis*/, XFuncType /*function_to_bind*/, GenericMemFuncType& /*bound_func*/)
			{
				static_assert(N > 100, "Unsupported member function pointer on this compiler");
				return nullptr;
			}
		};

		template <>
		struct SimplifyMemFunc<memberFunctionPointerSize>
		{
			template <typename X, typename XFuncType, typename GenericMemFuncType>
			inline static GenericClass* Convert(X* pthis, XFuncType function_to_bind, GenericMemFuncType& bound_func)
			{
				bound_func = CastMemFuncPtr<GenericMemFuncType>(function_to_bind);
				return reinterpret_cast<GenericClass*>(pthis);
			}
		};

#ifdef FASTDLGT_MICROSOFT_MFP

		template <>
		struct SimplifyMemFunc<memberFunctionPointerSize + sizeof(int)>
		{
			template <typename X, typename XFuncType, typename GenericMemFuncType>
			inline static GenericClass* Convert(X* pthis, XFuncType function_to_bind, GenericMemFuncType& bound_func)
			{
				union
				{
					XFuncType func;
					struct
					{
						GenericMemFuncType funcaddress;
						int delta;
					} s;
				} u;

				static_assert(sizeof(function_to_bind) == sizeof(u.s), "Can't use horrible cast");
				u.func = function_to_bind;
				bound_func = u.s.funcaddress;
				return reinterpret_cast<GenericClass*>(reinterpret_cast<char*>(pthis) + u.s.delta);
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
			using ProbePtrType = GenericVirtualClass* (GenericVirtualClass::*)();
			GenericVirtualClass* GetThis() { return this; }
		};


		template <>
		struct SimplifyMemFunc<memberFunctionPointerSize + 2 * sizeof(int)>
		{
			template <typename X, typename XFuncType, typename GenericMemFuncType>
			inline static GenericClass* Convert(X* pthis, XFuncType function_to_bind, GenericMemFuncType& bound_func)
			{
				union
				{
					XFuncType func;
					GenericClass* (X::*ProbeFunc)();
					MicrosoftVirtualMFP s;
				} u;

				u.func = function_to_bind;
				bound_func = CastMemFuncPtr<GenericMemFuncType>(u.s.codeptr);

				union
				{
					GenericVirtualClass::ProbePtrType virtfunc;
					MicrosoftVirtualMFP s;
				} u2;

				static_assert(sizeof(function_to_bind) == sizeof(u.s) && sizeof(function_to_bind) == sizeof(u.ProbeFunc) && sizeof(u2.virtfunc) == sizeof(u2.s), "Can't use horrible cast");

				u2.virtfunc = &GenericVirtualClass::GetThis;
				u.s.codeptr = u2.s.codeptr;
				return (pthis->*u.ProbeFunc)();
			}
		};


		template <>
		struct SimplifyMemFunc<memberFunctionPointerSize + 3 * sizeof(int)>
		{
			template <typename X, typename XFuncType, typename GenericMemFuncType>
			inline static GenericClass* Convert(X* pthis, XFuncType function_to_bind, GenericMemFuncType& bound_func)
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
					const int* vtable = *reinterpret_cast<const int* const*>(reinterpret_cast<const char*>(pthis) + u.s.vtordisp);
					virtual_delta = u.s.vtordisp + *reinterpret_cast<const int*>(reinterpret_cast<const char*>(vtable) + u.s.vtable_index);
				}
				return reinterpret_cast<GenericClass*>(reinterpret_cast<char*>(pthis) + u.s.delta + virtual_delta);
			}
		};
#endif
	}


	class DelegateMemento
	{
	protected:
		using GenericMemFuncType = void (detail::GenericClass::*)();
		detail::GenericClass* mTargetObject;
		GenericMemFuncType mTargetFunction;

	public:
		DelegateMemento() :
			mTargetObject(nullptr),
			mTargetFunction(nullptr)
		{}

		DelegateMemento(const DelegateMemento& right) :
			mTargetObject(right.mTargetObject),
			mTargetFunction(right.mTargetFunction)
		{}

		void clear()
		{
			mTargetObject = nullptr;
			mTargetFunction = nullptr;
		}

		inline bool operator==(const DelegateMemento& x) const = default;

		inline bool operator<(const DelegateMemento& right) const
		{
			if (mTargetObject != right.mTargetObject) return mTargetObject < right.mTargetObject;

			return memcmp(&mTargetFunction, &right.mTargetFunction, sizeof(mTargetFunction)) < 0;
		}

		inline bool operator>(const DelegateMemento& right) const { return right < *this; }

		inline bool operator!() const { return !mTargetObject && !mTargetFunction; }
		inline bool empty() const { return !mTargetObject && !mTargetFunction; }

		DelegateMemento& operator=(const DelegateMemento& right) = default;
	};


	namespace detail
	{
		template <typename GenericMemFunc, typename StaticFuncPtr>
		class ClosurePtr : public DelegateMemento
		{
		public:
			template <typename X, typename XMemFunc>
			inline void bindMemFunc(X* targetObject, XMemFunc targetMemberFunction)
			{
				mTargetObject = SimplifyMemFunc<sizeof(targetMemberFunction)>::Convert(targetObject, targetMemberFunction, mTargetFunction);
			}

			template <typename X, typename XMemFunc>
			inline void bindConstMemFunc(const X* targetObject, XMemFunc targetMemberFunction)
			{
				mTargetObject = SimplifyMemFunc<sizeof(targetMemberFunction)>::Convert(const_cast<X*>(targetObject), targetMemberFunction, mTargetFunction);
			}

			inline GenericClass* GetClosureThis() const
			{
				return mTargetObject;
			}

			inline GenericMemFunc GetClosureMemPtr() const
			{
				return CastMemFuncPtr<GenericMemFunc>(mTargetFunction);
			}

			template <typename DerivedClass, typename ParentInvokerSig>
			inline void bindStaticFunc(DerivedClass* pParent, ParentInvokerSig static_function_invoker, StaticFuncPtr targetStaticFunction)
			{
				if (!targetStaticFunction)
				{
					mTargetFunction = nullptr;
				}
				else
				{
					bindMemFunc(pParent, static_function_invoker);
				}
				static_assert(sizeof(GenericClass*) == sizeof(targetStaticFunction), "Can't use evil method");
				mTargetObject = horrible_cast<GenericClass*>(targetStaticFunction);
			}

			inline StaticFuncPtr GetStaticFunction() const
			{
				static_assert(sizeof(StaticFuncPtr) == sizeof(this), "Can't use evil method");
				return horrible_cast<StaticFuncPtr>(this);
			}

			inline bool IsEqualToStaticFuncPtr(StaticFuncPtr funcptr)
			{
				if (!funcptr)
				{
					return empty();
				}
				else
				{
					return funcptr == GetStaticFunction();
				}
			}
		};
	}


	template <typename RetType, typename... Params>
	class DelegateX
	{
	private:
		using StaticFunctionPtr = RetType (*)(Params...);
		using GenericMemFn = RetType (detail::GenericClass::*)(Params...);
		using ClosureType = detail::ClosurePtr<GenericMemFn, StaticFunctionPtr>;
		ClosureType mClosure{};

	public:
		DelegateX() = default;

		template <typename X, typename Y>
		DelegateX(Y* targetObject, RetType (X::*targetMemberFunction)(Params...))
		{
			mClosure.bindMemFunc(static_cast<X*>(targetObject), targetMemberFunction);
		}

		template <typename X, typename Y>
		DelegateX(const Y* targetObject, RetType (X::*targetMemberFunction)(Params...) const)
		{
			mClosure.bindConstMemFunc(static_cast<const X*>(targetObject), targetMemberFunction);
		}

		explicit DelegateX(RetType (*targetStaticFunction)(Params...))
		{
			Bind(targetStaticFunction);
		}

		DelegateX& operator=(RetType (*targetStaticFunction)(Params...))
		{
			Bind(targetStaticFunction);
			return *this;
		}

		bool operator==(const DelegateX& x) const { return mClosure == x.mClosure; }
		bool operator!=(const DelegateX& x) const { return mClosure != x.mClosure; }
		bool operator<(const DelegateX& x) const { return mClosure < x.mClosure; }
		bool operator>(const DelegateX& x) const { return mClosure > x.mClosure; }

		template <typename X, typename Y>
		inline void Bind(Y* targetObject, RetType (X::*targetMemberFunction)(Params...))
		{
			mClosure.bindMemFunc(static_cast<X*>(targetObject), targetMemberFunction);
		}

		template <typename X, typename Y>
		inline void Bind(const Y* targetObject, RetType (X::*targetMemberFunction)(Params...) const)
		{
			mClosure.bindConstMemFunc(static_cast<const X*>(targetObject), targetMemberFunction);
		}

		inline void Bind(RetType (*targetStaticFunction)(Params...))
		{
			mClosure.bindStaticFunc(this, &DelegateX::InvokeStaticFunction, targetStaticFunction);
		}

		RetType operator()(Params... params) const
		{
			return (mClosure.GetClosureThis()->*(mClosure.GetClosureMemPtr()))(NAS2D::forward<Params>(params)...);
		}

	public:
		explicit operator bool() const { return !empty(); }

		inline bool operator==(StaticFunctionPtr funcptr) { return mClosure.IsEqualToStaticFuncPtr(funcptr); }
		inline bool operator!=(StaticFunctionPtr funcptr) { return !mClosure.IsEqualToStaticFuncPtr(funcptr); }
		inline bool operator!() const { return !mClosure; }
		inline bool empty() const { return !mClosure; }
		void clear() { mClosure.clear(); }

	private:
		RetType InvokeStaticFunction(Params... params) const
		{
			return (*(mClosure.GetStaticFunction()))(NAS2D::forward<Params>(params)...);
		}
	};


	template <typename Signature>
	class Delegate;

	template <typename RetType, typename... Params>
	class Delegate<RetType(Params...)> : public DelegateX<RetType, Params...>
	{
	public:
		using BaseType = DelegateX<RetType, Params...>;

		Delegate() = default;

		template <typename X, typename Y>
		Delegate(Y* targetObject, RetType (X::*targetMemberFunction)(Params...)) :
			BaseType(targetObject, targetMemberFunction)
		{}

		template <typename X, typename Y>
		Delegate(const Y* targetObject, RetType (X::*targetMemberFunction)(Params...) const) :
			BaseType(targetObject, targetMemberFunction)
		{}

		explicit Delegate(RetType (*targetStaticFunction)(Params...)) :
			BaseType(targetStaticFunction)
		{}
	};

	template <typename X, typename Y, typename RetType, typename... Params>
	Delegate(Y*, RetType (X::*func)(Params...)) -> Delegate<RetType(Params...)>;

	template <typename X, typename Y, typename RetType, typename... Params>
	Delegate(Y*, RetType (X::*func)(Params...) const) -> Delegate<RetType(Params...)>;

	template <typename RetType, typename... Params>
	Delegate(RetType (*func)(Params...)) -> Delegate<RetType(Params...)>;
}
