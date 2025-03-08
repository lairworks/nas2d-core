#include "NAS2D/Math/Point.h"

#include <gtest/gtest.h>


TEST(Point, DefaultConstructibleZeroInit) {
	EXPECT_EQ((NAS2D::Point{0, 0}), NAS2D::Point<int>{});
}

TEST(Point, OperatorEqual) {
	EXPECT_EQ((NAS2D::Point{1, 1}), (NAS2D::Point{1, 1}));
	EXPECT_EQ((NAS2D::Point{2, 2}), (NAS2D::Point{2, 2}));
}

TEST(Point, OperatorNotEqual) {
	EXPECT_NE((NAS2D::Point{1, 1}), (NAS2D::Point{1, 2}));
	EXPECT_NE((NAS2D::Point{1, 1}), (NAS2D::Point{2, 1}));
}

TEST(Point, SelfAddVector) {
	NAS2D::Point point{1, 1};
	EXPECT_EQ(&point, &(point += NAS2D::Vector{1, 2}));
	EXPECT_EQ((NAS2D::Point{2, 3}), point);
}

TEST(Point, SelfSubtractVector) {
	NAS2D::Point point{2, 5};
	EXPECT_EQ(&point, &(point -= NAS2D::Vector{1, 3}));
	EXPECT_EQ((NAS2D::Point{1, 2}), point);
}

TEST(Point, AddVector) {
	EXPECT_EQ((NAS2D::Point{2, 3}), (NAS2D::Point<int>{1, 1}) + (NAS2D::Vector{1, 2}));
}

TEST(Point, SubtractVector) {
	EXPECT_EQ((NAS2D::Point{1, 2}), (NAS2D::Point<int>{2, 5}) - (NAS2D::Vector{1, 3}));
}

TEST(Point, SubtractPointToVector) {
	EXPECT_EQ((NAS2D::Vector{1, 1}), (NAS2D::Point<int>{2, 3}) - (NAS2D::Point{1, 2}));
}

TEST(Point, skewBy) {
	EXPECT_EQ((NAS2D::Point{1, 0}), (NAS2D::Point{1, 1}.skewBy(NAS2D::Vector{1, 0})));
	EXPECT_EQ((NAS2D::Point{2, 3}), (NAS2D::Point{1, 1}.skewBy(NAS2D::Vector{2, 3})));
	EXPECT_EQ((NAS2D::Point{4, 6}), (NAS2D::Point{1, 1}.skewBy(NAS2D::Vector{4, 6})));

	EXPECT_EQ((NAS2D::Point{2, 0}), (NAS2D::Point{2, 1}.skewBy(NAS2D::Vector{1, 0})));
	EXPECT_EQ((NAS2D::Point{4, 3}), (NAS2D::Point{2, 1}.skewBy(NAS2D::Vector{2, 3})));
	EXPECT_EQ((NAS2D::Point{8, 6}), (NAS2D::Point{2, 1}.skewBy(NAS2D::Vector{4, 6})));

	EXPECT_EQ((NAS2D::Point{1, 0}), (NAS2D::Point{1, 2}.skewBy(NAS2D::Vector{1, 0})));
	EXPECT_EQ((NAS2D::Point{2, 6}), (NAS2D::Point{1, 2}.skewBy(NAS2D::Vector{2, 3})));
	EXPECT_EQ((NAS2D::Point{4, 12}), (NAS2D::Point{1, 2}.skewBy(NAS2D::Vector{4, 6})));
}

TEST(Point, skewInverseBy) {
	EXPECT_EQ((NAS2D::Point{8, 4}), (NAS2D::Point{8, 8}.skewInverseBy(NAS2D::Vector{1, 2})));
	EXPECT_EQ((NAS2D::Point{4, 2}), (NAS2D::Point{8, 8}.skewInverseBy(NAS2D::Vector{2, 4})));

	EXPECT_EQ((NAS2D::Point{9, 2}), (NAS2D::Point{9, 6}.skewInverseBy(NAS2D::Vector{1, 3})));
	EXPECT_EQ((NAS2D::Point{3, 3}), (NAS2D::Point{9, 6}.skewInverseBy(NAS2D::Vector{3, 2})));

	EXPECT_EQ((NAS2D::Point{4, 2}), (NAS2D::Point{8, 6}.skewInverseBy(NAS2D::Vector{2, 3})));
	EXPECT_EQ((NAS2D::Point{2, 3}), (NAS2D::Point{8, 6}.skewInverseBy(NAS2D::Vector{4, 2})));
}

