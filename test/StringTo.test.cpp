#include "NAS2D/StringTo.h"

#include <gtest/gtest.h>
#include <gmock/gmock.h>

#include <string>
#include <limits>
#include <cmath>


TEST(StringTo, stringView) {
	EXPECT_EQ("SomeStringValue", NAS2D::stringTo<std::string_view>("SomeStringValue"));
	EXPECT_EQ("SomeStringValue", NAS2D::stringTo<std::string_view>(std::string_view{"SomeStringValue"}));
	// No conversion from temporary `std::string` as that would result in a dangling pointer
	// The overload is deleted to cause a compile error if this happens by accident

	// The following case is allowed, and there is no dangling pointer in the string_view
	std::string notTemporaryString{"SomeStringValue"};
	EXPECT_EQ("SomeStringValue", NAS2D::stringTo<std::string_view>(notTemporaryString));
}

TEST(StringTo, string) {
	EXPECT_EQ("SomeStringValue", NAS2D::stringTo<std::string>("SomeStringValue"));
	EXPECT_EQ("SomeStringValue", NAS2D::stringTo<std::string>(std::string_view{"SomeStringValue"}));
	EXPECT_EQ("SomeStringValue", NAS2D::stringTo<std::string>(std::string{"SomeStringValue"}));
}

TEST(StringTo, booleanErrorInvalid) {
	EXPECT_THROW(NAS2D::stringTo<bool>(""), std::invalid_argument);
	EXPECT_THROW(NAS2D::stringTo<bool>("random text"), std::invalid_argument);
}

TEST(StringTo, boolean) {
	EXPECT_EQ(false, NAS2D::stringTo<bool>("false"));
	EXPECT_EQ(true, NAS2D::stringTo<bool>("true"));
	EXPECT_EQ(false, NAS2D::stringTo<bool>("0"));
	EXPECT_EQ(true, NAS2D::stringTo<bool>("1"));

	EXPECT_EQ(false, NAS2D::stringTo<bool>(std::string_view{"false"}));
	EXPECT_EQ(true, NAS2D::stringTo<bool>(std::string_view{"true"}));
	EXPECT_EQ(false, NAS2D::stringTo<bool>(std::string_view{"0"}));
	EXPECT_EQ(true, NAS2D::stringTo<bool>(std::string_view{"1"}));
}

TEST(StringTo, integerErrorInvalidChar) {
	EXPECT_THROW(NAS2D::stringTo<char>(""), std::invalid_argument);
}

TEST(StringTo, integerErrorInvalidShort) {
	EXPECT_THROW(NAS2D::stringTo<short>(""), std::invalid_argument);
}

TEST(StringTo, integerErrorInvalidInt) {
	EXPECT_THROW(NAS2D::stringTo<int>(""), std::invalid_argument);
}

TEST(StringTo, integerErrorInvalidLong) {
	EXPECT_THROW(NAS2D::stringTo<long>(""), std::invalid_argument);
}

TEST(StringTo, integerErrorInvalidLongLong) {
	EXPECT_THROW(NAS2D::stringTo<long long>(""), std::invalid_argument);
}


TEST(StringTo, integerCharErrorOutOfRangeNegative) {
	// Whether `char` is signed or unsigned is implementation defined
	if constexpr (std::numeric_limits<char>::is_signed)
	{
		// For signed implementations, a value too far negative is out of range
		EXPECT_THROW(NAS2D::stringTo<char>(std::to_string(std::numeric_limits<char>::min() - 1)), std::out_of_range);
	}
	else
	{
		// For unsigned implementations, we leave it unspecified if a negative value is out of range or invalid
		// The '-' character may be considered invalid for an unsigned type
		try
		{
			NAS2D::stringTo<char>("-1");
			FAIL() << "Expected exception but got none";
		}
		catch (const std::out_of_range&) {} // Acceptable
		catch (const std::invalid_argument&) {} // Acceptable
		catch (const std::exception& error)
		{
			FAIL() << "Unexpected exception type: " << error.what();
		}
		catch (...)
		{
			FAIL() << "Unexpected exception of non-standard type";
		}
	}
}

