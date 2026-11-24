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
	using TypeParam = unsigned char;
	constexpr TypeParam max = std::numeric_limits<TypeParam>::max();

	EXPECT_EQ("0", NAS2D::stringFrom(TypeParam{0}));
	EXPECT_EQ(std::to_string(max), NAS2D::stringFrom(max));
}

TEST(StringFrom, integerUnsignedShort) {
	using TypeParam = unsigned short;
	constexpr TypeParam max = std::numeric_limits<TypeParam>::max();

	EXPECT_EQ("0", NAS2D::stringFrom(TypeParam{0}));
	EXPECT_EQ(std::to_string(max), NAS2D::stringFrom(max));
}

TEST(StringFrom, integerUnsignedInt) {
	using TypeParam = unsigned int;
	constexpr TypeParam max = std::numeric_limits<TypeParam>::max();

	EXPECT_EQ("0", NAS2D::stringFrom(TypeParam{0}));
	EXPECT_EQ(std::to_string(max), NAS2D::stringFrom(max));
}

TEST(StringFrom, integerUnsignedLong) {
	using TypeParam = unsigned long;
	constexpr TypeParam max = std::numeric_limits<TypeParam>::max();

	EXPECT_EQ("0", NAS2D::stringFrom(TypeParam{0}));
	EXPECT_EQ(std::to_string(max), NAS2D::stringFrom(max));
}

TEST(StringFrom, integerUnsignedLongLong) {
	using TypeParam = unsigned long long;
	constexpr TypeParam max = std::numeric_limits<TypeParam>::max();

	EXPECT_EQ("0", NAS2D::stringFrom(TypeParam{0}));
	EXPECT_EQ(std::to_string(max), NAS2D::stringFrom(max));
}

TEST(StringFrom, integerSignedChar) {
	using TypeParam = signed char;
	using Limits = std::numeric_limits<TypeParam>;

	constexpr TypeParam min = Limits::min();
	constexpr TypeParam max = Limits::max();

	EXPECT_EQ(std::to_string(min), NAS2D::stringFrom(min));
	EXPECT_EQ("-1", NAS2D::stringFrom(TypeParam{-1}));
	EXPECT_EQ("0", NAS2D::stringFrom(TypeParam{0}));
	EXPECT_EQ(std::to_string(max), NAS2D::stringFrom(max));
}

TEST(StringFrom, integerSignedShort) {
	using TypeParam = short;
	using Limits = std::numeric_limits<TypeParam>;

	constexpr TypeParam min = Limits::min();
	constexpr TypeParam max = Limits::max();

	EXPECT_EQ(std::to_string(min), NAS2D::stringFrom(min));
	EXPECT_EQ("-1", NAS2D::stringFrom(TypeParam{-1}));
	EXPECT_EQ("0", NAS2D::stringFrom(TypeParam{0}));
	EXPECT_EQ(std::to_string(max), NAS2D::stringFrom(max));
}

TEST(StringFrom, integerSignedInt) {
	using TypeParam = int;
	using Limits = std::numeric_limits<TypeParam>;

	constexpr TypeParam min = Limits::min();
	constexpr TypeParam max = Limits::max();

	EXPECT_EQ(std::to_string(min), NAS2D::stringFrom(min));
	EXPECT_EQ("-1", NAS2D::stringFrom(TypeParam{-1}));
	EXPECT_EQ("0", NAS2D::stringFrom(TypeParam{0}));
	EXPECT_EQ(std::to_string(max), NAS2D::stringFrom(max));
}

TEST(StringFrom, integerSignedLong) {
	using TypeParam = long;
	using Limits = std::numeric_limits<TypeParam>;

	constexpr TypeParam min = Limits::min();
	constexpr TypeParam max = Limits::max();

	EXPECT_EQ(std::to_string(min), NAS2D::stringFrom(min));
	EXPECT_EQ("-1", NAS2D::stringFrom(TypeParam{-1}));
	EXPECT_EQ("0", NAS2D::stringFrom(TypeParam{0}));
	EXPECT_EQ(std::to_string(max), NAS2D::stringFrom(max));
}

