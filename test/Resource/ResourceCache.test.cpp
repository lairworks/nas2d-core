#include "NAS2D/Resource/ResourceCache.h"

#include <gtest/gtest.h>


TEST(ResourceCache, load) {
	class MockResource {
	public:
		MockResource(const std::string& initString, int initValue) :
			string{initString},
			value{initValue}
		{}

		bool operator==(const MockResource& other) const {
			return string == other.string && value == other.value;
		}
	private:
		std::string string;
		int value;
	};

	NAS2D::ResourceCache<MockResource, std::string, int> cache;

	const auto& value1 = cache.load("abc", 123);
	const auto& value2 = cache.load("abc", 123);
	const auto& value3 = cache.load("abcd", 123);
	const auto& value4 = cache.load("abc", 1234);

	// Ensure expected values were returned
	EXPECT_EQ((MockResource{"abc", 123}), value1);
	EXPECT_EQ((MockResource{"abc", 123}), value2);
	EXPECT_EQ((MockResource{"abcd", 123}), value3);
	EXPECT_EQ((MockResource{"abc", 1234}), value4);

	// Ensure expected identity of objects (based on their address)
	// References to objects with the same load parameters should be the same object
	// References to objects with different load parameters should be different
	EXPECT_EQ(&value1, &value2);
	EXPECT_NE(&value1, &value3);
	EXPECT_NE(&value1, &value4);
	EXPECT_NE(&value3, &value4);

	EXPECT_EQ(3u, cache.size());
	EXPECT_NO_THROW(cache.unload("not found", 0));
	EXPECT_EQ(3u, cache.size());
	EXPECT_NO_THROW(cache.unload("abc", 123));
	EXPECT_EQ(2u, cache.size());
	EXPECT_NO_THROW(cache.clear());
	EXPECT_EQ(0u, cache.size());
}
