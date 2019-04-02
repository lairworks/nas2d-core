#include "NAS2D/Utility.h"
#include <gtest/gtest.h>


TEST(Utility, DefaultConstructibleType) {
	struct DefaultConstructible {
	};

	EXPECT_NO_THROW(NAS2D::Utility<DefaultConstructible>::get());
}
