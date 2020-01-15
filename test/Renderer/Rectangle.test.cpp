#include "NAS2D/Renderer/Rectangle.h"
#include <gtest/gtest.h>


TEST(Rectangle, contains) {
	NAS2D::Rectangle rect = {1, 1, 2, 2};

	// Start point inclusive, and interior
	EXPECT_TRUE(rect.contains(NAS2D::Point{1, 1}));
	EXPECT_TRUE(rect.contains(NAS2D::Point{1, 2}));
	EXPECT_TRUE(rect.contains(NAS2D::Point{2, 1}));
	EXPECT_TRUE(rect.contains(NAS2D::Point{2, 2}));

	// Endpoint exclusive
	EXPECT_FALSE(rect.contains(NAS2D::Point{1, 3}));
	EXPECT_FALSE(rect.contains(NAS2D::Point{3, 1}));
	EXPECT_FALSE(rect.contains(NAS2D::Point{3, 3}));

	// Far out of bounds
	EXPECT_FALSE(rect.contains(NAS2D::Point{0, 0}));
	EXPECT_FALSE(rect.contains(NAS2D::Point{0, 4}));
	EXPECT_FALSE(rect.contains(NAS2D::Point{4, 0}));
	EXPECT_FALSE(rect.contains(NAS2D::Point{4, 4}));
}
