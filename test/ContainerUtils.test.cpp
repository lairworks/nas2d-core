#include "NAS2D/ContainerUtils.h"

#include <gmock/gmock.h>
#include <gtest/gtest.h>

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


TEST(Container, mapToVector) {
	{
		int data[]{1, 2, 3};
		EXPECT_EQ((std::vector{1, 2, 3}), NAS2D::mapToVector(data, [](auto x){return x;}));
	}

	{
		const std::vector data{1, 2, 3};
		EXPECT_EQ((std::vector{1, 2, 3}), NAS2D::mapToVector(data, [](auto x){return x;}));
		EXPECT_EQ((std::vector{2, 3, 4}), NAS2D::mapToVector(data, [](auto x){return x + 1;}));
	}

	{
		const std::vector<std::string> data{"a", "bb", "ccc"};
		EXPECT_EQ((std::vector<std::size_t>{1, 2, 3}), NAS2D::mapToVector(data, [](const auto& x){return x.length();}));
		EXPECT_EQ((std::vector<std::size_t>{1, 2, 3}), NAS2D::mapToVector(data, std::mem_fn(&std::string::length)));
	}

	{
		struct Struct {
			const int field;
		};
		const std::array<Struct, 3> data{{{1}, {2}, {3}}};
		EXPECT_EQ((std::vector{1, 2, 3}), NAS2D::mapToVector(data, std::mem_fn(&Struct::field)));
	}
}

TEST(Container, flattenSize) {
	EXPECT_EQ(std::size_t{6}, NAS2D::flattenSize(std::vector<std::string>{"1", "23", "456"}));
	EXPECT_EQ(std::size_t{5}, NAS2D::flattenSize(std::vector<std::vector<int>>{{1, 2}, {3, 4, 5}}));
}

TEST(Container, flatten) {
	EXPECT_EQ(std::string{"123456"}, NAS2D::flatten(std::vector<std::string>{"1", "23", "456"}));
	EXPECT_EQ((std::vector{1, 2, 3, 4, 5}), NAS2D::flatten(std::vector<std::vector<int>>{{1, 2}, {3, 4, 5}}));
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
