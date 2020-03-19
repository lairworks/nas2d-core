#include "NAS2D/StringUtils.h"

#include <gmock/gmock.h>
#include <gtest/gtest.h>

TEST(String, stringFrom)
{
	const std::string bigNum(40, '9');

	EXPECT_EQ(std::string{""}, NAS2D::stringFrom<std::string>(""));
	EXPECT_EQ(std::string{"Hello World"}, NAS2D::stringFrom<std::string>("Hello World"));

	EXPECT_EQ(true, NAS2D::stringFrom<bool>("true"));
	EXPECT_EQ(false, NAS2D::stringFrom<bool>("false"));
	EXPECT_THROW(NAS2D::stringFrom<bool>("foobar"), std::invalid_argument);

	EXPECT_EQ(char{0}, NAS2D::stringFrom<char>("0"));
	EXPECT_EQ(unsigned char{129}, NAS2D::stringFrom<unsigned char>("129"));
	EXPECT_EQ(signed char{-1}, NAS2D::stringFrom<signed char>("-1"));

	EXPECT_EQ(unsigned short{0}, NAS2D::stringFrom<unsigned short>("0"));
	EXPECT_EQ(unsigned short{1}, NAS2D::stringFrom<unsigned short>("1"));
	EXPECT_THROW(NAS2D::stringFrom<unsigned short>(""), std::invalid_argument);
	EXPECT_THROW(NAS2D::stringFrom<unsigned short>(bigNum), std::out_of_range);

	EXPECT_EQ(unsigned int{0}, NAS2D::stringFrom<unsigned int>("0"));
	EXPECT_EQ(unsigned int{1}, NAS2D::stringFrom<unsigned int>("1"));
	EXPECT_THROW(NAS2D::stringFrom<unsigned int>(""), std::invalid_argument);
	EXPECT_THROW(NAS2D::stringFrom<unsigned int>(bigNum), std::out_of_range);

	EXPECT_EQ(unsigned long{0}, NAS2D::stringFrom<unsigned long>("0"));
	EXPECT_EQ(unsigned long{1}, NAS2D::stringFrom<unsigned long>("1"));
	EXPECT_THROW(NAS2D::stringFrom<unsigned long>(""), std::invalid_argument);
	EXPECT_THROW(NAS2D::stringFrom<unsigned long>(bigNum), std::out_of_range);

	EXPECT_EQ(unsigned long long{0}, NAS2D::stringFrom<unsigned long long>("0"));
	EXPECT_EQ(unsigned long long{1}, NAS2D::stringFrom<unsigned long long>("1"));
	EXPECT_THROW(NAS2D::stringFrom<unsigned long long>(""), std::invalid_argument);
	EXPECT_THROW(NAS2D::stringFrom<unsigned long long>(bigNum), std::out_of_range);
	
	EXPECT_EQ(short{-1}, NAS2D::stringFrom<short>("-1"));
	EXPECT_EQ(short{0}, NAS2D::stringFrom<short>("0"));
	EXPECT_EQ(short{1}, NAS2D::stringFrom<short>("1"));
	EXPECT_THROW(NAS2D::stringFrom<short>(""), std::invalid_argument);
	EXPECT_THROW(NAS2D::stringFrom<short>(bigNum), std::out_of_range);

	EXPECT_EQ(int{-1}, NAS2D::stringFrom<int>("-1"));
	EXPECT_EQ(int{0}, NAS2D::stringFrom<int>("0"));
	EXPECT_EQ(int{1}, NAS2D::stringFrom<int>("1"));
	EXPECT_THROW(NAS2D::stringFrom<int>(""), std::invalid_argument);
	EXPECT_THROW(NAS2D::stringFrom<int>(bigNum), std::out_of_range);

	EXPECT_THROW(NAS2D::stringFrom<long>("-" + bigNum), std::out_of_range);
	EXPECT_EQ(long{-1}, NAS2D::stringFrom<long>("-1"));
	EXPECT_EQ(long{0}, NAS2D::stringFrom<long>("0"));
	EXPECT_EQ(long{1}, NAS2D::stringFrom<long>("1"));
	EXPECT_THROW(NAS2D::stringFrom<long>(""), std::invalid_argument);
	EXPECT_THROW(NAS2D::stringFrom<long>(bigNum), std::out_of_range);

	EXPECT_THROW(NAS2D::stringFrom<long long>("-" + bigNum), std::out_of_range);
	EXPECT_EQ(long long{-1}, NAS2D::stringFrom<long long>("-1"));
	EXPECT_EQ(long long{0}, NAS2D::stringFrom<long long>("0"));
	EXPECT_EQ(long long{1}, NAS2D::stringFrom<long long>("1"));
	EXPECT_THROW(NAS2D::stringFrom<long long>(""), std::invalid_argument);
	EXPECT_THROW(NAS2D::stringFrom<long long>(bigNum), std::out_of_range);

	EXPECT_THROW(NAS2D::stringFrom<float>("-" + bigNum), std::out_of_range);
	EXPECT_EQ(float{-1.0}, NAS2D::stringFrom<float>("-1"));
	EXPECT_EQ(float{-1.0}, NAS2D::stringFrom<float>("-1.0"));
	EXPECT_EQ(float{0.0}, NAS2D::stringFrom<float>("0"));
	EXPECT_EQ(float{0.0}, NAS2D::stringFrom<float>("0.0"));
	EXPECT_EQ(float{1.0}, NAS2D::stringFrom<float>("1"));
	EXPECT_EQ(float{1.0}, NAS2D::stringFrom<float>("1.0"));
	EXPECT_THROW(NAS2D::stringFrom<float>(""), std::invalid_argument);
	EXPECT_THROW(NAS2D::stringFrom<float>(bigNum), std::out_of_range);

	EXPECT_EQ(double{-1.0}, NAS2D::stringFrom<double>("-1"));
	EXPECT_EQ(double{-1.0}, NAS2D::stringFrom<double>("-1.0"));
	EXPECT_EQ(double{0.0}, NAS2D::stringFrom<double>("0"));
	EXPECT_EQ(double{0.0}, NAS2D::stringFrom<double>("0.0"));
	EXPECT_EQ(double{1.0}, NAS2D::stringFrom<double>("1"));
	EXPECT_EQ(double{1.0}, NAS2D::stringFrom<double>("1.0"));
	EXPECT_THROW(NAS2D::stringFrom<double>(""), std::invalid_argument);

	EXPECT_EQ(long double{-1.0}, NAS2D::stringFrom<long double>("-1"));
	EXPECT_EQ(long double{-1.0}, NAS2D::stringFrom<long double>("-1.0"));
	EXPECT_EQ(long double{0.0}, NAS2D::stringFrom<long double>("0"));
	EXPECT_EQ(long double{0.0}, NAS2D::stringFrom<long double>("0.0"));
	EXPECT_EQ(long double{1.0}, NAS2D::stringFrom<long double>("1"));
	EXPECT_EQ(long double{1.0}, NAS2D::stringFrom<long double>("1.0"));
	EXPECT_THROW(NAS2D::stringFrom<long double>(""), std::invalid_argument);
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
