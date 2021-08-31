#include "NAS2D/Renderer/Vector3.h"
#include <gtest/gtest.h>
#include <type_traits>


TEST(Vector3, Aggregate) {
	EXPECT_TRUE((std::is_aggregate_v<NAS2D::Vector3<int>>));
}

TEST(Vector3, DeductionGuidedConstruction) {
	EXPECT_TRUE((std::is_same_v<NAS2D::Vector3<int>, decltype(NAS2D::Vector3{0, 0, 0})>));
	EXPECT_TRUE((std::is_same_v<NAS2D::Vector3<double>, decltype(NAS2D::Vector3{0.0, 0.0, 0.0})>));
	EXPECT_TRUE((std::is_same_v<NAS2D::Vector3<float>, decltype(NAS2D::Vector3{0.0f, 0.0f, 0.0f})>));
}

TEST(Vector3, DefaultConstructible) {
	EXPECT_EQ((NAS2D::Vector3{0, 0, 0}), NAS2D::Vector3<int>{});
	EXPECT_EQ((NAS2D::Vector3{0.0f, 0.0f, 0.0f}), NAS2D::Vector3<float>{});
	EXPECT_EQ((NAS2D::Vector3{0.0, 0.0, 0.0}), NAS2D::Vector3<double>{});
}

TEST(Vector3, OperatorEqualNotEqual) {
	EXPECT_EQ((NAS2D::Vector3{1, 1, 1}), (NAS2D::Vector3{1, 1, 1}));
	EXPECT_EQ((NAS2D::Vector3{2, 2, 2}), (NAS2D::Vector3{2, 2, 2}));

	EXPECT_NE((NAS2D::Vector3{1, 1, 1}), (NAS2D::Vector3{1, 2, 3}));
	EXPECT_NE((NAS2D::Vector3{1, 1, 1}), (NAS2D::Vector3{2, 1, 3}));
}

TEST(Vector3, Conversion) {
	// Allow explicit conversion
	EXPECT_EQ((NAS2D::Vector3<int>{1, 1, 1}), static_cast<NAS2D::Vector3<int>>(NAS2D::Vector3<float>{1.0f, 1.0f, 1.0f}));
	EXPECT_EQ((NAS2D::Vector3<int>{1, 1, 1}), static_cast<NAS2D::Vector3<int>>(NAS2D::Vector3<float>{1.5f, 1.5f, 1.5f}));
	EXPECT_EQ((NAS2D::Vector3<int>{-2, -2, -2}), static_cast<NAS2D::Vector3<int>>(NAS2D::Vector3<float>{-1.5f, -1.5f, -1.5f}));
	EXPECT_EQ((NAS2D::Vector3<float>{1.0, 1.0, 1.0}), static_cast<NAS2D::Vector3<float>>(NAS2D::Vector3<int>{1, 1, 1}));
}

TEST(Vector3, SelfAdd) {
	NAS2D::Vector3<int> vector{1, 1, 1};
	EXPECT_EQ(&vector, &(vector += NAS2D::Vector3{1, 2, 3}));
	EXPECT_EQ((NAS2D::Vector3{2, 3, 4}), vector);
}

TEST(Vector3, SelfSubtract) {
	NAS2D::Vector3<int> vector{2, 3, 4};
	EXPECT_EQ(&vector, &(vector -= NAS2D::Vector3{1, 2, 3}));
	EXPECT_EQ((NAS2D::Vector3{1, 1, 1}), vector);
}

TEST(Vector3, Add) {
	EXPECT_EQ((NAS2D::Vector3{2, 3, 4}), (NAS2D::Vector3{1, 1, 1} + NAS2D::Vector3{1, 2, 3}));
}

TEST(Vector3, Subtract) {
	EXPECT_EQ((NAS2D::Vector3{1, 1, 1}), (NAS2D::Vector3{2, 3, 4} - NAS2D::Vector3{1, 2, 3}));
}

TEST(Vector3, SelfMultiplyScalar) {
	NAS2D::Vector3<int> vector{1, 2, 3};
	EXPECT_EQ(&vector, &(vector *= 2));
	EXPECT_EQ((NAS2D::Vector3{2, 4, 6}), vector);
}

TEST(Vector3, SelfDivideScalar) {
	NAS2D::Vector3<int> vector{2, 4, 6};
	EXPECT_EQ(&vector, &(vector /= 2));
	EXPECT_EQ((NAS2D::Vector3{1, 2, 3}), vector);

	EXPECT_THROW(vector /= 0, std::domain_error);
}

