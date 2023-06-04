#include "NAS2D/StringUtils.h"

#include <gmock/gmock.h>
#include <gtest/gtest.h>

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

TEST(String, stringTo) {
	EXPECT_EQ("SomeStringValue", NAS2D::stringTo<std::string>("SomeStringValue"));
	EXPECT_EQ("SomeStringValue", NAS2D::stringTo<std::string>(std::string{"SomeStringValue"}));

	EXPECT_THROW(NAS2D::stringTo<bool>(""), std::invalid_argument);
	EXPECT_THROW(NAS2D::stringTo<bool>("random text"), std::invalid_argument);
	EXPECT_EQ(false, NAS2D::stringTo<bool>("false"));
	EXPECT_EQ(true, NAS2D::stringTo<bool>("true"));
	EXPECT_EQ(false, NAS2D::stringTo<bool>("0"));
	EXPECT_EQ(true, NAS2D::stringTo<bool>("1"));

	EXPECT_THROW(NAS2D::stringTo<char>(std::to_string(std::numeric_limits<char>::min() - 1)), std::out_of_range);
	EXPECT_THROW(NAS2D::stringTo<char>(std::to_string(std::numeric_limits<char>::max() + 1)), std::out_of_range);
	EXPECT_THROW(NAS2D::stringTo<char>(""), std::invalid_argument);
	EXPECT_EQ(char{0}, NAS2D::stringTo<char>("0"));
	using signedChar = signed char;
	EXPECT_EQ(signedChar{-1}, NAS2D::stringTo<signedChar>("-1"));
	EXPECT_EQ(signedChar{0}, NAS2D::stringTo<signedChar>("0"));
	using unsignedChar = unsigned char;
	EXPECT_EQ(unsignedChar{0}, NAS2D::stringTo<unsignedChar>("0"));

	EXPECT_THROW(NAS2D::stringTo<short>(std::to_string(std::numeric_limits<short>::min() - 1)), std::out_of_range);
	EXPECT_THROW(NAS2D::stringTo<short>(std::to_string(std::numeric_limits<short>::max() + 1)), std::out_of_range);
	EXPECT_THROW(NAS2D::stringTo<short>(""), std::invalid_argument);
	EXPECT_EQ(short{-1}, NAS2D::stringTo<short>("-1"));
	EXPECT_EQ(short{0}, NAS2D::stringTo<short>("0"));
	using unsignedShort = unsigned short;
	EXPECT_EQ(unsignedShort{0}, NAS2D::stringTo<unsignedShort>("0"));

	EXPECT_THROW(NAS2D::stringTo<int>(""), std::invalid_argument);
	EXPECT_EQ(int{-1}, NAS2D::stringTo<int>("-1"));
	EXPECT_EQ(int{0}, NAS2D::stringTo<int>("0"));
	using unsignedInt = unsigned int;
	EXPECT_EQ(unsignedInt{0}, NAS2D::stringTo<unsignedInt>("0"));

	EXPECT_THROW(NAS2D::stringTo<long>(""), std::invalid_argument);
	EXPECT_EQ(long{-1}, NAS2D::stringTo<long>("-1"));
	EXPECT_EQ(long{0}, NAS2D::stringTo<long>("0"));
	using unsignedLong = unsigned long;
	EXPECT_EQ(unsignedLong{0}, NAS2D::stringTo<unsignedLong>("0"));

	using longLong = long long;
	EXPECT_THROW(NAS2D::stringTo<longLong>(""), std::invalid_argument);
	EXPECT_EQ(longLong{-1}, NAS2D::stringTo<longLong>("-1"));
	EXPECT_EQ(longLong{0}, NAS2D::stringTo<longLong>("0"));
	using unsignedLongLong = unsigned long long;
	EXPECT_EQ(unsignedLongLong{0}, NAS2D::stringTo<unsignedLongLong>("0"));

	EXPECT_THROW(NAS2D::stringTo<float>(""), std::invalid_argument);
	EXPECT_THROW(NAS2D::stringTo<double>(""), std::invalid_argument);
	EXPECT_THROW(NAS2D::stringTo<long double>(""), std::invalid_argument);
	EXPECT_EQ(0.0f, NAS2D::stringTo<float>("0.0"));
	EXPECT_EQ(0.0, NAS2D::stringTo<double>("0.0"));
	EXPECT_EQ(0.0l, NAS2D::stringTo<long double>("0.0"));
}

TEST(String, stringDataString) {
	EXPECT_EQ("Some string value", NAS2D::stringFrom<std::string>(NAS2D::stringTo<std::string>("Some string value")));

	EXPECT_EQ("false", NAS2D::stringFrom<bool>(NAS2D::stringTo<bool>("false")));
	EXPECT_EQ("true", NAS2D::stringFrom<bool>(NAS2D::stringTo<bool>("true")));

	EXPECT_EQ("-1", NAS2D::stringFrom<int>(NAS2D::stringTo<int>("-1")));
	EXPECT_EQ("0", NAS2D::stringFrom<int>(NAS2D::stringTo<int>("0")));

	EXPECT_THAT(NAS2D::stringFrom<float>(NAS2D::stringTo<float>("-1.0")), testing::StartsWith("-1.0"));
	EXPECT_THAT(NAS2D::stringFrom<float>(NAS2D::stringTo<float>("1.0")), testing::StartsWith("1.0"));
}

