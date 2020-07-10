#include "NAS2D/Trig.h"

#include <gmock/gmock.h>
#include <gtest/gtest.h>


TEST(Trig, getAngle) {
	EXPECT_EQ(90.0f, (getAngle(NAS2D::Vector{0, 0})));

	EXPECT_EQ(0.0f, (getAngle(NAS2D::Vector{0, -1})));
	EXPECT_EQ(90.0f, (getAngle(NAS2D::Vector{1, 0})));
	EXPECT_EQ(180.0f, (getAngle(NAS2D::Vector{0, 1})));
	EXPECT_EQ(270.0f, (getAngle(NAS2D::Vector{-1, 0})));

	EXPECT_EQ(135.0f, (getAngle(NAS2D::Vector{1.0, 1.0})));
}
