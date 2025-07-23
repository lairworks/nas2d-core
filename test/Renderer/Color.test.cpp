#include "NAS2D/Renderer/Color.h"

#include <gtest/gtest.h>


TEST(Color, OperatorCompare) {
	EXPECT_EQ((NAS2D::Color{255, 255, 255, 255}), (NAS2D::Color{255, 255, 255, 255}));
	EXPECT_EQ((NAS2D::Color{0, 0, 0, 0}), (NAS2D::Color{0, 0, 0, 0}));
	EXPECT_EQ((NAS2D::Color{0, 1, 2, 3}), (NAS2D::Color{0, 1, 2, 3}));
	EXPECT_EQ((NAS2D::Color{3, 2, 1, 0}), (NAS2D::Color{3, 2, 1, 0}));

	EXPECT_NE((NAS2D::Color{1, 0, 0, 0}), (NAS2D::Color{0, 0, 0, 0}));
	EXPECT_NE((NAS2D::Color{0, 1, 0, 0}), (NAS2D::Color{0, 0, 0, 0}));
	EXPECT_NE((NAS2D::Color{0, 0, 1, 0}), (NAS2D::Color{0, 0, 0, 0}));
	EXPECT_NE((NAS2D::Color{0, 0, 0, 1}), (NAS2D::Color{0, 0, 0, 0}));
}

TEST(Color, alphaFade) {
	EXPECT_EQ((NAS2D::Color{255, 255, 255, 255}), NAS2D::Color::White.alphaFade(255));
	EXPECT_EQ((NAS2D::Color{255, 255, 255, 128}), NAS2D::Color::White.alphaFade(128));
	EXPECT_EQ((NAS2D::Color{255, 255, 255, 0}), NAS2D::Color::White.alphaFade(0));

	EXPECT_EQ((NAS2D::Color{255, 0, 0, 255}), NAS2D::Color::Red.alphaFade(255));
	EXPECT_EQ((NAS2D::Color{255, 0, 0, 128}), NAS2D::Color::Red.alphaFade(128));
	EXPECT_EQ((NAS2D::Color{255, 0, 0, 0}), NAS2D::Color::Red.alphaFade(0));

	EXPECT_EQ((NAS2D::Color{0, 255, 0, 255}), NAS2D::Color::Green.alphaFade(255));
	EXPECT_EQ((NAS2D::Color{0, 255, 0, 128}), NAS2D::Color::Green.alphaFade(128));
	EXPECT_EQ((NAS2D::Color{0, 255, 0, 0}), NAS2D::Color::Green.alphaFade(0));

	EXPECT_EQ((NAS2D::Color{0, 0, 255, 255}), NAS2D::Color::Blue.alphaFade(255));
	EXPECT_EQ((NAS2D::Color{0, 0, 255, 128}), NAS2D::Color::Blue.alphaFade(128));
	EXPECT_EQ((NAS2D::Color{0, 0, 255, 0}), NAS2D::Color::Blue.alphaFade(0));
}

TEST(Color, alphaBlend) {
	EXPECT_EQ((NAS2D::Color{0, 0, 0, 255}), (NAS2D::Color{0, 0, 0, 255}.alphaBlend({255, 255, 255, 255})));
	EXPECT_EQ((NAS2D::Color{255, 255, 255, 255}), (NAS2D::Color{255, 255, 255, 255}.alphaBlend({255, 255, 255, 255})));

	EXPECT_EQ((NAS2D::Color{255, 255, 255, 255}), (NAS2D::Color{0, 0, 0, 0}.alphaBlend({255, 255, 255, 255})));
	EXPECT_EQ((NAS2D::Color{255, 255, 255, 255}), (NAS2D::Color{255, 255, 255, 0}.alphaBlend({255, 255, 255, 255})));
	EXPECT_EQ((NAS2D::Color{0, 0, 0, 255}), (NAS2D::Color{255, 255, 255, 0}.alphaBlend({0, 0, 0, 255})));

	EXPECT_EQ((NAS2D::Color{128, 127, 0, 191}), (NAS2D::Color{255, 0, 0, 128}.alphaBlend({0, 255, 0, 255})));
}
