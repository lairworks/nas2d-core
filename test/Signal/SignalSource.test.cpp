#include "NAS2D/Signal/SignalSource.h"

#include <gtest/gtest.h>
#include <gmock/gmock.h>


namespace {
	class MockHandler {
	public:
		MOCK_CONST_METHOD0(MockMethod, void());
	};
}


TEST(SignalSource, InitCountZero) {
	NAS2D::SignalSource<> signalSource;
	EXPECT_EQ(std::size_t{0}, signalSource.count());
}

TEST(SignalSource, ConnectCountOne) {
	NAS2D::SignalSource<> signalSource;
	MockHandler handler{};
	auto delegate = NAS2D::Delegate{&handler, &MockHandler::MockMethod};

	signalSource.connect(delegate);
	EXPECT_EQ(std::size_t{1}, signalSource.count());
}

TEST(SignalSource, ConnectCountTwo) {
	NAS2D::SignalSource<> signalSource;
	MockHandler handler1{};
	MockHandler handler2{};
	auto delegate1 = NAS2D::Delegate{&handler1, &MockHandler::MockMethod};
	auto delegate2 = NAS2D::Delegate{&handler2, &MockHandler::MockMethod};

	signalSource.connect(delegate1);
	signalSource.connect(delegate2);
	EXPECT_EQ(std::size_t{2}, signalSource.count());
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
