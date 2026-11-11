#include "NAS2D/StringFrom.h"
#include "NAS2D/StringTo.h"

#include <gtest/gtest.h>
#include <gmock/gmock.h>

#include <string>


TEST(StringTwoWayFromTo, dataStringData) {
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
