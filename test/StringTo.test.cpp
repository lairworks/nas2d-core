#include "NAS2D/StringTo.h"

#include <gtest/gtest.h>
#include <gmock/gmock.h>

#include <string>
#include <limits>
#include <cmath>


namespace {
	template <typename TypeParam>
	void testArithmeticErrorInvalid() {
		EXPECT_THROW(NAS2D::stringTo<TypeParam>(""), std::invalid_argument);
	}

	template <typename TypeParam>
	void testIntegerUnsigned() {
		EXPECT_EQ(TypeParam{0}, NAS2D::stringTo<TypeParam>("0"));

		EXPECT_EQ(TypeParam{0}, NAS2D::stringTo<TypeParam>(std::string_view{"0"}));
	}

	template <typename TypeParam>
	void testIntegerSigned() {
		EXPECT_EQ(TypeParam{-1}, NAS2D::stringTo<TypeParam>("-1"));
		EXPECT_EQ(TypeParam{0}, NAS2D::stringTo<TypeParam>("0"));

		EXPECT_EQ(TypeParam{-1}, NAS2D::stringTo<TypeParam>(std::string_view{"-1"}));
		EXPECT_EQ(TypeParam{0}, NAS2D::stringTo<TypeParam>(std::string_view{"0"}));
	}

	template <typename TypeParam>
	void testFloatingPoint() {
		EXPECT_TRUE(std::signbit(NAS2D::stringTo<TypeParam>("-0.0")));

		EXPECT_EQ(TypeParam{0.0l}, NAS2D::stringTo<TypeParam>("0.0"));
		EXPECT_EQ(TypeParam{0.5l}, NAS2D::stringTo<TypeParam>("0.5"));
		EXPECT_EQ(TypeParam{1.0l}, NAS2D::stringTo<TypeParam>("1.0"));

		EXPECT_EQ(TypeParam{0.0l}, NAS2D::stringTo<TypeParam>(std::string_view{"0.0"}));
		EXPECT_EQ(TypeParam{0.5l}, NAS2D::stringTo<TypeParam>(std::string_view{"0.5"}));
		EXPECT_EQ(TypeParam{1.0l}, NAS2D::stringTo<TypeParam>(std::string_view{"1.0"}));
	}
}


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
	testArithmeticErrorInvalid<char>();
}

TEST(StringTo, integerErrorInvalidShort) {
	testArithmeticErrorInvalid<short>();
}

TEST(StringTo, integerErrorInvalidInt) {
	testArithmeticErrorInvalid<int>();
}

TEST(StringTo, integerErrorInvalidLong) {
	testArithmeticErrorInvalid<long>();
}

TEST(StringTo, integerErrorInvalidLongLong) {
	testArithmeticErrorInvalid<long long>();
}

TEST(StringTo, floatingPointErrorInvalidFloat) {
	testArithmeticErrorInvalid<float>();
}

TEST(StringTo, floatingPointErrorInvalidDouble) {
	testArithmeticErrorInvalid<double>();
}

TEST(StringTo, floatingPointErrorInvalidLongDouble) {
	testArithmeticErrorInvalid<long double>();
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
	testIntegerUnsigned<unsigned char>();
}

TEST(StringTo, integerUnsignedShort) {
	testIntegerUnsigned<unsigned short>();
}

TEST(StringTo, integerUnsignedInt) {
	testIntegerUnsigned<unsigned int>();
}

TEST(StringTo, integerUnsignedLong) {
	testIntegerUnsigned<unsigned long>();
}

TEST(StringTo, integerUnsignedLongLong) {
	testIntegerUnsigned<unsigned long long>();
}


TEST(StringTo, integerSignedChar) {
	testIntegerSigned<signed char>();
}

TEST(StringTo, integerSignedShort) {
	testIntegerSigned<short>();
}

TEST(StringTo, integerSignedInt) {
	testIntegerSigned<int>();
}

TEST(StringTo, integerSignedLong) {
	testIntegerSigned<long>();
}

TEST(StringTo, integerSignedLongLong) {
	testIntegerSigned<long long>();
}


TEST(StringTo, floatingPointFloat) {
	testFloatingPoint<float>();
}

TEST(StringTo, floatingPointDouble) {
	testFloatingPoint<double>();
}

TEST(StringTo, floatingPointLongDouble) {
	testFloatingPoint<long double>();
}
