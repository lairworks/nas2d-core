#include "NAS2D/StringFrom.h"
#include "NAS2D/StringTo.h"

#include <gtest/gtest.h>
#include <gmock/gmock.h>

#include <string>


namespace {
	template <typename TypeParam>
	void testIntegerSigned() {
		EXPECT_EQ("-1", NAS2D::stringFrom<TypeParam>(NAS2D::stringTo<TypeParam>("-1")));
		EXPECT_EQ("0", NAS2D::stringFrom<TypeParam>(NAS2D::stringTo<TypeParam>("0")));
	}
}


TEST(StringTwoWayToFrom, string) {
	EXPECT_EQ("Some string value", NAS2D::stringFrom<std::string>(NAS2D::stringTo<std::string>("Some string value")));
}

TEST(StringTwoWayToFrom, boolean) {
	EXPECT_EQ("false", NAS2D::stringFrom<bool>(NAS2D::stringTo<bool>("false")));
	EXPECT_EQ("true", NAS2D::stringFrom<bool>(NAS2D::stringTo<bool>("true")));
}

TEST(StringTwoWayToFrom, integerSignedInt) {
	testIntegerSigned<int>();
}

TEST(StringTwoWayToFrom, floatingPoint) {
	EXPECT_THAT(NAS2D::stringFrom<float>(NAS2D::stringTo<float>("-1.0")), testing::StartsWith("-1.0"));
	EXPECT_THAT(NAS2D::stringFrom<float>(NAS2D::stringTo<float>("1.0")), testing::StartsWith("1.0"));
}
