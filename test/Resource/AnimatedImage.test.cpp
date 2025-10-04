#include <NAS2D/Resource/AnimatedImage.h>

#include <NAS2D/Resource/AnimationFrame.h>
#include <NAS2D/Resource/AnimationSequence.h>
#include <NAS2D/Resource/Image.h>
#include <NAS2D/Math/Vector.h>
#include <NAS2D/Math/Rectangle.h>

#include <gtest/gtest.h>


namespace {
	constexpr NAS2D::Vector imageSize{1, 1};
	constexpr NAS2D::Rectangle imageRect{{0, 0}, imageSize};
	constexpr NAS2D::Vector anchorOffset{0, 0};
	uint32_t imageBuffer[imageSize.x * imageSize.y];
	NAS2D::Image image{&imageBuffer, 4, imageSize};
	NAS2D::AnimationFrame frame{image, imageRect, anchorOffset, {2}};
	NAS2D::AnimationFrame frameStop{image, imageRect, anchorOffset, {0}};
	NAS2D::AnimationSequence sequenceFrame{{frame}};
	NAS2D::AnimationSequence sequenceFrameFrameLoop{{frame, frame}};
	NAS2D::AnimationSequence sequenceFrameFrameStop{{frame, frameStop}};
}


TEST(AnimatedImage, initReinit) {
	auto animatedImage = NAS2D::AnimatedImage{sequenceFrame};
	EXPECT_EQ(1, animatedImage.frameCount());
	EXPECT_EQ(0, animatedImage.frameIndex());
	animatedImage = NAS2D::AnimatedImage{sequenceFrameFrameLoop};
	EXPECT_EQ(2, animatedImage.frameCount());
	EXPECT_EQ(0, animatedImage.frameIndex());
}

TEST(AnimatedImage, animateSingleFrameLoop) {
	auto animatedImage = NAS2D::AnimatedImage{sequenceFrame};
	EXPECT_EQ(0, animatedImage.frameIndex());
	animatedImage.advanceFrame();
	EXPECT_EQ(0, animatedImage.frameIndex());
}

TEST(AnimatedImage, animateMultiFrameLoop) {
	auto animatedImage = NAS2D::AnimatedImage{sequenceFrameFrameLoop};
	EXPECT_EQ(0, animatedImage.frameIndex());
	animatedImage.advanceFrame();
	EXPECT_EQ(1, animatedImage.frameIndex());
	animatedImage.advanceFrame();
	EXPECT_EQ(0, animatedImage.frameIndex());
}

TEST(AnimatedImage, animateFrameFrameStop) {
	auto animatedImage = NAS2D::AnimatedImage{sequenceFrameFrameStop};
	EXPECT_EQ(0, animatedImage.frameIndex());
	animatedImage.advanceFrame();
	EXPECT_EQ(1, animatedImage.frameIndex());
	animatedImage.advanceFrame();
	EXPECT_EQ(1, animatedImage.frameIndex());
}

TEST(AnimatedImage, setFrame) {
	auto animatedImage = NAS2D::AnimatedImage{sequenceFrameFrameLoop};
	animatedImage.setFrame(0);
	EXPECT_EQ(0, animatedImage.frameIndex());
	animatedImage.setFrame(1);
	EXPECT_EQ(1, animatedImage.frameIndex());
}

TEST(AnimatedImage, setFrameWrap) {
	auto animatedImage = NAS2D::AnimatedImage{sequenceFrameFrameLoop};
	animatedImage.setFrame(2);
	EXPECT_EQ(0, animatedImage.frameIndex());
}

TEST(AnimatedImage, advanceFrameTimeDelta) {
	auto animatedImage = NAS2D::AnimatedImage{sequenceFrameFrameLoop};
	EXPECT_EQ(NAS2D::Duration{0u}, animatedImage.advanceFrame(NAS2D::Duration{0u}));
	EXPECT_EQ(NAS2D::Duration{0u}, animatedImage.advanceFrame(NAS2D::Duration{1u}));
	EXPECT_EQ(NAS2D::Duration{2u}, animatedImage.advanceFrame(NAS2D::Duration{2u}));
	EXPECT_EQ(NAS2D::Duration{2u}, animatedImage.advanceFrame(NAS2D::Duration{3u}));
	EXPECT_EQ(NAS2D::Duration{4u}, animatedImage.advanceFrame(NAS2D::Duration{4u}));
}
