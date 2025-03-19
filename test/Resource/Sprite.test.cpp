#include "NAS2D/Resource/Sprite.h"

#include <gtest/gtest.h>
#include <gmock/gmock.h>


namespace {
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

		static constexpr NAS2D::Vector imageSize{1, 1};
		static constexpr NAS2D::Rectangle imageRect{{0, 0}, imageSize};
		static constexpr NAS2D::Vector anchorOffset{0, 0};
		uint32_t imageBuffer[imageSize.x * imageSize.y];
		NAS2D::Image image{&imageBuffer, 4, imageSize};
		NAS2D::AnimationSet::Frame frame{image, imageRect, anchorOffset, 2};
		NAS2D::AnimationSet::Frame frameStop{image, imageRect, anchorOffset, 0};
		NAS2D::AnimationSet testAnimationSet{{}, {{"defaultAction", {frame}}, {"frameStopAction", {frameStop}}}};
		SpriteDerived sprite{testAnimationSet, "defaultAction"};
	};
}


TEST_F(Sprite, size) {
	EXPECT_EQ((NAS2D::Vector{1, 1}), sprite.size());
}

TEST_F(Sprite, isPaused) {
	EXPECT_FALSE(sprite.isPaused());
}

TEST_F(Sprite, isPausedStopAnimation) {
	sprite.play("frameStopAction");
	EXPECT_TRUE(sprite.isPaused());
}

TEST_F(Sprite, advanceByTimeDelta) {
	EXPECT_EQ(0u, sprite.advanceByTimeDelta(0u));
	EXPECT_EQ(0u, sprite.advanceByTimeDelta(1u));
	EXPECT_EQ(2u, sprite.advanceByTimeDelta(2u));
	EXPECT_EQ(2u, sprite.advanceByTimeDelta(3u));
	EXPECT_EQ(4u, sprite.advanceByTimeDelta(4u));
}


namespace {
	class SpriteCompleteSignal : public Sprite {
	protected:
		class MockHandler {
		public:
			MOCK_CONST_METHOD0(MockMethod, void());
		};

		MockHandler handler{};
		NAS2D::Delegate<void()> delegate{&handler, &MockHandler::MockMethod};

		void SetUp() override {
			sprite.animationCompleteSignalSource().connect(delegate);
		}
	};
}


TEST_F(SpriteCompleteSignal, animationCompleteSignalNone) {
	EXPECT_CALL(handler, MockMethod()).Times(0);
	sprite.advanceByTimeDelta(0u);
	sprite.advanceByTimeDelta(1u);
}

TEST_F(SpriteCompleteSignal, animationCompleteSignalOnceDelta2) {
	EXPECT_CALL(handler, MockMethod());
	sprite.advanceByTimeDelta(2u);
}

TEST_F(SpriteCompleteSignal, animationCompleteSignalOnceDelta3) {
	EXPECT_CALL(handler, MockMethod());
	sprite.advanceByTimeDelta(3u);
}

TEST_F(SpriteCompleteSignal, animationCompleteSignalTwice) {
	EXPECT_CALL(handler, MockMethod()).Times(2);
	sprite.advanceByTimeDelta(4u);
}

TEST_F(SpriteCompleteSignal, animationCompleteSignalStopFrameDelta0) {
	EXPECT_CALL(handler, MockMethod());
	sprite.play("frameStopAction");
	sprite.advanceByTimeDelta(0u); // Trigger handler
	sprite.advanceByTimeDelta(0u); // No additional trigger
}

TEST_F(SpriteCompleteSignal, animationCompleteSignalStopFrameDelta4) {
	EXPECT_CALL(handler, MockMethod());
	sprite.play("frameStopAction");
	sprite.advanceByTimeDelta(4u); // Trigger handler
	sprite.advanceByTimeDelta(4u); // No additional trigger
}
