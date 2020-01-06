#include "NAS2D/StringUtils.h"

#include <gmock/gmock.h>
#include <gtest/gtest.h>

TEST(String, split)
{
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

TEST(String, splitSkipEmpty)
{
	EXPECT_EQ((NAS2D::StringList{"a", "b", "c"}), NAS2D::splitSkipEmpty("a,b,c"));
	EXPECT_EQ((NAS2D::StringList{"abc"}), NAS2D::splitSkipEmpty("abc"));
	EXPECT_EQ((NAS2D::StringList{"abc"}), NAS2D::splitSkipEmpty(",abc"));
	EXPECT_EQ((NAS2D::StringList{"a", "bc"}), NAS2D::splitSkipEmpty("a,bc"));
	EXPECT_EQ((NAS2D::StringList{"ab", "c"}), NAS2D::splitSkipEmpty("ab,c"));
	EXPECT_EQ((NAS2D::StringList{"abc"}), NAS2D::splitSkipEmpty("abc,"));

	EXPECT_EQ((NAS2D::StringList{"a", "b", "c"}), NAS2D::splitSkipEmpty("a.b.c", '.'));
	EXPECT_EQ((NAS2D::StringList{"abc"}), NAS2D::splitSkipEmpty("abc", '.'));
	EXPECT_EQ((NAS2D::StringList{"abc"}), NAS2D::splitSkipEmpty(".abc", '.'));
	EXPECT_EQ((NAS2D::StringList{"a", "bc"}), NAS2D::splitSkipEmpty("a.bc", '.'));
	EXPECT_EQ((NAS2D::StringList{"ab", "c"}), NAS2D::splitSkipEmpty("ab.c", '.'));
	EXPECT_EQ((NAS2D::StringList{"abc"}), NAS2D::splitSkipEmpty("abc.", '.'));
}

TEST(String, string_format)
{
	// Numbers
	EXPECT_EQ("-1", NAS2D::string_format("%d", -1));
	EXPECT_EQ("0", NAS2D::string_format("%d", 0));
	EXPECT_EQ("1", NAS2D::string_format("%d", 1));

	// Padded numbers
	EXPECT_EQ(" 10", NAS2D::string_format("%3d", 10));
	EXPECT_EQ("010", NAS2D::string_format("%03d", 10));
	// Negative padded numbers
	EXPECT_EQ("-10", NAS2D::string_format("%3d", -10));
	EXPECT_EQ("-10", NAS2D::string_format("%03d", -10));
	EXPECT_EQ(" -10", NAS2D::string_format("%4d", -10));
	EXPECT_EQ("-010", NAS2D::string_format("%04d", -10));

	// Strings
	EXPECT_EQ("Hello World", NAS2D::string_format("Hello %s", "World"));

	// Padded strings
	EXPECT_EQ("  ABC", NAS2D::string_format("%5s", "ABC"));
}
