#include "NAS2D/Common.h"
#include <gtest/gtest.h>
#include <gmock/gmock.h>


TEST(Version, versionString) {
	EXPECT_THAT(NAS2D::versionString(), testing::MatchesRegex(R"([0-9]+\.[0-9]+\.[0-9]+)");
}

TEST(String, split)
{
	EXPECT_EQ((NAS2D::StringList{"a", "b", "c"}), NAS2D::split("a,b,c", ','));
	EXPECT_EQ((NAS2D::StringList{"abc"}), NAS2D::split("abc", ','));
	EXPECT_EQ((NAS2D::StringList{"", "abc"}), NAS2D::split(",abc", ','));
	EXPECT_EQ((NAS2D::StringList{"a", "bc"}), NAS2D::split("a,bc", ','));
	EXPECT_EQ((NAS2D::StringList{"ab", "c"}), NAS2D::split("ab,c", ','));
	// EXPECT_EQ((NAS2D::StringList{"abc", ""}), NAS2D::split("abc,", ','));  // Actual: {"abc"}

	EXPECT_EQ((NAS2D::StringList{"a", "b", "c"}), NAS2D::split("a.b.c", '.'));
	EXPECT_EQ((NAS2D::StringList{"abc"}), NAS2D::split("abc", '.'));
	EXPECT_EQ((NAS2D::StringList{"abc"}), NAS2D::split(".abc", '.'));
	EXPECT_EQ((NAS2D::StringList{"a", "bc"}), NAS2D::split("a.bc", '.'));
	EXPECT_EQ((NAS2D::StringList{"ab", "c"}), NAS2D::split("ab.c", '.'));
	EXPECT_EQ((NAS2D::StringList{"abc"}), NAS2D::split("abc.", '.'));
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
