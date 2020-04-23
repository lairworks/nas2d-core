#include "NAS2D/Dictionary.h"

#include <gmock/gmock.h>
#include <gtest/gtest.h>


TEST(Dictionary, setGet) {
	NAS2D::Dictionary dictionary;

	// Set some test values
	dictionary.set("Key1", "Some string value");
	dictionary.set("Key2", std::string{"Another string value"});
	dictionary.set("Key3", true);
	dictionary.set("Key4", 1);

	// Read back typed values
	EXPECT_EQ("Some string value", dictionary.get("Key1"));
	EXPECT_EQ("Another string value", dictionary.get("Key2"));
	EXPECT_EQ(true, dictionary.get<bool>("Key3"));
	EXPECT_EQ(1, dictionary.get<int>("Key4"));
}
