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

TEST(MathUtils, isPointInRectComponent)
{
	//Neghbors
	EXPECT_EQ(false, NAS2D::isPointInRect(4, 4, 5, 5, 1, 1));
	EXPECT_EQ(false, NAS2D::isPointInRect(5, 4, 5, 5, 1, 1));
	EXPECT_EQ(false, NAS2D::isPointInRect(6, 4, 5, 5, 1, 1));
	EXPECT_EQ(false, NAS2D::isPointInRect(7, 4, 5, 5, 1, 1));
	EXPECT_EQ(false, NAS2D::isPointInRect(7, 5, 5, 5, 1, 1));
	EXPECT_EQ(false, NAS2D::isPointInRect(7, 6, 5, 5, 1, 1));
	EXPECT_EQ(false, NAS2D::isPointInRect(7, 7, 5, 5, 1, 1));
	EXPECT_EQ(false, NAS2D::isPointInRect(6, 7, 5, 5, 1, 1));
	EXPECT_EQ(false, NAS2D::isPointInRect(5, 7, 5, 5, 1, 1));
	EXPECT_EQ(false, NAS2D::isPointInRect(4, 7, 5, 5, 1, 1));
	EXPECT_EQ(false, NAS2D::isPointInRect(4, 6, 5, 5, 1, 1));
	EXPECT_EQ(false, NAS2D::isPointInRect(4, 5, 5, 5, 1, 1));
	EXPECT_EQ(false, NAS2D::isPointInRect(4, 4, 5, 5, 1, 1));

	//Top
	EXPECT_EQ(false, NAS2D::isPointInRect(4, 5, 5, 5, 1, 1));
	EXPECT_EQ(true, NAS2D::isPointInRect(5, 5, 5, 5, 1, 1));
	EXPECT_EQ(true, NAS2D::isPointInRect(6, 5, 5, 5, 1, 1));
	EXPECT_EQ(false, NAS2D::isPointInRect(7, 5, 5, 5, 1, 1));

	//Bottom
	EXPECT_EQ(false, NAS2D::isPointInRect(4, 6, 5, 5, 1, 1));
	EXPECT_EQ(true, NAS2D::isPointInRect(5, 6, 5, 5, 1, 1));
	EXPECT_EQ(true, NAS2D::isPointInRect(6, 6, 5, 5, 1, 1));
	EXPECT_EQ(false, NAS2D::isPointInRect(7, 6, 5, 5, 1, 1));

	//Left
	EXPECT_EQ(false, NAS2D::isPointInRect(5, 4, 5, 5, 1, 1));
	EXPECT_EQ(true, NAS2D::isPointInRect(5, 5, 5, 5, 1, 1));
	EXPECT_EQ(true, NAS2D::isPointInRect(5, 6, 5, 5, 1, 1));
	EXPECT_EQ(false, NAS2D::isPointInRect(5, 7, 5, 5, 1, 1));

	//Right
	EXPECT_EQ(false, NAS2D::isPointInRect(6, 4, 5, 5, 1, 1));
	EXPECT_EQ(true, NAS2D::isPointInRect(6, 5, 5, 5, 1, 1));
	EXPECT_EQ(true, NAS2D::isPointInRect(6, 6, 5, 5, 1, 1));
	EXPECT_EQ(false, NAS2D::isPointInRect(6, 7, 5, 5, 1, 1));

	//Center
	EXPECT_EQ(false, NAS2D::isPointInRect(4, 6, 5, 5, 2, 2));
	EXPECT_EQ(true, NAS2D::isPointInRect(5, 6, 5, 5, 2, 2));
	EXPECT_EQ(true, NAS2D::isPointInRect(6, 6, 5, 5, 2, 2));
	EXPECT_EQ(true, NAS2D::isPointInRect(7, 6, 5, 5, 2, 2));
	EXPECT_EQ(false, NAS2D::isPointInRect(8, 6, 5, 5, 2, 2));
}

