#include "NAS2D/StringFrom.h"
#include "NAS2D/StringTo.h"

#include <gtest/gtest.h>
#include <gmock/gmock.h>

#include <string>


namespace {
	template <typename TypeParam>
	void testIntegerSigned() {
		EXPECT_EQ(-1, NAS2D::stringTo<TypeParam>(NAS2D::stringFrom<TypeParam>(-1)));
		EXPECT_EQ(0, NAS2D::stringTo<TypeParam>(NAS2D::stringFrom<TypeParam>(0)));
		EXPECT_EQ(1, NAS2D::stringTo<TypeParam>(NAS2D::stringFrom<TypeParam>(1)));
	}
}


TEST(StringTwoWayFromTo, string) {
	EXPECT_EQ("Some string value", NAS2D::stringTo<std::string>(NAS2D::stringFrom<std::string>("Some string value")));
}

TEST(StringTwoWayFromTo, boolean) {
	EXPECT_EQ(false, NAS2D::stringTo<bool>(NAS2D::stringFrom<bool>(false)));
	EXPECT_EQ(true, NAS2D::stringTo<bool>(NAS2D::stringFrom<bool>(true)));
}

TEST(StringTwoWayFromTo, integerSignedInt) {
	testIntegerSigned<int>();
}

TEST(StringTwoWayFromTo, floatingPoint) {
	EXPECT_EQ(-1, NAS2D::stringTo<float>(NAS2D::stringFrom<float>(-1)));
	EXPECT_EQ(0, NAS2D::stringTo<float>(NAS2D::stringFrom<float>(0)));
	EXPECT_EQ(1, NAS2D::stringTo<float>(NAS2D::stringFrom<float>(1)));
}
