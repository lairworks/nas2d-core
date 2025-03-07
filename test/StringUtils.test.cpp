#include "NAS2D/StringUtils.h"

#include <gtest/gtest.h>
#include <gmock/gmock.h>

#include <string>


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
	using StringList = std::vector<std::string>;

	EXPECT_EQ((StringList{}), NAS2D::split(""));
	EXPECT_EQ((StringList{"a"}), NAS2D::split("a"));

	EXPECT_EQ((StringList{"a", "b", "c"}), NAS2D::split("a,b,c"));
	EXPECT_EQ((StringList{"abc"}), NAS2D::split("abc"));
	EXPECT_EQ((StringList{"", "abc"}), NAS2D::split(",abc"));
	EXPECT_EQ((StringList{"a", "bc"}), NAS2D::split("a,bc"));
	EXPECT_EQ((StringList{"ab", "c"}), NAS2D::split("ab,c"));
	EXPECT_EQ((StringList{"abc", ""}), NAS2D::split("abc,"));

	EXPECT_EQ((StringList{"a", "b", "c"}), NAS2D::split("a.b.c", '.'));
	EXPECT_EQ((StringList{"abc"}), NAS2D::split("abc", '.'));
	EXPECT_EQ((StringList{"", "abc"}), NAS2D::split(".abc", '.'));
	EXPECT_EQ((StringList{"a", "bc"}), NAS2D::split("a.bc", '.'));
	EXPECT_EQ((StringList{"ab", "c"}), NAS2D::split("ab.c", '.'));
	EXPECT_EQ((StringList{"abc", ""}), NAS2D::split("abc.", '.'));
}

TEST(String, join) {
	using StringList = std::vector<std::string>;

	EXPECT_EQ("", NAS2D::join(StringList{}));
	EXPECT_EQ("a", NAS2D::join(StringList{"a"}));
	EXPECT_EQ("ab", NAS2D::join(StringList{"a", "b"}));
	EXPECT_EQ("abc", NAS2D::join(StringList{"a", "b", "c"}));

	EXPECT_EQ("ac", NAS2D::join(StringList{"a", "", "c"}));

	EXPECT_EQ("", NAS2D::join(StringList{}, ","));
	EXPECT_EQ("a", NAS2D::join(StringList{"a"}, ","));
	EXPECT_EQ("a,b", NAS2D::join(StringList{"a", "b"}, ","));
	EXPECT_EQ("a,b,c", NAS2D::join(StringList{"a", "b", "c"}, ","));

	EXPECT_EQ("a,,c", NAS2D::join(StringList{"a", "", "c"}, ","));

	EXPECT_EQ("a, b, c", NAS2D::join(StringList{"a", "b", "c"}, ", "));
}
