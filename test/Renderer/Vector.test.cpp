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
	EXPECT_EQ((NAS2D::Vector{2, 3}), (NAS2D::Vector{1, 1}) + (NAS2D::Vector{1, 2}));
}

TEST(Vector, Subtract) {
	EXPECT_EQ((NAS2D::Vector{1, 1}), (NAS2D::Vector{2, 3}) - (NAS2D::Vector{1, 2}));
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

TEST(Vector, OperatorType) {
	EXPECT_EQ((NAS2D::Vector<int>{1, 2}), static_cast<NAS2D::Vector<int>>(NAS2D::Vector<float>{1.0, 2.0}));
	EXPECT_EQ((NAS2D::Vector<float>{1.0, 2.0}), static_cast<NAS2D::Vector<float>>(NAS2D::Vector<int>{1, 2}));
}

TEST(Vector, to) {
	EXPECT_EQ((NAS2D::Vector<int>{1, 2}), (NAS2D::Vector<float>{1.0, 2.0}.to<int>()));
	EXPECT_EQ((NAS2D::Vector<float>{1.0, 2.0}), (NAS2D::Vector<int>{1, 2}.to<float>()));
}
