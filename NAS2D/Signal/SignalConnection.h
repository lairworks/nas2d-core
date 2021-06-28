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
#include "SignalSource.h"


namespace NAS2D {


template<typename ... Params>
class SignalConnection
{
public:
	using SignalType = SignalSource<Params...>;
	using DelegateType = typename SignalType::DelegateType;

	SignalConnection(const SignalConnection&) = delete;
	SignalConnection(SignalConnection&&) = delete;
	SignalConnection& operator=(const SignalConnection&) = delete;
	SignalConnection& operator=(SignalConnection&&) = delete;

	SignalConnection(SignalType& signalSource, DelegateType delegate) :
		mSignalSource{signalSource},
		mDelegate{delegate}
	{
		mSignalSource.connect(mDelegate);
	}

	~SignalConnection()
	{
		mSignalSource.disconnect(mDelegate);
	}

private:
	SignalType& mSignalSource;
	DelegateType mDelegate;
};

}
