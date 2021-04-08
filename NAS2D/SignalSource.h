#pragma once

#include "Delegate.h"
#include <set>


namespace NAS2D::Signals {


template<typename ... Params>
class SignalSource
{
public:
	using DelegateType = DelegateX<void, Params...>;

	class Connection
	{
	public:
		Connection(SignalSource& signalSource, DelegateType delegate) :
			mSignalSource{signalSource},
			mDelegate{delegate}
		{
			mSignalSource.connect(mDelegate);
		}

		~Connection()
		{
			mSignalSource.disconnect(mDelegate);
		}

	private:
		SignalSource& mSignalSource;
		DelegateType mDelegate;
	};

public:
	bool empty() const { return delegateList.empty(); }

	void clear() { delegateList.clear(); }

	void connect(DelegateType delegate) { delegateList.insert(delegate); }

	template<typename X, typename Y>
	void connect(Y * obj, void (X::*func)(Params...)) { delegateList.insert(MakeDelegate(obj, func)); }

	template<typename X, typename Y>
	void connect(Y * obj, void (X::*func)(Params...) const) { delegateList.insert(MakeDelegate(obj, func)); }

	void disconnect(DelegateType delegate) { delegateList.erase(delegate); }

	template<typename X, typename Y>
	void disconnect(Y * obj, void (X::*func)(Params...)) { delegateList.erase(MakeDelegate(obj, func)); }

	template<typename X, typename Y>
	void disconnect(Y * obj, void (X::*func)(Params...) const) { delegateList.erase(MakeDelegate(obj, func)); }

protected:
	std::set<DelegateType> delegateList;
};


} // namespace
