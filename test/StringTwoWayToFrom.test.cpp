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
		const auto max = std::to_string(Limits::max());

		EXPECT_EQ("0", NAS2D::stringFrom<TypeParam>(NAS2D::stringTo<TypeParam>("0")));
		EXPECT_EQ(max, NAS2D::stringFrom<TypeParam>(NAS2D::stringTo<TypeParam>(max)));
	}

	template <typename TypeParam>
	void testIntegerSigned() {
		using Limits = std::numeric_limits<TypeParam>;
		const auto min = std::to_string(Limits::min());
		const auto max = std::to_string(Limits::max());

		EXPECT_EQ(min, NAS2D::stringFrom<TypeParam>(NAS2D::stringTo<TypeParam>(min)));
		EXPECT_EQ("-1", NAS2D::stringFrom<TypeParam>(NAS2D::stringTo<TypeParam>("-1")));
		EXPECT_EQ("0", NAS2D::stringFrom<TypeParam>(NAS2D::stringTo<TypeParam>("0")));
		EXPECT_EQ(max, NAS2D::stringFrom<TypeParam>(NAS2D::stringTo<TypeParam>(max)));
	}

	template <typename TypeParam>
	void testFloatingPoint() {
		EXPECT_THAT(NAS2D::stringFrom<TypeParam>(NAS2D::stringTo<TypeParam>("-1.0")), testing::StartsWith("-1.0"));
		EXPECT_THAT(NAS2D::stringFrom<TypeParam>(NAS2D::stringTo<TypeParam>("-0.0")), testing::StartsWith("-0.0"));
		EXPECT_THAT(NAS2D::stringFrom<TypeParam>(NAS2D::stringTo<TypeParam>("0.0")), testing::StartsWith("0.0"));
		EXPECT_THAT(NAS2D::stringFrom<TypeParam>(NAS2D::stringTo<TypeParam>("1.0")), testing::StartsWith("1.0"));
	}
}


TEST(StringTwoWayToFrom, string) {
	EXPECT_EQ("Some string value", NAS2D::stringFrom<std::string>(NAS2D::stringTo<std::string>("Some string value")));
}

TEST(StringTwoWayToFrom, boolean) {
	EXPECT_EQ("false", NAS2D::stringFrom<bool>(NAS2D::stringTo<bool>("false")));
	EXPECT_EQ("true", NAS2D::stringFrom<bool>(NAS2D::stringTo<bool>("true")));
}


TEST(StringTwoWayToFrom, integerChar) {
	using TypeParam = char;
	using Limits = std::numeric_limits<TypeParam>;
	const auto min = std::to_string(Limits::min());
	const auto max = std::to_string(Limits::max());

	EXPECT_EQ(min, NAS2D::stringFrom<TypeParam>(NAS2D::stringTo<TypeParam>(min)));
	EXPECT_EQ(max, NAS2D::stringFrom<TypeParam>(NAS2D::stringTo<TypeParam>(max)));
}


TEST(StringTwoWayToFrom, integerUnsignedChar) {
	testIntegerUnsigned<unsigned char>();
}

TEST(StringTwoWayToFrom, integerUnsignedShort) {
	testIntegerUnsigned<unsigned short>();
}

TEST(StringTwoWayToFrom, integerUnsignedInt) {
	testIntegerUnsigned<unsigned int>();
}

TEST(StringTwoWayToFrom, integerUnsignedLong) {
	testIntegerUnsigned<unsigned long>();
}

TEST(StringTwoWayToFrom, integerUnsignedLongLong) {
	testIntegerUnsigned<unsigned long long>();
}


TEST(StringTwoWayToFrom, integerSignedChar) {
	testIntegerSigned<signed char>();
}

TEST(StringTwoWayToFrom, integerSignedShort) {
	testIntegerSigned<short>();
}

TEST(StringTwoWayToFrom, integerSignedInt) {
	testIntegerSigned<int>();
}

TEST(StringTwoWayToFrom, integerSignedLong) {
	testIntegerSigned<long>();
}

TEST(StringTwoWayToFrom, integerSignedLongLong) {
	testIntegerSigned<long long>();
}


TEST(StringTwoWayToFrom, floatingPoint) {
	testFloatingPoint<float>();
}

TEST(StringTwoWayToFrom, floatingPointDouble) {
	testFloatingPoint<double>();
}

TEST(StringTwoWayToFrom, floatingPointLongDouble) {
	testFloatingPoint<long double>();
}
