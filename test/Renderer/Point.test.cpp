#include "NAS2D/Renderer/Point.h"
#include <gtest/gtest.h>


TEST(Point, DefaultConstructibleZeroInit) {
	EXPECT_EQ((NAS2D::Point<int>{0, 0}), NAS2D::Point<int>{});
}

TEST(Point, OperatorEqualNotEqual) {
	EXPECT_EQ((NAS2D::Point<int>{1, 1}), (NAS2D::Point<int>{1, 1}));
	EXPECT_EQ((NAS2D::Point<int>{2, 2}), (NAS2D::Point<int>{2, 2}));

	EXPECT_NE((NAS2D::Point<int>{1, 1}), (NAS2D::Point<int>{1, 2}));
	EXPECT_NE((NAS2D::Point<int>{1, 1}), (NAS2D::Point<int>{2, 1}));
}

TEST(Point, Conversion) {
	// Allow explicit conversion
	EXPECT_EQ((NAS2D::Point<int>{1, 1}), static_cast<NAS2D::Point<int>>(NAS2D::Point<float>{1.0, 1.0}));
	EXPECT_EQ((NAS2D::Point<float>{1.0, 1.0}), static_cast<NAS2D::Point<float>>(NAS2D::Point<int>{1, 1}));

	// Allow implicit conversion (may be deprecated in the future)
	EXPECT_EQ((NAS2D::Point<int>{1, 1}), (NAS2D::Point<float>{1.0, 1.0}));
	EXPECT_EQ((NAS2D::Point<float>{1.0, 1.0}), (NAS2D::Point<int>{1, 1}));
}
