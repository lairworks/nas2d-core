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

	EXPECT_EQ('0', NAS2D::stringTo<char>("0"));
	EXPECT_EQ(129, NAS2D::stringTo<unsigned char>("129"));
	EXPECT_EQ(-1, NAS2D::stringTo<signed char>("-1"));

	EXPECT_EQ(0, NAS2D::stringTo<unsigned short>("0"));
	EXPECT_EQ(1, NAS2D::stringTo<unsigned short>("1"));
	EXPECT_THROW(NAS2D::stringTo<unsigned short>(""), std::invalid_argument);
	EXPECT_THROW(NAS2D::stringTo<unsigned short>(bigNum), std::out_of_range);

	EXPECT_EQ(0, NAS2D::stringTo<unsigned int>("0"));
	EXPECT_EQ(1, NAS2D::stringTo<unsigned int>("1"));
	EXPECT_THROW(NAS2D::stringTo<unsigned int>(""), std::invalid_argument);
	EXPECT_THROW(NAS2D::stringTo<unsigned int>(bigNum), std::out_of_range);

	EXPECT_EQ(0, NAS2D::stringTo<unsigned long>("0"));
	EXPECT_EQ(1, NAS2D::stringTo<unsigned long>("1"));
	EXPECT_THROW(NAS2D::stringTo<unsigned long>(""), std::invalid_argument);
	EXPECT_THROW(NAS2D::stringTo<unsigned long>(bigNum), std::out_of_range);

	EXPECT_EQ(0, NAS2D::stringTo<unsigned long long>("0"));
	EXPECT_EQ(1, NAS2D::stringTo<unsigned long long>("1"));
	EXPECT_THROW(NAS2D::stringTo<unsigned long long>(""), std::invalid_argument);
	EXPECT_THROW(NAS2D::stringTo<unsigned long long>(bigNum), std::out_of_range);

	EXPECT_EQ(-1, NAS2D::stringTo<short>("-1"));
	EXPECT_EQ(0, NAS2D::stringTo<short>("0"));
	EXPECT_EQ(1, NAS2D::stringTo<short>("1"));
	EXPECT_THROW(NAS2D::stringTo<short>(""), std::invalid_argument);
	EXPECT_THROW(NAS2D::stringTo<short>(bigNum), std::out_of_range);

	EXPECT_EQ(-1, NAS2D::stringTo<int>("-1"));
	EXPECT_EQ(0, NAS2D::stringTo<int>("0"));
	EXPECT_EQ(1, NAS2D::stringTo<int>("1"));
	EXPECT_THROW(NAS2D::stringTo<int>(""), std::invalid_argument);
	EXPECT_THROW(NAS2D::stringTo<int>(bigNum), std::out_of_range);

	EXPECT_THROW(NAS2D::stringTo<long>("-" + bigNum), std::out_of_range);
	EXPECT_EQ(-1, NAS2D::stringTo<long>("-1"));
	EXPECT_EQ(0, NAS2D::stringTo<long>("0"));
	EXPECT_EQ(1, NAS2D::stringTo<long>("1"));
	EXPECT_THROW(NAS2D::stringTo<long>(""), std::invalid_argument);
	EXPECT_THROW(NAS2D::stringTo<long>(bigNum), std::out_of_range);

	EXPECT_THROW(NAS2D::stringTo<long long>("-" + bigNum), std::out_of_range);
	EXPECT_EQ(-1, NAS2D::stringTo<long long>("-1"));
	EXPECT_EQ(0, NAS2D::stringTo<long long>("0"));
	EXPECT_EQ(1, NAS2D::stringTo<long long>("1"));
	EXPECT_THROW(NAS2D::stringTo<long long>(""), std::invalid_argument);
	EXPECT_THROW(NAS2D::stringTo<long long>(bigNum), std::out_of_range);

	EXPECT_THROW(NAS2D::stringTo<float>("-" + bigNum), std::out_of_range);
	EXPECT_EQ(-1.0, NAS2D::stringTo<float>("-1"));
	EXPECT_EQ(-1.0, NAS2D::stringTo<float>("-1.0"));
	EXPECT_EQ(0.0, NAS2D::stringTo<float>("0"));
	EXPECT_EQ(0.0, NAS2D::stringTo<float>("0.0"));
	EXPECT_EQ(1.0, NAS2D::stringTo<float>("1"));
	EXPECT_EQ(1.0, NAS2D::stringTo<float>("1.0"));
	EXPECT_EQ(1.1f, NAS2D::stringTo<float>("1.1")); // Subject to rounding
	EXPECT_THROW(NAS2D::stringTo<float>(""), std::invalid_argument);
	EXPECT_THROW(NAS2D::stringTo<float>(bigNum), std::out_of_range);

	EXPECT_EQ(-1.0, NAS2D::stringTo<double>("-1"));
	EXPECT_EQ(-1.0, NAS2D::stringTo<double>("-1.0"));
	EXPECT_EQ(0.0, NAS2D::stringTo<double>("0"));
	EXPECT_EQ(0.0, NAS2D::stringTo<double>("0.0"));
	EXPECT_EQ(1.0, NAS2D::stringTo<double>("1"));
	EXPECT_EQ(1.0, NAS2D::stringTo<double>("1.0"));
	EXPECT_EQ(1.1, NAS2D::stringTo<double>("1.1")); // Subject to rounding
	EXPECT_THROW(NAS2D::stringTo<double>(""), std::invalid_argument);

	EXPECT_EQ(-1.0, NAS2D::stringTo<long double>("-1"));
	EXPECT_EQ(-1.0, NAS2D::stringTo<long double>("-1.0"));
	EXPECT_EQ(0.0, NAS2D::stringTo<long double>("0"));
	EXPECT_EQ(0.0, NAS2D::stringTo<long double>("0.0"));
	EXPECT_EQ(1.0, NAS2D::stringTo<long double>("1"));
	EXPECT_EQ(1.0, NAS2D::stringTo<long double>("1.0"));
	EXPECT_EQ(1.1L, NAS2D::stringTo<long double>("1.1")); // Subject to rounding
	EXPECT_THROW(NAS2D::stringTo<long double>(""), std::invalid_argument);
}

