#include "NAS2D/Resources/Image.h"
#include <gtest/gtest.h>


TEST(Image, size) {
	EXPECT_EQ((NAS2D::Vector<int>{1, 1}), NAS2D::Image(1, 1).size());
	EXPECT_EQ((NAS2D::Vector<int>{4, 2}), NAS2D::Image(4, 2).size());
}

TEST(Image, center) {
	EXPECT_EQ(0, NAS2D::Image(1, 1).center_x());
	EXPECT_EQ(0, NAS2D::Image(1, 1).center_y());

	EXPECT_EQ(2, NAS2D::Image(4, 2).center_x());
	EXPECT_EQ(1, NAS2D::Image(4, 2).center_y());
}
