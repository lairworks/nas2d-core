#include "NAS2D/Signal/Signal.h"

#include <gtest/gtest.h>
#include <gmock/gmock.h>


namespace {
	class MockHandler {
	public:
		MOCK_CONST_METHOD0(MockMethod, void());
	};
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

TEST(Signal, MultiListener) {
	NAS2D::Signal<> signal;
	MockHandler handler1{};
	MockHandler handler2{};
	auto delegate1 = NAS2D::Delegate{&handler1, &MockHandler::MockMethod};
	auto delegate2 = NAS2D::Delegate{&handler2, &MockHandler::MockMethod};

	EXPECT_CALL(handler1, MockMethod()).Times(1);
	EXPECT_CALL(handler2, MockMethod()).Times(1);
	signal.connect(delegate1);
	signal.connect(delegate2);
	signal.emit();
}

TEST(Signal, DelegateWrappingSignal) {
	NAS2D::Signal<> signal;
	MockHandler handler1{};
	MockHandler handler2{};
	auto delegate1 = NAS2D::Delegate{&handler1, &MockHandler::MockMethod};
	auto delegate2 = NAS2D::Delegate{&handler2, &MockHandler::MockMethod};

	EXPECT_CALL(handler1, MockMethod()).Times(1);
	EXPECT_CALL(handler2, MockMethod()).Times(1);
	signal.connect(delegate1);
	signal.connect(delegate2);

	auto delegateHandler = NAS2D::Delegate{&signal, &decltype(signal)::emit};

	delegateHandler();
}