TEST(MathUtils, isPointInRectClass)
{
	using namespace NAS2D;
	//Neghbors
	EXPECT_EQ(false, NAS2D::isPointInRect({4, 4}, {5, 5, 1, 1}));
	EXPECT_EQ(false, NAS2D::isPointInRect({5, 4}, {5, 5, 1, 1}));
	EXPECT_EQ(false, NAS2D::isPointInRect({6, 4}, {5, 5, 1, 1}));
	EXPECT_EQ(false, NAS2D::isPointInRect({7, 4}, {5, 5, 1, 1}));
	EXPECT_EQ(false, NAS2D::isPointInRect({7, 5}, {5, 5, 1, 1}));
	EXPECT_EQ(false, NAS2D::isPointInRect({7, 6}, {5, 5, 1, 1}));
	EXPECT_EQ(false, NAS2D::isPointInRect({7, 7}, {5, 5, 1, 1}));
	EXPECT_EQ(false, NAS2D::isPointInRect({6, 7}, {5, 5, 1, 1}));
	EXPECT_EQ(false, NAS2D::isPointInRect({5, 7}, {5, 5, 1, 1}));
	EXPECT_EQ(false, NAS2D::isPointInRect({4, 7}, {5, 5, 1, 1}));
	EXPECT_EQ(false, NAS2D::isPointInRect({4, 6}, {5, 5, 1, 1}));
	EXPECT_EQ(false, NAS2D::isPointInRect({4, 5}, {5, 5, 1, 1}));
	EXPECT_EQ(false, NAS2D::isPointInRect({4, 4}, {5, 5, 1, 1}));

	//Top
	EXPECT_EQ(false, NAS2D::isPointInRect({4, 5}, {5, 5, 1, 1}));
	EXPECT_EQ(true, NAS2D::isPointInRect({5, 5}, {5, 5, 1, 1}));
	EXPECT_EQ(true, NAS2D::isPointInRect({6, 5}, {5, 5, 1, 1}));
	EXPECT_EQ(false, NAS2D::isPointInRect({7, 5}, {5, 5, 1, 1}));

	//Bottom
	EXPECT_EQ(false, NAS2D::isPointInRect({4, 6}, {5, 5, 1, 1}));
	EXPECT_EQ(true, NAS2D::isPointInRect({5, 6}, {5, 5, 1, 1}));
	EXPECT_EQ(true, NAS2D::isPointInRect({6, 6}, {5, 5, 1, 1}));
	EXPECT_EQ(false, NAS2D::isPointInRect({7, 6}, {5, 5, 1, 1}));

	//Left
	EXPECT_EQ(false, NAS2D::isPointInRect({5, 4}, {5, 5, 1, 1}));
	EXPECT_EQ(true, NAS2D::isPointInRect({5, 5}, {5, 5, 1, 1}));
	EXPECT_EQ(true, NAS2D::isPointInRect({5, 6}, {5, 5, 1, 1}));
	EXPECT_EQ(false, NAS2D::isPointInRect({5, 7}, {5, 5, 1, 1}));

	//Right
	EXPECT_EQ(false, NAS2D::isPointInRect({6, 4}, {5, 5, 1, 1}));
	EXPECT_EQ(true, NAS2D::isPointInRect({6, 5}, {5, 5, 1, 1}));
	EXPECT_EQ(true, NAS2D::isPointInRect({6, 6}, {5, 5, 1, 1}));
	EXPECT_EQ(false, NAS2D::isPointInRect({6, 7}, {5, 5, 1, 1}));

	//Center
	EXPECT_EQ(false, NAS2D::isPointInRect({4, 6}, {5, 5, 2, 2}));
	EXPECT_EQ(true, NAS2D::isPointInRect({5, 6}, {5, 5, 2, 2}));
	EXPECT_EQ(true, NAS2D::isPointInRect({6, 6}, {5, 5, 2, 2}));
	EXPECT_EQ(true, NAS2D::isPointInRect({7, 6}, {5, 5, 2, 2}));
	EXPECT_EQ(false, NAS2D::isPointInRect({8, 6}, {5, 5, 2, 2}));
}