TEST(String, stringFromStdString)
{
	using namespace std::literals::string_literals;

	EXPECT_EQ(""s, NAS2D::stringFrom<std::string>(""));
	EXPECT_EQ("Hello World"s, NAS2D::stringFrom<std::string>("Hello World"));
}


TEST(String, stringFromBool)
{
	using namespace std::literals::string_literals;

	EXPECT_EQ(std::string{""}, NAS2D::stringFrom<std::string>(""));
	EXPECT_EQ(std::string{"Hello World"}, NAS2D::stringFrom<std::string>("Hello World"));

	EXPECT_EQ("true"s, NAS2D::stringFrom<bool>(true));
	EXPECT_EQ("false"s, NAS2D::stringFrom<bool>(false));
}

TEST(String, stringFromChar)
{
	using namespace std::literals::string_literals;

	EXPECT_EQ("A"s, NAS2D::stringFrom<char>(65));
	EXPECT_EQ("B"s, NAS2D::stringFrom<char>(66));
	EXPECT_EQ("0"s, NAS2D::stringFrom<char>('0'));
	EXPECT_EQ("1"s, NAS2D::stringFrom<char>('1'));
	EXPECT_EQ("a"s, NAS2D::stringFrom<char>('a'));
	EXPECT_EQ("b"s, NAS2D::stringFrom<char>('b'));
	EXPECT_EQ("129"s, NAS2D::stringFrom<unsigned char>(129));
	EXPECT_EQ("-1"s, NAS2D::stringFrom<signed char>(-1));

}


template<typename T>
class StringFromSignedIntegerTest : public ::testing::Test
{
  public:
	  StringFromSignedIntegerTest<T>()
	{}

	T negOne{-1};
	T zero{0};
	T one{1};
};

