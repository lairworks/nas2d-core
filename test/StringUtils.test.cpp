#include "NAS2D/StringUtils.h"

#include <gtest/gtest.h>
#include <gmock/gmock.h>

#include <string>


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
