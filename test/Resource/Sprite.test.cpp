#include "NAS2D/Resource/Sprite.h"

#include <gtest/gtest.h>
#include <gmock/gmock.h>


class Sprite : public ::testing::Test {
protected:
	// Use a derived class to access protected methods
	class SpriteDerived : public NAS2D::Sprite {
	public:
		SpriteDerived(const NAS2D::AnimationSet& animationSet, const std::string& initialAction) :
			Sprite{animationSet, initialAction}
		{}
		// Re-export protected method as public
		using Sprite::advanceByTimeDelta;
	};

	class MockHandler {
	public:
		MOCK_CONST_METHOD0(MockMethod, void());
	};

	uint32_t imageBuffer[1 * 1];
	NAS2D::Image image{&imageBuffer, 4, {1, 1}};
	NAS2D::AnimationSet::Frame frame{image, {{0, 0}, {1, 1}}, {0, 0}, 2};
	NAS2D::AnimationSet::Frame frameStop{image, {{0, 0}, {1, 1}}, {0, 0}, 0};
	NAS2D::AnimationSet animationSet{{}, {{"defaultAction", {frame}}, {"frameStopAction", {frameStop}}}};
	SpriteDerived sprite{animationSet, "defaultAction"};
};


TEST_F(Sprite, size) {
	EXPECT_EQ((NAS2D::Vector{1, 1}), sprite.size());
}

TEST_F(Sprite, advanceByTimeDelta) {
	EXPECT_EQ(0u, sprite.advanceByTimeDelta(0u));
	EXPECT_EQ(0u, sprite.advanceByTimeDelta(1u));
	EXPECT_EQ(2u, sprite.advanceByTimeDelta(2u));
	EXPECT_EQ(2u, sprite.advanceByTimeDelta(3u));
	EXPECT_EQ(4u, sprite.advanceByTimeDelta(4u));
}

TEST_F(Sprite, animationCompleteSignal) {
	MockHandler handler{};
	auto delegate = NAS2D::Delegate{&handler, &MockHandler::MockMethod};
	sprite.animationCompleteSignalSource().connect(delegate);

	sprite.advanceByTimeDelta(0u);
	sprite.advanceByTimeDelta(1u);

	EXPECT_CALL(handler, MockMethod());
	sprite.advanceByTimeDelta(2u);

	EXPECT_CALL(handler, MockMethod());
	sprite.advanceByTimeDelta(3u);

	EXPECT_CALL(handler, MockMethod()).Times(2);
	sprite.advanceByTimeDelta(4u);

	for (auto i = 0u; i <= 4u; ++i) {
		sprite.play("frameStopAction");
		EXPECT_CALL(handler, MockMethod());
		sprite.advanceByTimeDelta(i);
		EXPECT_CALL(handler, MockMethod()).Times(0);
		sprite.advanceByTimeDelta(i);
	}
}
