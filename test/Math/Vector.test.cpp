#include "NAS2D/Math/Vector.h"

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

TEST(Vector, OperatorEqual) {
	EXPECT_EQ((NAS2D::Vector{1, 1}), (NAS2D::Vector{1, 1}));
	EXPECT_EQ((NAS2D::Vector{2, 2}), (NAS2D::Vector{2, 2}));
}

TEST(Vector, OperatorNotEqual) {
	EXPECT_NE((NAS2D::Vector{1, 1}), (NAS2D::Vector{1, 2}));
	EXPECT_NE((NAS2D::Vector{1, 1}), (NAS2D::Vector{2, 1}));
}

TEST(Vector, Conversion) {
	// Allow explicit conversion
	EXPECT_EQ((NAS2D::Vector<int>{1, 1}), NAS2D::Vector<int>(NAS2D::Vector<float>{1.0, 1.0}));
	EXPECT_EQ((NAS2D::Vector<float>{1.0, 1.0}), NAS2D::Vector<float>(NAS2D::Vector<int>{1, 1}));
}

TEST(Vector, SelfAdd) {
	auto vector = NAS2D::Vector{1, 1};
	EXPECT_EQ(&vector, &(vector += NAS2D::Vector{1, 2}));
	EXPECT_EQ((NAS2D::Vector{2, 3}), vector);
}

TEST(Vector, SelfSubtract) {
	auto vector = NAS2D::Vector{2, 3};
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
	auto vector = NAS2D::Vector{1, 2};
	EXPECT_EQ(&vector, &(vector *= 2));
	EXPECT_EQ((NAS2D::Vector{2, 4}), vector);
}

TEST(Vector, SelfDivideScalar) {
	{
		NAS2D::Vector<int> vector{2, 4};
		EXPECT_EQ(&vector, &(vector /= 2));
		EXPECT_EQ((NAS2D::Vector{1, 2}), vector);

		EXPECT_THROW(vector /= 0, std::domain_error);
	}
	{
		auto vector = NAS2D::Vector{2.0, 4.0};
		EXPECT_EQ(&vector, &(vector /= 2.0));
		EXPECT_EQ((NAS2D::Vector{1.0, 2.0}), vector);

		EXPECT_THROW(vector /= 0.0, std::domain_error);
	}
}

TEST(Vector, MultiplyScalar) {
	EXPECT_EQ((NAS2D::Vector{2, 4}), (NAS2D::Vector{1, 2} * 2));
}

TEST(Vector, DivideScalar) {
	EXPECT_EQ((NAS2D::Vector{1, 2}), (NAS2D::Vector{2, 4} / 2));
	EXPECT_THROW((NAS2D::Vector{2, 4} /= 0), std::domain_error);

	EXPECT_EQ((NAS2D::Vector{1.0, 2.0}), (NAS2D::Vector{2.0, 4.0} / 2.0));
	EXPECT_THROW((NAS2D::Vector{2.0, 4.0} /= 0.0), std::domain_error);
}

TEST(Vector, skewBy) {
	EXPECT_EQ((NAS2D::Vector{1, 0}), (NAS2D::Vector{1, 1}.skewBy(NAS2D::Vector{1, 0})));
	EXPECT_EQ((NAS2D::Vector{2, 3}), (NAS2D::Vector{1, 1}.skewBy(NAS2D::Vector{2, 3})));
	EXPECT_EQ((NAS2D::Vector{4, 6}), (NAS2D::Vector{1, 1}.skewBy(NAS2D::Vector{4, 6})));

	EXPECT_EQ((NAS2D::Vector{2, 0}), (NAS2D::Vector{2, 1}.skewBy(NAS2D::Vector{1, 0})));
	EXPECT_EQ((NAS2D::Vector{4, 3}), (NAS2D::Vector{2, 1}.skewBy(NAS2D::Vector{2, 3})));
	EXPECT_EQ((NAS2D::Vector{8, 6}), (NAS2D::Vector{2, 1}.skewBy(NAS2D::Vector{4, 6})));

	EXPECT_EQ((NAS2D::Vector{1, 0}), (NAS2D::Vector{1, 2}.skewBy(NAS2D::Vector{1, 0})));
	EXPECT_EQ((NAS2D::Vector{2, 6}), (NAS2D::Vector{1, 2}.skewBy(NAS2D::Vector{2, 3})));
	EXPECT_EQ((NAS2D::Vector{4, 12}), (NAS2D::Vector{1, 2}.skewBy(NAS2D::Vector{4, 6})));
}

