#include "NAS2D/Math/MathUtils.h"

#include "NAS2D/Math/Point.h"

#include <gtest/gtest.h>


TEST(MathUtils, lineIntersectsCircle) {
	const auto circleCenter = NAS2D::Point{0, 0};

	// Interior
	EXPECT_TRUE((lineIntersectsCircle(NAS2D::Point{-1, 0}, NAS2D::Point{1, 0}, circleCenter, 1)));
	EXPECT_TRUE((lineIntersectsCircle(NAS2D::Point{0, -1}, NAS2D::Point{0, 1}, circleCenter, 1)));
	EXPECT_TRUE((lineIntersectsCircle(NAS2D::Point{-1, -1}, NAS2D::Point{1, 1}, circleCenter, 1)));
	EXPECT_TRUE((lineIntersectsCircle(NAS2D::Point{-1, 1}, NAS2D::Point{1, -1}, circleCenter, 1)));
	EXPECT_TRUE((lineIntersectsCircle(NAS2D::Point{-1, -1}, NAS2D::Point{1, 2}, circleCenter, 1)));
	EXPECT_TRUE((lineIntersectsCircle(NAS2D::Point{-1, -1}, NAS2D::Point{2, 1}, circleCenter, 1)));

	// Boundary
	EXPECT_FALSE((lineIntersectsCircle(NAS2D::Point{-1, -1}, NAS2D::Point{1, -1}, circleCenter, 1)));
	EXPECT_FALSE((lineIntersectsCircle(NAS2D::Point{-1, 1}, NAS2D::Point{1, 1}, circleCenter, 1)));
	EXPECT_FALSE((lineIntersectsCircle(NAS2D::Point{-1, -1}, NAS2D::Point{-1, 1}, circleCenter, 1)));
	EXPECT_FALSE((lineIntersectsCircle(NAS2D::Point{1, -1}, NAS2D::Point{1, 1}, circleCenter, 1)));

	// Exterior
	EXPECT_FALSE((lineIntersectsCircle(NAS2D::Point{-1, -2}, NAS2D::Point{1, -2}, circleCenter, 1)));
	EXPECT_FALSE((lineIntersectsCircle(NAS2D::Point{-1, 2}, NAS2D::Point{1, 2}, circleCenter, 1)));
	EXPECT_FALSE((lineIntersectsCircle(NAS2D::Point{-2, -1}, NAS2D::Point{-2, 1}, circleCenter, 1)));
	EXPECT_FALSE((lineIntersectsCircle(NAS2D::Point{2, -1}, NAS2D::Point{2, 1}, circleCenter, 1)));
}

TEST(MathUtils, divideUpByZero) {
	EXPECT_THROW(NAS2D::divideUp(0, 0), std::domain_error);
	EXPECT_THROW(NAS2D::divideUp(1, 0), std::domain_error);
	EXPECT_THROW(NAS2D::divideUp(2, 0), std::domain_error);
	EXPECT_THROW(NAS2D::divideUp(256, 0), std::domain_error);
}

TEST(MathUtils, divideUp) {
	EXPECT_EQ(0, NAS2D::divideUp(0, 1));
	EXPECT_EQ(0, NAS2D::divideUp(0, 2));
	EXPECT_EQ(0, NAS2D::divideUp(0, 3));
	EXPECT_EQ(1, NAS2D::divideUp(1, 1));
	EXPECT_EQ(1, NAS2D::divideUp(1, 2));
	EXPECT_EQ(1, NAS2D::divideUp(1, 3));
	EXPECT_EQ(2, NAS2D::divideUp(2, 1));
	EXPECT_EQ(1, NAS2D::divideUp(2, 2));
	EXPECT_EQ(1, NAS2D::divideUp(2, 3));
	EXPECT_EQ(1, NAS2D::divideUp(5, 8));
	EXPECT_EQ(256, NAS2D::divideUp(256, 1));
	EXPECT_EQ(128, NAS2D::divideUp(256, 2));
	EXPECT_EQ(86, NAS2D::divideUp(256, 3));
	EXPECT_EQ(64, NAS2D::divideUp(256, 4));
	EXPECT_EQ(52, NAS2D::divideUp(256, 5));
	EXPECT_EQ(43, NAS2D::divideUp(256, 6));
	EXPECT_EQ(37, NAS2D::divideUp(256, 7));
	EXPECT_EQ(32, NAS2D::divideUp(256, 8));
	EXPECT_EQ(29, NAS2D::divideUp(256, 9));
	EXPECT_EQ(26, NAS2D::divideUp(256, 10));
	EXPECT_EQ(24, NAS2D::divideUp(256, 11));
	EXPECT_EQ(22, NAS2D::divideUp(256, 12));
	EXPECT_EQ(20, NAS2D::divideUp(256, 13));
}

