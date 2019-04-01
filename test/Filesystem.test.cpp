#include "NAS2D/Filesystem.h"
#include <gtest/gtest.h>


TEST(Filesystem, DISABLED_ConstructDestruct) {
	// Check if object can be safely constructed and destructed
	EXPECT_NO_THROW(NAS2D::Filesystem fs);
}

TEST(Filesystem, Init) {
	NAS2D::Filesystem fs;
	EXPECT_NO_THROW(fs.init("", "NAS2DUnitTests", "LairWorks", "./"));
}
