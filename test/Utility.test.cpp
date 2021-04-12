#include "NAS2D/Utility.h"

#include <gtest/gtest.h>


TEST(Utility, DefaultConstructibleType) {
	struct DefaultConstructible {
	};

	EXPECT_NO_THROW(NAS2D::Utility<DefaultConstructible>::get());
}

TEST(Utility, NonDefaultConstructibleType) {
	struct NonDefaultConstructible {
		NonDefaultConstructible() = delete;
		explicit NonDefaultConstructible(int) {}
	};

	NAS2D::Utility<NonDefaultConstructible>::init(0);
	NAS2D::Utility<NonDefaultConstructible>::get();
}
