#include "NAS2D/Signal.h"
#include <gtest/gtest.h>
#include <gmock/gmock.h>


namespace {
	class MockHandler {
	public:
		MOCK_CONST_METHOD0(MockMethod, void());
	};
}


TEST(Signal, ConnectEmitDisconnect) {
	MockHandler handler;
	auto delegate = NAS2D::MakeDelegate(&handler, &MockHandler::MockMethod);
	NAS2D::Signals::Signal<> signal;

	EXPECT_TRUE(signal.empty());

	signal.connect(delegate);
	EXPECT_FALSE(signal.empty());

	EXPECT_CALL(handler, MockMethod());
	signal.emit();

	signal.disconnect(delegate);
	EXPECT_TRUE(signal.empty());

	EXPECT_CALL(handler, MockMethod()).Times(0);
	signal.emit();
}

TEST(Signal, Connection) {
	MockHandler handler;
	auto delegate = NAS2D::MakeDelegate(&handler, &MockHandler::MockMethod);
	NAS2D::Signals::Signal<> signal;

	// Expect a single call to MockMethod
	EXPECT_CALL(handler, MockMethod()).Times(1);

	{
		// Connection is only valid in this code block
		auto connection = decltype(signal)::Connection{signal, delegate};
		signal.emit();
	}

	// Disconnected - No additional calls to MockMethod
	signal.emit();
}
