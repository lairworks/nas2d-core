#include "NAS2D/DataUtils.h"

#include <gtest/gtest.h>

TEST(DataUtils, DictionarySetGet)
{
	NAS2D::Dictionary dict;

	// Set some test values
	dict.set("Key2", 500);
	dict.set("Key3", true);

	// Read back typed values
	EXPECT_EQ(500, dict.get<int>("Key2"));
	EXPECT_EQ(true, dict.get<bool>("Key3"));

	// Set values override defaults
	EXPECT_EQ(500, dict.get_or<int>("Key2", 1000));
	EXPECT_EQ(true, dict.get_or<bool>("Key3", false));

	// Default values returned when key does not exist
	EXPECT_EQ(1000, dict.get_or<int>("Key4", 1000));
	EXPECT_EQ(false, dict.get_or<bool>("Key4", false));
}