using StringFromSignedIntegerTestTypes = ::testing::Types<short, int, long, long long>;
TYPED_TEST_SUITE(StringFromSignedIntegerTest, StringFromSignedIntegerTestTypes);

TYPED_TEST(StringFromSignedIntegerTest, StringFromSignedIntegerTestEdgeCases)
{
	using namespace std::literals::string_literals;

	EXPECT_EQ("-1"s, NAS2D::stringFrom<TypeParam>(this->negOne));
	EXPECT_EQ("0"s, NAS2D::stringFrom<TypeParam>(this->zero));
	EXPECT_EQ("1"s, NAS2D::stringFrom<TypeParam>(this->one));
}


template<typename T>
class StringFromUnsignedIntegerTest : public ::testing::Test
{
  public:
	StringFromUnsignedIntegerTest<T>()
	{}

	T zero{0};
	T one{1};
};

using StringFromUnsignedIntegerTestTypes = ::testing::Types<unsigned short, unsigned int, unsigned long, unsigned long long>;
TYPED_TEST_SUITE(StringFromUnsignedIntegerTest, StringFromUnsignedIntegerTestTypes);

TYPED_TEST(StringFromUnsignedIntegerTest, StringFromUnsignedIntegerTestEdgeCases)
{
	using namespace std::literals::string_literals;

	EXPECT_EQ("0"s, NAS2D::stringFrom<TypeParam>(this->zero));
	EXPECT_EQ("1"s, NAS2D::stringFrom<TypeParam>(this->one));
}

template<typename T>
class StringFromFloatingPointTest : public ::testing::Test
{
  public:
	  StringFromFloatingPointTest<T>()
	{}

	T negOneAndHalf{-1.5};
	T negOne{-1.0};
	T zero{0.0};
	T one{1.0};
	T oneAndHalf{1.5};
};


using StringFromFloatingPointTestTypes = ::testing::Types<float, double, long double>;
TYPED_TEST_SUITE(StringFromFloatingPointTest, StringFromFloatingPointTestTypes);

TYPED_TEST(StringFromFloatingPointTest, StringFromFloatingPointTestEdgeCases)
{
	using namespace std::literals::string_literals;

	EXPECT_EQ("-1.500000"s, NAS2D::stringFrom<TypeParam>(this->negOneAndHalf));
	EXPECT_EQ("-1.000000"s, NAS2D::stringFrom<TypeParam>(this->negOne));
	EXPECT_EQ("0.000000"s, NAS2D::stringFrom<TypeParam>(this->zero));
	EXPECT_EQ("1.000000"s, NAS2D::stringFrom<TypeParam>(this->one));
	EXPECT_EQ("1.500000"s, NAS2D::stringFrom<TypeParam>(this->oneAndHalf));
}