TEST(Vector, skewInverseBy) {
	EXPECT_EQ((NAS2D::Vector{8, 4}), (NAS2D::Vector{8, 8}.skewInverseBy(NAS2D::Vector{1, 2})));
	EXPECT_EQ((NAS2D::Vector{4, 2}), (NAS2D::Vector{8, 8}.skewInverseBy(NAS2D::Vector{2, 4})));

	EXPECT_EQ((NAS2D::Vector{9, 2}), (NAS2D::Vector{9, 6}.skewInverseBy(NAS2D::Vector{1, 3})));
	EXPECT_EQ((NAS2D::Vector{3, 3}), (NAS2D::Vector{9, 6}.skewInverseBy(NAS2D::Vector{3, 2})));

	EXPECT_EQ((NAS2D::Vector{4, 2}), (NAS2D::Vector{8, 6}.skewInverseBy(NAS2D::Vector{2, 3})));
	EXPECT_EQ((NAS2D::Vector{2, 3}), (NAS2D::Vector{8, 6}.skewInverseBy(NAS2D::Vector{4, 2})));
}

TEST(Vector, skewInverseByRounded) {
	EXPECT_EQ((NAS2D::Vector{2, 1}), (NAS2D::Vector{8, 8}.skewInverseBy(NAS2D::Vector{3, 5})));
	EXPECT_EQ((NAS2D::Vector{2, 1}), (NAS2D::Vector{9, 6}.skewInverseBy(NAS2D::Vector{4, 5})));
	EXPECT_EQ((NAS2D::Vector{2, 1}), (NAS2D::Vector{8, 6}.skewInverseBy(NAS2D::Vector{3, 5})));
}

TEST(Vector, skewInverseByDomainErrorInt) {
	EXPECT_THROW((NAS2D::Vector{1, 1}.skewInverseBy(NAS2D::Vector{0, 0})), std::domain_error);
	EXPECT_THROW((NAS2D::Vector{1, 1}.skewInverseBy(NAS2D::Vector{0, 1})), std::domain_error);
	EXPECT_THROW((NAS2D::Vector{1, 1}.skewInverseBy(NAS2D::Vector{1, 0})), std::domain_error);
}

TEST(Vector, skewInverseByDomainErrorDouble) {
	EXPECT_THROW((NAS2D::Vector{1.0, 1.0}.skewInverseBy(NAS2D::Vector{0.0, 0.0})), std::domain_error);
	EXPECT_THROW((NAS2D::Vector{1.0, 1.0}.skewInverseBy(NAS2D::Vector{0.0, 1.0})), std::domain_error);
	EXPECT_THROW((NAS2D::Vector{1.0, 1.0}.skewInverseBy(NAS2D::Vector{1.0, 0.0})), std::domain_error);
}

