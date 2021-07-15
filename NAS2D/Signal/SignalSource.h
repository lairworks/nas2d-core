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

#include "Delegate.h"
#include <set>


namespace NAS2D
{
	template <typename... Params>
	class SignalSource
	{
	public:
		using Source = SignalSource; // Restricted base interface for use in derived
		using DelegateType = DelegateX<void, Params...>;

	public:
		bool empty() const { return delegateList.empty(); }

		void clear() { delegateList.clear(); }

		void connect(DelegateType delegate) { delegateList.insert(delegate); }

		template <typename X, typename Y>
		void connect(Y* obj, void (X::*func)(Params...)) { delegateList.insert(MakeDelegate(obj, func)); }

		template <typename X, typename Y>
		void connect(Y* obj, void (X::*func)(Params...) const) { delegateList.insert(MakeDelegate(obj, func)); }

		void disconnect(DelegateType delegate) { delegateList.erase(delegate); }

		template <typename X, typename Y>
		void disconnect(Y* obj, void (X::*func)(Params...)) { delegateList.erase(MakeDelegate(obj, func)); }

		template <typename X, typename Y>
		void disconnect(Y* obj, void (X::*func)(Params...) const) { delegateList.erase(MakeDelegate(obj, func)); }

	protected:
		std::set<DelegateType> delegateList;
	};
} // namespace
