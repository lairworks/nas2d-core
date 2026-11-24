#include "NAS2D/StringFrom.h"

#include "NAS2D/Math/Point.h"
#include "NAS2D/Math/Vector.h"
#include "NAS2D/Math/Rectangle.h"

#include <gtest/gtest.h>
#include <gmock/gmock.h>

#include <string_view>
#include <string>
#include <limits>
#include <cmath>


namespace {
	template <typename TypeParam>
	void testIntegerUnsigned() {
		constexpr TypeParam max = std::numeric_limits<TypeParam>::max();

		EXPECT_EQ("0", NAS2D::stringFrom(TypeParam{0}));
		EXPECT_EQ(std::to_string(max), NAS2D::stringFrom(max));
	}

	template <typename TypeParam>
	void testIntegerSigned() {
		using Limits = std::numeric_limits<TypeParam>;

		constexpr TypeParam min = Limits::min();
		constexpr TypeParam max = Limits::max();

		EXPECT_EQ(std::to_string(min), NAS2D::stringFrom(min));
		EXPECT_EQ("-1", NAS2D::stringFrom(TypeParam{-1}));
		EXPECT_EQ("0", NAS2D::stringFrom(TypeParam{0}));
		EXPECT_EQ(std::to_string(max), NAS2D::stringFrom(max));
	}

	template <typename TypeParam>
	void testFloatingPoint() {
		EXPECT_THAT(NAS2D::stringFrom(TypeParam{-0.0l}), testing::StartsWith("-0.0"));

		// Ignore precision beyond one decimal place
		EXPECT_THAT(NAS2D::stringFrom(TypeParam{0.0l}), testing::StartsWith("0.0"));
		EXPECT_THAT(NAS2D::stringFrom(TypeParam{0.5l}), testing::StartsWith("0.5"));
		EXPECT_THAT(NAS2D::stringFrom(TypeParam{1.0l}), testing::StartsWith("1.0"));

		using Limits = std::numeric_limits<TypeParam>;
		EXPECT_NO_THROW(NAS2D::stringFrom(Limits::max()));
		EXPECT_NO_THROW(NAS2D::stringFrom(Limits::min()));
		EXPECT_NO_THROW(NAS2D::stringFrom(-Limits::min()));
		EXPECT_NO_THROW(NAS2D::stringFrom(Limits::lowest()));
		if constexpr (Limits::has_denorm != std::denorm_absent) {
			EXPECT_NO_THROW(NAS2D::stringFrom(Limits::denorm_min()));
			EXPECT_NO_THROW(NAS2D::stringFrom(-Limits::denorm_min()));
		}
	}
}


TEST(StringFrom, string) {
	EXPECT_EQ("SomeStringValue", NAS2D::stringFrom(std::string{"SomeStringValue"}));
}

TEST(StringFrom, stringView) {
	EXPECT_EQ("SomeStringValue", NAS2D::stringFrom(std::string_view{"SomeStringValue"}));
}

TEST(StringFrom, stringLiteral) {
	EXPECT_EQ("SomeStringValue", NAS2D::stringFrom("SomeStringValue"));
}


TEST(StringFrom, boolean) {
	EXPECT_EQ("false", NAS2D::stringFrom(false));
	EXPECT_EQ("true", NAS2D::stringFrom(true));
}


TEST(StringFrom, integerChar) {
	EXPECT_EQ("0", NAS2D::stringFrom(char{0}));
}


TEST(StringFrom, integerUnsignedChar) {
	testIntegerUnsigned<unsigned char>();
}

TEST(StringFrom, integerUnsignedShort) {
	testIntegerUnsigned<unsigned short>();
}

TEST(StringFrom, integerUnsignedInt) {
	testIntegerUnsigned<unsigned int>();
}

TEST(StringFrom, integerUnsignedLong) {
	testIntegerUnsigned<unsigned long>();
}

TEST(StringFrom, integerUnsignedLongLong) {
	testIntegerUnsigned<unsigned long long>();
}


TEST(StringFrom, integerSignedChar) {
	testIntegerSigned<signed char>();
}

TEST(StringFrom, integerSignedShort) {
	testIntegerSigned<short>();
}

TEST(StringFrom, integerSignedInt) {
	testIntegerSigned<int>();
}

TEST(StringFrom, integerSignedLong) {
	testIntegerSigned<long>();
}

TEST(StringFrom, integerSignedLongLong) {
	testIntegerSigned<long long>();
}


TEST(StringFrom, floatingPointFloat) {
	testFloatingPoint<float>();
}

TEST(StringFrom, floatingPointDouble) {
	testFloatingPoint<double>();
}

TEST(StringFrom, floatingPointLongDouble) {
	testFloatingPoint<long double>();
}


TEST(StringFrom, implicitConversion) {
	class ImplicitStringConversionTestFixture
	{
	public:
		std::string data;

		operator std::string() const
		{
			return data;
		}
	};

	ImplicitStringConversionTestFixture implicitStringConversionTestFixture{"testString"};
	EXPECT_EQ("testString", NAS2D::stringFrom(implicitStringConversionTestFixture));
}


TEST(StringFrom, point) {
	EXPECT_EQ("(0, 0)", NAS2D::stringFrom(NAS2D::Point{0, 0}));
	EXPECT_EQ("(1, 0)", NAS2D::stringFrom(NAS2D::Point{1, 0}));
	EXPECT_EQ("(0, 1)", NAS2D::stringFrom(NAS2D::Point{0, 1}));
}

TEST(StringFrom, vector) {
	EXPECT_EQ("(0, 0)", NAS2D::stringFrom(NAS2D::Vector{0, 0}));
	EXPECT_EQ("(1, 0)", NAS2D::stringFrom(NAS2D::Vector{1, 0}));
	EXPECT_EQ("(0, 1)", NAS2D::stringFrom(NAS2D::Vector{0, 1}));
}

TEST(StringFrom, rectangle) {
	EXPECT_EQ("((0, 0), (0, 0))", NAS2D::stringFrom(NAS2D::Rectangle{NAS2D::Point{0, 0}, NAS2D::Vector{0, 0}}));
	EXPECT_EQ("((1, 0), (2, 0))", NAS2D::stringFrom(NAS2D::Rectangle{NAS2D::Point{1, 0}, NAS2D::Vector{2, 0}}));
	EXPECT_EQ("((0, 1), (0, 2))", NAS2D::stringFrom(NAS2D::Rectangle{NAS2D::Point{0, 1}, NAS2D::Vector{0, 2}}));
}
