#include "NAS2D/Resource/Sprite.h"

#include <gtest/gtest.h>


class Sprite : public ::testing::Test {
protected:
	uint32_t imageBuffer[1 * 1];
	NAS2D::Image image{&imageBuffer, 4, {1, 1}};
	NAS2D::AnimationSet::Frame frame{image, {0, 0, 1, 1}, {0, 0}, 0};
	NAS2D::AnimationSet animationSet{"", {}, {{"defaultAction", {frame}}}};
	NAS2D::Sprite sprite{animationSet, "defaultAction"};
};


TEST_F(Sprite, size) {
	EXPECT_EQ((NAS2D::Vector{1, 1}), sprite.size());
}
