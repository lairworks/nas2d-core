#include "NAS2D/Signal/Signal.h"

#include <gtest/gtest.h>
#include <gmock/gmock.h>


namespace {
	class MockHandler {
	public:
		MOCK_CONST_METHOD0(MockMethod, void());
	};

	struct CopyCounter {
		int numCopies;

		CopyCounter() noexcept : numCopies{} {}
		CopyCounter(CopyCounter&& other) noexcept : numCopies{other.numCopies} {}
		CopyCounter(const CopyCounter& other) noexcept : numCopies{other.numCopies + 1} {}
	};

	struct CopyReceiver {
		int copyCount{};
		NAS2D::Signal<CopyCounter> signal{};

		CopyReceiver() { signal.connect({this, &CopyReceiver::receiveCopy}); }

		void receiveCopy(CopyCounter copyCounter) { copyCount = copyCounter.numCopies; }

		int callAndCount(CopyCounter copyCounter) { signal(std::move(copyCounter)); return copyCount; }
	};
}


TEST(Signal, ConnectEmitDisconnect) {
	NAS2D::Signal<> signal;
	MockHandler handler{};
	auto delegate = NAS2D::Delegate{&handler, &MockHandler::MockMethod};

	EXPECT_CALL(handler, MockMethod()).Times(1);
	signal.connect(delegate);
	signal();
	signal.disconnect(delegate);
	signal();
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
	signal();
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

	auto delegateHandler = NAS2D::Delegate{&signal, &decltype(signal)::operator()};

	delegateHandler();
}

TEST(Signal, EmitParameterCopyLimit) {
	CopyReceiver copyReceiver;
	CopyCounter copyCounter;
	EXPECT_LE(2, copyReceiver.callAndCount(copyCounter));
	EXPECT_LE(1, copyReceiver.callAndCount(CopyCounter{}));
}
