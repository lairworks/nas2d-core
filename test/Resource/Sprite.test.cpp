#include "NAS2D/Resource/Sprite.h"

#include <gtest/gtest.h>


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

	uint32_t imageBuffer[1 * 1];
	NAS2D::Image image{&imageBuffer, 4, {1, 1}};
	NAS2D::AnimationSet::Frame frame{image, {0, 0, 1, 1}, {0, 0}, 2};
	NAS2D::AnimationSet animationSet{"", {}, {{"defaultAction", {frame}}}};
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
