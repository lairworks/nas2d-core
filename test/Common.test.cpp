#include "NAS2D/Common.h"

#include <gmock/gmock.h>
#include <gtest/gtest.h>

TEST(Version, versionString)
{
#if GTEST_USES_POSIX_RE == 1
	EXPECT_THAT(NAS2D::versionString(), testing::MatchesRegex(R"([0-9]+\.[0-9]+\.[0-9]+)"));
#elif GTEST_USES_SIMPLE_RE == 1
	EXPECT_THAT(NAS2D::versionString(), testing::MatchesRegex(R"(\d+\.\d+\.\d+)"));
#endif
}

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

TEST(String, SplitOnUnquotedSkipEmpty)
{
	using StringList = std::vector<std::string>;
	std::string input =
		R"(
a=b

c=d
e="Hello
World"
)";
	auto a = StringList{"a=b", "c=d", "e=\"Hello\nWorld\""};
	auto b = NAS2D::splitOnUnquotedSkipEmpty(input, '\n');
	EXPECT_EQ(a, b);
}

TEST(String, SplitOnUnquotedNoSkipEmpty)
{
	using StringList = std::vector<std::string>;
	std::string input =
		R"(
a=b

c=d
e="Hello
World"
)";
	auto a = StringList{"", "a=b", "", "c=d", "e=\"Hello\nWorld\"", ""};
	auto b = NAS2D::splitOnUnquoted(input, '\n');
	EXPECT_EQ(a, b);
}
