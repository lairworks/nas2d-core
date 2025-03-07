#include "NAS2D/StringFrom.h"

#include <gtest/gtest.h>
#include <gmock/gmock.h>

#include <string>


class ImplicitStringConversionTestFixture
{
public:
	std::string data;

	operator std::string() const
	{
		return data;
	}
};


TEST(String, stringFrom) {
	EXPECT_EQ("SomeStringValue", NAS2D::stringFrom("SomeStringValue"));
	EXPECT_EQ("SomeStringValue", NAS2D::stringFrom(std::string{"SomeStringValue"}));

	EXPECT_EQ("false", NAS2D::stringFrom(false));
	EXPECT_EQ("true", NAS2D::stringFrom(true));

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

	// Implicit string conversion
	ImplicitStringConversionTestFixture implicitStringConversionTestFixture{"testString"};
	EXPECT_EQ("testString", NAS2D::stringFrom(implicitStringConversionTestFixture));
}
