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

TEST(PointInRectangleRange, IterationEmpty) {
	using Items = std::vector<NAS2D::Point<int>>;
	const auto pointRangeEmpty = NAS2D::PointInRectangleRange{NAS2D::Rectangle<int>{{0, 0}, {0, 0}}};
	EXPECT_EQ(fillByRangeFor(pointRangeEmpty), (Items{}));
}

TEST(PointInRectangleRange, IterationSingle) {
	using Items = std::vector<NAS2D::Point<int>>;
	const auto pointRangeSingle = NAS2D::PointInRectangleRange{NAS2D::Rectangle<int>{{1, 1}, {1, 1}}};
	EXPECT_EQ(fillByRangeFor(pointRangeSingle), (Items{{1, 1}}));
}

TEST(PointInRectangleRange, IterationMultiWrap) {
	using Items = std::vector<NAS2D::Point<int>>;
	const auto pointRangeMultiWrap = NAS2D::PointInRectangleRange{NAS2D::Rectangle<int>{{4, 5}, {2, 3}}};
	EXPECT_EQ(fillByRangeFor(pointRangeMultiWrap), (Items{{4, 5}, {5, 5}, {4, 6}, {5, 6}, {4, 7}, {5, 7}}));
}
