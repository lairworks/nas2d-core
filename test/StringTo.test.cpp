#include "NAS2D/StringTo.h"

#include <gtest/gtest.h>
#include <gmock/gmock.h>

#include <string>


TEST(StringTo, stringTo) {
	EXPECT_EQ("SomeStringValue", NAS2D::stringTo<std::string>("SomeStringValue"));
	EXPECT_EQ("SomeStringValue", NAS2D::stringTo<std::string>(std::string{"SomeStringValue"}));

	EXPECT_THROW(NAS2D::stringTo<bool>(""), std::invalid_argument);
	EXPECT_THROW(NAS2D::stringTo<bool>("random text"), std::invalid_argument);
	EXPECT_EQ(false, NAS2D::stringTo<bool>("false"));
	EXPECT_EQ(true, NAS2D::stringTo<bool>("true"));
	EXPECT_EQ(false, NAS2D::stringTo<bool>("0"));
	EXPECT_EQ(true, NAS2D::stringTo<bool>("1"));

	EXPECT_THROW(NAS2D::stringTo<char>(std::to_string(std::numeric_limits<char>::min() - 1)), std::out_of_range);
	EXPECT_THROW(NAS2D::stringTo<char>(std::to_string(std::numeric_limits<char>::max() + 1)), std::out_of_range);
	EXPECT_THROW(NAS2D::stringTo<char>(""), std::invalid_argument);
	EXPECT_EQ(char{0}, NAS2D::stringTo<char>("0"));
	using signedChar = signed char;
	EXPECT_EQ(signedChar{-1}, NAS2D::stringTo<signedChar>("-1"));
	EXPECT_EQ(signedChar{0}, NAS2D::stringTo<signedChar>("0"));
	using unsignedChar = unsigned char;
	EXPECT_EQ(unsignedChar{0}, NAS2D::stringTo<unsignedChar>("0"));

	EXPECT_THROW(NAS2D::stringTo<short>(std::to_string(std::numeric_limits<short>::min() - 1)), std::out_of_range);
	EXPECT_THROW(NAS2D::stringTo<short>(std::to_string(std::numeric_limits<short>::max() + 1)), std::out_of_range);
	EXPECT_THROW(NAS2D::stringTo<short>(""), std::invalid_argument);
	EXPECT_EQ(short{-1}, NAS2D::stringTo<short>("-1"));
	EXPECT_EQ(short{0}, NAS2D::stringTo<short>("0"));
	using unsignedShort = unsigned short;
	EXPECT_EQ(unsignedShort{0}, NAS2D::stringTo<unsignedShort>("0"));

	EXPECT_THROW(NAS2D::stringTo<int>(""), std::invalid_argument);
	EXPECT_EQ(int{-1}, NAS2D::stringTo<int>("-1"));
	EXPECT_EQ(int{0}, NAS2D::stringTo<int>("0"));
	using unsignedInt = unsigned int;
	EXPECT_EQ(unsignedInt{0}, NAS2D::stringTo<unsignedInt>("0"));

	EXPECT_THROW(NAS2D::stringTo<long>(""), std::invalid_argument);
	EXPECT_EQ(long{-1}, NAS2D::stringTo<long>("-1"));
	EXPECT_EQ(long{0}, NAS2D::stringTo<long>("0"));
	using unsignedLong = unsigned long;
	EXPECT_EQ(unsignedLong{0}, NAS2D::stringTo<unsignedLong>("0"));

	using longLong = long long;
	EXPECT_THROW(NAS2D::stringTo<longLong>(""), std::invalid_argument);
	EXPECT_EQ(longLong{-1}, NAS2D::stringTo<longLong>("-1"));
	EXPECT_EQ(longLong{0}, NAS2D::stringTo<longLong>("0"));
	using unsignedLongLong = unsigned long long;
	EXPECT_EQ(unsignedLongLong{0}, NAS2D::stringTo<unsignedLongLong>("0"));

	EXPECT_THROW(NAS2D::stringTo<float>(""), std::invalid_argument);
	EXPECT_THROW(NAS2D::stringTo<double>(""), std::invalid_argument);
	EXPECT_THROW(NAS2D::stringTo<long double>(""), std::invalid_argument);
	EXPECT_EQ(0.0f, NAS2D::stringTo<float>("0.0"));
	EXPECT_EQ(0.0, NAS2D::stringTo<double>("0.0"));
	EXPECT_EQ(0.0l, NAS2D::stringTo<long double>("0.0"));
}
