#include "NAS2D/Trig.h"

#include <gmock/gmock.h>
#include <gtest/gtest.h>


TEST(Trig, degToRadToDeg) {
	EXPECT_FLOAT_EQ(0.0f, NAS2D::degToRad(NAS2D::radToDeg(0.0f)));
	EXPECT_FLOAT_EQ(90.0f, NAS2D::degToRad(NAS2D::radToDeg(90.0f)));
	EXPECT_FLOAT_EQ(180.0f, NAS2D::degToRad(NAS2D::radToDeg(180.0f)));
	EXPECT_FLOAT_EQ(270.0f, NAS2D::degToRad(NAS2D::radToDeg(270.0f)));
	EXPECT_FLOAT_EQ(-90.0f, NAS2D::degToRad(NAS2D::radToDeg(-90.0f)));
}

TEST(Trig, getAngle) {
	EXPECT_EQ(0.0f, NAS2D::radToDeg(NAS2D::getAngle(NAS2D::Vector{0, 0})));

	EXPECT_EQ(0.0f, NAS2D::radToDeg(NAS2D::getAngle(NAS2D::Vector{1, 0})));
	EXPECT_EQ(90.0f, NAS2D::radToDeg(NAS2D::getAngle(NAS2D::Vector{0, 1})));
	EXPECT_EQ(180.0f, NAS2D::radToDeg(NAS2D::getAngle(NAS2D::Vector{-1, 0})));
	EXPECT_EQ(-90.0f, NAS2D::radToDeg(NAS2D::getAngle(NAS2D::Vector{0, -1})));

	EXPECT_EQ(45.0f, NAS2D::radToDeg(NAS2D::getAngle(NAS2D::Vector{1.0, 1.0})));
}

TEST(Trig, getDirectionVector) {
	EXPECT_EQ((NAS2D::Vector{1, 0}), NAS2D::getDirectionVector(NAS2D::degToRad(0.0f)));
	EXPECT_EQ((NAS2D::Vector{0, 1}), NAS2D::getDirectionVector(NAS2D::degToRad(90.0f)));
	EXPECT_EQ((NAS2D::Vector{-1, 0}), NAS2D::getDirectionVector(NAS2D::degToRad(180.0f)));
	EXPECT_EQ((NAS2D::Vector{0, -1}), NAS2D::getDirectionVector(NAS2D::degToRad(270.0f)));
	EXPECT_EQ((NAS2D::Vector{0, -1}), NAS2D::getDirectionVector(NAS2D::degToRad(-90.0f)));
}

TEST(Trig, vectorToAngleToVector) {
	EXPECT_EQ((NAS2D::Vector{1, 0}), NAS2D::getDirectionVector(getAngle(NAS2D::Vector{1, 0})));
	EXPECT_EQ((NAS2D::Vector{0, 1}), NAS2D::getDirectionVector(getAngle(NAS2D::Vector{0, 1})));
	EXPECT_EQ((NAS2D::Vector{-1, 0}), NAS2D::getDirectionVector(getAngle(NAS2D::Vector{-1, 0})));
	EXPECT_EQ((NAS2D::Vector{0, -1}), NAS2D::getDirectionVector(getAngle(NAS2D::Vector{0, -1})));
}
