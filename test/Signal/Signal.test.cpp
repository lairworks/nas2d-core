#include "NAS2D/Signal/Signal.h"

#include <gtest/gtest.h>
#include <gmock/gmock.h>


namespace {
	class MockHandler {
	public:
		MOCK_CONST_METHOD0(MockMethod, void());
	};
}


TEST(Signal, InitEmpty) {
	NAS2D::Signal<> signal;
	EXPECT_TRUE(signal.empty());
}

TEST(Signal, ConnectNonEmpty) {
	NAS2D::Signal<> signal;
	MockHandler handler{};
	auto delegate = NAS2D::Delegate{&handler, &MockHandler::MockMethod};

	signal.connect(delegate);
	EXPECT_FALSE(signal.empty());
}

TEST(Signal, DisconnectEmpty) {
	NAS2D::Signal<> signal;
	MockHandler handler{};
	auto delegate = NAS2D::Delegate{&handler, &MockHandler::MockMethod};

	signal.connect(delegate);
	signal.disconnect(delegate);
	EXPECT_TRUE(signal.empty());
}

TEST(Signal, ConnectEmitDisconnect) {
	NAS2D::Signal<> signal;
	MockHandler handler{};
	auto delegate = NAS2D::Delegate{&handler, &MockHandler::MockMethod};

	EXPECT_CALL(handler, MockMethod()).Times(1);
	signal.connect(delegate);
	signal.emit();
	signal.disconnect(delegate);
	signal.emit();
}
