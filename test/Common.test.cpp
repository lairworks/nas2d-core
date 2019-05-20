#include "NAS2D/Common.h"
#include <gtest/gtest.h>
#include <gmock/gmock.h>


TEST(Version, versionString) {
	EXPECT_THAT(NAS2D::versionString(), testing::MatchesRegex("[[:digit:]]+\\.[[:digit:]]+\\.[[:digit:]]+"));
}
