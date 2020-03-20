#include "NAS2D/StringUtils.h"

#include <string>

#include <gmock/gmock.h>
#include <gtest/gtest.h>

TEST(String, stringTo)
{
	const std::string bigNum(40, '9');

	EXPECT_EQ(std::string{""}, NAS2D::stringTo<std::string>(""));
	EXPECT_EQ(std::string{"Hello World"}, NAS2D::stringTo<std::string>("Hello World"));

	EXPECT_EQ(true, NAS2D::stringTo<bool>("true"));
	EXPECT_EQ(false, NAS2D::stringTo<bool>("false"));
	EXPECT_THROW(NAS2D::stringTo<bool>("foobar"), std::invalid_argument);

	EXPECT_EQ(char{0}, NAS2D::stringTo<char>("0"));
	EXPECT_EQ(unsigned char{129}, NAS2D::stringTo<unsigned char>("129"));
	EXPECT_EQ(signed char{-1}, NAS2D::stringTo<signed char>("-1"));

	EXPECT_EQ(unsigned short{0}, NAS2D::stringTo<unsigned short>("0"));
	EXPECT_EQ(unsigned short{1}, NAS2D::stringTo<unsigned short>("1"));
	EXPECT_THROW(NAS2D::stringTo<unsigned short>(""), std::invalid_argument);
	EXPECT_THROW(NAS2D::stringTo<unsigned short>(bigNum), std::out_of_range);

	EXPECT_EQ(unsigned int{0}, NAS2D::stringTo<unsigned int>("0"));
	EXPECT_EQ(unsigned int{1}, NAS2D::stringTo<unsigned int>("1"));
	EXPECT_THROW(NAS2D::stringTo<unsigned int>(""), std::invalid_argument);
	EXPECT_THROW(NAS2D::stringTo<unsigned int>(bigNum), std::out_of_range);

	EXPECT_EQ(unsigned long{0}, NAS2D::stringTo<unsigned long>("0"));
	EXPECT_EQ(unsigned long{1}, NAS2D::stringTo<unsigned long>("1"));
	EXPECT_THROW(NAS2D::stringTo<unsigned long>(""), std::invalid_argument);
	EXPECT_THROW(NAS2D::stringTo<unsigned long>(bigNum), std::out_of_range);

	EXPECT_EQ(unsigned long long{0}, NAS2D::stringTo<unsigned long long>("0"));
	EXPECT_EQ(unsigned long long{1}, NAS2D::stringTo<unsigned long long>("1"));
	EXPECT_THROW(NAS2D::stringTo<unsigned long long>(""), std::invalid_argument);
	EXPECT_THROW(NAS2D::stringTo<unsigned long long>(bigNum), std::out_of_range);
	
	EXPECT_EQ(short{-1}, NAS2D::stringTo<short>("-1"));
	EXPECT_EQ(short{0}, NAS2D::stringTo<short>("0"));
	EXPECT_EQ(short{1}, NAS2D::stringTo<short>("1"));
	EXPECT_THROW(NAS2D::stringTo<short>(""), std::invalid_argument);
	EXPECT_THROW(NAS2D::stringTo<short>(bigNum), std::out_of_range);

	EXPECT_EQ(int{-1}, NAS2D::stringTo<int>("-1"));
	EXPECT_EQ(int{0}, NAS2D::stringTo<int>("0"));
	EXPECT_EQ(int{1}, NAS2D::stringTo<int>("1"));
	EXPECT_THROW(NAS2D::stringTo<int>(""), std::invalid_argument);
	EXPECT_THROW(NAS2D::stringTo<int>(bigNum), std::out_of_range);

	EXPECT_THROW(NAS2D::stringTo<long>("-" + bigNum), std::out_of_range);
	EXPECT_EQ(long{-1}, NAS2D::stringTo<long>("-1"));
	EXPECT_EQ(long{0}, NAS2D::stringTo<long>("0"));
	EXPECT_EQ(long{1}, NAS2D::stringTo<long>("1"));
	EXPECT_THROW(NAS2D::stringTo<long>(""), std::invalid_argument);
	EXPECT_THROW(NAS2D::stringTo<long>(bigNum), std::out_of_range);

	EXPECT_THROW(NAS2D::stringTo<long long>("-" + bigNum), std::out_of_range);
	EXPECT_EQ(long long{-1}, NAS2D::stringTo<long long>("-1"));
	EXPECT_EQ(long long{0}, NAS2D::stringTo<long long>("0"));
	EXPECT_EQ(long long{1}, NAS2D::stringTo<long long>("1"));
	EXPECT_THROW(NAS2D::stringTo<long long>(""), std::invalid_argument);
	EXPECT_THROW(NAS2D::stringTo<long long>(bigNum), std::out_of_range);

	EXPECT_THROW(NAS2D::stringTo<float>("-" + bigNum), std::out_of_range);
	EXPECT_EQ(float{-1.0}, NAS2D::stringTo<float>("-1"));
	EXPECT_EQ(float{-1.0}, NAS2D::stringTo<float>("-1.0"));
	EXPECT_EQ(float{0.0}, NAS2D::stringTo<float>("0"));
	EXPECT_EQ(float{0.0}, NAS2D::stringTo<float>("0.0"));
	EXPECT_EQ(float{1.0}, NAS2D::stringTo<float>("1"));
	EXPECT_EQ(float{1.0}, NAS2D::stringTo<float>("1.0"));
	EXPECT_THROW(NAS2D::stringTo<float>(""), std::invalid_argument);
	EXPECT_THROW(NAS2D::stringTo<float>(bigNum), std::out_of_range);

	EXPECT_EQ(double{-1.0}, NAS2D::stringTo<double>("-1"));
	EXPECT_EQ(double{-1.0}, NAS2D::stringTo<double>("-1.0"));
	EXPECT_EQ(double{0.0}, NAS2D::stringTo<double>("0"));
	EXPECT_EQ(double{0.0}, NAS2D::stringTo<double>("0.0"));
	EXPECT_EQ(double{1.0}, NAS2D::stringTo<double>("1"));
	EXPECT_EQ(double{1.0}, NAS2D::stringTo<double>("1.0"));
	EXPECT_THROW(NAS2D::stringTo<double>(""), std::invalid_argument);

	EXPECT_EQ(long double{-1.0}, NAS2D::stringTo<long double>("-1"));
	EXPECT_EQ(long double{-1.0}, NAS2D::stringTo<long double>("-1.0"));
	EXPECT_EQ(long double{0.0}, NAS2D::stringTo<long double>("0"));
	EXPECT_EQ(long double{0.0}, NAS2D::stringTo<long double>("0.0"));
	EXPECT_EQ(long double{1.0}, NAS2D::stringTo<long double>("1"));
	EXPECT_EQ(long double{1.0}, NAS2D::stringTo<long double>("1.0"));
	EXPECT_THROW(NAS2D::stringTo<long double>(""), std::invalid_argument);
}

