#include "NAS2D/Signal/SignalConnection.h"
#include "NAS2D/Signal/Signal.h"

#include <gtest/gtest.h>
#include <gmock/gmock.h>


namespace {
	class MockHandler {
	public:
		MOCK_CONST_METHOD0(MockMethod, void());
	};
}


TEST(SignalConnection, Scope) {
	MockHandler handler{};
	auto delegate = NAS2D::Delegate{&handler, &MockHandler::MockMethod};
	NAS2D::Signal<> signal;

	// Expect a single call to MockMethod
	EXPECT_CALL(handler, MockMethod()).Times(1);

	{
		// SignalConnection is only valid in this code block
		auto connection = NAS2D::SignalConnection{signal, delegate};
		signal();
	}

	// Disconnected - No additional calls to MockMethod
	signal();
}