TEST(StringTo, integerCharErrorOutOfRange) {
	EXPECT_THROW(NAS2D::stringTo<char>(std::to_string(std::numeric_limits<char>::max() + 1)), std::out_of_range);
}

TEST(StringTo, integerShortErrorOutOfRange) {
	EXPECT_THROW(NAS2D::stringTo<short>(std::to_string(std::numeric_limits<short>::min() - 1)), std::out_of_range);
	EXPECT_THROW(NAS2D::stringTo<short>(std::to_string(std::numeric_limits<short>::max() + 1)), std::out_of_range);
}

TEST(StringTo, integerChar) {
	using TypeParam = char;
	EXPECT_EQ(TypeParam{0}, NAS2D::stringTo<TypeParam>("0"));

	EXPECT_EQ(TypeParam{0}, NAS2D::stringTo<TypeParam>(std::string_view{"0"}));
}


TEST(StringTo, integerUnsignedChar) {
	using TypeParam = unsigned char;
	EXPECT_EQ(TypeParam{0}, NAS2D::stringTo<TypeParam>("0"));

	EXPECT_EQ(TypeParam{0}, NAS2D::stringTo<TypeParam>(std::string_view{"0"}));
}

TEST(StringTo, integerUnsignedShort) {
	using TypeParam = unsigned short;
	EXPECT_EQ(TypeParam{0}, NAS2D::stringTo<TypeParam>("0"));

	EXPECT_EQ(TypeParam{0}, NAS2D::stringTo<TypeParam>(std::string_view{"0"}));
}

TEST(StringTo, integerUnsignedInt) {
	using TypeParam = unsigned int;
	EXPECT_EQ(TypeParam{0}, NAS2D::stringTo<TypeParam>("0"));

	EXPECT_EQ(TypeParam{0}, NAS2D::stringTo<TypeParam>(std::string_view{"0"}));
}

TEST(StringTo, integerUnsignedLong) {
	using TypeParam = unsigned long;
	EXPECT_EQ(TypeParam{0}, NAS2D::stringTo<TypeParam>("0"));

	EXPECT_EQ(TypeParam{0}, NAS2D::stringTo<TypeParam>(std::string_view{"0"}));
}

TEST(StringTo, integerUnsignedLongLong) {
	using TypeParam = unsigned long long;
	EXPECT_EQ(TypeParam{0}, NAS2D::stringTo<TypeParam>("0"));

	EXPECT_EQ(TypeParam{0}, NAS2D::stringTo<TypeParam>(std::string_view{"0"}));
}


TEST(StringTo, integerSignedChar) {
	using TypeParam = signed char;
	EXPECT_EQ(TypeParam{-1}, NAS2D::stringTo<TypeParam>("-1"));
	EXPECT_EQ(TypeParam{0}, NAS2D::stringTo<TypeParam>("0"));

	EXPECT_EQ(TypeParam{-1}, NAS2D::stringTo<TypeParam>(std::string_view{"-1"}));
	EXPECT_EQ(TypeParam{0}, NAS2D::stringTo<TypeParam>(std::string_view{"0"}));
}

TEST(StringTo, integerSignedShort) {
	using TypeParam = short;
	EXPECT_EQ(TypeParam{-1}, NAS2D::stringTo<TypeParam>("-1"));
	EXPECT_EQ(TypeParam{0}, NAS2D::stringTo<TypeParam>("0"));

	EXPECT_EQ(TypeParam{-1}, NAS2D::stringTo<TypeParam>(std::string_view{"-1"}));
	EXPECT_EQ(TypeParam{0}, NAS2D::stringTo<TypeParam>(std::string_view{"0"}));
}

TEST(StringTo, integerSignedInt) {
	using TypeParam = int;
	EXPECT_EQ(TypeParam{-1}, NAS2D::stringTo<TypeParam>("-1"));
	EXPECT_EQ(TypeParam{0}, NAS2D::stringTo<TypeParam>("0"));

	EXPECT_EQ(TypeParam{-1}, NAS2D::stringTo<TypeParam>(std::string_view{"-1"}));
	EXPECT_EQ(TypeParam{0}, NAS2D::stringTo<TypeParam>(std::string_view{"0"}));
}

