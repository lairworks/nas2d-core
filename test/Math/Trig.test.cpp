#include "NAS2D/Math/Trig.h"

#include "NAS2D/Math/Angle.h"

#include <gtest/gtest.h>


TEST(Trig, getAngle) {
	EXPECT_FLOAT_EQ(0.0f, NAS2D::getAngle(NAS2D::Vector<float>{0, 0}).degrees());

	EXPECT_FLOAT_EQ(0.0f, NAS2D::getAngle(NAS2D::Vector<float>{1, 0}).degrees());
	EXPECT_FLOAT_EQ(90.0f, NAS2D::getAngle(NAS2D::Vector<float>{0, 1}).degrees());
	EXPECT_FLOAT_EQ(180.0f, NAS2D::getAngle(NAS2D::Vector<float>{-1, 0}).degrees());
	EXPECT_FLOAT_EQ(-90.0f, NAS2D::getAngle(NAS2D::Vector<float>{0, -1}).degrees());

	EXPECT_FLOAT_EQ(45.0f, NAS2D::getAngle(NAS2D::Vector<float>{1, 1}).degrees());
}

TEST(Trig, getDirectionVector) {
	EXPECT_EQ((NAS2D::Vector{1, 0}), NAS2D::getDirectionVector(NAS2D::Angle::degrees(0.0f)).to<int>());
	EXPECT_EQ((NAS2D::Vector{0, 1}), NAS2D::getDirectionVector(NAS2D::Angle::degrees(90.0f)).to<int>());
	EXPECT_EQ((NAS2D::Vector{-1, 0}), NAS2D::getDirectionVector(NAS2D::Angle::degrees(180.0f)).to<int>());
	EXPECT_EQ((NAS2D::Vector{0, -1}), NAS2D::getDirectionVector(NAS2D::Angle::degrees(270.0f)).to<int>());
	EXPECT_EQ((NAS2D::Vector{0, -1}), NAS2D::getDirectionVector(NAS2D::Angle::degrees(-90.0f)).to<int>());
}

TEST(Trig, vectorToAngleToVector) {
	EXPECT_EQ((NAS2D::Vector{1, 0}), NAS2D::getDirectionVector(getAngle(NAS2D::Vector<float>{1, 0})).to<int>());
	EXPECT_EQ((NAS2D::Vector{0, 1}), NAS2D::getDirectionVector(getAngle(NAS2D::Vector<float>{0, 1})).to<int>());
	EXPECT_EQ((NAS2D::Vector{-1, 0}), NAS2D::getDirectionVector(getAngle(NAS2D::Vector<float>{-1, 0})).to<int>());
	EXPECT_EQ((NAS2D::Vector{0, -1}), NAS2D::getDirectionVector(getAngle(NAS2D::Vector<float>{0, -1})).to<int>());
}