TEST(String, stringToFromRoundtrip)
{
	using namespace std::literals::string_literals;
	const std::string bigNum(40, '9');

	EXPECT_EQ(""s, NAS2D::stringFrom<std::string>(NAS2D::stringTo<std::string>("")));
	EXPECT_EQ("Hello World"s, NAS2D::stringFrom<std::string>(NAS2D::stringTo<std::string>("Hello World")));

	EXPECT_EQ("true"s, NAS2D::stringFrom<bool>(NAS2D::stringTo<bool>("true"s)));
	EXPECT_EQ("false"s, NAS2D::stringFrom<bool>(NAS2D::stringTo<bool>("false"s)));

	EXPECT_THROW(NAS2D::stringFrom<char>(NAS2D::stringTo<char>(""s)), std::invalid_argument);
	EXPECT_EQ("A"s, NAS2D::stringFrom<char>(NAS2D::stringTo<char>("A"s)));
	EXPECT_EQ("B"s, NAS2D::stringFrom<char>(NAS2D::stringTo<char>("B"s)));
	EXPECT_EQ("0"s, NAS2D::stringFrom<char>(NAS2D::stringTo<char>("0"s)));
	EXPECT_EQ("1"s, NAS2D::stringFrom<char>(NAS2D::stringTo<char>("1"s)));
	EXPECT_EQ("2"s, NAS2D::stringFrom<char>(NAS2D::stringTo<char>("2"s)));
	EXPECT_EQ("3"s, NAS2D::stringFrom<char>(NAS2D::stringTo<char>("3"s)));
	EXPECT_EQ("4"s, NAS2D::stringFrom<char>(NAS2D::stringTo<char>("4"s)));
	EXPECT_EQ("5"s, NAS2D::stringFrom<char>(NAS2D::stringTo<char>("5"s)));
	EXPECT_EQ("6"s, NAS2D::stringFrom<char>(NAS2D::stringTo<char>("6"s)));
	EXPECT_EQ("7"s, NAS2D::stringFrom<char>(NAS2D::stringTo<char>("7"s)));
	EXPECT_EQ("8"s, NAS2D::stringFrom<char>(NAS2D::stringTo<char>("8"s)));
	EXPECT_EQ("9"s, NAS2D::stringFrom<char>(NAS2D::stringTo<char>("9"s)));
	EXPECT_EQ("a"s, NAS2D::stringFrom<char>(NAS2D::stringTo<char>("a"s)));
	EXPECT_EQ("b"s, NAS2D::stringFrom<char>(NAS2D::stringTo<char>("b"s)));
	EXPECT_THROW(NAS2D::stringFrom<char>(NAS2D::stringTo<char>("true"s)), std::invalid_argument);
	EXPECT_THROW(NAS2D::stringFrom<char>(NAS2D::stringTo<char>("false"s)), std::invalid_argument);

	EXPECT_EQ("129"s, NAS2D::stringFrom<unsigned char>(NAS2D::stringTo<unsigned char>("129"s)));
	EXPECT_EQ("-1"s, NAS2D::stringFrom<signed char>(NAS2D::stringTo<signed char>("-1"s)));

	EXPECT_EQ("0"s, NAS2D::stringFrom<unsigned short>(NAS2D::stringTo<unsigned short>("0"s)));
	EXPECT_EQ("1"s, NAS2D::stringFrom<unsigned short>(NAS2D::stringTo<unsigned short>("1"s)));

	EXPECT_EQ("0"s, NAS2D::stringFrom<unsigned int>(NAS2D::stringTo<unsigned int>("0"s)));
	EXPECT_EQ("1"s, NAS2D::stringFrom<unsigned int>(NAS2D::stringTo<unsigned int>("1"s)));

	EXPECT_EQ("0"s, NAS2D::stringFrom<unsigned long>(NAS2D::stringTo<unsigned long>("0"s)));
	EXPECT_EQ("1"s, NAS2D::stringFrom<unsigned long>(NAS2D::stringTo<unsigned long>("1"s)));

	EXPECT_EQ("0"s, NAS2D::stringFrom<unsigned long long>(NAS2D::stringTo<unsigned long long>("0"s)));
	EXPECT_EQ("1"s, NAS2D::stringFrom<unsigned long long>(NAS2D::stringTo<unsigned long long>("1"s)));

	EXPECT_EQ("-1"s, NAS2D::stringFrom<short>(NAS2D::stringTo<short>("-1"s)));
	EXPECT_EQ("0"s, NAS2D::stringFrom<short>(NAS2D::stringTo<short>("0"s)));
	EXPECT_EQ("1"s, NAS2D::stringFrom<short>(NAS2D::stringTo<short>("1"s)));

	EXPECT_EQ("-1"s, NAS2D::stringFrom<int>(NAS2D::stringTo<int>("-1"s)));
	EXPECT_EQ("0"s, NAS2D::stringFrom<int>(NAS2D::stringTo<int>("0"s)));
	EXPECT_EQ("1"s, NAS2D::stringFrom<int>(NAS2D::stringTo<int>("1"s)));

	EXPECT_EQ("-1"s, NAS2D::stringFrom<long>(NAS2D::stringTo<long>("-1"s)));
	EXPECT_EQ("0"s, NAS2D::stringFrom<long>(NAS2D::stringTo<long>("0"s)));
	EXPECT_EQ("1"s, NAS2D::stringFrom<long>(NAS2D::stringTo<long>("1"s)));

	EXPECT_EQ("-1"s, NAS2D::stringFrom<long long>(NAS2D::stringTo<long long>("-1"s)));
	EXPECT_EQ("0"s, NAS2D::stringFrom<long long>(NAS2D::stringTo<long long>("0"s)));
	EXPECT_EQ("1"s, NAS2D::stringFrom<long long>(NAS2D::stringTo<long long>("1"s)));

	EXPECT_EQ("-1.000000"s, NAS2D::stringFrom<float>(NAS2D::stringTo<float>("-1"s)));
	EXPECT_EQ("-1.000000"s, NAS2D::stringFrom<float>(NAS2D::stringTo<float>("-1.0"s)));
	EXPECT_EQ("-1.100000"s, NAS2D::stringFrom<float>(NAS2D::stringTo<float>("-1.1"s)));
	EXPECT_EQ("0.000000"s, NAS2D::stringFrom<float>(NAS2D::stringTo<float>("0"s)));
	EXPECT_EQ("0.000000"s, NAS2D::stringFrom<float>(NAS2D::stringTo<float>("0.0"s)));
	EXPECT_EQ("1.000000"s, NAS2D::stringFrom<float>(NAS2D::stringTo<float>("1"s)));
	EXPECT_EQ("1.000000"s, NAS2D::stringFrom<float>(NAS2D::stringTo<float>("1.0"s)));
	EXPECT_EQ("1.100000"s, NAS2D::stringFrom<float>(NAS2D::stringTo<float>("1.1"s)));

	EXPECT_EQ("-1.000000"s, NAS2D::stringFrom<double>(NAS2D::stringTo<double>("-1"s)));
	EXPECT_EQ("-1.000000"s, NAS2D::stringFrom<double>(NAS2D::stringTo<double>("-1.0"s)));
	EXPECT_EQ("-1.100000"s, NAS2D::stringFrom<double>(NAS2D::stringTo<double>("-1.1"s)));
	EXPECT_EQ("0.000000"s, NAS2D::stringFrom<double> (NAS2D::stringTo<double>("0"s)));
	EXPECT_EQ("0.000000"s, NAS2D::stringFrom<double> (NAS2D::stringTo<double>("0.0"s)));
	EXPECT_EQ("1.000000"s, NAS2D::stringFrom<double> (NAS2D::stringTo<double>("1"s)));
	EXPECT_EQ("1.000000"s, NAS2D::stringFrom<double> (NAS2D::stringTo<double>("1.0"s)));
	EXPECT_EQ("1.100000"s, NAS2D::stringFrom<double> (NAS2D::stringTo<double>("1.1"s)));

	EXPECT_EQ("-1.000000"s, NAS2D::stringFrom<long double>(NAS2D::stringTo<long double>("-1"s)));
	EXPECT_EQ("-1.000000"s, NAS2D::stringFrom<long double>(NAS2D::stringTo<long double>("-1.0"s)));
	EXPECT_EQ("-1.100000"s, NAS2D::stringFrom<long double>(NAS2D::stringTo<long double>("-1.1"s)));
	EXPECT_EQ("0.000000"s, NAS2D::stringFrom<long double>(NAS2D::stringTo<long double>("0"s)));
	EXPECT_EQ("0.000000"s, NAS2D::stringFrom<long double>(NAS2D::stringTo<long double>("0.0"s)));
	EXPECT_EQ("1.000000"s, NAS2D::stringFrom<long double>(NAS2D::stringTo<long double>("1"s)));
	EXPECT_EQ("1.100000"s, NAS2D::stringFrom<long double>(NAS2D::stringTo<long double>("1.1"s)));
}


