#include "NAS2D/Duration.h"

#include <gtest/gtest.h>


TEST(Duration, OperatorCompare) {
	EXPECT_EQ(NAS2D::Duration{0}, NAS2D::Duration{0});
	EXPECT_NE(NAS2D::Duration{0}, NAS2D::Duration{1});

	EXPECT_LT(NAS2D::Duration{0}, NAS2D::Duration{1});
	EXPECT_GT(NAS2D::Duration{1}, NAS2D::Duration{0});

	EXPECT_LE(NAS2D::Duration{0}, NAS2D::Duration{0});
	EXPECT_LE(NAS2D::Duration{0}, NAS2D::Duration{1});

	EXPECT_GE(NAS2D::Duration{0}, NAS2D::Duration{0});
	EXPECT_GE(NAS2D::Duration{1}, NAS2D::Duration{0});
}

TEST(Duration, OperatorPlus) {
	EXPECT_EQ(NAS2D::Duration{0}, NAS2D::Duration{0} + NAS2D::Duration{0});
	EXPECT_EQ(NAS2D::Duration{1}, NAS2D::Duration{0} + NAS2D::Duration{1});
	EXPECT_EQ(NAS2D::Duration{1}, NAS2D::Duration{1} + NAS2D::Duration{0});
	EXPECT_EQ(NAS2D::Duration{2}, NAS2D::Duration{1} + NAS2D::Duration{1});
}

TEST(Duration, OperatorMinus) {
	EXPECT_EQ(NAS2D::Duration{0}, NAS2D::Duration{0} - NAS2D::Duration{0});
	EXPECT_EQ(NAS2D::Duration{0}, NAS2D::Duration{1} - NAS2D::Duration{1});
	EXPECT_EQ(NAS2D::Duration{1}, NAS2D::Duration{1} - NAS2D::Duration{0});
	EXPECT_EQ(NAS2D::Duration{1}, NAS2D::Duration{2} - NAS2D::Duration{1});
}

TEST(Duration, OperatorPlusEqual) {
	auto duration1 = NAS2D::Duration{1};
	const auto duration2 = NAS2D::Duration{2};
	EXPECT_EQ(NAS2D::Duration{3}, duration1 += duration2);
	EXPECT_EQ(NAS2D::Duration{3}, duration1);
}

TEST(Duration, OperatorMinusEqual) {
	auto duration1 = NAS2D::Duration{3};
	const auto duration2 = NAS2D::Duration{1};
	EXPECT_EQ(NAS2D::Duration{2}, duration1 -= duration2);
	EXPECT_EQ(NAS2D::Duration{2}, duration1);
}

TEST(Duration, OperatorMultiply) {
	EXPECT_EQ(NAS2D::Duration{0}, NAS2D::Duration{0} * 0);
	EXPECT_EQ(NAS2D::Duration{0}, NAS2D::Duration{1} * 0);
	EXPECT_EQ(NAS2D::Duration{1}, NAS2D::Duration{1} * 1);
	EXPECT_EQ(NAS2D::Duration{4}, NAS2D::Duration{2} * 2);
}

TEST(Duration, OperatorDivide) {
	EXPECT_EQ(NAS2D::Duration{0}, NAS2D::Duration{0} / 1);
	EXPECT_EQ(NAS2D::Duration{1}, NAS2D::Duration{1} / 1);
	EXPECT_EQ(NAS2D::Duration{2}, NAS2D::Duration{2} / 1);
	EXPECT_EQ(NAS2D::Duration{2}, NAS2D::Duration{4} / 2);
}

TEST(Duration, OperatorDivideDuration) {
	EXPECT_EQ(0u, NAS2D::Duration{0} / NAS2D::Duration{1});
	EXPECT_EQ(1u, NAS2D::Duration{1} / NAS2D::Duration{1});
	EXPECT_EQ(2u, NAS2D::Duration{2} / NAS2D::Duration{1});
	EXPECT_EQ(2u, NAS2D::Duration{4} / NAS2D::Duration{2});
}

TEST(Duration, OperatorModulo) {
	EXPECT_EQ(NAS2D::Duration{0}, NAS2D::Duration{0} % 1);
	EXPECT_EQ(NAS2D::Duration{0}, NAS2D::Duration{1} % 1);
	EXPECT_EQ(NAS2D::Duration{0}, NAS2D::Duration{0} % 2);
	EXPECT_EQ(NAS2D::Duration{1}, NAS2D::Duration{1} % 2);
	EXPECT_EQ(NAS2D::Duration{0}, NAS2D::Duration{2} % 2);
}