TEST(String, dataStringData) {
	EXPECT_EQ("Some string value", NAS2D::stringTo<std::string>(NAS2D::stringFrom<std::string>("Some string value")));

	EXPECT_EQ(false, NAS2D::stringTo<bool>(NAS2D::stringFrom<bool>(false)));
	EXPECT_EQ(true, NAS2D::stringTo<bool>(NAS2D::stringFrom<bool>(true)));

	EXPECT_EQ(-1, NAS2D::stringTo<int>(NAS2D::stringFrom<int>(-1)));
	EXPECT_EQ(0, NAS2D::stringTo<int>(NAS2D::stringFrom<int>(0)));
	EXPECT_EQ(1, NAS2D::stringTo<int>(NAS2D::stringFrom<int>(1)));

	EXPECT_EQ(-1, NAS2D::stringTo<float>(NAS2D::stringFrom<float>(-1)));
	EXPECT_EQ(0, NAS2D::stringTo<float>(NAS2D::stringFrom<float>(0)));
	EXPECT_EQ(1, NAS2D::stringTo<float>(NAS2D::stringFrom<float>(1)));
}


TEST(String, countDelimiters) {
	EXPECT_EQ(0u, NAS2D::countDelimiters(""));
	EXPECT_EQ(1u, NAS2D::countDelimiters(","));
	EXPECT_EQ(2u, NAS2D::countDelimiters(",,"));
	EXPECT_EQ(3u, NAS2D::countDelimiters(",,,"));

	EXPECT_EQ(0u, NAS2D::countDelimiters("a"));
	EXPECT_EQ(1u, NAS2D::countDelimiters("a,b"));
	EXPECT_EQ(2u, NAS2D::countDelimiters("a,b,c"));
	EXPECT_EQ(3u, NAS2D::countDelimiters("a,b,c,d"));

	EXPECT_EQ(3u, NAS2D::countDelimiters(",b,c,d"));
	EXPECT_EQ(3u, NAS2D::countDelimiters("a,,c,d"));
	EXPECT_EQ(3u, NAS2D::countDelimiters("a,b,,d"));
	EXPECT_EQ(3u, NAS2D::countDelimiters("a,b,c,"));

	EXPECT_EQ(0u, NAS2D::countDelimiters("", '.'));
	EXPECT_EQ(1u, NAS2D::countDelimiters(".", '.'));
	EXPECT_EQ(2u, NAS2D::countDelimiters("..", '.'));
	EXPECT_EQ(3u, NAS2D::countDelimiters("...", '.'));
}

TEST(String, split) {
	EXPECT_EQ((NAS2D::StringList{}), NAS2D::split(""));
	EXPECT_EQ((NAS2D::StringList{"a"}), NAS2D::split("a"));

	EXPECT_EQ((NAS2D::StringList{"a", "b", "c"}), NAS2D::split("a,b,c"));
	EXPECT_EQ((NAS2D::StringList{"abc"}), NAS2D::split("abc"));
	EXPECT_EQ((NAS2D::StringList{"", "abc"}), NAS2D::split(",abc"));
	EXPECT_EQ((NAS2D::StringList{"a", "bc"}), NAS2D::split("a,bc"));
	EXPECT_EQ((NAS2D::StringList{"ab", "c"}), NAS2D::split("ab,c"));
	EXPECT_EQ((NAS2D::StringList{"abc", ""}), NAS2D::split("abc,"));

	EXPECT_EQ((NAS2D::StringList{"a", "b", "c"}), NAS2D::split("a.b.c", '.'));
	EXPECT_EQ((NAS2D::StringList{"abc"}), NAS2D::split("abc", '.'));
	EXPECT_EQ((NAS2D::StringList{"", "abc"}), NAS2D::split(".abc", '.'));
	EXPECT_EQ((NAS2D::StringList{"a", "bc"}), NAS2D::split("a.bc", '.'));
	EXPECT_EQ((NAS2D::StringList{"ab", "c"}), NAS2D::split("ab.c", '.'));
	EXPECT_EQ((NAS2D::StringList{"abc", ""}), NAS2D::split("abc.", '.'));
}

TEST(String, join) {
	EXPECT_EQ("", NAS2D::join(NAS2D::StringList{}));
	EXPECT_EQ("a", NAS2D::join(NAS2D::StringList{"a"}));
	EXPECT_EQ("ab", NAS2D::join(NAS2D::StringList{"a", "b"}));
	EXPECT_EQ("abc", NAS2D::join(NAS2D::StringList{"a", "b", "c"}));

	EXPECT_EQ("ac", NAS2D::join(NAS2D::StringList{"a", "", "c"}));

	EXPECT_EQ("", NAS2D::join(NAS2D::StringList{}, ","));
	EXPECT_EQ("a", NAS2D::join(NAS2D::StringList{"a"}, ","));
	EXPECT_EQ("a,b", NAS2D::join(NAS2D::StringList{"a", "b"}, ","));
	EXPECT_EQ("a,b,c", NAS2D::join(NAS2D::StringList{"a", "b", "c"}, ","));

	EXPECT_EQ("a,,c", NAS2D::join(NAS2D::StringList{"a", "", "c"}, ","));

	EXPECT_EQ("a, b, c", NAS2D::join(NAS2D::StringList{"a", "b", "c"}, ", "));
}