TEST(String, stringFromToRoundtrip)
{
	using namespace std::literals::string_literals;
	const std::string bigNum(40, '9');

	EXPECT_EQ(""s, NAS2D::stringTo<std::string>(NAS2D::stringFrom<std::string>("")));
	EXPECT_EQ("Hello World"s, NAS2D::stringTo<std::string>(NAS2D::stringFrom<std::string>("Hello World")));

	EXPECT_EQ(true, NAS2D::stringTo<bool>(NAS2D::stringFrom<bool>(true)));
	EXPECT_EQ(false, NAS2D::stringTo<bool>(NAS2D::stringFrom<bool>(false)));

	EXPECT_EQ(65, NAS2D::stringTo<char>(NAS2D::stringFrom<char>(65)));
	EXPECT_EQ(66, NAS2D::stringTo<char>(NAS2D::stringFrom<char>(66)));
	EXPECT_EQ(0, NAS2D::stringTo<char>(NAS2D::stringFrom<char>(0)));
	EXPECT_EQ(1, NAS2D::stringTo<char>(NAS2D::stringFrom<char>(1)));
	EXPECT_EQ(2, NAS2D::stringTo<char>(NAS2D::stringFrom<char>(2)));
	EXPECT_EQ(3, NAS2D::stringTo<char>(NAS2D::stringFrom<char>(3)));
	EXPECT_EQ(4, NAS2D::stringTo<char>(NAS2D::stringFrom<char>(4)));
	EXPECT_EQ(5, NAS2D::stringTo<char>(NAS2D::stringFrom<char>(5)));
	EXPECT_EQ(6, NAS2D::stringTo<char>(NAS2D::stringFrom<char>(6)));
	EXPECT_EQ(7, NAS2D::stringTo<char>(NAS2D::stringFrom<char>(7)));
	EXPECT_EQ(8, NAS2D::stringTo<char>(NAS2D::stringFrom<char>(8)));
	EXPECT_EQ(9, NAS2D::stringTo<char>(NAS2D::stringFrom<char>(9)));
	EXPECT_EQ(10, NAS2D::stringTo<char>(NAS2D::stringFrom<char>(10)));
	EXPECT_EQ(97, NAS2D::stringTo<char>(NAS2D::stringFrom<char>(97)));
	EXPECT_EQ(98, NAS2D::stringTo<char>(NAS2D::stringFrom<char>(98)));

	EXPECT_EQ(129, NAS2D::stringTo<unsigned char>(NAS2D::stringFrom<unsigned char>(129)));
	EXPECT_EQ(-1, NAS2D::stringTo<signed char>(NAS2D::stringFrom<signed char>(-1)));

	EXPECT_EQ(0, NAS2D::stringTo<unsigned short>(NAS2D::stringFrom<unsigned short>(0)));
	EXPECT_EQ(1, NAS2D::stringTo<unsigned short>(NAS2D::stringFrom<unsigned short>(1)));

	EXPECT_EQ(0, NAS2D::stringTo<unsigned int>(NAS2D::stringFrom<unsigned int>(0)));
	EXPECT_EQ(1, NAS2D::stringTo<unsigned int>(NAS2D::stringFrom<unsigned int>(1)));

	EXPECT_EQ(0, NAS2D::stringTo<unsigned long>(NAS2D::stringFrom<unsigned long>(0)));
	EXPECT_EQ(1, NAS2D::stringTo<unsigned long>(NAS2D::stringFrom<unsigned long>(1)));

	EXPECT_EQ(0, NAS2D::stringTo<unsigned long long>(NAS2D::stringFrom<unsigned long long>(0)));
	EXPECT_EQ(1, NAS2D::stringTo<unsigned long long>(NAS2D::stringFrom<unsigned long long>(1)));

	EXPECT_EQ(-1, NAS2D::stringTo<short>(NAS2D::stringFrom<short>(-1)));
	EXPECT_EQ(0, NAS2D::stringTo<short>(NAS2D::stringFrom<short>(0)));
	EXPECT_EQ(1, NAS2D::stringTo<short>(NAS2D::stringFrom<short>(1)));

	EXPECT_EQ(-1, NAS2D::stringTo<int>(NAS2D::stringFrom<int>(-1)));
	EXPECT_EQ(0, NAS2D::stringTo<int>(NAS2D::stringFrom<int>(0)));
	EXPECT_EQ(1, NAS2D::stringTo<int>(NAS2D::stringFrom<int>(1)));

	EXPECT_EQ(-1, NAS2D::stringTo<long>(NAS2D::stringFrom<long>(-1)));
	EXPECT_EQ(0, NAS2D::stringTo<long>(NAS2D::stringFrom<long>(0)));
	EXPECT_EQ(1, NAS2D::stringTo<long>(NAS2D::stringFrom<long>(1)));

	EXPECT_EQ(-1, NAS2D::stringTo<long long>(NAS2D::stringFrom<long long>(-1)));
	EXPECT_EQ(0, NAS2D::stringTo<long long>(NAS2D::stringFrom<long long>(0)));
	EXPECT_EQ(1, NAS2D::stringTo<long long>(NAS2D::stringFrom<long long>(1)));

	EXPECT_EQ(-1.000000, NAS2D::stringTo<float>(NAS2D::stringFrom<float>(-1)));
	EXPECT_EQ(-1.100000f, NAS2D::stringTo<float>(NAS2D::stringFrom<float>(-1.1f)));
	EXPECT_EQ(0.000000, NAS2D::stringTo<float>(NAS2D::stringFrom<float>(0)));
	EXPECT_EQ(0.000000, NAS2D::stringTo<float>(NAS2D::stringFrom<float>(0.0)));
	EXPECT_EQ(1.000000, NAS2D::stringTo<float>(NAS2D::stringFrom<float>(1)));
	EXPECT_EQ(1.000000, NAS2D::stringTo<float>(NAS2D::stringFrom<float>(1.0)));
	EXPECT_EQ(1.100000f, NAS2D::stringTo<float>(NAS2D::stringFrom<float>(1.1f)));

	EXPECT_EQ(-1.000000, NAS2D::stringTo<double>(NAS2D::stringFrom<double>(-1)));
	EXPECT_EQ(-1.100000, NAS2D::stringTo<double>(NAS2D::stringFrom<double>(-1.1)));
	EXPECT_EQ(0.000000, NAS2D::stringTo<double>(NAS2D::stringFrom<double>(0)));
	EXPECT_EQ(0.000000, NAS2D::stringTo<double>(NAS2D::stringFrom<double>(0.0)));
	EXPECT_EQ(1.000000, NAS2D::stringTo<double>(NAS2D::stringFrom<double>(1)));
	EXPECT_EQ(1.000000, NAS2D::stringTo<double>(NAS2D::stringFrom<double>(1.0)));
	EXPECT_EQ(1.100000, NAS2D::stringTo<double>(NAS2D::stringFrom<double>(1.1)));

	EXPECT_EQ(-1.000000, NAS2D::stringTo<long double>(NAS2D::stringFrom<long double>(-1)));
	EXPECT_EQ(-1.000000, NAS2D::stringTo<long double>(NAS2D::stringFrom<long double>(-1.0)));
	EXPECT_EQ(-1.100000L, NAS2D::stringTo<long double>(NAS2D::stringFrom<long double>(-1.1L)));
	EXPECT_EQ(0.000000, NAS2D::stringTo<long double>(NAS2D::stringFrom<long double>(0)));
	EXPECT_EQ(0.000000, NAS2D::stringTo<long double>(NAS2D::stringFrom<long double>(0.0)));
	EXPECT_EQ(1.000000, NAS2D::stringTo<long double>(NAS2D::stringFrom<long double>(1)));
	EXPECT_EQ(1.000000, NAS2D::stringTo<long double>(NAS2D::stringFrom<long double>(1.0)));
	EXPECT_EQ(1.100000L, NAS2D::stringTo<long double>(NAS2D::stringFrom<long double>(1.1L)));
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