TEST(Point, skewInverseByRounded) {
	EXPECT_EQ((NAS2D::Point{2, 1}), (NAS2D::Point{8, 8}.skewInverseBy(NAS2D::Vector{3, 5})));
	EXPECT_EQ((NAS2D::Point{2, 1}), (NAS2D::Point{9, 6}.skewInverseBy(NAS2D::Vector{4, 5})));
	EXPECT_EQ((NAS2D::Point{2, 1}), (NAS2D::Point{8, 6}.skewInverseBy(NAS2D::Vector{3, 5})));
}

TEST(Point, skewInverseByDomainError) {
	EXPECT_THROW((NAS2D::Point{1, 1}.skewInverseBy(NAS2D::Vector{0, 0})), std::domain_error);
	EXPECT_THROW((NAS2D::Point{1, 1}.skewInverseBy(NAS2D::Vector{0, 1})), std::domain_error);
	EXPECT_THROW((NAS2D::Point{1, 1}.skewInverseBy(NAS2D::Vector{1, 0})), std::domain_error);
}

TEST(Point, OperatorType) {
	// Allow explicit conversion
	EXPECT_EQ((NAS2D::Point<int>{1, 2}), NAS2D::Point<int>(NAS2D::Point<float>{1.0, 2.0}));
	EXPECT_EQ((NAS2D::Point<float>{1.0, 2.0}), NAS2D::Point<float>(NAS2D::Point<int>{1, 2}));
}

TEST(Point, to) {
	EXPECT_EQ((NAS2D::Point<int>{1, 2}), (NAS2D::Point<float>{1.0, 2.0}.to<int>()));
	EXPECT_EQ((NAS2D::Point<float>{1.0, 2.0}), (NAS2D::Point<int>{1, 2}.to<float>()));
}

TEST(Point, VectorPointAdd) {
	EXPECT_EQ((NAS2D::Point{2, 3}), (NAS2D::Vector{1, 2} + NAS2D::Point<int>{1, 1}));
}

TEST(Point, PartialOrderLessEqual) {
	EXPECT_LE((NAS2D::Point{0, 0}), (NAS2D::Point{1, 1}));
	EXPECT_LE((NAS2D::Point{0, 1}), (NAS2D::Point{1, 1}));
	EXPECT_LE((NAS2D::Point{1, 0}), (NAS2D::Point{1, 1}));
	EXPECT_LE((NAS2D::Point{1, 1}), (NAS2D::Point{1, 1}));

	EXPECT_FALSE((NAS2D::Point{0, 1}) <= (NAS2D::Point{1, 0}));
	EXPECT_FALSE((NAS2D::Point{1, 0}) <= (NAS2D::Point{0, 1}));
}

TEST(Point, PartialOrderGreaterEqual) {
	EXPECT_GE((NAS2D::Point{1, 1}), (NAS2D::Point{0, 0}));
	EXPECT_GE((NAS2D::Point{1, 1}), (NAS2D::Point{0, 1}));
	EXPECT_GE((NAS2D::Point{1, 1}), (NAS2D::Point{1, 0}));
	EXPECT_GE((NAS2D::Point{1, 1}), (NAS2D::Point{1, 1}));

	EXPECT_FALSE((NAS2D::Point{0, 1}) >= (NAS2D::Point{1, 0}));
	EXPECT_FALSE((NAS2D::Point{1, 0}) >= (NAS2D::Point{0, 1}));
}

TEST(Point, PartialOrderLess) {
	EXPECT_LT((NAS2D::Point{0, 0}), (NAS2D::Point{1, 1}));

	EXPECT_FALSE((NAS2D::Point{0, 1}) < (NAS2D::Point{1, 1}));
	EXPECT_FALSE((NAS2D::Point{1, 0}) < (NAS2D::Point{1, 1}));
	EXPECT_FALSE((NAS2D::Point{1, 1}) < (NAS2D::Point{1, 1}));

	EXPECT_FALSE((NAS2D::Point{0, 1}) < (NAS2D::Point{1, 0}));
	EXPECT_FALSE((NAS2D::Point{1, 0}) < (NAS2D::Point{0, 1}));
}

TEST(Point, PartialOrderGreater) {
	EXPECT_GT((NAS2D::Point{1, 1}), (NAS2D::Point{0, 0}));

	EXPECT_FALSE((NAS2D::Point{1, 1}) > (NAS2D::Point{0, 1}));
	EXPECT_FALSE((NAS2D::Point{1, 1}) > (NAS2D::Point{1, 0}));
	EXPECT_FALSE((NAS2D::Point{1, 1}) > (NAS2D::Point{1, 1}));

	EXPECT_FALSE((NAS2D::Point{0, 1}) > (NAS2D::Point{1, 0}));
	EXPECT_FALSE((NAS2D::Point{1, 0}) > (NAS2D::Point{0, 1}));
}