TEST(Vector3, MultiplyScalar) {
	EXPECT_EQ((NAS2D::Vector3{2, 4, 6}), (NAS2D::Vector3{1, 2, 3} * 2));
}

TEST(Vector3, DivideScalar) {
	EXPECT_EQ((NAS2D::Vector3{1, 2, 3}), (NAS2D::Vector3{2, 4, 6} / 2));

	EXPECT_THROW((NAS2D::Vector3{2, 4, 6} /= 0), std::domain_error);
}

TEST(Vector3, skewBy) {
	EXPECT_EQ((NAS2D::Vector3{2, 8, 18}), (NAS2D::Vector3{1, 2, 3}.skewBy(NAS2D::Vector3{2, 4, 6})));
	EXPECT_EQ((NAS2D::Vector3{2, 4, 6}), (NAS2D::Vector3{1, 1, 1}.skewBy(NAS2D::Vector3{2, 4, 6})));
	EXPECT_EQ((NAS2D::Vector3{-2, 8, 6}), (NAS2D::Vector3{-1, 2, 1}.skewBy(NAS2D::Vector3{2, 4, 6})));

	EXPECT_EQ((NAS2D::Vector3{2, 0, -1}), (NAS2D::Vector3{2, 1, 1}.skewBy(NAS2D::Vector3{1, 0, -1})));
	EXPECT_EQ((NAS2D::Vector3{4, 3, 6}), (NAS2D::Vector3{2, 1, 3}.skewBy(NAS2D::Vector3{2, 3, 2})));
	EXPECT_EQ((NAS2D::Vector3{8, 6, 12}), (NAS2D::Vector3{2, 1, 6}.skewBy(NAS2D::Vector3{4, 6, 2})));

	EXPECT_EQ((NAS2D::Vector3{1, 0, 3}), (NAS2D::Vector3{1, 2, 3}.skewBy(NAS2D::Vector3{1, 0, 1})));
	EXPECT_EQ((NAS2D::Vector3{2, 6, 18}), (NAS2D::Vector3{1, 2, 3}.skewBy(NAS2D::Vector3{2, 3, 6})));
	EXPECT_EQ((NAS2D::Vector3{4, 12, 24}), (NAS2D::Vector3{1, 2, 3}.skewBy(NAS2D::Vector3{4, 6, 8})));
}

TEST(Vector3, skewInverseBy) {
	EXPECT_THROW((NAS2D::Vector3{1, 1, 1}.skewInverseBy(NAS2D::Vector3{0, 0, 1})), std::domain_error);
	EXPECT_THROW((NAS2D::Vector3{1, 1, 1}.skewInverseBy(NAS2D::Vector3{0, 1, 0})), std::domain_error);
	EXPECT_THROW((NAS2D::Vector3{1, 1, 1}.skewInverseBy(NAS2D::Vector3{1, 0, 0})), std::domain_error);

	EXPECT_EQ((NAS2D::Vector3{8, 4, 2}), (NAS2D::Vector3{8, 8, 8}.skewInverseBy(NAS2D::Vector3{1, 2, 4})));
	EXPECT_EQ((NAS2D::Vector3{4, 2, 1}), (NAS2D::Vector3{8, 8, 8}.skewInverseBy(NAS2D::Vector3{2, 4, 6})));
	EXPECT_EQ((NAS2D::Vector3{2, 1, 1}), (NAS2D::Vector3{8, 8, 8}.skewInverseBy(NAS2D::Vector3{3, 5, 7})));

	EXPECT_EQ((NAS2D::Vector3{9, 2, 2}), (NAS2D::Vector3{9, 6, 10}.skewInverseBy(NAS2D::Vector3{1, 3, 5})));
	EXPECT_EQ((NAS2D::Vector3{3, 3, 10}), (NAS2D::Vector3{9, 6, 10}.skewInverseBy(NAS2D::Vector3{3, 2, 1})));
	EXPECT_EQ((NAS2D::Vector3{2, 1, 2}), (NAS2D::Vector3{9, 6, 12}.skewInverseBy(NAS2D::Vector3{4, 5, 6})));

	EXPECT_EQ((NAS2D::Vector3{4, 2, 1}), (NAS2D::Vector3{8, 6, 4}.skewInverseBy(NAS2D::Vector3{2, 3, 4})));
	EXPECT_EQ((NAS2D::Vector3{2, 3, 4}), (NAS2D::Vector3{8, 6, 4}.skewInverseBy(NAS2D::Vector3{4, 2, 1})));
	EXPECT_EQ((NAS2D::Vector3{2, 1, 0}), (NAS2D::Vector3{8, 6, 4}.skewInverseBy(NAS2D::Vector3{3, 5, 7})));
}

