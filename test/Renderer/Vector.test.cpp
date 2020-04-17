#include "NAS2D/Renderer/Vector.h"
#include <gtest/gtest.h>
#include <type_traits>


TEST(Vector, Aggregate) {
	EXPECT_TRUE((std::is_aggregate_v<NAS2D::Vector<int>>));
}

TEST(Vector, DeductionGuidedConstruction) {
	EXPECT_TRUE((std::is_same_v<NAS2D::Vector<int>, decltype(NAS2D::Vector{0, 0})>));
	EXPECT_TRUE((std::is_same_v<NAS2D::Vector<double>, decltype(NAS2D::Vector{0.0, 0.0})>));
	EXPECT_TRUE((std::is_same_v<NAS2D::Vector<float>, decltype(NAS2D::Vector{0.0f, 0.0f})>));
}

TEST(Vector, DefaultConstructible) {
	EXPECT_EQ((NAS2D::Vector{0, 0}), NAS2D::Vector<int>{});
}

TEST(Vector, OperatorEqualNotEqual) {
	EXPECT_EQ((NAS2D::Vector{1, 1}), (NAS2D::Vector{1, 1}));
	EXPECT_EQ((NAS2D::Vector{2, 2}), (NAS2D::Vector{2, 2}));

	EXPECT_NE((NAS2D::Vector{1, 1}), (NAS2D::Vector{1, 2}));
	EXPECT_NE((NAS2D::Vector{1, 1}), (NAS2D::Vector{2, 1}));
}

TEST(Vector, Conversion) {
	// Allow explicit conversion
	EXPECT_EQ((NAS2D::Vector<int>{1, 1}), static_cast<NAS2D::Vector<int>>(NAS2D::Vector<float>{1.0, 1.0}));
	EXPECT_EQ((NAS2D::Vector<float>{1.0, 1.0}), static_cast<NAS2D::Vector<float>>(NAS2D::Vector<int>{1, 1}));

	// Allow implicit conversion (may be deprecated in the future)
	EXPECT_EQ((NAS2D::Vector<int>{1, 1}), (NAS2D::Vector<float>{1.0, 1.0}));
	EXPECT_EQ((NAS2D::Vector<float>{1.0, 1.0}), (NAS2D::Vector<int>{1, 1}));
}

TEST(Vector, SelfAdd) {
	NAS2D::Vector<int> vector{1, 1};
	EXPECT_EQ(&vector, &(vector += NAS2D::Vector{1, 2}));
	EXPECT_EQ((NAS2D::Vector{2, 3}), vector);
}

TEST(Vector, SelfSubtract) {
	NAS2D::Vector<int> vector{2, 3};
	EXPECT_EQ(&vector, &(vector -= NAS2D::Vector{1, 2}));
	EXPECT_EQ((NAS2D::Vector{1, 1}), vector);
}

TEST(Vector, Add) {
	EXPECT_EQ((NAS2D::Vector{2, 3}), (NAS2D::Vector{1, 1} + NAS2D::Vector{1, 2}));
}

TEST(Vector, Subtract) {
	EXPECT_EQ((NAS2D::Vector{1, 1}), (NAS2D::Vector{2, 3} - NAS2D::Vector{1, 2}));
}

TEST(Vector, SelfMultiplyScalar) {
	NAS2D::Vector<int> vector{1, 2};
	EXPECT_EQ(&vector, &(vector *= 2));
	EXPECT_EQ((NAS2D::Vector{2, 4}), vector);
}

TEST(Vector, SelfDivideScalar) {
	NAS2D::Vector<int> vector{2, 4};
	EXPECT_EQ(&vector, &(vector /= 2));
	EXPECT_EQ((NAS2D::Vector{1, 2}), vector);

	EXPECT_THROW(vector /= 0, std::domain_error);
}

TEST(Vector, MultiplyScalar) {
	EXPECT_EQ((NAS2D::Vector{2, 4}), (NAS2D::Vector{1, 2} * 2));
}

TEST(Vector, DivideScalar) {
	EXPECT_EQ((NAS2D::Vector{1, 2}), (NAS2D::Vector{2, 4} / 2));

	EXPECT_THROW((NAS2D::Vector{2, 4} /= 0), std::domain_error);
}

TEST(Vector, lengthSquared) {
	// Test a few simple vectors
	for (int i = 0; i < 10; ++i) {
		// Single coordinate is simple squaring
		// Test symmetry in both coordinates
		EXPECT_EQ(i*i, (NAS2D::Vector{i, 0}).lengthSquared());
		EXPECT_EQ(i*i, (NAS2D::Vector{0, i}).lengthSquared());
		// Double equal coordinates doubles result
		EXPECT_EQ(2 * i*i, (NAS2D::Vector{i, i}).lengthSquared());
	}

	// Test a few mixed values
	EXPECT_EQ(5, (NAS2D::Vector{1, 2}).lengthSquared());
	EXPECT_EQ(13, (NAS2D::Vector{2, 3}).lengthSquared());
	EXPECT_EQ(25, (NAS2D::Vector{3, 4}).lengthSquared());
}

TEST(Vector, dotProduct) {
	// Test a few simple vectors
	for (int i = 0; i < 10; ++i) {
		// Pairwise disjoint coordinates
		EXPECT_EQ(0, (NAS2D::Vector{i, 0}.dotProduct(NAS2D::Vector{0, i})));
		// Equal single coordinates
		EXPECT_EQ(i*i, (NAS2D::Vector{i, 0}.dotProduct(NAS2D::Vector{i, 0})));
		EXPECT_EQ(i*i, (NAS2D::Vector{0, i}.dotProduct(NAS2D::Vector{0, i})));
		// Equal pair coordinates
		EXPECT_EQ(2 * i*i, (NAS2D::Vector{i, i}.dotProduct(NAS2D::Vector{i, i})));
	}

	// Test a few mixed values
	EXPECT_EQ(3, (NAS2D::Vector{1, 2}).dotProduct(NAS2D::Vector{1, 1}));
	EXPECT_EQ(8, (NAS2D::Vector{2, 3}).dotProduct(NAS2D::Vector{1, 2}));
	EXPECT_EQ(18, (NAS2D::Vector{3, 4}).dotProduct(NAS2D::Vector{2, 3}));
}

TEST(Vector, OperatorType) {
	EXPECT_EQ((NAS2D::Vector<int>{1, 2}), static_cast<NAS2D::Vector<int>>(NAS2D::Vector<float>{1.0, 2.0}));
	EXPECT_EQ((NAS2D::Vector<float>{1.0, 2.0}), static_cast<NAS2D::Vector<float>>(NAS2D::Vector<int>{1, 2}));
}

TEST(Vector, to) {
	EXPECT_EQ((NAS2D::Vector<int>{1, 2}), (NAS2D::Vector<float>{1.0, 2.0}.to<int>()));
	EXPECT_EQ((NAS2D::Vector<float>{1.0, 2.0}), (NAS2D::Vector<int>{1, 2}.to<float>()));
}
