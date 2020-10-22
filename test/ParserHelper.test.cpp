#include "NAS2D/ParserHelper.h"

#include <gtest/gtest.h>


TEST(ParserHelper, reportMissingOrUnexpected) {
	// Nothing missing or unexpected
	EXPECT_NO_THROW(NAS2D::reportMissingOrUnexpected({}, {}));
	// Unexpected result
	EXPECT_THROW(NAS2D::reportMissingOrUnexpected({"a"}, {}), std::runtime_error);
	EXPECT_THROW(NAS2D::reportMissingOrUnexpected({}, {"b"}), std::runtime_error);
	EXPECT_THROW(NAS2D::reportMissingOrUnexpected({"a"}, {"b"}), std::runtime_error);

	// Nothing missing or unexpected
	EXPECT_NO_THROW(NAS2D::reportMissingOrUnexpected({}, {}, {}));
	EXPECT_NO_THROW(NAS2D::reportMissingOrUnexpected({}, {}, {"a"}));
	EXPECT_NO_THROW(NAS2D::reportMissingOrUnexpected({"a"}, {}, {"a"}));
	// Missing required value
	EXPECT_THROW(NAS2D::reportMissingOrUnexpected({}, {"a"}, {}), std::runtime_error);
	// Unexpected value
	EXPECT_THROW(NAS2D::reportMissingOrUnexpected({"a"}, {}, {}), std::runtime_error);
	// Both missing and unexpected
	EXPECT_THROW(NAS2D::reportMissingOrUnexpected({"b"}, {"a"}, {}), std::runtime_error);
}
