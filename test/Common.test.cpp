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

TEST(String, splitOnUnquoted)
{
	EXPECT_EQ((NAS2D::StringList{"a", "b", "c"}), NAS2D::splitOnUnquoted("a,b,c"));
	EXPECT_EQ((NAS2D::StringList{"abc"}), NAS2D::splitOnUnquoted("abc"));
	EXPECT_EQ((NAS2D::StringList{"", "abc"}), NAS2D::splitOnUnquoted(",abc"));
	EXPECT_EQ((NAS2D::StringList{"a", "bc"}), NAS2D::splitOnUnquoted("a,bc"));
	EXPECT_EQ((NAS2D::StringList{"ab", "c"}), NAS2D::splitOnUnquoted("ab,c"));
	EXPECT_EQ((NAS2D::StringList{"abc", ""}), NAS2D::splitOnUnquoted("abc,"));
	EXPECT_EQ((NAS2D::StringList{R"("a,b,c")", "abc", "def"}), NAS2D::splitOnUnquoted(R"("a,b,c",abc,def)"));
	EXPECT_EQ((NAS2D::StringList{"abc", R"("a,b,c")", "def"}), NAS2D::splitOnUnquoted(R"(abc,"a,b,c",def)"));
	EXPECT_EQ((NAS2D::StringList{"abc", "def", R"("a,b,c")"}), NAS2D::splitOnUnquoted(R"(abc,def,"a,b,c")"));
	EXPECT_EQ((NAS2D::StringList{"", "abc", "def"}), NAS2D::splitOnUnquoted(R"("",abc,def)"));
	EXPECT_EQ((NAS2D::StringList{"abc", "", "def"}), NAS2D::splitOnUnquoted(R"(abc,"",def)"));
	EXPECT_EQ((NAS2D::StringList{"abc", "def", ""}), NAS2D::splitOnUnquoted(R"(abc,def,"")"));

	EXPECT_EQ((NAS2D::StringList{"a", "b", "c"}), NAS2D::splitOnUnquoted("a.b.c", '.'));
	EXPECT_EQ((NAS2D::StringList{"abc"}), NAS2D::splitOnUnquoted("abc", '.'));
	EXPECT_EQ((NAS2D::StringList{"", "abc"}), NAS2D::splitOnUnquoted(".abc", '.'));
	EXPECT_EQ((NAS2D::StringList{"a", "bc"}), NAS2D::splitOnUnquoted("a.bc", '.'));
	EXPECT_EQ((NAS2D::StringList{"ab", "c"}), NAS2D::splitOnUnquoted("ab.c", '.'));
	EXPECT_EQ((NAS2D::StringList{"abc", ""}), NAS2D::splitOnUnquoted("abc.", '.'));
	EXPECT_EQ((NAS2D::StringList{R"("a.b.c")", "abc", "def"}), NAS2D::splitOnUnquoted(R"("a.b.c".abc.def)", '.'));
	EXPECT_EQ((NAS2D::StringList{"abc", R"("a.b.c")", "def"}), NAS2D::splitOnUnquoted(R"(abc."a.b.c".def)", '.'));
	EXPECT_EQ((NAS2D::StringList{"abc", "def", R"("a.b.c")"}), NAS2D::splitOnUnquoted(R"(abc.def."a.b.c")", '.'));
	EXPECT_EQ((NAS2D::StringList{"", "abc", "def"}), NAS2D::splitOnUnquoted(R"("".abc.def)", '.'));
	EXPECT_EQ((NAS2D::StringList{"abc", "", "def"}), NAS2D::splitOnUnquoted(R"(abc."".def)", '.'));
	EXPECT_EQ((NAS2D::StringList{"abc", "def", ""}), NAS2D::splitOnUnquoted(R"(abc.def."")", '.'));
	EXPECT_EQ((NAS2D::StringList{"Hello", R"("Hello World")", "World"}), NAS2D::splitOnUnquoted(R"(Hello "Hello World" World)", ' '));
	EXPECT_EQ((NAS2D::StringList{"Hello", R"(" ")", "World"}), NAS2D::splitOnUnquoted(R"(Hello " " World)", ' '));
	EXPECT_EQ((NAS2D::StringList{"Hello", "", "World"}), NAS2D::splitOnUnquoted(R"(Hello "" World)", ' '));

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

TEST(String, splitOnUnquotedSkipEmpty)
{
	EXPECT_EQ((NAS2D::StringList{"a", "b", "c"}), NAS2D::splitOnUnquotedSkipEmpty("a,b,c"));
	EXPECT_EQ((NAS2D::StringList{"abc"}), NAS2D::splitOnUnquotedSkipEmpty("abc"));
	EXPECT_EQ((NAS2D::StringList{"abc"}), NAS2D::splitOnUnquotedSkipEmpty(",abc"));
	EXPECT_EQ((NAS2D::StringList{"a", "bc"}), NAS2D::splitOnUnquotedSkipEmpty("a,bc"));
	EXPECT_EQ((NAS2D::StringList{"ab", "c"}), NAS2D::splitOnUnquotedSkipEmpty("ab,c"));
	EXPECT_EQ((NAS2D::StringList{"abc"}), NAS2D::splitOnUnquotedSkipEmpty("abc,"));
	EXPECT_EQ((NAS2D::StringList{"abc", "def"}), NAS2D::splitOnUnquotedSkipEmpty("\"\",abc,def"));
	EXPECT_EQ((NAS2D::StringList{"abc", "def"}), NAS2D::splitOnUnquotedSkipEmpty("abc,\"\",def"));
	EXPECT_EQ((NAS2D::StringList{"abc", "def"}), NAS2D::splitOnUnquotedSkipEmpty("abc,def,\"\""));

	EXPECT_EQ((NAS2D::StringList{"a", "b", "c"}), NAS2D::splitOnUnquotedSkipEmpty("a.b.c", '.'));
	EXPECT_EQ((NAS2D::StringList{"abc"}), NAS2D::splitOnUnquotedSkipEmpty("abc", '.'));
	EXPECT_EQ((NAS2D::StringList{"abc"}), NAS2D::splitOnUnquotedSkipEmpty(".abc", '.'));
	EXPECT_EQ((NAS2D::StringList{"a", "bc"}), NAS2D::splitOnUnquotedSkipEmpty("a.bc", '.'));
	EXPECT_EQ((NAS2D::StringList{"ab", "c"}), NAS2D::splitOnUnquotedSkipEmpty("ab.c", '.'));
	EXPECT_EQ((NAS2D::StringList{"abc"}), NAS2D::splitOnUnquotedSkipEmpty("abc.", '.'));
	EXPECT_EQ((NAS2D::StringList{"abc", "def"}), NAS2D::splitOnUnquotedSkipEmpty("\"\".abc.def", '.'));
	EXPECT_EQ((NAS2D::StringList{"abc", "def"}), NAS2D::splitOnUnquotedSkipEmpty("abc.\"\".def", '.'));
	EXPECT_EQ((NAS2D::StringList{"abc", "def"}), NAS2D::splitOnUnquotedSkipEmpty("abc.def.\"\"", '.'));
	EXPECT_EQ((NAS2D::StringList{"Hello", " ", "World"}), NAS2D::splitOnUnquotedSkipEmpty("Hello \" \" World", ' '));
	EXPECT_EQ((NAS2D::StringList{"Hello", "World"}), NAS2D::splitOnUnquotedSkipEmpty("Hello \"\" World", ' '));
}
