#include "NAS2D/Math/PointInRectangleRange.h"

#include <gtest/gtest.h>

#include <vector>


TEST(PointInRectangleRange, Iteration) {
	using Items = std::vector<NAS2D::Point<int>>;

	const auto pointRange1 = NAS2D::PointInRectangleRange{NAS2D::Rectangle<int>{{0, 0}, {0, 0}}};
	const auto pointRange2 = NAS2D::PointInRectangleRange{NAS2D::Rectangle<int>{{1, 1}, {1, 1}}};
	const auto pointRange3 = NAS2D::PointInRectangleRange{NAS2D::Rectangle<int>{{4, 5}, {2, 3}}};

	// Dereference produces the expected starting value
	// (Later tests also proves the range is restartable)
	EXPECT_EQ((NAS2D::Point{0, 0}), *pointRange1.begin());
	EXPECT_EQ((NAS2D::Point{1, 1}), *pointRange2.begin());
	EXPECT_EQ((NAS2D::Point{4, 5}), *pointRange3.begin());

	// Test head of range for expected values
	EXPECT_EQ((NAS2D::Point{4, 5}), *pointRange3.begin());
	EXPECT_EQ((NAS2D::Point{5, 5}), *++pointRange3.begin());
	EXPECT_EQ((NAS2D::Point{4, 6}), *++++pointRange3.begin());
	EXPECT_EQ((NAS2D::Point{5, 6}), *++++++pointRange3.begin());

	// Range-for syntax is supported
	const auto fillByRangeFor = [](auto pointRange) {
		Items collection;
		for (const auto point : pointRange) {
			collection.push_back(point);
		}
		return collection;
	};
	EXPECT_EQ(fillByRangeFor(pointRange1), (Items{}));
	EXPECT_EQ(fillByRangeFor(pointRange2), (Items{{1, 1}}));
	EXPECT_EQ(fillByRangeFor(pointRange3), (Items{{4, 5}, {5, 5}, {4, 6}, {5, 6}, {4, 7}, {5, 7}}));
}