TEST(StringTo, integerSignedLong) {
	using TypeParam = long;
	EXPECT_EQ(TypeParam{-1}, NAS2D::stringTo<TypeParam>("-1"));
	EXPECT_EQ(TypeParam{0}, NAS2D::stringTo<TypeParam>("0"));

	EXPECT_EQ(TypeParam{-1}, NAS2D::stringTo<TypeParam>(std::string_view{"-1"}));
	EXPECT_EQ(TypeParam{0}, NAS2D::stringTo<TypeParam>(std::string_view{"0"}));
}

TEST(StringTo, integerSignedLongLong) {
	using TypeParam = long long;
	EXPECT_EQ(TypeParam{-1}, NAS2D::stringTo<TypeParam>("-1"));
	EXPECT_EQ(TypeParam{0}, NAS2D::stringTo<TypeParam>("0"));

	EXPECT_EQ(TypeParam{-1}, NAS2D::stringTo<TypeParam>(std::string_view{"-1"}));
	EXPECT_EQ(TypeParam{0}, NAS2D::stringTo<TypeParam>(std::string_view{"0"}));
}


TEST(StringTo, floatingPointErrorInvalidFloat) {
	EXPECT_THROW(NAS2D::stringTo<float>(""), std::invalid_argument);
}

TEST(StringTo, floatingPointErrorInvalidDouble) {
	EXPECT_THROW(NAS2D::stringTo<double>(""), std::invalid_argument);
}

TEST(StringTo, floatingPointErrorInvalidLongDouble) {
	EXPECT_THROW(NAS2D::stringTo<long double>(""), std::invalid_argument);
}


TEST(StringTo, floatingPointFloat) {
	EXPECT_TRUE(std::signbit(NAS2D::stringTo<float>("-0.0")));

	EXPECT_EQ(0.0f, NAS2D::stringTo<float>("0.0"));
	EXPECT_EQ(0.5f, NAS2D::stringTo<float>("0.5"));
	EXPECT_EQ(1.0f, NAS2D::stringTo<float>("1.0"));

	EXPECT_EQ(0.0f, NAS2D::stringTo<float>(std::string_view{"0.0"}));
	EXPECT_EQ(0.5f, NAS2D::stringTo<float>(std::string_view{"0.5"}));
	EXPECT_EQ(1.0f, NAS2D::stringTo<float>(std::string_view{"1.0"}));
}

TEST(StringTo, floatingPointDouble) {
	EXPECT_TRUE(std::signbit(NAS2D::stringTo<double>("-0.0")));

	EXPECT_EQ(0.0, NAS2D::stringTo<double>("0.0"));
	EXPECT_EQ(0.5, NAS2D::stringTo<double>("0.5"));
	EXPECT_EQ(1.0, NAS2D::stringTo<double>("1.0"));

	EXPECT_EQ(0.0, NAS2D::stringTo<double>(std::string_view{"0.0"}));
	EXPECT_EQ(0.5, NAS2D::stringTo<double>(std::string_view{"0.5"}));
	EXPECT_EQ(1.0, NAS2D::stringTo<double>(std::string_view{"1.0"}));
}

TEST(StringTo, floatingPointLongDouble) {
	EXPECT_TRUE(std::signbit(NAS2D::stringTo<long double>("-0.0")));

	EXPECT_EQ(0.0l, NAS2D::stringTo<long double>("0.0"));
	EXPECT_EQ(0.5l, NAS2D::stringTo<long double>("0.5"));
	EXPECT_EQ(1.0l, NAS2D::stringTo<long double>("1.0"));

	// This may create a temporary `std::string` object on some platforms
	EXPECT_EQ(0.0l, NAS2D::stringTo<long double>(std::string_view{"0.0"}));
	EXPECT_EQ(0.5l, NAS2D::stringTo<long double>(std::string_view{"0.5"}));
	EXPECT_EQ(1.0l, NAS2D::stringTo<long double>(std::string_view{"1.0"}));
}
