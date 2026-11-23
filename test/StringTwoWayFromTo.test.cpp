#include "NAS2D/StringFrom.h"
#include "NAS2D/StringTo.h"

#include <gtest/gtest.h>
#include <gmock/gmock.h>

#include <string>
#include <limits>


namespace {
	template <typename TypeParam>
	void testIntegerUnsigned() {
		using Limits = std::numeric_limits<TypeParam>;

		constexpr auto zero = TypeParam{0};
		constexpr auto one = TypeParam{1};
		constexpr auto max = Limits::max();

		EXPECT_EQ(zero, NAS2D::stringTo<TypeParam>(NAS2D::stringFrom<TypeParam>(zero)));
		EXPECT_EQ(one, NAS2D::stringTo<TypeParam>(NAS2D::stringFrom<TypeParam>(one)));
		EXPECT_EQ(max, NAS2D::stringTo<TypeParam>(NAS2D::stringFrom<TypeParam>(max)));
	}

	template <typename TypeParam>
	void testIntegerSigned() {
		using Limits = std::numeric_limits<TypeParam>;

		constexpr auto min = Limits::min();
		constexpr auto max = Limits::max();

		EXPECT_EQ(min, NAS2D::stringTo<TypeParam>(NAS2D::stringFrom<TypeParam>(min)));
		EXPECT_EQ(-1, NAS2D::stringTo<TypeParam>(NAS2D::stringFrom<TypeParam>(-1)));
		EXPECT_EQ(0, NAS2D::stringTo<TypeParam>(NAS2D::stringFrom<TypeParam>(0)));
		EXPECT_EQ(1, NAS2D::stringTo<TypeParam>(NAS2D::stringFrom<TypeParam>(1)));
		EXPECT_EQ(max, NAS2D::stringTo<TypeParam>(NAS2D::stringFrom<TypeParam>(max)));
	}

	template <typename TypeParam>
	void testFloatingPoint() {
		EXPECT_EQ(TypeParam{-1.0L}, NAS2D::stringTo<TypeParam>(NAS2D::stringFrom<TypeParam>(TypeParam{-1.0L})));
		EXPECT_EQ(TypeParam{-0.0L}, NAS2D::stringTo<TypeParam>(NAS2D::stringFrom<TypeParam>(TypeParam{-0.0L})));
		EXPECT_EQ(TypeParam{0.0L}, NAS2D::stringTo<TypeParam>(NAS2D::stringFrom<TypeParam>(TypeParam{0.0L})));
		EXPECT_EQ(TypeParam{1.0L}, NAS2D::stringTo<TypeParam>(NAS2D::stringFrom<TypeParam>(TypeParam{1.0L})));
	}
}


TEST(StringTwoWayFromTo, string) {
	EXPECT_EQ("Some string value", NAS2D::stringTo<std::string>(NAS2D::stringFrom<std::string>("Some string value")));
}

TEST(StringTwoWayFromTo, boolean) {
	EXPECT_EQ(false, NAS2D::stringTo<bool>(NAS2D::stringFrom<bool>(false)));
	EXPECT_EQ(true, NAS2D::stringTo<bool>(NAS2D::stringFrom<bool>(true)));
}


TEST(StringTwoWayFromTo, integerChar) {
	using TypeParam = char;
	using Limits = std::numeric_limits<TypeParam>;

	constexpr auto min = Limits::min();
	constexpr auto max = Limits::max();

	EXPECT_EQ(min, NAS2D::stringTo<TypeParam>(NAS2D::stringFrom<TypeParam>(min)));
	EXPECT_EQ(max, NAS2D::stringTo<TypeParam>(NAS2D::stringFrom<TypeParam>(max)));
}


TEST(StringTwoWayFromTo, integerUnsignedChar) {
	testIntegerUnsigned<unsigned char>();
}

TEST(StringTwoWayFromTo, integerUnsignedShort) {
	testIntegerUnsigned<unsigned short>();
}

TEST(StringTwoWayFromTo, integerUnsignedInt) {
	testIntegerUnsigned<unsigned int>();
}

TEST(StringTwoWayFromTo, integerUnsignedLong) {
	testIntegerUnsigned<unsigned long>();
}

TEST(StringTwoWayFromTo, integerUnsignedLongLong) {
	testIntegerUnsigned<unsigned long long>();
}


TEST(StringTwoWayFromTo, integerSignedChar) {
	testIntegerSigned<signed char>();
}

TEST(StringTwoWayFromTo, integerSignedShort) {
	testIntegerSigned<short>();
}

TEST(StringTwoWayFromTo, integerSignedInt) {
	testIntegerSigned<int>();
}

TEST(StringTwoWayFromTo, integerSignedLong) {
	testIntegerSigned<long>();
}

TEST(StringTwoWayFromTo, integerSignedLongLong) {
	testIntegerSigned<long long>();
}


TEST(StringTwoWayFromTo, floatingPointFloat) {
	testFloatingPoint<float>();
}

TEST(StringTwoWayFromTo, floatingPointDouble) {
	testFloatingPoint<double>();
}

TEST(StringTwoWayFromTo, floatingPointLongDouble) {
	testFloatingPoint<long double>();
}
