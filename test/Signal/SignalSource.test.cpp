#include "NAS2D/Signal/SignalSource.h"

#include <gtest/gtest.h>
#include <gmock/gmock.h>


namespace {
	class MockHandler {
	public:
		MOCK_CONST_METHOD0(MockMethod, void());
	};
}


TEST(SignalSource, InitEmpty) {
	NAS2D::SignalSource<> signalSource;
	EXPECT_TRUE(signalSource.isEmpty());
}

TEST(SignalSource, ConnectNonEmpty) {
	NAS2D::SignalSource<> signalSource;
	MockHandler handler{};
	auto delegate = NAS2D::Delegate{&handler, &MockHandler::MockMethod};

	signalSource.connect(delegate);
	EXPECT_FALSE(signalSource.isEmpty());
}

TEST(SignalSource, DisconnectEmpty) {
	NAS2D::SignalSource<> signalSource;
	MockHandler handler{};
	auto delegate = NAS2D::Delegate{&handler, &MockHandler::MockMethod};

	signalSource.connect(delegate);
	signalSource.disconnect(delegate);
	EXPECT_TRUE(signalSource.isEmpty());
}
