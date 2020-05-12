#include "NAS2D/StringValue.h"

#include <gmock/gmock.h>
#include <gtest/gtest.h>

#include <vector>
#include <map>


TEST(StringValue, ContainerInitialization) {
	std::vector<NAS2D::StringValue> vect{"", "1", "11"};
	EXPECT_EQ("", vect[0].to<std::string>());
	EXPECT_EQ(1, vect[1].to<int>());
	EXPECT_EQ(11, vect[2].to<int>());

	std::map<std::string, NAS2D::StringValue> someMap{{"abc", "def"}, {"1", 11}};
	EXPECT_EQ("def", someMap.at("abc").to<std::string>());
	EXPECT_EQ(11, someMap.at("1").to<int>());
}


TEST(StringValue, Constructor) {
	EXPECT_EQ("", NAS2D::StringValue{}.value);
	EXPECT_EQ("", NAS2D::StringValue{""}.value);
	EXPECT_EQ("", NAS2D::StringValue{std::string{}}.value);
	EXPECT_EQ("Some string value", NAS2D::StringValue{"Some string value"}.value);

	EXPECT_EQ("false", NAS2D::StringValue{false}.value);
	EXPECT_EQ("true", NAS2D::StringValue{true}.value);

	EXPECT_EQ("-1", NAS2D::StringValue{-1}.value);
	EXPECT_EQ("0", NAS2D::StringValue{0}.value);
	EXPECT_EQ("1", NAS2D::StringValue{1}.value);

	// Ignore precision beyond one decimal place
	EXPECT_THAT(NAS2D::StringValue{0.0}.value, testing::StartsWith("0.0"));
}

TEST(StringValue, OperatorComparison) {
	// Identical constructor types
	EXPECT_EQ(NAS2D::StringValue{}, NAS2D::StringValue{});
	EXPECT_EQ(NAS2D::StringValue{"Value"}, NAS2D::StringValue{"Value"});
	EXPECT_EQ(NAS2D::StringValue{false}, NAS2D::StringValue{false});
	EXPECT_EQ(NAS2D::StringValue{true}, NAS2D::StringValue{true});
	EXPECT_EQ(NAS2D::StringValue{-1}, NAS2D::StringValue{-1});
	EXPECT_EQ(NAS2D::StringValue{0}, NAS2D::StringValue{0});
	EXPECT_EQ(NAS2D::StringValue{1}, NAS2D::StringValue{1});

	// Mixed constructor types
	EXPECT_EQ(NAS2D::StringValue{"false"}, NAS2D::StringValue{false});
	EXPECT_EQ(NAS2D::StringValue{"true"}, NAS2D::StringValue{true});
	EXPECT_EQ(NAS2D::StringValue{"-1"}, NAS2D::StringValue{-1});
	EXPECT_EQ(NAS2D::StringValue{"0"}, NAS2D::StringValue{0});
	EXPECT_EQ(NAS2D::StringValue{"1"}, NAS2D::StringValue{1});
}

TEST(StringValue, OperatorConversion) {
	// Explicit conversion
	EXPECT_EQ(false, static_cast<bool>(NAS2D::StringValue{false}));
	EXPECT_EQ(true, static_cast<bool>(NAS2D::StringValue{true}));
	EXPECT_EQ(0, static_cast<int>(NAS2D::StringValue{0}));

	// Implicit conversion
	EXPECT_EQ(false, bool{NAS2D::StringValue{false}});
	EXPECT_EQ(true, bool{NAS2D::StringValue{true}});
	EXPECT_EQ(-1, int{NAS2D::StringValue{-1}});
	EXPECT_EQ(0, int{NAS2D::StringValue{0}});
	EXPECT_EQ(1, int{NAS2D::StringValue{1}});
	EXPECT_EQ(0.0, int{NAS2D::StringValue{0.0}});
}

TEST(StringValue, to) {
	EXPECT_EQ(false, NAS2D::StringValue{false}.to<bool>());
	EXPECT_EQ(true, NAS2D::StringValue{true}.to<bool>());
	EXPECT_EQ(-1, NAS2D::StringValue{-1}.to<int>());
	EXPECT_EQ(0, NAS2D::StringValue{0}.to<int>());
	EXPECT_EQ(1, NAS2D::StringValue{1}.to<int>());
	EXPECT_EQ(0.0, NAS2D::StringValue{0.0}.to<double>());
}

TEST(StringValue, from) {
	NAS2D::StringValue s;
	EXPECT_EQ("false", s.from<bool>(false).value);
	EXPECT_EQ("true", s.from<bool>(true).value);
	EXPECT_EQ("-1", s.from<int>(-1).value);
	EXPECT_EQ("0", s.from<int>(0).value);
	EXPECT_EQ("1", s.from<int>(1).value);
	EXPECT_THAT(s.from<double>(0.0).value, testing::StartsWith("0.0"));
}
