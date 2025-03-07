#include "NAS2D/StringFrom.h"
#include "NAS2D/StringTo.h"

#include <gtest/gtest.h>
#include <gmock/gmock.h>

#include <string>


TEST(StringFromTo, stringDataString) {
	EXPECT_EQ("Some string value", NAS2D::stringFrom<std::string>(NAS2D::stringTo<std::string>("Some string value")));

	EXPECT_EQ("false", NAS2D::stringFrom<bool>(NAS2D::stringTo<bool>("false")));
	EXPECT_EQ("true", NAS2D::stringFrom<bool>(NAS2D::stringTo<bool>("true")));

	EXPECT_EQ("-1", NAS2D::stringFrom<int>(NAS2D::stringTo<int>("-1")));
	EXPECT_EQ("0", NAS2D::stringFrom<int>(NAS2D::stringTo<int>("0")));

	EXPECT_THAT(NAS2D::stringFrom<float>(NAS2D::stringTo<float>("-1.0")), testing::StartsWith("-1.0"));
	EXPECT_THAT(NAS2D::stringFrom<float>(NAS2D::stringTo<float>("1.0")), testing::StartsWith("1.0"));
}

TEST(StringFromTo, dataStringData) {
	EXPECT_EQ("Some string value", NAS2D::stringTo<std::string>(NAS2D::stringFrom<std::string>("Some string value")));

	EXPECT_EQ(false, NAS2D::stringTo<bool>(NAS2D::stringFrom<bool>(false)));
	EXPECT_EQ(true, NAS2D::stringTo<bool>(NAS2D::stringFrom<bool>(true)));

	EXPECT_EQ(-1, NAS2D::stringTo<int>(NAS2D::stringFrom<int>(-1)));
	EXPECT_EQ(0, NAS2D::stringTo<int>(NAS2D::stringFrom<int>(0)));
	EXPECT_EQ(1, NAS2D::stringTo<int>(NAS2D::stringFrom<int>(1)));

	EXPECT_EQ(-1, NAS2D::stringTo<float>(NAS2D::stringFrom<float>(-1)));
	EXPECT_EQ(0, NAS2D::stringTo<float>(NAS2D::stringFrom<float>(0)));
	EXPECT_EQ(1, NAS2D::stringTo<float>(NAS2D::stringFrom<float>(1)));
}