TEST(Vector3, lengthSquared) {
	// Test a few simple vectors
	for (int i = 0; i < 10; ++i) {
		// Single coordinate is simple squaring
		// Test symmetry in all coordinates
		const auto vectorX = NAS2D::Vector3{i, 0, 0};
		const auto vectorY = NAS2D::Vector3{0, i, 0};
		const auto vectorZ = NAS2D::Vector3{0, 0, i};
		EXPECT_EQ(i*i, vectorX.lengthSquared());
		EXPECT_EQ(i*i, vectorY.lengthSquared());
		EXPECT_EQ(i*i, vectorZ.lengthSquared());
		// Double equal coordinates doubles result
		const auto vectorXY = NAS2D::Vector3{i, i, 0};
		EXPECT_EQ(2 * i*i, vectorXY.lengthSquared());
	}

	// Test a few mixed values
	EXPECT_EQ(14, (NAS2D::Vector3{1, 2, 3}).lengthSquared());
	EXPECT_EQ(29, (NAS2D::Vector3{2, 3, 4}).lengthSquared());
	EXPECT_EQ(50, (NAS2D::Vector3{3, 4, 5}).lengthSquared());
}

TEST(Vector3, dotProduct) {
	EXPECT_EQ(3, (NAS2D::Vector3{1, 3, -5}).dotProduct(NAS2D::Vector3{4, -2, -1}));
	EXPECT_EQ(3, (NAS2D::Vector3{3, 4, 5}).dotProduct(NAS2D::Vector3<int>::X_Axis));
	EXPECT_EQ(4, (NAS2D::Vector3{3, 4, 5}).dotProduct(NAS2D::Vector3<int>::Y_Axis));
	EXPECT_EQ(5, (NAS2D::Vector3{3, 4, 5}).dotProduct(NAS2D::Vector3<int>::Z_Axis));
	EXPECT_EQ(1.0f, (NAS2D::Vector3<float>::X_Axis.dotProduct(NAS2D::Vector3<float>::X_Axis)));
	EXPECT_EQ(-1.0f, (NAS2D::Vector3<float>::X_Axis.dotProduct(-NAS2D::Vector3<float>::X_Axis)));
	EXPECT_EQ(0.0f, (NAS2D::Vector3<float>::X_Axis.dotProduct(NAS2D::Vector3<float>::Y_Axis)));
	EXPECT_EQ(0.0f, (NAS2D::Vector3<float>::X_Axis.dotProduct(-NAS2D::Vector3<float>::Y_Axis)));
	EXPECT_GT((NAS2D::Vector3<float>::X_Axis).dotProduct((NAS2D::Vector3<float>{0.8f, 0.6f, 0.0f})), 0.0f);
	EXPECT_LT((NAS2D::Vector3<float>::X_Axis.dotProduct(NAS2D::Vector3<float>{-0.8f, 0.6f, 0.0f})), 0.0f);
}

TEST(Vector3, reflectX) {
	EXPECT_EQ((NAS2D::Vector3{-1, 1, 1}), (NAS2D::Vector3{1, 1, 1}).reflectX());
	EXPECT_EQ((NAS2D::Vector3{-1, 2, 1}), (NAS2D::Vector3{1, 2, 1}).reflectX());
	EXPECT_EQ((NAS2D::Vector3{-2, 1, 1}), (NAS2D::Vector3{2, 1, 1}).reflectX());
	EXPECT_EQ((NAS2D::Vector3{-2, 2, 1}), (NAS2D::Vector3{2, 2, 1}).reflectX());

	EXPECT_EQ((NAS2D::Vector3{1, 1, 1}), (NAS2D::Vector3{-1, 1, 1}).reflectX());
	EXPECT_EQ((NAS2D::Vector3{1, 2, 1}), (NAS2D::Vector3{-1, 2, 1}).reflectX());
	EXPECT_EQ((NAS2D::Vector3{2, 1, 1}), (NAS2D::Vector3{-2, 1, 1}).reflectX());
	EXPECT_EQ((NAS2D::Vector3{2, 2, 1}), (NAS2D::Vector3{-2, 2, 1}).reflectX());
}

