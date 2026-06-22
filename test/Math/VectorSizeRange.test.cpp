#include "NAS2D/Math/VectorSizeRange.h"

#include <gtest/gtest.h>

#include <vector>


namespace {
	template <typename BaseType>
	auto fillByRangeFor(NAS2D::VectorSizeRange<BaseType> vectorRange) -> std::vector<NAS2D::Vector<BaseType>> {
		std::vector<NAS2D::Vector<BaseType>> collection;
		for (const auto vector : vectorRange) {
			collection.push_back(vector);
		}
		return collection;
	};
}


TEST(VectorSizeRange, EmptyRangeBeginIsEnd) {
	const auto vectorRangeEmpty = NAS2D::VectorSizeRange{NAS2D::Vector{0, 0}};
	EXPECT_EQ(vectorRangeEmpty.begin(), vectorRangeEmpty.end());
}

TEST(VectorSizeRange, EndIsEnd) {
	const auto vectorRangeSingleElement = NAS2D::VectorSizeRange{NAS2D::Vector{1, 1}};
	EXPECT_EQ(vectorRangeSingleElement.end(), vectorRangeSingleElement.end());
}

TEST(VectorSizeRange, BeginToEnd) {
	const auto vectorRangeSingleElement = NAS2D::VectorSizeRange{NAS2D::Vector{1, 1}};
	EXPECT_EQ(vectorRangeSingleElement.end(), ++vectorRangeSingleElement.begin());
}

TEST(VectorSizeRange, BeginIsOrigin) {
	EXPECT_EQ((NAS2D::Vector{0, 0}), (*NAS2D::VectorSizeRange{NAS2D::Vector{1, 1}}.begin()));
	EXPECT_EQ((NAS2D::Vector{0, 0}), (*NAS2D::VectorSizeRange{NAS2D::Vector{2, 3}}.begin()));
}

TEST(VectorSizeRange, EndDecrementIsLast) {
	EXPECT_EQ((NAS2D::Vector{0, 0}), (*--NAS2D::VectorSizeRange{NAS2D::Vector{1, 1}}.end()));
	EXPECT_EQ((NAS2D::Vector{1, 2}), (*--NAS2D::VectorSizeRange{NAS2D::Vector{2, 3}}.end()));
}

TEST(VectorSizeRange, IterationEmpty) {
	using Items = std::vector<NAS2D::Vector<int>>;
	const auto vectorRangeEmpty = NAS2D::VectorSizeRange{NAS2D::Vector{0, 0}};
	EXPECT_EQ(fillByRangeFor(vectorRangeEmpty), (Items{}));
}

TEST(VectorSizeRange, IterationSingle) {
	using Items = std::vector<NAS2D::Vector<int>>;
	const auto vectorRangeSingle = NAS2D::VectorSizeRange{NAS2D::Vector{1, 1}};
	EXPECT_EQ(fillByRangeFor(vectorRangeSingle), (Items{{0, 0}}));
}

TEST(VectorSizeRange, IterationMultiNoWrap) {
	using Items = std::vector<NAS2D::Vector<int>>;
	const auto vectorRangeMultiWrap = NAS2D::VectorSizeRange{NAS2D::Vector{3, 1}};
	EXPECT_EQ(fillByRangeFor(vectorRangeMultiWrap), (Items{{0, 0}, {1, 0}, {2, 0}}));
}

TEST(VectorSizeRange, IterationMultiWrap) {
	using Items = std::vector<NAS2D::Vector<int>>;
	const auto vectorRangeMultiWrap = NAS2D::VectorSizeRange{NAS2D::Vector{2, 3}};
	EXPECT_EQ(fillByRangeFor(vectorRangeMultiWrap), (Items{{0, 0}, {1, 0}, {0, 1}, {1, 1}, {0, 2}, {1, 2}}));
}
