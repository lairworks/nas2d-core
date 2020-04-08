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