TEST(MathUtils, roundUpPowerOf2) {
	EXPECT_EQ(1u, NAS2D::roundUpPowerOf2(1));
	EXPECT_EQ(2u, NAS2D::roundUpPowerOf2(2));
	EXPECT_EQ(4u, NAS2D::roundUpPowerOf2(3));
	EXPECT_EQ(4u, NAS2D::roundUpPowerOf2(4));
	EXPECT_EQ(8u, NAS2D::roundUpPowerOf2(5));
	EXPECT_EQ(8u, NAS2D::roundUpPowerOf2(8));
	EXPECT_EQ(16u, NAS2D::roundUpPowerOf2(9));
	EXPECT_EQ(16u, NAS2D::roundUpPowerOf2(16));
	EXPECT_EQ(32u, NAS2D::roundUpPowerOf2(17));
	EXPECT_EQ(32u, NAS2D::roundUpPowerOf2(32));
	EXPECT_EQ(64u, NAS2D::roundUpPowerOf2(33));
	EXPECT_EQ(64u, NAS2D::roundUpPowerOf2(64));
	EXPECT_EQ(128u, NAS2D::roundUpPowerOf2(65));
	EXPECT_EQ(128u, NAS2D::roundUpPowerOf2(128));
	EXPECT_EQ(256u, NAS2D::roundUpPowerOf2(129));
	EXPECT_EQ(256u, NAS2D::roundUpPowerOf2(256));
	EXPECT_EQ(512u, NAS2D::roundUpPowerOf2(257));
	EXPECT_EQ(512u, NAS2D::roundUpPowerOf2(512));
	EXPECT_EQ(1024u, NAS2D::roundUpPowerOf2(513));
	EXPECT_EQ(1024u, NAS2D::roundUpPowerOf2(1024));
	EXPECT_EQ(2048u, NAS2D::roundUpPowerOf2(1025));
	EXPECT_EQ(2048u, NAS2D::roundUpPowerOf2(2048));
	EXPECT_EQ(4096u, NAS2D::roundUpPowerOf2(2049));
	EXPECT_EQ(4096u, NAS2D::roundUpPowerOf2(4096));

	EXPECT_EQ(32768u, NAS2D::roundUpPowerOf2(32768));
	EXPECT_EQ(65536u, NAS2D::roundUpPowerOf2(32769));
	EXPECT_EQ(65536u, NAS2D::roundUpPowerOf2(65536));

	EXPECT_EQ(8388608u, NAS2D::roundUpPowerOf2(8388608));
	EXPECT_EQ(16777216u, NAS2D::roundUpPowerOf2(8388609));
	EXPECT_EQ(16777216u, NAS2D::roundUpPowerOf2(16777216));

	// Largest value before 32-bit overflow of result
	EXPECT_EQ(2147483648u, NAS2D::roundUpPowerOf2(2147483648));
}

TEST(MathUtils, scaleLinearForwardUint8toUint8) {
	EXPECT_EQ(uint8_t{0}, NAS2D::scaleLinear(uint8_t{0}, uint8_t{0}, uint8_t{255}, uint8_t{0}, uint8_t{255}));
	EXPECT_EQ(uint8_t{64}, NAS2D::scaleLinear(uint8_t{64}, uint8_t{0}, uint8_t{255}, uint8_t{0}, uint8_t{255}));
	EXPECT_EQ(uint8_t{128}, NAS2D::scaleLinear(uint8_t{128}, uint8_t{0}, uint8_t{255}, uint8_t{0}, uint8_t{255}));
	EXPECT_EQ(uint8_t{192}, NAS2D::scaleLinear(uint8_t{192}, uint8_t{0}, uint8_t{255}, uint8_t{0}, uint8_t{255}));
	EXPECT_EQ(uint8_t{255}, NAS2D::scaleLinear(uint8_t{255}, uint8_t{0}, uint8_t{255}, uint8_t{0}, uint8_t{255}));
}

