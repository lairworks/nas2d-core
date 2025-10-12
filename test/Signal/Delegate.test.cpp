#include "NAS2D/Signal/Delegate.h"

#include <gtest/gtest.h>
#include <gmock/gmock.h>


namespace {
	class MockHandler {
	public:
		MOCK_CONST_METHOD1(MockMethod, void(int value));
		MOCK_CONST_METHOD1(MockMethod2, void(int value));
	};
}


TEST(Delegate, DelegateCall) {
	const MockHandler handler{};
	auto delegate = NAS2D::Delegate{&handler, &MockHandler::MockMethod};
	EXPECT_CALL(handler, MockMethod(0));
	EXPECT_CALL(handler, MockMethod(1));
	delegate(0);
	delegate(1);
}

TEST(Delegate, DelegateCallConst) {
	const MockHandler handler{};
	auto delegate = NAS2D::Delegate{&handler, &MockHandler::MockMethod};
	EXPECT_CALL(handler, MockMethod(0));
	EXPECT_CALL(handler, MockMethod(1));
	delegate(0);
	delegate(1);
}

TEST(Delegate, CopyDelegate) {
	const MockHandler handler{};
	// Copy delegate (value copy, with value equal compare)
	auto delegate1 = NAS2D::Delegate{&handler, &MockHandler::MockMethod};
	auto delegate2 = delegate1;
	EXPECT_EQ(delegate1, delegate2);
}

TEST(Delegate, DelegateEqual) {
	const MockHandler handler{};
	// Make identical delegates, which should compare equal
	auto delegate1 = NAS2D::Delegate{&handler, &MockHandler::MockMethod};
	auto delegate2 = NAS2D::Delegate{&handler, &MockHandler::MockMethod};
	EXPECT_EQ(delegate1, delegate2);
}

TEST(Delegate, DelegateNotEqualMethods) {
	const MockHandler handler{};
	// Different handler methods should compare not equal
	auto delegate1 = NAS2D::Delegate{&handler, &MockHandler::MockMethod};
	auto delegate2 = NAS2D::Delegate{&handler, &MockHandler::MockMethod2};
	EXPECT_NE(delegate1, delegate2);
}

TEST(Delegate, DelegateNotEqualObjects) {
	const MockHandler handler1{};
	const MockHandler handler2{};
	// Different handler objects should compare not equal
	auto delegate1 = NAS2D::Delegate{&handler1, &MockHandler::MockMethod};
	auto delegate2 = NAS2D::Delegate{&handler2, &MockHandler::MockMethod};
	EXPECT_NE(delegate1, delegate2);
}

TEST(Delegate, LambdaVariableCapture) {
	const auto lambda = [](){ return 42; };
	auto delegate = NAS2D::Delegate{&lambda, &decltype(lambda)::operator()};
	EXPECT_EQ(42, delegate());
}

TEST(Delegate, ForwardWithoutCopy) {
	struct CopyCounter {
		int numCopies;

		CopyCounter() noexcept : numCopies{} {}
		CopyCounter(CopyCounter&& other) noexcept : numCopies{other.numCopies} {}
		CopyCounter(const CopyCounter& other) noexcept : numCopies{other.numCopies + 1} {}
	};

	struct CopyReceiver {
		int receiveCopy(CopyCounter copyCounter) { return copyCounter.numCopies; }
	};

	CopyReceiver copyReceiver;
	auto delegate = NAS2D::Delegate{&copyReceiver, &CopyReceiver::receiveCopy};

	CopyCounter copyCounter;
	EXPECT_EQ(1, delegate(copyCounter));
	EXPECT_EQ(0, delegate(CopyCounter{}));
}
