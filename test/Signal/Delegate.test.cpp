#include "NAS2D/Signal/Delegate.h"

#include <gtest/gtest.h>
#include <gmock/gmock.h>

#include <type_traits>


namespace {
	int staticFunction(int value) { return value; }

	class MockHandler {
	public:
		MOCK_CONST_METHOD1(MockMethod, void(int value));
		MOCK_CONST_METHOD1(MockMethod2, void(int value));
	};
}


TEST(Delegate, CallStatic) {
	const auto delegate = NAS2D::Delegate{&staticFunction};
	EXPECT_EQ(0, delegate(0));
	EXPECT_EQ(1, delegate(1));
}

TEST(Delegate, DelegateCall) {
	const MockHandler handler{};
	const auto delegate = NAS2D::Delegate{&handler, &MockHandler::MockMethod};
	EXPECT_CALL(handler, MockMethod(0));
	EXPECT_CALL(handler, MockMethod(1));
	delegate(0);
	delegate(1);
}

TEST(Delegate, DelegateCallConst) {
	const MockHandler handler{};
	const auto delegate = NAS2D::Delegate{&handler, &MockHandler::MockMethod};
	EXPECT_CALL(handler, MockMethod(0));
	EXPECT_CALL(handler, MockMethod(1));
	delegate(0);
	delegate(1);
}

TEST(Delegate, CopyInitialize) {
	const MockHandler handler{};
	// Copy delegate (value copy, with value equal compare)
	const auto delegate1 = NAS2D::Delegate{&handler, &MockHandler::MockMethod};
	const auto delegate2 = delegate1;
	EXPECT_EQ(delegate1, delegate2);
}

TEST(Delegate, CopyAssign) {
	const MockHandler handler{};
	// Copy delegate (value copy, with value equal compare)
	const auto delegate1 = NAS2D::Delegate{&handler, &MockHandler::MockMethod};
	auto delegate2 = NAS2D::Delegate<void(int)>{};
	delegate2 = delegate1;
	EXPECT_EQ(delegate1, delegate2);
}

TEST(Delegate, DelegateEqual) {
	const MockHandler handler{};
	// Make identical delegates, which should compare equal
	const auto delegate1 = NAS2D::Delegate{&handler, &MockHandler::MockMethod};
	const auto delegate2 = NAS2D::Delegate{&handler, &MockHandler::MockMethod};
	EXPECT_EQ(delegate1, delegate2);
}

TEST(Delegate, DelegateNotEqualMethods) {
	const MockHandler handler{};
	// Different handler methods should compare not equal
	const auto delegate1 = NAS2D::Delegate{&handler, &MockHandler::MockMethod};
	const auto delegate2 = NAS2D::Delegate{&handler, &MockHandler::MockMethod2};
	EXPECT_NE(delegate1, delegate2);
}

TEST(Delegate, DelegateNotEqualObjects) {
	const MockHandler handler1{};
	const MockHandler handler2{};
	// Different handler objects should compare not equal
	const auto delegate1 = NAS2D::Delegate{&handler1, &MockHandler::MockMethod};
	const auto delegate2 = NAS2D::Delegate{&handler2, &MockHandler::MockMethod};
	EXPECT_NE(delegate1, delegate2);
}

TEST(Delegate, BoolConversionTrue) {
	const MockHandler handler{};
	const auto delegate = NAS2D::Delegate{&handler, &MockHandler::MockMethod};
	EXPECT_CALL(handler, MockMethod(0)).Times(1);
	if (delegate) { delegate(0); }
}

TEST(Delegate, BoolConversionFalse) {
	const MockHandler handler{};
	const auto delegate = NAS2D::Delegate<void(int)>{};
	EXPECT_CALL(handler, MockMethod(0)).Times(0);
	if (delegate) { delegate(0); }
}

TEST(Delegate, IntConversionFail) {
	// Ensure there are no conversions to `int` (potentially through an implicit conversion to `bool`)
	// This would allow code such as: if (delegate > 0)
	EXPECT_FALSE((std::is_convertible_v<NAS2D::Delegate<void(int)>, int>));
}

TEST(Delegate, ImplicitBoolConversionFail) {
	// Ensure there are no implicit conversions to `bool`
	// This is a stronger guarantee than the `int` conversion fail
	EXPECT_FALSE((std::is_convertible_v<NAS2D::Delegate<void(int)>, bool>));
}

TEST(Delegate, LambdaVariableCapture) {
	const auto lambda = [](){ return 42; };
	const auto delegate = NAS2D::Delegate{&lambda, &decltype(lambda)::operator()};
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
	const auto delegate = NAS2D::Delegate{&copyReceiver, &CopyReceiver::receiveCopy};

	CopyCounter copyCounter;
	EXPECT_EQ(1, delegate(copyCounter));
	EXPECT_EQ(0, delegate(CopyCounter{}));
}