TEST(Vector, lengthSquared) {
	// Test a few simple vectors
	for (int i = 0; i < 10; ++i) {
		// Single coordinate is simple squaring
		// Test symmetry in both coordinates
		const auto vectorX = NAS2D::Vector{i, 0};
		const auto vectorY = NAS2D::Vector{0, i};
		EXPECT_EQ(i*i, vectorX.lengthSquared());
		EXPECT_EQ(i*i, vectorY.lengthSquared());
		// Double equal coordinates doubles result
		const auto vectorXY = NAS2D::Vector{i, i};
		EXPECT_EQ(2 * i*i, vectorXY.lengthSquared());
	}

	// Test a few mixed values
	EXPECT_EQ(5, (NAS2D::Vector{1, 2}).lengthSquared());
	EXPECT_EQ(13, (NAS2D::Vector{2, 3}).lengthSquared());
	EXPECT_EQ(25, (NAS2D::Vector{3, 4}).lengthSquared());
}

TEST(Vector, dotProduct) {
	// Test a few simple vectors
	for (int i = 0; i < 10; ++i) {
		const auto vectorX = NAS2D::Vector{i, 0};
		const auto vectorY = NAS2D::Vector{0, i};
		// Pairwise disjoint coordinates
		EXPECT_EQ(0, vectorX.dotProduct(vectorY));
		// Equal single coordinates
		EXPECT_EQ(i*i, vectorX.dotProduct(vectorX));
		EXPECT_EQ(i*i, vectorY.dotProduct(vectorY));
		// Equal pair coordinates
		const auto vectorXY = NAS2D::Vector{i, i};
		EXPECT_EQ(2 * i*i, vectorXY.dotProduct(vectorXY));
	}

	// Test a few mixed values
	EXPECT_EQ(3, (NAS2D::Vector{1, 2}).dotProduct(NAS2D::Vector{1, 1}));
	EXPECT_EQ(8, (NAS2D::Vector{2, 3}).dotProduct(NAS2D::Vector{1, 2}));
	EXPECT_EQ(18, (NAS2D::Vector{3, 4}).dotProduct(NAS2D::Vector{2, 3}));
}

TEST(Vector, reflectX) {
	EXPECT_EQ((NAS2D::Vector{-1, 1}), (NAS2D::Vector{1, 1}).reflectX());
	EXPECT_EQ((NAS2D::Vector{-1, 2}), (NAS2D::Vector{1, 2}).reflectX());
	EXPECT_EQ((NAS2D::Vector{-2, 1}), (NAS2D::Vector{2, 1}).reflectX());
	EXPECT_EQ((NAS2D::Vector{-2, 2}), (NAS2D::Vector{2, 2}).reflectX());

	EXPECT_EQ((NAS2D::Vector{1, 1}), (NAS2D::Vector{-1, 1}).reflectX());
	EXPECT_EQ((NAS2D::Vector{1, 2}), (NAS2D::Vector{-1, 2}).reflectX());
	EXPECT_EQ((NAS2D::Vector{2, 1}), (NAS2D::Vector{-2, 1}).reflectX());
	EXPECT_EQ((NAS2D::Vector{2, 2}), (NAS2D::Vector{-2, 2}).reflectX());
}

TEST(Vector, reflectY) {
	EXPECT_EQ((NAS2D::Vector{1, -1}), (NAS2D::Vector{1, 1}).reflectY());
	EXPECT_EQ((NAS2D::Vector{1, -2}), (NAS2D::Vector{1, 2}).reflectY());
	EXPECT_EQ((NAS2D::Vector{2, -1}), (NAS2D::Vector{2, 1}).reflectY());
	EXPECT_EQ((NAS2D::Vector{2, -2}), (NAS2D::Vector{2, 2}).reflectY());

	EXPECT_EQ((NAS2D::Vector{1, 1}), (NAS2D::Vector{1, -1}).reflectY());
	EXPECT_EQ((NAS2D::Vector{1, 2}), (NAS2D::Vector{1, -2}).reflectY());
	EXPECT_EQ((NAS2D::Vector{2, 1}), (NAS2D::Vector{2, -1}).reflectY());
	EXPECT_EQ((NAS2D::Vector{2, 2}), (NAS2D::Vector{2, -2}).reflectY());
}

