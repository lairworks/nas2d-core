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
