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
	NAS2D::Signals::Signal0<> signal;

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