TEST(Vector3, reflectY) {
	EXPECT_EQ((NAS2D::Vector3{1, -1, 1}), (NAS2D::Vector3{1, 1, 1}).reflectY());
	EXPECT_EQ((NAS2D::Vector3{1, -2, 1}), (NAS2D::Vector3{1, 2, 1}).reflectY());
	EXPECT_EQ((NAS2D::Vector3{2, -1, 1}), (NAS2D::Vector3{2, 1, 1}).reflectY());
	EXPECT_EQ((NAS2D::Vector3{2, -2, 1}), (NAS2D::Vector3{2, 2, 1}).reflectY());

	EXPECT_EQ((NAS2D::Vector3{1, 1, 1}), (NAS2D::Vector3{1, -1, 1}).reflectY());
	EXPECT_EQ((NAS2D::Vector3{1, 2, 1}), (NAS2D::Vector3{1, -2, 1}).reflectY());
	EXPECT_EQ((NAS2D::Vector3{2, 1, 1}), (NAS2D::Vector3{2, -1, 1}).reflectY());
	EXPECT_EQ((NAS2D::Vector3{2, 2, 1}), (NAS2D::Vector3{2, -2, 1}).reflectY());
}

TEST(Vector3, reflectZ) {
	EXPECT_EQ((NAS2D::Vector3{1, 1, -1}), (NAS2D::Vector3{1, 1, 1}).reflectZ());
	EXPECT_EQ((NAS2D::Vector3{1, 2, -1}), (NAS2D::Vector3{1, 2, 1}).reflectZ());
	EXPECT_EQ((NAS2D::Vector3{2, 1, -1}), (NAS2D::Vector3{2, 1, 1}).reflectZ());
	EXPECT_EQ((NAS2D::Vector3{2, 2, -1}), (NAS2D::Vector3{2, 2, 1}).reflectZ());

	EXPECT_EQ((NAS2D::Vector3{1, 1, 1}), (NAS2D::Vector3{1, 1, -1}).reflectZ());
	EXPECT_EQ((NAS2D::Vector3{1, 2, 1}), (NAS2D::Vector3{1, 2, -1}).reflectZ());
	EXPECT_EQ((NAS2D::Vector3{2, 1, 1}), (NAS2D::Vector3{2, 1, -1}).reflectZ());
	EXPECT_EQ((NAS2D::Vector3{2, 2, 1}), (NAS2D::Vector3{2, 2, -1}).reflectZ());
}

TEST(Vector3, Negate) {
	EXPECT_EQ((NAS2D::Vector3{-1, -1, -1}), (-NAS2D::Vector3{1, 1, 1}));
}

TEST(Vector3, OperatorType) {
	EXPECT_EQ((NAS2D::Vector3<int>{1, 2}), static_cast<NAS2D::Vector3<int>>(NAS2D::Vector3<float>{1.0, 2.0}));
	EXPECT_EQ((NAS2D::Vector3<float>{1.0, 2.0}), static_cast<NAS2D::Vector3<float>>(NAS2D::Vector3<int>{1, 2}));
}

TEST(Vector3, to) {
	EXPECT_EQ((NAS2D::Vector3<int>{1, 2}), (NAS2D::Vector3<float>{1.0f, 2.0f}.to<int>()));
	EXPECT_EQ((NAS2D::Vector3<int>{1, 2}), (NAS2D::Vector3<double>{1.0, 2.0}.to<int>()));
	EXPECT_EQ((NAS2D::Vector3<int>{1, 2}), (NAS2D::Vector3<float>{1.4f, 2.6f}.to<int>()));
	EXPECT_EQ((NAS2D::Vector3<int>{1, 2}), (NAS2D::Vector3<double>{1.4, 2.6}.to<int>()));
	EXPECT_EQ((NAS2D::Vector3<int>{-4, -9}), (NAS2D::Vector3<float>{-3.4f, -8.6f}.to<int>()));
	EXPECT_EQ((NAS2D::Vector3<int>{3, 8}), (NAS2D::Vector3<double>{3.4, 8.6}.to<int>()));
	EXPECT_EQ((NAS2D::Vector3<float>{1.0, 2.0}), (NAS2D::Vector3<int>{1, 2}.to<float>()));
}

TEST(Vector3, scalarVector3Multiply) {
	EXPECT_EQ((NAS2D::Vector3{2, 4, 6}), (2 * NAS2D::Vector3{1, 2, 3}));
}