TEST(Vector, OperatorType) {
	EXPECT_EQ((NAS2D::Vector<int>{1, 2}), static_cast<NAS2D::Vector<int>>(NAS2D::Vector<float>{1.0, 2.0}));
	EXPECT_EQ((NAS2D::Vector<float>{1.0, 2.0}), static_cast<NAS2D::Vector<float>>(NAS2D::Vector<int>{1, 2}));
}

TEST(Vector, to) {
	EXPECT_EQ((NAS2D::Vector<int>{1, 2}), (NAS2D::Vector<float>{1.0, 2.0}.to<int>()));
	EXPECT_EQ((NAS2D::Vector<float>{1.0, 2.0}), (NAS2D::Vector<int>{1, 2}.to<float>()));
}

TEST(Vector, scalarVectorMultiply) {
	EXPECT_EQ((NAS2D::Vector{2, 4}), (2 * NAS2D::Vector{1, 2}));
}

TEST(Vector, PartialOrderLessEqual) {
	EXPECT_LE((NAS2D::Vector{0, 0}), (NAS2D::Vector{1, 1}));
	EXPECT_LE((NAS2D::Vector{0, 1}), (NAS2D::Vector{1, 1}));
	EXPECT_LE((NAS2D::Vector{1, 0}), (NAS2D::Vector{1, 1}));
	EXPECT_LE((NAS2D::Vector{1, 1}), (NAS2D::Vector{1, 1}));

	EXPECT_FALSE((NAS2D::Vector{0, 1}) <= (NAS2D::Vector{1, 0}));
	EXPECT_FALSE((NAS2D::Vector{1, 0}) <= (NAS2D::Vector{0, 1}));
}

TEST(Vector, PartialOrderGreaterEqual) {
	EXPECT_GE((NAS2D::Vector{1, 1}), (NAS2D::Vector{0, 0}));
	EXPECT_GE((NAS2D::Vector{1, 1}), (NAS2D::Vector{0, 1}));
	EXPECT_GE((NAS2D::Vector{1, 1}), (NAS2D::Vector{1, 0}));
	EXPECT_GE((NAS2D::Vector{1, 1}), (NAS2D::Vector{1, 1}));

	EXPECT_FALSE((NAS2D::Vector{0, 1}) >= (NAS2D::Vector{1, 0}));
	EXPECT_FALSE((NAS2D::Vector{1, 0}) >= (NAS2D::Vector{0, 1}));
}

TEST(Vector, PartialOrderLess) {
	EXPECT_LT((NAS2D::Vector{0, 0}), (NAS2D::Vector{1, 1}));

	EXPECT_FALSE((NAS2D::Vector{0, 1}) < (NAS2D::Vector{1, 1}));
	EXPECT_FALSE((NAS2D::Vector{1, 0}) < (NAS2D::Vector{1, 1}));
	EXPECT_FALSE((NAS2D::Vector{1, 1}) < (NAS2D::Vector{1, 1}));

	EXPECT_FALSE((NAS2D::Vector{0, 1}) < (NAS2D::Vector{1, 0}));
	EXPECT_FALSE((NAS2D::Vector{1, 0}) < (NAS2D::Vector{0, 1}));
}

TEST(Vector, PartialOrderGreater) {
	EXPECT_GT((NAS2D::Vector{1, 1}), (NAS2D::Vector{0, 0}));

	EXPECT_FALSE((NAS2D::Vector{1, 1}) > (NAS2D::Vector{0, 1}));
	EXPECT_FALSE((NAS2D::Vector{1, 1}) > (NAS2D::Vector{1, 0}));
	EXPECT_FALSE((NAS2D::Vector{1, 1}) > (NAS2D::Vector{1, 1}));

	EXPECT_FALSE((NAS2D::Vector{0, 1}) > (NAS2D::Vector{1, 0}));
	EXPECT_FALSE((NAS2D::Vector{1, 0}) > (NAS2D::Vector{0, 1}));
}
