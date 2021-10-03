#include "NAS2D/Math/VectorSizeRange.h"

#include <gtest/gtest.h>

#include <vector>


TEST(VectorSizeRange, Iteration) {
	using Items = std::vector<NAS2D::Vector<int>>;

	const auto vectorRange1 = NAS2D::VectorSizeRange{NAS2D::Vector{0, 0}};
	const auto vectorRange2 = NAS2D::VectorSizeRange{NAS2D::Vector{1, 1}};
	const auto vectorRange3 = NAS2D::VectorSizeRange{NAS2D::Vector{2, 3}};

	// Dereference produces the expected starting value
	// (Later tests also proves the range is restartable)
	EXPECT_EQ((NAS2D::Vector{0, 0}), *vectorRange1.begin());
	EXPECT_EQ((NAS2D::Vector{0, 0}), *vectorRange2.begin());
	EXPECT_EQ((NAS2D::Vector{0, 0}), *vectorRange3.begin());

	// Range-for syntax is supported
	const auto fillByRangeFor = [](auto vectorRange) {
		Items collection;
		for (const auto vector : vectorRange) {
			collection.push_back(vector);
		}
		return collection;
	};
	EXPECT_EQ(fillByRangeFor(vectorRange1), (Items{}));
	EXPECT_EQ(fillByRangeFor(vectorRange2), (Items{{0, 0}}));
	EXPECT_EQ(fillByRangeFor(vectorRange3), (Items{{0, 0}, {1, 0}, {0, 1}, {1, 1}, {0, 2}, {1, 2}}));
}
