#include "NAS2D/StringFrom.h"

#include "NAS2D/Math/Point.h"
#include "NAS2D/Math/Vector.h"
#include "NAS2D/Math/Rectangle.h"

#include <gtest/gtest.h>
#include <gmock/gmock.h>

#include <string_view>
#include <string>
#include <limits>


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

TEST(StringFrom, integerCharUnsigned) {
	using unsignedChar = unsigned char;
	EXPECT_EQ("0", NAS2D::stringFrom(unsignedChar{0}));

	constexpr unsignedChar max = std::numeric_limits<unsignedChar>::max();
	EXPECT_EQ(std::to_string(max), NAS2D::stringFrom(max));
}

TEST(StringFrom, integerShortUnsigned) {
	using unsignedShort = unsigned short;
	EXPECT_EQ("0", NAS2D::stringFrom(unsignedShort{0}));

	constexpr unsignedShort max = std::numeric_limits<unsignedShort>::max();
	EXPECT_EQ(std::to_string(max), NAS2D::stringFrom(max));
}

TEST(StringFrom, integerIntUnsigned) {
	EXPECT_EQ("0", NAS2D::stringFrom(0u));

	constexpr unsigned int max = std::numeric_limits<unsigned int>::max();
	EXPECT_EQ(std::to_string(max), NAS2D::stringFrom(max));
}

TEST(StringFrom, integerLongUnsigned) {
	EXPECT_EQ("0", NAS2D::stringFrom(0ul));

	constexpr unsigned long max = std::numeric_limits<unsigned long>::max();
	EXPECT_EQ(std::to_string(max), NAS2D::stringFrom(max));
}

TEST(StringFrom, integerLongLongUnsigned) {
	EXPECT_EQ("0", NAS2D::stringFrom(0ull));

	constexpr unsigned long long max = std::numeric_limits<unsigned long long>::max();
	EXPECT_EQ(std::to_string(max), NAS2D::stringFrom(max));
}

TEST(StringFrom, integerCharSigned) {
	using signedChar = signed char;
	EXPECT_EQ("-1", NAS2D::stringFrom(signedChar{-1}));
	EXPECT_EQ("0", NAS2D::stringFrom(signedChar{0}));

	constexpr signedChar min = std::numeric_limits<signedChar>::min();
	EXPECT_EQ(std::to_string(min), NAS2D::stringFrom(min));
	constexpr signedChar max = std::numeric_limits<signedChar>::max();
	EXPECT_EQ(std::to_string(max), NAS2D::stringFrom(max));
}

TEST(StringFrom, integerShort) {
	EXPECT_EQ("-1", NAS2D::stringFrom(short{-1}));
	EXPECT_EQ("0", NAS2D::stringFrom(short{0}));

	constexpr short min = std::numeric_limits<short>::min();
	EXPECT_EQ(std::to_string(min), NAS2D::stringFrom(min));
	constexpr short max = std::numeric_limits<short>::max();
	EXPECT_EQ(std::to_string(max), NAS2D::stringFrom(max));
}

TEST(StringFrom, integerInt) {
	EXPECT_EQ("-1", NAS2D::stringFrom(-1));
	EXPECT_EQ("0", NAS2D::stringFrom(0));

	constexpr int min = std::numeric_limits<int>::min();
	EXPECT_EQ(std::to_string(min), NAS2D::stringFrom(min));
	constexpr int max = std::numeric_limits<int>::max();
	EXPECT_EQ(std::to_string(max), NAS2D::stringFrom(max));
}

TEST(StringFrom, integerLong) {
	EXPECT_EQ("-1", NAS2D::stringFrom(-1l));
	EXPECT_EQ("0", NAS2D::stringFrom(0l));

	constexpr long min = std::numeric_limits<long>::min();
	EXPECT_EQ(std::to_string(min), NAS2D::stringFrom(min));
	constexpr long max = std::numeric_limits<long>::max();
	EXPECT_EQ(std::to_string(max), NAS2D::stringFrom(max));
}

TEST(StringFrom, integerLongLong) {
	EXPECT_EQ("-1", NAS2D::stringFrom(-1ll));
	EXPECT_EQ("0", NAS2D::stringFrom(0ll));

	constexpr long long min = std::numeric_limits<long long>::min();
	EXPECT_EQ(std::to_string(min), NAS2D::stringFrom(min));
	constexpr long long max = std::numeric_limits<long long>::max();
	EXPECT_EQ(std::to_string(max), NAS2D::stringFrom(max));
}

TEST(StringFrom, floatingPointFloat) {
	// Ignore precision beyond one decimal place
	EXPECT_THAT(NAS2D::stringFrom(0.0f), testing::StartsWith("0.0"));
	EXPECT_THAT(NAS2D::stringFrom(0.5f), testing::StartsWith("0.5"));
	EXPECT_THAT(NAS2D::stringFrom(1.0f), testing::StartsWith("1.0"));
}

TEST(StringFrom, floatingPointDouble) {
	// Ignore precision beyond one decimal place
	EXPECT_THAT(NAS2D::stringFrom(0.0), testing::StartsWith("0.0"));
	EXPECT_THAT(NAS2D::stringFrom(0.5), testing::StartsWith("0.5"));
	EXPECT_THAT(NAS2D::stringFrom(1.0), testing::StartsWith("1.0"));
}

TEST(StringFrom, floatingPointLongDouble) {
	// Ignore precision beyond one decimal place
	EXPECT_THAT(NAS2D::stringFrom(0.0l), testing::StartsWith("0.0"));
	EXPECT_THAT(NAS2D::stringFrom(0.5l), testing::StartsWith("0.5"));
	EXPECT_THAT(NAS2D::stringFrom(1.0l), testing::StartsWith("1.0"));
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
