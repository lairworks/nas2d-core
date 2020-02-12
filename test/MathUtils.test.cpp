#include "NAS2D/MathUtils.h"

#include <gmock/gmock.h>
#include <gtest/gtest.h>

TEST(MathUtils, divideUpByZero)
{
	EXPECT_THROW(NAS2D::divideUp(0, 0), std::domain_error);
	EXPECT_THROW(NAS2D::divideUp(1, 0), std::domain_error);
	EXPECT_THROW(NAS2D::divideUp(2, 0), std::domain_error);
	EXPECT_THROW(NAS2D::divideUp(256, 0), std::domain_error);
}

TEST(MathUtils, divideUp)
{
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

TEST(MathUtils, mapDomainToRange)
{
	// Fahrenheit to Celcius
	EXPECT_NEAR(-18.33333f, NAS2D::scaleLinear(-1.0f, 32.0f, 212.0f, 0.0f, 100.0f), 0.0001f);
	EXPECT_NEAR(-18.33333f, NAS2D::scaleLinear(-1.0f, 212.0f, 32.0f, 100.0f, 0.0f), 0.0001f);

	EXPECT_NEAR(0.0f, NAS2D::scaleLinear(32.0f, 32.0f, 212.0f, 0.0f, 100.0f), 0.0001f);
	EXPECT_NEAR(-17.77777f, NAS2D::scaleLinear(0.0f, 32.0f, 212.0f, 0.0f, 100.0f), 0.0001f);
	EXPECT_NEAR(-17.22222f, NAS2D::scaleLinear(1.0f, 32.0f, 212.0f, 0.0f, 100.0f), 0.0001f);
	EXPECT_NEAR(100.0f, NAS2D::scaleLinear(212.0f, 32.0f, 212.0f, 0.0f, 100.0f), 0.0001f);

	// Celcius to Fahrenheit
	EXPECT_NEAR(30.2f, NAS2D::scaleLinear(-1.0f, 0.0f, 100.0f, 32.0f, 212.0f), 0.0001f);
	EXPECT_NEAR(32.0f, NAS2D::scaleLinear(0.0f, 0.0f, 100.0f, 32.0f, 212.0f), 0.0001f);
	EXPECT_NEAR(33.8f, NAS2D::scaleLinear(1.0f, 0.0f, 100.0f, 32.0f, 212.0f), 0.0001f);
	EXPECT_NEAR(212.0f, NAS2D::scaleLinear(100.0f, 0.0f, 100.0f, 32.0f, 212.0f), 0.0001f);

	// unsigned char to normalized float
	EXPECT_NEAR(0.0f, (NAS2D::scaleLinear<unsigned char, float>(0, 0, 255, 0.0f, 1.0f)), 0.01f);
	EXPECT_NEAR(0.5f, (NAS2D::scaleLinear<unsigned char, float>(128, 0, 255, 0.0f, 1.0f)), 0.01f);
	EXPECT_NEAR(1.0f, (NAS2D::scaleLinear<unsigned char, float>(255, 0, 255, 0.0f, 1.0f)), 0.01f);

	EXPECT_NEAR(-1.0f, (NAS2D::scaleLinear<unsigned char, float>(0, 0, 255, -1.0f, 1.0f)), 0.01f);
	EXPECT_NEAR(0.0f, (NAS2D::scaleLinear<unsigned char, float>(128, 0, 255, -1.0f, 1.0f)), 0.01f);
	EXPECT_NEAR(1.0f, (NAS2D::scaleLinear<unsigned char, float>(255, 0, 255, -1.0f, 1.0f)), 0.01f);

}
