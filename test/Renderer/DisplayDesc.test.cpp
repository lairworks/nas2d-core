#include "NAS2D/Renderer/DisplayDesc.h"

#include <gtest/gtest.h>


TEST(DisplayDesc, OperatorEqual) {
	// Check full equality
	EXPECT_EQ((NAS2D::DisplayDesc{0, 0, 0}), (NAS2D::DisplayDesc{0, 0, 0}));
	EXPECT_EQ((NAS2D::DisplayDesc{640, 480, 30}), (NAS2D::DisplayDesc{640, 480, 30}));

	// Check inequality for each individual component
	EXPECT_NE((NAS2D::DisplayDesc{640, 0, 0}), (NAS2D::DisplayDesc{0, 0, 0}));
	EXPECT_NE((NAS2D::DisplayDesc{0, 480, 0}), (NAS2D::DisplayDesc{0, 0, 0}));
	EXPECT_NE((NAS2D::DisplayDesc{0, 0, 30}), (NAS2D::DisplayDesc{0, 0, 0}));

	// Check inequality of all components
	EXPECT_NE((NAS2D::DisplayDesc{640, 480, 30}), (NAS2D::DisplayDesc{1024, 768, 60}));
}

TEST(DisplayDesc, OperatorLessEqual) {
	// Check full equlity
	EXPECT_LE((NAS2D::DisplayDesc{0, 0, 0}), (NAS2D::DisplayDesc{0, 0, 0}));
	EXPECT_LE((NAS2D::DisplayDesc{640, 480, 30}), (NAS2D::DisplayDesc{640, 480, 30}));

	// Check single component difference comparison
	EXPECT_LE((NAS2D::DisplayDesc{1, 480, 30}), (NAS2D::DisplayDesc{640, 480, 30}));
	EXPECT_LE((NAS2D::DisplayDesc{640, 1, 30}), (NAS2D::DisplayDesc{640, 480, 30}));
	EXPECT_LE((NAS2D::DisplayDesc{640, 480, 1}), (NAS2D::DisplayDesc{640, 480, 30}));

	// Check all component difference comparison
	EXPECT_LE((NAS2D::DisplayDesc{640, 480, 30}), (NAS2D::DisplayDesc{1024, 768, 60}));

	// Check incomparable
	EXPECT_FALSE((NAS2D::DisplayDesc{1, 480, 30}) <= (NAS2D::DisplayDesc{640, 1, 30}));
	EXPECT_FALSE((NAS2D::DisplayDesc{1, 480, 30}) <= (NAS2D::DisplayDesc{640, 480, 1}));
	EXPECT_FALSE((NAS2D::DisplayDesc{640, 1, 30}) <= (NAS2D::DisplayDesc{640, 480, 1}));
}

TEST(DisplayDesc, OperatorGreaterEqual) {
	// Check full equlity
	EXPECT_GE((NAS2D::DisplayDesc{0, 0, 0}), (NAS2D::DisplayDesc{0, 0, 0}));
	EXPECT_GE((NAS2D::DisplayDesc{640, 480, 30}), (NAS2D::DisplayDesc{640, 480, 30}));

	// Check single component difference comparison
	EXPECT_GE((NAS2D::DisplayDesc{640, 480, 30}), (NAS2D::DisplayDesc{1, 480, 30}));
	EXPECT_GE((NAS2D::DisplayDesc{640, 480, 30}), (NAS2D::DisplayDesc{640, 1, 30}));
	EXPECT_GE((NAS2D::DisplayDesc{640, 480, 30}), (NAS2D::DisplayDesc{640, 480, 1}));

	// Check all component difference comparison
	EXPECT_GE((NAS2D::DisplayDesc{1024, 768, 60}), (NAS2D::DisplayDesc{640, 480, 30}));

	// Check incomparable
	EXPECT_FALSE((NAS2D::DisplayDesc{1, 480, 30}) >= (NAS2D::DisplayDesc{640, 1, 30}));
	EXPECT_FALSE((NAS2D::DisplayDesc{1, 480, 30}) >= (NAS2D::DisplayDesc{640, 480, 1}));
	EXPECT_FALSE((NAS2D::DisplayDesc{640, 1, 30}) >= (NAS2D::DisplayDesc{640, 480, 1}));
}

TEST(DisplayDesc, OperatorLess) {
	// Check full equlity
	EXPECT_FALSE((NAS2D::DisplayDesc{0, 0, 0}) < (NAS2D::DisplayDesc{0, 0, 0}));
	EXPECT_FALSE((NAS2D::DisplayDesc{640, 480, 30}) < (NAS2D::DisplayDesc{640, 480, 30}));

	// Check single component difference comparison
	EXPECT_LT((NAS2D::DisplayDesc{1, 480, 30}), (NAS2D::DisplayDesc{640, 480, 30}));
	EXPECT_LT((NAS2D::DisplayDesc{640, 1, 30}), (NAS2D::DisplayDesc{640, 480, 30}));
	EXPECT_LT((NAS2D::DisplayDesc{640, 480, 1}), (NAS2D::DisplayDesc{640, 480, 30}));

	// Check all component difference comparison
	EXPECT_LT((NAS2D::DisplayDesc{640, 480, 30}), (NAS2D::DisplayDesc{1024, 768, 60}));

	// Check incomparable
	EXPECT_FALSE((NAS2D::DisplayDesc{1, 480, 30}) < (NAS2D::DisplayDesc{640, 1, 30}));
	EXPECT_FALSE((NAS2D::DisplayDesc{1, 480, 30}) < (NAS2D::DisplayDesc{640, 480, 1}));
	EXPECT_FALSE((NAS2D::DisplayDesc{640, 1, 30}) < (NAS2D::DisplayDesc{640, 480, 1}));
}

TEST(DisplayDesc, OperatorGreater) {
	// Check full equlity
	EXPECT_FALSE((NAS2D::DisplayDesc{0, 0, 0}) > (NAS2D::DisplayDesc{0, 0, 0}));
	EXPECT_FALSE((NAS2D::DisplayDesc{640, 480, 30}) > (NAS2D::DisplayDesc{640, 480, 30}));

	// Check single component difference comparison
	EXPECT_GT((NAS2D::DisplayDesc{640, 480, 30}), (NAS2D::DisplayDesc{1, 480, 30}));
	EXPECT_GT((NAS2D::DisplayDesc{640, 480, 30}), (NAS2D::DisplayDesc{640, 1, 30}));
	EXPECT_GT((NAS2D::DisplayDesc{640, 480, 30}), (NAS2D::DisplayDesc{640, 480, 1}));

	// Check all component difference comparison
	EXPECT_GT((NAS2D::DisplayDesc{1024, 768, 60}), (NAS2D::DisplayDesc{640, 480, 30}));

	// Check incomparable
	EXPECT_FALSE((NAS2D::DisplayDesc{1, 480, 30}) > (NAS2D::DisplayDesc{640, 1, 30}));
	EXPECT_FALSE((NAS2D::DisplayDesc{1, 480, 30}) > (NAS2D::DisplayDesc{640, 480, 1}));
	EXPECT_FALSE((NAS2D::DisplayDesc{640, 1, 30}) > (NAS2D::DisplayDesc{640, 480, 1}));
}
