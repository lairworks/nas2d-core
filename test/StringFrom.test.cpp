#include "NAS2D/StringFrom.h"

#include "NAS2D/Math/Point.h"
#include "NAS2D/Math/Vector.h"

#include <gtest/gtest.h>
#include <gmock/gmock.h>

#include <string>


namespace
{
	class ImplicitStringConversionTestFixture
	{
	public:
		std::string data;

		operator std::string() const
		{
			return data;
		}
	};
}


TEST(StringFrom, implicitConversion) {
	ImplicitStringConversionTestFixture implicitStringConversionTestFixture{"testString"};
	EXPECT_EQ("testString", NAS2D::stringFrom(implicitStringConversionTestFixture));
}

TEST(StringFrom, string) {
	EXPECT_EQ("SomeStringValue", NAS2D::stringFrom("SomeStringValue"));
	EXPECT_EQ("SomeStringValue", NAS2D::stringFrom(std::string{"SomeStringValue"}));
}

TEST(StringFrom, boolean) {
	EXPECT_EQ("false", NAS2D::stringFrom(false));
	EXPECT_EQ("true", NAS2D::stringFrom(true));
}

TEST(StringFrom, stringFrom) {
	using signedChar = signed char;
	using unsignedChar = unsigned char;
	EXPECT_EQ("-1", NAS2D::stringFrom(signedChar{-1}));
	EXPECT_EQ("0", NAS2D::stringFrom(signedChar{0}));
	EXPECT_EQ("0", NAS2D::stringFrom(unsignedChar{0}));
	EXPECT_EQ("0", NAS2D::stringFrom(char{0}));

	using unsignedShort = unsigned short;
	EXPECT_EQ("-1", NAS2D::stringFrom(short{-1}));
	EXPECT_EQ("0", NAS2D::stringFrom(short{0}));
	EXPECT_EQ("0", NAS2D::stringFrom(unsignedShort{0}));

	EXPECT_EQ("-1", NAS2D::stringFrom(-1));
	EXPECT_EQ("0", NAS2D::stringFrom(0));
	EXPECT_EQ("1", NAS2D::stringFrom(1u));

	EXPECT_EQ("-1", NAS2D::stringFrom(-1l));
	EXPECT_EQ("0", NAS2D::stringFrom(0l));
	EXPECT_EQ("1", NAS2D::stringFrom(1ul));

	EXPECT_EQ("-1", NAS2D::stringFrom(-1ll));
	EXPECT_EQ("0", NAS2D::stringFrom(0ll));
	EXPECT_EQ("1", NAS2D::stringFrom(1ull));

	// Ignore precision beyond one decimal place
	EXPECT_THAT(NAS2D::stringFrom(0.0f), testing::StartsWith("0.0"));
	EXPECT_THAT(NAS2D::stringFrom(0.0), testing::StartsWith("0.0"));
	EXPECT_THAT(NAS2D::stringFrom(0.0l), testing::StartsWith("0.0"));
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
