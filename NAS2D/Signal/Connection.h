#pragma once

#include "Delegate.h"
#include "SignalSource.h"


namespace NAS2D {


template<typename ... Params>
class Connection
{
public:
	using SignalType = SignalSource<Params...>;
	using DelegateType = DelegateX<void, Params...>;

	// No copy/move construction/assignment
	// This class is designed to handle a parent object's connection to a signal
	// The delegate parameter is likely tied to the address of the parent object
	// When parent object is copied/moved, a new updated connection must be formed
	// Disable copy/move to remove default copy/move from parent objects
	Connection(const Connection&) = delete;
	Connection(Connection&&) = delete;
	Connection& operator=(const Connection&) = delete;
	Connection& operator=(Connection&&) = delete;

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
