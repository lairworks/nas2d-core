#include "NAS2D/Math/PointInRectangleRange.h"

#include <gtest/gtest.h>

#include <vector>


namespace {
	template <typename BaseType>
	auto fillByRangeFor(NAS2D::PointInRectangleRange<BaseType> pointRange) -> std::vector<NAS2D::Point<BaseType>> {
		std::vector<NAS2D::Point<BaseType>> collection;
		for (const auto point : pointRange) {
			collection.push_back(point);
		}
		return collection;
	};
}


TEST(PointInRectangleRange, EmptyRangeBeginIsEnd) {
	const auto pointRangeEmpty = NAS2D::PointInRectangleRange{NAS2D::Rectangle<int>{{0, 0}, {0, 0}}};
	EXPECT_EQ(pointRangeEmpty.begin(), pointRangeEmpty.end());
}

TEST(PointInRectangleRange, EndIsEnd) {
	const auto pointRangeSingleElement = NAS2D::PointInRectangleRange{NAS2D::Rectangle<int>{{1, 1}, {1, 1}}};
	EXPECT_EQ(pointRangeSingleElement.end(), pointRangeSingleElement.end());
}

TEST(PointInRectangleRange, BeginToEnd) {
	const auto pointRangeSingleElement = NAS2D::PointInRectangleRange{NAS2D::Rectangle<int>{{1, 1}, {1, 1}}};
	EXPECT_EQ(pointRangeSingleElement.end(), ++pointRangeSingleElement.begin());
}

TEST(PointInRectangleRange, BeginIsOrigin) {
	EXPECT_EQ((NAS2D::Point{1, 1}), (*NAS2D::PointInRectangleRange{NAS2D::Rectangle<int>{{1, 1}, {1, 1}}}.begin()));
	EXPECT_EQ((NAS2D::Point{4, 5}), (*NAS2D::PointInRectangleRange{NAS2D::Rectangle<int>{{4, 5}, {2, 3}}}.begin()));
}

TEST(PointInRectangleRange, EndDecrementIsLast) {
	EXPECT_EQ((NAS2D::Point{1, 1}), (*--NAS2D::PointInRectangleRange{NAS2D::Rectangle<int>{{1, 1}, {1, 1}}}.end()));
	EXPECT_EQ((NAS2D::Point{5, 7}), (*--NAS2D::PointInRectangleRange{NAS2D::Rectangle<int>{{4, 5}, {2, 3}}}.end()));
}

TEST(PointInRectangleRange, Iteration) {
	using Items = std::vector<NAS2D::Point<int>>;

	const auto pointRange1 = NAS2D::PointInRectangleRange{NAS2D::Rectangle<int>{{0, 0}, {0, 0}}};
	const auto pointRange2 = NAS2D::PointInRectangleRange{NAS2D::Rectangle<int>{{1, 1}, {1, 1}}};
	const auto pointRange3 = NAS2D::PointInRectangleRange{NAS2D::Rectangle<int>{{4, 5}, {2, 3}}};

	// Test head of range for expected values
	EXPECT_EQ((NAS2D::Point{4, 5}), *pointRange3.begin());
	EXPECT_EQ((NAS2D::Point{5, 5}), *++pointRange3.begin());
	EXPECT_EQ((NAS2D::Point{4, 6}), *++++pointRange3.begin());
	EXPECT_EQ((NAS2D::Point{5, 6}), *++++++pointRange3.begin());

	EXPECT_EQ(fillByRangeFor(pointRange1), (Items{}));
	EXPECT_EQ(fillByRangeFor(pointRange2), (Items{{1, 1}}));
	EXPECT_EQ(fillByRangeFor(pointRange3), (Items{{4, 5}, {5, 5}, {4, 6}, {5, 6}, {4, 7}, {5, 7}}));
}
