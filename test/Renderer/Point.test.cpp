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

TEST(Point, SelfAddVector) {
	NAS2D::Point<int> point{1, 1};
	EXPECT_EQ(&point, &(point += NAS2D::Vector<int>{1, 2}));
	EXPECT_EQ((NAS2D::Point<int>{2, 3}), point);
}

TEST(Point, SelfSubtractVector) {
	NAS2D::Point<int> point{2, 5};
	EXPECT_EQ(&point, &(point -= NAS2D::Vector<int>{1, 3}));
	EXPECT_EQ((NAS2D::Point<int>{1, 2}), point);
}

TEST(Vector, AddVector) {
	EXPECT_EQ((NAS2D::Point<int>{2, 3}), (NAS2D::Point<int>{1, 1}) + (NAS2D::Vector<int>{1, 2}));
}

TEST(Vector, SubtractVector) {
	EXPECT_EQ((NAS2D::Point<int>{1, 2}), (NAS2D::Point<int>{2, 5}) - (NAS2D::Vector<int>{1, 3}));
}

TEST(Vector, SubtractPointToVector) {
	EXPECT_EQ((NAS2D::Vector<int>{1, 1}), (NAS2D::Point<int>{2, 3}) - (NAS2D::Point<int>{1, 2}));
}

TEST(Point, OperatorType) {
	// Allow explicit conversion
	EXPECT_EQ((NAS2D::Point<int>{1, 2}), static_cast<NAS2D::Point<int>>(NAS2D::Point<float>{1.0, 2.0}));
	EXPECT_EQ((NAS2D::Point<float>{1.0, 2.0}), static_cast<NAS2D::Point<float>>(NAS2D::Point<int>{1, 2}));

	// Allow implicit conversion (may be deprecated in the future)
	EXPECT_EQ((NAS2D::Point<int>{1, 2}), (NAS2D::Point<float>{1.0, 2.0}));
	EXPECT_EQ((NAS2D::Point<float>{1.0, 2.0}), (NAS2D::Point<int>{1, 2}));
}

TEST(Point, to) {
	EXPECT_EQ((NAS2D::Point<int>{1, 2}), (NAS2D::Point<float>{1.0, 2.0}.to<int>()));
	EXPECT_EQ((NAS2D::Point<float>{1.0, 2.0}), (NAS2D::Point<int>{1, 2}.to<float>()));
}