TEST(Vector3, PartialOrderLessEqual) {
	EXPECT_LE((NAS2D::Vector3{0, 0, 0}), (NAS2D::Vector3{1, 1, 1}));
	EXPECT_LE((NAS2D::Vector3{0, 0, 1}), (NAS2D::Vector3{1, 1, 1}));
	EXPECT_LE((NAS2D::Vector3{0, 1, 0}), (NAS2D::Vector3{1, 1, 1}));
	EXPECT_LE((NAS2D::Vector3{0, 1, 1}), (NAS2D::Vector3{1, 1, 1}));
	EXPECT_LE((NAS2D::Vector3{1, 0, 0}), (NAS2D::Vector3{1, 1, 1}));
	EXPECT_LE((NAS2D::Vector3{1, 0, 1}), (NAS2D::Vector3{1, 1, 1}));
	EXPECT_LE((NAS2D::Vector3{1, 1, 0}), (NAS2D::Vector3{1, 1, 1}));
	EXPECT_LE((NAS2D::Vector3{1, 1, 1}), (NAS2D::Vector3{1, 1, 1}));

	EXPECT_TRUE((NAS2D::Vector3{0, 1, 0}) <= (NAS2D::Vector3{1, 0, 0}));
	EXPECT_FALSE((NAS2D::Vector3{1, 0, 0}) <= (NAS2D::Vector3{0, 1, 0}));
}

TEST(Vector3, PartialOrderGreaterEqual) {
	EXPECT_GE((NAS2D::Vector3{1, 1, 0}), (NAS2D::Vector3{0, 0, 0}));
	EXPECT_GE((NAS2D::Vector3{1, 1, 0}), (NAS2D::Vector3{0, 1, 0}));
	EXPECT_GE((NAS2D::Vector3{1, 1, 0}), (NAS2D::Vector3{1, 0, 0}));
	EXPECT_GE((NAS2D::Vector3{1, 1, 0}), (NAS2D::Vector3{1, 1, 0}));
	EXPECT_GE((NAS2D::Vector3{0, 0, 1}), (NAS2D::Vector3{0, 0, 0}));
	EXPECT_GE((NAS2D::Vector3{1, 0, 1}), (NAS2D::Vector3{1, 0, 0}));
	EXPECT_GE((NAS2D::Vector3{0, 1, 1}), (NAS2D::Vector3{0, 1, 0}));
	EXPECT_GE((NAS2D::Vector3{1, 1, 1}), (NAS2D::Vector3{1, 1, 0}));
	EXPECT_GE((NAS2D::Vector3{0, 0, 1}), (NAS2D::Vector3{0, 0, 1}));
}

TEST(Vector3, PartialOrderLess) {
	EXPECT_LT((NAS2D::Vector3{0, 0, 0}), (NAS2D::Vector3{1, 1, 0}));
	EXPECT_LT((NAS2D::Vector3{0, 0, 1}), (NAS2D::Vector3{1, 1, 0}));

	EXPECT_TRUE((NAS2D::Vector3{0, 1, 1}) < (NAS2D::Vector3{1, 1, 0}));
	EXPECT_TRUE((NAS2D::Vector3{1, 0, 1}) < (NAS2D::Vector3{1, 1, 0}));
	EXPECT_FALSE((NAS2D::Vector3{1, 1, 1}) < (NAS2D::Vector3{1, 1, 0}));

	EXPECT_FALSE((NAS2D::Vector3{1, 0, 0}) < (NAS2D::Vector3{0, 1, 1}));
	EXPECT_FALSE((NAS2D::Vector3{1, 0, 1}) < (NAS2D::Vector3{0, 1, 0}));
}

TEST(Vector3, PartialOrderGreater) {
	EXPECT_GT((NAS2D::Vector3{1, 1, 0}), (NAS2D::Vector3{0, 0, 0}));

	EXPECT_TRUE((NAS2D::Vector3{1, 1, 0}) > (NAS2D::Vector3{0, 1, 0}));
	EXPECT_TRUE((NAS2D::Vector3{1, 1, 0}) > (NAS2D::Vector3{1, 0, 0}));
	EXPECT_FALSE((NAS2D::Vector3{1, 1, 0}) > (NAS2D::Vector3{1, 1, 0}));

	EXPECT_FALSE((NAS2D::Vector3{0, 1, 0}) > (NAS2D::Vector3{1, 0, 0}));
	EXPECT_TRUE((NAS2D::Vector3{1, 0, 0}) > (NAS2D::Vector3{0, 1, 0}));
}
