#include "NAS2D/Connection.h"
#include "NAS2D/Signal.h"

#include <gtest/gtest.h>
#include <gmock/gmock.h>


namespace {
	class MockHandler {
	public:
		MOCK_CONST_METHOD0(MockMethod, void());
	};
}


TEST(Signal, Connection) {
	MockHandler handler;
	auto delegate = NAS2D::MakeDelegate(&handler, &MockHandler::MockMethod);
	NAS2D::Signals::Signal<> signal;

	// Expect a single call to MockMethod
	EXPECT_CALL(handler, MockMethod()).Times(1);

	{
		// Connection is only valid in this code block
		auto connection = NAS2D::Signals::Connection{signal, delegate};
		signal.emit();
	}

	// Disconnected - No additional calls to MockMethod
	signal.emit();
}
