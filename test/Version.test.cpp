#include "NAS2D/Version.h"

#include <gmock/gmock.h>
#include <gtest/gtest.h>


TEST(Version, versionString) {
#if GTEST_USES_POSIX_RE == 1
	EXPECT_THAT(NAS2D::versionString(), testing::MatchesRegex(R"([0-9]+\.[0-9]+\.[0-9]+)"));
#elif GTEST_USES_SIMPLE_RE == 1
	EXPECT_THAT(NAS2D::versionString(), testing::MatchesRegex(R"(\d+\.\d+\.\d+)"));
#endif
}
