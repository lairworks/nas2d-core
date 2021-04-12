#pragma once

#include "Delegate.h"
#include "SignalSource.h"


namespace NAS2D::Signals {


template<typename ... Params>
class Connection
{
public:
	using SignalType = SignalSource<Params...>;
	using DelegateType = DelegateX<void, Params...>;

	Connection(SignalType& signalSource, DelegateType delegate) :
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
	SignalType& mSignalSource;
	DelegateType mDelegate;
};

}
