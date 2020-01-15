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

TEST(Rectangle, overlaps) {
	NAS2D::Rectangle rect = {1, 1, 2, 2};

	// Identical overlap, and interior
	EXPECT_TRUE(rect.overlaps(NAS2D::Rectangle{1, 1, 2, 2}));
	EXPECT_TRUE(rect.overlaps(NAS2D::Rectangle{1, 1, 1, 1}));
	EXPECT_TRUE(rect.overlaps(NAS2D::Rectangle{1, 2, 1, 1}));
	EXPECT_TRUE(rect.overlaps(NAS2D::Rectangle{2, 1, 1, 1}));
	EXPECT_TRUE(rect.overlaps(NAS2D::Rectangle{2, 2, 1, 1}));

	// Partial overlap
	EXPECT_TRUE(rect.overlaps(NAS2D::Rectangle{0, 0, 2, 2}));
	EXPECT_TRUE(rect.overlaps(NAS2D::Rectangle{0, 2, 2, 2}));
	EXPECT_TRUE(rect.overlaps(NAS2D::Rectangle{2, 0, 2, 2}));
	EXPECT_TRUE(rect.overlaps(NAS2D::Rectangle{2, 2, 2, 2}));

	// Touching, with no overlap, 8 surrounding boxes
	// Corner points (bottom left, top left, bottom right, top right)
	EXPECT_FALSE(rect.overlaps(NAS2D::Rectangle{0, 0, 1, 1}));
	EXPECT_FALSE(rect.overlaps(NAS2D::Rectangle{0, 3, 1, 1}));
	EXPECT_FALSE(rect.overlaps(NAS2D::Rectangle{3, 0, 1, 1}));
	EXPECT_FALSE(rect.overlaps(NAS2D::Rectangle{3, 3, 1, 1}));
	// Side edges (bottom, top, left, right)
	EXPECT_FALSE(rect.overlaps(NAS2D::Rectangle{1, 0, 2, 1}));
	EXPECT_FALSE(rect.overlaps(NAS2D::Rectangle{1, 3, 2, 1}));
	EXPECT_FALSE(rect.overlaps(NAS2D::Rectangle{0, 1, 1, 2}));
	EXPECT_FALSE(rect.overlaps(NAS2D::Rectangle{3, 1, 1, 2}));

	// Disjoint
	EXPECT_FALSE(rect.overlaps(NAS2D::Rectangle{-1, -1, 1, 1}));
	EXPECT_FALSE(rect.overlaps(NAS2D::Rectangle{-1, 4, 1, 1}));
	EXPECT_FALSE(rect.overlaps(NAS2D::Rectangle{4, -1, 1, 1}));
	EXPECT_FALSE(rect.overlaps(NAS2D::Rectangle{4, 4, 1, 1}));

	// No overlap for zero area, even if start point matches
	EXPECT_FALSE((NAS2D::Rectangle{0, 0, 0, 0}.overlaps(NAS2D::Rectangle{0, 0, 0, 0})));
}
