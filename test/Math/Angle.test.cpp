#include "NAS2D/Math/Angle.h"

#include <gtest/gtest.h>

#include <numbers>


TEST(Angle, degrees) {
	constexpr auto angle = NAS2D::Angle::degrees(90.0f);
	EXPECT_FLOAT_EQ(90.0f, angle.degrees());
}

TEST(Angle, radians) {
	constexpr auto angle = NAS2D::Angle::radians(1.0f);
	EXPECT_FLOAT_EQ(1.0f, angle.radians());
}

TEST(Angle, degreesToRadians) {
	constexpr auto angle = NAS2D::Angle::degrees(180.0f);
	EXPECT_FLOAT_EQ(std::numbers::pi_v<float>, angle.radians());
}

TEST(Angle, unaryMinus) {
	constexpr auto angle = -NAS2D::Angle::degrees(180.0f);
	EXPECT_FLOAT_EQ(-180.0f, angle.degrees());
}

TEST(Angle, addition) {
	constexpr auto angle = NAS2D::Angle::degrees(90.0f) + NAS2D::Angle::degrees(90.0f);
	EXPECT_FLOAT_EQ(180.0f, angle.degrees());
}

TEST(Angle, subtraction) {
	constexpr auto angle = NAS2D::Angle::degrees(180.0f) - NAS2D::Angle::degrees(90.0f);
	EXPECT_FLOAT_EQ(90.0f, angle.degrees());
}