TEST(StringFrom, integerSignedLongLong) {
	using TypeParam = long long;
	using Limits = std::numeric_limits<TypeParam>;

	constexpr TypeParam min = Limits::min();
	constexpr TypeParam max = Limits::max();

	EXPECT_EQ(std::to_string(min), NAS2D::stringFrom(min));
	EXPECT_EQ("-1", NAS2D::stringFrom(TypeParam{-1}));
	EXPECT_EQ("0", NAS2D::stringFrom(TypeParam{0}));
	EXPECT_EQ(std::to_string(max), NAS2D::stringFrom(max));
}

TEST(StringFrom, floatingPointFloat) {
	EXPECT_THAT(NAS2D::stringFrom(-0.0f), testing::StartsWith("-0.0"));

	// Ignore precision beyond one decimal place
	EXPECT_THAT(NAS2D::stringFrom(0.0f), testing::StartsWith("0.0"));
	EXPECT_THAT(NAS2D::stringFrom(0.5f), testing::StartsWith("0.5"));
	EXPECT_THAT(NAS2D::stringFrom(1.0f), testing::StartsWith("1.0"));

	using Limits = std::numeric_limits<float>;
	EXPECT_NO_THROW(NAS2D::stringFrom(Limits::max()));
	EXPECT_NO_THROW(NAS2D::stringFrom(Limits::min()));
	EXPECT_NO_THROW(NAS2D::stringFrom(-Limits::min()));
	EXPECT_NO_THROW(NAS2D::stringFrom(Limits::lowest()));
	if constexpr (Limits::has_denorm != std::denorm_absent) {
		EXPECT_NO_THROW(NAS2D::stringFrom(Limits::denorm_min()));
		EXPECT_NO_THROW(NAS2D::stringFrom(-Limits::denorm_min()));
	}
}

TEST(StringFrom, floatingPointDouble) {
	EXPECT_THAT(NAS2D::stringFrom(-0.0), testing::StartsWith("-0.0"));

	// Ignore precision beyond one decimal place
	EXPECT_THAT(NAS2D::stringFrom(0.0), testing::StartsWith("0.0"));
	EXPECT_THAT(NAS2D::stringFrom(0.5), testing::StartsWith("0.5"));
	EXPECT_THAT(NAS2D::stringFrom(1.0), testing::StartsWith("1.0"));

	using Limits = std::numeric_limits<double>;
	EXPECT_NO_THROW(NAS2D::stringFrom(Limits::max()));
	EXPECT_NO_THROW(NAS2D::stringFrom(Limits::min()));
	EXPECT_NO_THROW(NAS2D::stringFrom(-Limits::min()));
	EXPECT_NO_THROW(NAS2D::stringFrom(Limits::lowest()));
	if constexpr (Limits::has_denorm != std::denorm_absent) {
		EXPECT_NO_THROW(NAS2D::stringFrom(Limits::denorm_min()));
		EXPECT_NO_THROW(NAS2D::stringFrom(-Limits::denorm_min()));
	}
}

TEST(StringFrom, floatingPointLongDouble) {
	EXPECT_THAT(NAS2D::stringFrom(-0.0l), testing::StartsWith("-0.0"));

	// Ignore precision beyond one decimal place
	EXPECT_THAT(NAS2D::stringFrom(0.0l), testing::StartsWith("0.0"));
	EXPECT_THAT(NAS2D::stringFrom(0.5l), testing::StartsWith("0.5"));
	EXPECT_THAT(NAS2D::stringFrom(1.0l), testing::StartsWith("1.0"));

	using Limits = std::numeric_limits<long double>;
	EXPECT_NO_THROW(NAS2D::stringFrom(Limits::max()));
	EXPECT_NO_THROW(NAS2D::stringFrom(Limits::min()));
	EXPECT_NO_THROW(NAS2D::stringFrom(-Limits::min()));
	EXPECT_NO_THROW(NAS2D::stringFrom(Limits::lowest()));
	if constexpr (Limits::has_denorm != std::denorm_absent) {
		EXPECT_NO_THROW(NAS2D::stringFrom(Limits::denorm_min()));
		EXPECT_NO_THROW(NAS2D::stringFrom(-Limits::denorm_min()));
	}
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
