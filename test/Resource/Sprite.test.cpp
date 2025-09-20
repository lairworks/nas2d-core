#include "NAS2D/Resource/Sprite.h"

#include "NAS2D/Duration.h"

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
		NAS2D::AnimationSet::AnimationFrame frame{image, imageRect, anchorOffset, {2}};
		NAS2D::AnimationSet::AnimationFrame frameStop{image, imageRect, anchorOffset, {0}};
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
	EXPECT_EQ(NAS2D::Duration{0u}, sprite.advanceByTimeDelta(NAS2D::Duration{0u}));
	EXPECT_EQ(NAS2D::Duration{0u}, sprite.advanceByTimeDelta(NAS2D::Duration{1u}));
	EXPECT_EQ(NAS2D::Duration{2u}, sprite.advanceByTimeDelta(NAS2D::Duration{2u}));
	EXPECT_EQ(NAS2D::Duration{2u}, sprite.advanceByTimeDelta(NAS2D::Duration{3u}));
	EXPECT_EQ(NAS2D::Duration{4u}, sprite.advanceByTimeDelta(NAS2D::Duration{4u}));
}
