#include "NAS2D/ContainerUtils.h"

#include <gmock/gmock.h>
#include <gtest/gtest.h>

#include <string>
#include <array>
#include <vector>
#include <list>
#include <map>
#include <unordered_map>


TEST(Container, VectorSelfAdd) {
	const std::vector b{3, 4};

	{
		std::vector a{1, 2};
		EXPECT_EQ((std::vector{1, 2, 3, 4}), NAS2D::ContainerOperators::operator+=(a, b));
		EXPECT_EQ((std::vector{1, 2, 3, 4}), a);
	}

	{
		std::vector a{1, 2};
		using namespace NAS2D::ContainerOperators;
		EXPECT_EQ((std::vector{1, 2, 3, 4}), a += b);
		EXPECT_EQ((std::vector{1, 2, 3, 4}), a);
	}
}

TEST(Container, VectorAdd) {
	const std::vector a{1, 2};
	const std::vector b{3, 4};

	EXPECT_EQ((std::vector{1, 2, 3, 4}), NAS2D::ContainerOperators::operator+(a, b));

	{
		using namespace NAS2D::ContainerOperators;
		EXPECT_EQ((std::vector{1, 2, 3, 4}), a + b);
	}
}

TEST(Container, VectorSelfSubtract) {
	const std::vector b{2, 3};

	{
		std::vector a{1, 2, 3, 4};
		EXPECT_EQ((std::vector{1, 4}), NAS2D::ContainerOperators::operator-=(a, b));
		EXPECT_EQ((std::vector{1, 4}), a);
	}

	{
		std::vector a{1, 2, 3, 4};
		using namespace NAS2D::ContainerOperators;
		EXPECT_EQ((std::vector{1, 4}), a -= b);
		EXPECT_EQ((std::vector{1, 4}), a);
	}
}

TEST(Container, VectorSubtract) {
	const std::vector a{1, 2, 3, 4};
	const std::vector b{2, 3};

	EXPECT_EQ((std::vector{1, 4}), NAS2D::ContainerOperators::operator-(a, b));

	{
		using namespace NAS2D::ContainerOperators;
		EXPECT_EQ((std::vector{1, 4}), a - b);
	}
}

TEST(Container, has) {
	EXPECT_FALSE(NAS2D::has(std::array<int, 0>{}, 1));
	EXPECT_FALSE(NAS2D::has(std::array{2}, 1));
	EXPECT_FALSE(NAS2D::has(std::array{2, 3}, 1));
	EXPECT_FALSE(NAS2D::has(std::array{-1, 0, 2, 3}, 1));

	EXPECT_TRUE(NAS2D::has(std::array{1}, 1));
	EXPECT_TRUE(NAS2D::has(std::array{0, 1}, 1));
	EXPECT_TRUE(NAS2D::has(std::array{0, 1, 2}, 1));

	EXPECT_FALSE(NAS2D::has(std::vector<int>{}, 1));
	EXPECT_FALSE(NAS2D::has(std::vector{2}, 1));
	EXPECT_FALSE(NAS2D::has(std::vector{2, 3}, 1));
	EXPECT_FALSE(NAS2D::has(std::vector{-1, 0, 2, 3}, 1));

	EXPECT_TRUE(NAS2D::has(std::vector{1}, 1));
	EXPECT_TRUE(NAS2D::has(std::vector{0, 1}, 1));
	EXPECT_TRUE(NAS2D::has(std::vector{0, 1, 2}, 1));
}

TEST(Container, getKeys) {
	EXPECT_EQ((std::vector<std::string>{}), NAS2D::getKeys(std::map<std::string, int>{}));
	EXPECT_EQ((std::vector<std::string>{"Key1"}), NAS2D::getKeys(std::map<std::string, int>{{"Key1", 1}}));
	EXPECT_EQ((std::vector<std::string>{"Key1", "Key2"}), NAS2D::getKeys(std::map<std::string, int>{{"Key1", 1}, {"Key2", 2}}));

	EXPECT_EQ((std::vector<int>{}), NAS2D::getKeys(std::map<int, int>{}));
	EXPECT_EQ((std::vector<int>{1}), NAS2D::getKeys(std::map<int, int>{{1, 10}}));
	EXPECT_EQ((std::vector<int>{1, 2}), NAS2D::getKeys(std::map<int, int>{{1, 10}, {2, 20}}));

	EXPECT_EQ((std::vector<int>{}), NAS2D::getKeys(std::multimap<int, int>{}));
	EXPECT_EQ((std::vector<int>{1}), NAS2D::getKeys(std::multimap<int, int>{{1, 10}}));
	EXPECT_EQ((std::vector<int>{1, 2}), NAS2D::getKeys(std::multimap<int, int>{{1, 10}, {2, 20}}));

	EXPECT_EQ((std::vector<int>{}), NAS2D::getKeys(std::unordered_map<int, int>{}));
	EXPECT_EQ((std::vector<int>{1}), NAS2D::getKeys(std::unordered_map<int, int>{{1, 10}}));
	{
		auto result = NAS2D::getKeys(std::unordered_map<int, int>{{1, 10}, {2, 20}});
		std::sort(std::begin(result), std::end(result));
		EXPECT_EQ((std::vector<int>{1, 2}), result);
	}

	EXPECT_EQ((std::vector<int>{}), NAS2D::getKeys(std::unordered_multimap<int, int>{}));
	EXPECT_EQ((std::vector<int>{1}), NAS2D::getKeys(std::unordered_multimap<int, int>{{1, 10}}));
	{
		auto result = NAS2D::getKeys(std::unordered_multimap<int, int>{{1, 10}, {2, 20}});
		std::sort(std::begin(result), std::end(result));
		EXPECT_EQ((std::vector<int>{1, 2}), result);
	}
}

TEST(Container, findMissingAndUnexpected) {
	using ResultType = NAS2D::MissingAndUnexpected<std::string>;

	EXPECT_EQ((ResultType{{}, {}}), NAS2D::findMissingAndUnexpected<std::string>({}, {}, {}));
	EXPECT_EQ((ResultType{{}, {}}), NAS2D::findMissingAndUnexpected<std::string>({}, {}, {"a"}));
	EXPECT_EQ((ResultType{{}, {}}), NAS2D::findMissingAndUnexpected<std::string>({"a"}, {}, {"a"}));
	EXPECT_EQ((ResultType{{}, {}}), NAS2D::findMissingAndUnexpected<std::string>({"a"}, {"a"}, {}));
	EXPECT_EQ((ResultType{{}, {"a"}}), NAS2D::findMissingAndUnexpected<std::string>({"a"}, {}, {}));
	EXPECT_EQ((ResultType{{"a"}, {}}), NAS2D::findMissingAndUnexpected<std::string>({}, {"a"}, {}));
}