TEST(MathUtils, isRectInRectComponent)
{
	// Coincident
	EXPECT_EQ(true, NAS2D::isRectInRect(5, 5, 6, 6, 5, 5, 6, 6));
	// Contained all sides, both directions
	EXPECT_EQ(true, NAS2D::isRectInRect(5, 5, 6, 6, 4, 4, 7, 7));
	EXPECT_EQ(true, NAS2D::isRectInRect(4, 4, 7, 7, 5, 5, 6, 6));

	// Contained 1 size coincident
	EXPECT_EQ(true, NAS2D::isRectInRect(5, 5, 6, 6, 4, 4, 7, 6));
	EXPECT_EQ(true, NAS2D::isRectInRect(5, 5, 6, 6, 4, 4, 6, 7));
	EXPECT_EQ(true, NAS2D::isRectInRect(5, 5, 6, 6, 4, 5, 7, 7));
	EXPECT_EQ(true, NAS2D::isRectInRect(5, 5, 6, 6, 5, 4, 7, 7));

	// Touching at point (top left, top right, bottom left, bottom right)
	EXPECT_EQ(true, NAS2D::isRectInRect(5, 5, 6, 6, 4, 6, 5, 7));
	EXPECT_EQ(true, NAS2D::isRectInRect(5, 5, 6, 6, 6, 6, 7, 7));
	EXPECT_EQ(true, NAS2D::isRectInRect(5, 5, 6, 6, 4, 4, 5, 5));
	EXPECT_EQ(true, NAS2D::isRectInRect(5, 5, 6, 6, 6, 4, 7, 5));

	// Touching along edge (top, right, bottom, left)
	EXPECT_EQ(true, NAS2D::isRectInRect(5, 5, 6, 6, 5, 6, 6, 7));
	EXPECT_EQ(true, NAS2D::isRectInRect(5, 5, 6, 6, 6, 5, 7, 6));
	EXPECT_EQ(true, NAS2D::isRectInRect(5, 5, 6, 6, 5, 4, 6, 5));
	EXPECT_EQ(true, NAS2D::isRectInRect(5, 5, 6, 6, 4, 5, 5, 6));

	// Disjoint x
	EXPECT_EQ(false, NAS2D::isRectInRect(5, 5, 6, 6, 7, 5, 8, 6));
	EXPECT_EQ(false, NAS2D::isRectInRect(5, 5, 6, 6, 3, 5, 4, 6));
	// Disjoint y
	EXPECT_EQ(false, NAS2D::isRectInRect(5, 5, 6, 6, 5, 3, 6, 4));
	EXPECT_EQ(false, NAS2D::isRectInRect(5, 5, 6, 6, 5, 7, 6, 8));

	// Disjoint x/y
	EXPECT_EQ(false, NAS2D::isRectInRect(5, 5, 6, 6, 3, 7, 4, 8));
	EXPECT_EQ(false, NAS2D::isRectInRect(5, 5, 6, 6, 7, 7, 8, 8));
	EXPECT_EQ(false, NAS2D::isRectInRect(5, 5, 6, 6, 3, 3, 4, 4));
	EXPECT_EQ(false, NAS2D::isRectInRect(5, 5, 6, 6, 7, 3, 8, 4));
}

TEST(MathUtils, isRectInRectClass)
{
	using namespace NAS2D;

	// Coincident
	EXPECT_EQ(true, NAS2D::isRectInRect({5, 5, 1, 1}, {5, 5, 1, 1}));
	// Contained all sides, both directions
	EXPECT_EQ(true, NAS2D::isRectInRect({5, 5, 1, 1}, {4, 4, 3, 3}));
	EXPECT_EQ(true, NAS2D::isRectInRect({4, 4, 3, 3}, {5, 5, 1, 1}));

	// Contained 1 size coincident
	EXPECT_EQ(true, NAS2D::isRectInRect({5, 5, 1, 1}, {4, 4, 3, 2}));
	EXPECT_EQ(true, NAS2D::isRectInRect({5, 5, 1, 1}, {4, 4, 2, 3}));
	EXPECT_EQ(true, NAS2D::isRectInRect({5, 5, 1, 1}, {4, 5, 3, 2}));
	EXPECT_EQ(true, NAS2D::isRectInRect({5, 5, 1, 1}, {5, 4, 2, 3}));

	// Touching at point (top left, top right, bottom left, bottom right)
	EXPECT_EQ(true, NAS2D::isRectInRect({5, 5, 1, 1}, {4, 6, 1, 1}));
	EXPECT_EQ(true, NAS2D::isRectInRect({5, 5, 1, 1}, {6, 6, 1, 1}));
	EXPECT_EQ(true, NAS2D::isRectInRect({5, 5, 1, 1}, {4, 4, 1, 1}));
	EXPECT_EQ(true, NAS2D::isRectInRect({5, 5, 1, 1}, {6, 4, 1, 1}));

	// Touching along edge (top, right, bottom, left)
	EXPECT_EQ(true, NAS2D::isRectInRect({5, 5, 1, 1}, {5, 6, 1, 1}));
	EXPECT_EQ(true, NAS2D::isRectInRect({5, 5, 1, 1}, {6, 5, 1, 1}));
	EXPECT_EQ(true, NAS2D::isRectInRect({5, 5, 1, 1}, {5, 4, 1, 1}));
	EXPECT_EQ(true, NAS2D::isRectInRect({5, 5, 1, 1}, {4, 5, 1, 1}));

	// Disjoint x
	EXPECT_EQ(false, NAS2D::isRectInRect({5, 5, 1, 1}, {7, 5, 1, 1}));
	EXPECT_EQ(false, NAS2D::isRectInRect({5, 5, 1, 1}, {3, 5, 1, 1}));
	// Disjoint y
	EXPECT_EQ(false, NAS2D::isRectInRect({5, 5, 1, 1}, {5, 3, 1, 1}));
	EXPECT_EQ(false, NAS2D::isRectInRect({5, 5, 1, 1}, {5, 7, 1, 1}));

	// Disjoint x/y
	EXPECT_EQ(false, NAS2D::isRectInRect({5, 5, 1, 1}, {3, 7, 1, 1}));
	EXPECT_EQ(false, NAS2D::isRectInRect({5, 5, 1, 1}, {7, 7, 1, 1}));
	EXPECT_EQ(false, NAS2D::isRectInRect({5, 5, 1, 1}, {3, 3, 1, 1}));
	EXPECT_EQ(false, NAS2D::isRectInRect({5, 5, 1, 1}, {7, 3, 1, 1}));
}