TEST(String, stringFrom)
{
	using namespace std::literals::string_literals;
	const std::string bigNum(40, '9');

	EXPECT_EQ(std::string{""}, NAS2D::stringFrom<std::string>(""));
	EXPECT_EQ(std::string{"Hello World"}, NAS2D::stringFrom<std::string>("Hello World"));

	EXPECT_EQ("true"s, NAS2D::stringFrom<bool>(true));
	EXPECT_EQ("false"s, NAS2D::stringFrom<bool>(false));

	EXPECT_EQ("A"s, NAS2D::stringFrom<char>(65));
	EXPECT_EQ("B"s, NAS2D::stringFrom<char>(66));
	EXPECT_EQ("0"s, NAS2D::stringFrom<char>('0'));
	EXPECT_EQ("1"s, NAS2D::stringFrom<char>('1'));
	EXPECT_EQ("a"s, NAS2D::stringFrom<char>('a'));
	EXPECT_EQ("b"s, NAS2D::stringFrom<char>('b'));
	EXPECT_EQ("129"s, NAS2D::stringFrom<unsigned char>(129));
	EXPECT_EQ("-1"s, NAS2D::stringFrom<signed char>(-1));

	EXPECT_EQ("0"s, NAS2D::stringFrom<unsigned short>(0));
	EXPECT_EQ("1"s, NAS2D::stringFrom<unsigned short>(1));

	EXPECT_EQ("0"s, NAS2D::stringFrom<unsigned int>(0));
	EXPECT_EQ("1"s, NAS2D::stringFrom<unsigned int>(1));

	EXPECT_EQ("0"s, NAS2D::stringFrom<unsigned long>(0));
	EXPECT_EQ("1"s, NAS2D::stringFrom<unsigned long>(1));

	EXPECT_EQ("0"s, NAS2D::stringFrom<unsigned long long>(0));
	EXPECT_EQ("1"s, NAS2D::stringFrom<unsigned long long>(1));

	EXPECT_EQ("-1"s, NAS2D::stringFrom<short>(-1));
	EXPECT_EQ("0"s, NAS2D::stringFrom<short>(0));
	EXPECT_EQ("1"s, NAS2D::stringFrom<short>(1));

	EXPECT_EQ("-1"s, NAS2D::stringFrom<int>(-1));
	EXPECT_EQ("0"s, NAS2D::stringFrom<int>(0));
	EXPECT_EQ("1"s, NAS2D::stringFrom<int>(1));

	EXPECT_EQ("-1"s, NAS2D::stringFrom<long>(-1));
	EXPECT_EQ("0"s, NAS2D::stringFrom<long>(0));
	EXPECT_EQ("1"s, NAS2D::stringFrom<long>(1));

	EXPECT_EQ("-1"s, NAS2D::stringFrom<long long>(-1));
	EXPECT_EQ("0"s, NAS2D::stringFrom<long long>(0));
	EXPECT_EQ("1"s, NAS2D::stringFrom<long long>(1));

	EXPECT_EQ("-1.000000"s, NAS2D::stringFrom<float>(-1));
	EXPECT_EQ("-1.100000"s, NAS2D::stringFrom<float>(-1.1));
	EXPECT_EQ("0.000000"s, NAS2D::stringFrom<float>(0));
	EXPECT_EQ("0.000000"s, NAS2D::stringFrom<float>(0.0));
	EXPECT_EQ("1.000000"s, NAS2D::stringFrom<float>(1));
	EXPECT_EQ("1.000000"s, NAS2D::stringFrom<float>(1.0));

	EXPECT_EQ("-1.000000"s, NAS2D::stringFrom<double>(-1));
	EXPECT_EQ("-1.000000"s, NAS2D::stringFrom<double>(-1.0));
	EXPECT_EQ("0.000000"s, NAS2D::stringFrom<double>(0));
	EXPECT_EQ("0.000000"s, NAS2D::stringFrom<double>(0.0));
	EXPECT_EQ("1.000000"s, NAS2D::stringFrom<double>(1));
	EXPECT_EQ("1.000000"s, NAS2D::stringFrom<double>(1.0));

	EXPECT_EQ("-1.000000"s, NAS2D::stringFrom<long double>(-1));
	EXPECT_EQ("-1.000000"s, NAS2D::stringFrom<long double>(-1.0));
	EXPECT_EQ("0.000000"s, NAS2D::stringFrom<long double>(0));
	EXPECT_EQ("0.000000"s, NAS2D::stringFrom<long double>(0.0));
	EXPECT_EQ("1.000000"s, NAS2D::stringFrom<long double>(1));
	EXPECT_EQ("1.000000"s, NAS2D::stringFrom<long double>(1.0));
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
