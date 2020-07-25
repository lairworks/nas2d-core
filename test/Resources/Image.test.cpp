#include "NAS2D/Resources/Image.h"
#include <gtest/gtest.h>


TEST(Image, size) {
	EXPECT_EQ((NAS2D::Vector{1, 1}), NAS2D::Image(1, 1).size());
	EXPECT_EQ((NAS2D::Vector{4, 2}), NAS2D::Image(4, 2).size());
}