TEST(MathUtils, scaleLinearReverseUint8toUint8) {
	EXPECT_EQ(uint8_t{255}, NAS2D::scaleLinear(uint8_t{0}, uint8_t{0}, uint8_t{255}, uint8_t{255}, uint8_t{0}));
	EXPECT_EQ(uint8_t{191}, NAS2D::scaleLinear(uint8_t{64}, uint8_t{0}, uint8_t{255}, uint8_t{255}, uint8_t{0}));
	EXPECT_EQ(uint8_t{127}, NAS2D::scaleLinear(uint8_t{128}, uint8_t{0}, uint8_t{255}, uint8_t{255}, uint8_t{0}));
	EXPECT_EQ(uint8_t{63}, NAS2D::scaleLinear(uint8_t{192}, uint8_t{0}, uint8_t{255}, uint8_t{255}, uint8_t{0}));
	EXPECT_EQ(uint8_t{0}, NAS2D::scaleLinear(uint8_t{255}, uint8_t{0}, uint8_t{255}, uint8_t{255}, uint8_t{0}));
}

TEST(MathUtils, scaleLinearFloatFahrenheitToCelsius) {
	// Fahrenheit to Celsius
	EXPECT_NEAR(-18.33333f, NAS2D::scaleLinear(-1.0f, 32.0f, 212.0f, 0.0f, 100.0f), 0.0001f);
	EXPECT_NEAR(-18.33333f, NAS2D::scaleLinear(-1.0f, 212.0f, 32.0f, 100.0f, 0.0f), 0.0001f);

	EXPECT_NEAR(0.0f, NAS2D::scaleLinear(32.0f, 32.0f, 212.0f, 0.0f, 100.0f), 0.0001f);
	EXPECT_NEAR(-17.77777f, NAS2D::scaleLinear(0.0f, 32.0f, 212.0f, 0.0f, 100.0f), 0.0001f);
	EXPECT_NEAR(-17.22222f, NAS2D::scaleLinear(1.0f, 32.0f, 212.0f, 0.0f, 100.0f), 0.0001f);
	EXPECT_NEAR(100.0f, NAS2D::scaleLinear(212.0f, 32.0f, 212.0f, 0.0f, 100.0f), 0.0001f);
}

TEST(MathUtils, scaleLinearFloatCelsiusToFahrenheit) {
	// Celsius to Fahrenheit
	EXPECT_NEAR(30.2f, NAS2D::scaleLinear(-1.0f, 0.0f, 100.0f, 32.0f, 212.0f), 0.0001f);
	EXPECT_NEAR(32.0f, NAS2D::scaleLinear(0.0f, 0.0f, 100.0f, 32.0f, 212.0f), 0.0001f);
	EXPECT_NEAR(33.8f, NAS2D::scaleLinear(1.0f, 0.0f, 100.0f, 32.0f, 212.0f), 0.0001f);
	EXPECT_NEAR(212.0f, NAS2D::scaleLinear(100.0f, 0.0f, 100.0f, 32.0f, 212.0f), 0.0001f);
}

TEST(MathUtils, scaleLinearFloatUnitInterval) {
	// unsigned char to normalized float
	EXPECT_NEAR(0.0f, (NAS2D::scaleLinear<unsigned char, float>(0, 0, 255, 0.0f, 1.0f)), 0.01f);
	EXPECT_NEAR(0.5f, (NAS2D::scaleLinear<unsigned char, float>(128, 0, 255, 0.0f, 1.0f)), 0.01f);
	EXPECT_NEAR(1.0f, (NAS2D::scaleLinear<unsigned char, float>(255, 0, 255, 0.0f, 1.0f)), 0.01f);
}

TEST(MathUtils, scaleLinearFloatSymmetricRange) {
	EXPECT_NEAR(-1.0f, (NAS2D::scaleLinear<unsigned char, float>(0, 0, 255, -1.0f, 1.0f)), 0.01f);
	EXPECT_NEAR(0.0f, (NAS2D::scaleLinear<unsigned char, float>(128, 0, 255, -1.0f, 1.0f)), 0.01f);
	EXPECT_NEAR(1.0f, (NAS2D::scaleLinear<unsigned char, float>(255, 0, 255, -1.0f, 1.0f)), 0.01f);
}
