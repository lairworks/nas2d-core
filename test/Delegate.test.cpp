#include "NAS2D/Delegate.h"
#include <gtest/gtest.h>
#include <gmock/gmock.h>


namespace {
	class MockHandler {
	public:
		MOCK_CONST_METHOD1(MockMethod, void(int value));
		MOCK_CONST_METHOD1(MockMethod2, void(int value));
	};
}


// Disabled due to GoogleMock crashes on Windows
TEST(Delegate, DISABLED_DelegateCall) {
	MockHandler handler;
	auto delegate = NAS2D::MakeDelegate(&handler, &MockHandler::MockMethod);
	EXPECT_CALL(handler, MockMethod(0));
	EXPECT_CALL(handler, MockMethod(1));
	delegate(0);
	delegate(1);
}

// Disabled due to GoogleMock crashes on Windows
TEST(Delegate, DISABLED_DelegateCallConst) {
	const MockHandler handler;
	auto delegate = NAS2D::MakeDelegate(&handler, &MockHandler::MockMethod);
	EXPECT_CALL(handler, MockMethod(0));
	EXPECT_CALL(handler, MockMethod(1));
	delegate(0);
	delegate(1);
}

TEST(Delegate, CopyDelegate) {
	const MockHandler handler;
	// Copy delegate (value copy, with value equal compare)
	auto delegate1 = NAS2D::MakeDelegate(&handler, &MockHandler::MockMethod);
	auto delegate2 = delegate1;
	EXPECT_EQ(delegate1, delegate2);
}

TEST(Delegate, MakeDelegateEqual) {
	const MockHandler handler;
	// Make identical delegates, which should compare equal
	auto delegate1 = NAS2D::MakeDelegate(&handler, &MockHandler::MockMethod);
	auto delegate2 = NAS2D::MakeDelegate(&handler, &MockHandler::MockMethod);
	EXPECT_EQ(delegate1, delegate2);
}

TEST(Delegate, MakeDelegateNotEqualMethods) {
	const MockHandler handler;
	// Different handler methods should compare not equal
	auto delegate1 = NAS2D::MakeDelegate(&handler, &MockHandler::MockMethod);
	auto delegate2 = NAS2D::MakeDelegate(&handler, &MockHandler::MockMethod2);
	EXPECT_NE(delegate1, delegate2);
}

TEST(Delegate, MakeDelegateNotEqualObjects) {
	const MockHandler handler1;
	const MockHandler handler2;
	// Different handler objects should compare not equal
	auto delegate1 = NAS2D::MakeDelegate(&handler1, &MockHandler::MockMethod);
	auto delegate2 = NAS2D::MakeDelegate(&handler2, &MockHandler::MockMethod);
	EXPECT_NE(delegate1, delegate2);
}