TEST(MathUtils, mapDomainToRange)
{
	// Fahrenheit to Celcius
	EXPECT_NEAR(-18.33333f, NAS2D::convertDomainToRange(-1.0f, 32.0f, 212.0f, 0.0f, 100.0f), 0.0001f);
	EXPECT_NEAR(0.0f, NAS2D::convertDomainToRange(32.0f, 32.0f, 212.0f, 0.0f, 100.0f), 0.0001f);
	EXPECT_NEAR(-17.77777f, NAS2D::convertDomainToRange(0.0f, 32.0f, 212.0f, 0.0f, 100.0f), 0.0001f);
	EXPECT_NEAR(-17.22222f, NAS2D::convertDomainToRange(1.0f, 32.0f, 212.0f, 0.0f, 100.0f), 0.0001f);
	EXPECT_NEAR(100.0f, NAS2D::convertDomainToRange(212.0f, 32.0f, 212.0f, 0.0f, 100.0f), 0.0001f);

	// Celcius to Fahrenheit
	EXPECT_NEAR(30.2f, NAS2D::convertDomainToRange(-1.0f, 0.0f, 100.0f, 32.0f, 212.0f), 0.0001f);
	EXPECT_NEAR(32.0f, NAS2D::convertDomainToRange(0.0f, 0.0f, 100.0f, 32.0f, 212.0f), 0.0001f);
	EXPECT_NEAR(33.8f, NAS2D::convertDomainToRange(1.0f, 0.0f, 100.0f, 32.0f, 212.0f), 0.0001f);
	EXPECT_NEAR(212.0f, NAS2D::convertDomainToRange(100.0f, 0.0f, 100.0f, 32.0f, 212.0f), 0.0001f);

	// unsigned char to normalized float
	EXPECT_NEAR(0.0f, NAS2D::convertDomainToRange(0.0f, 0.0f, 255.0f, 0.0f, 1.0f), 0.01f);
	EXPECT_NEAR(0.5f, NAS2D::convertDomainToRange(128.0f, 0.0f, 255.0f, 0.0f, 1.0f), 0.01f);
	EXPECT_NEAR(1.0f, NAS2D::convertDomainToRange(255.0f, 0.0f, 255.0f, 0.0f, 1.0f), 0.01f);

	EXPECT_NEAR(-1.0f, NAS2D::convertDomainToRange(0.0f, 0.0f, 255.0f, -1.0f, 1.0f), 0.01f);
	EXPECT_NEAR(0.0f, NAS2D::convertDomainToRange(128.0f, 0.0f, 255.0f, -1.0f, 1.0f), 0.01f);
	EXPECT_NEAR(1.0f, NAS2D::convertDomainToRange(255.0f, 0.0f, 255.0f, -1.0f, 1.0f), 0.01f);

}
