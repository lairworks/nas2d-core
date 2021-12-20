#include "NAS2D/Parsing/XmlSection.h"
#include "NAS2D/Parsing/XmlFile.h"

#include <gtest/gtest.h>

#include <string>
#include <vector>


class XmlSection : public ::testing::Test {
protected:
	const std::string fileData =
		"<rootElement attributeName=\"attributeValue\">"
			"<subElement subElementAttributeName=\"subElementAttributeValue\">Value</subElement>"
			"<subElementValue>Value</subElementValue>"
		"</rootElement>";
	NAS2D::XmlFile xmlFile{fileData};
	NAS2D::XmlSection root = xmlFile.root();
};


TEST_F(XmlSection, name) {
	EXPECT_EQ("rootElement", root.name());
}

TEST_F(XmlSection, throwError) {
	EXPECT_THROW(root.throwError("Some error string"), std::runtime_error);
}

TEST_F(XmlSection, verifySubSections) {
	EXPECT_NO_THROW(root.verifySubSections({"subElement", "subElementValue"}));
	EXPECT_NO_THROW(root.verifySubSections({"subElement", "subElementValue"}, {"optionalNotFound"}));
	EXPECT_NO_THROW(root.verifySubSections({"subElement"}, {"subElementValue"}));
	EXPECT_NO_THROW(root.verifySubSections({}, {"subElement", "subElementValue"}));
	EXPECT_NO_THROW(root.verifySubSections({}, {"subElement", "subElementValue", "optionalNotFound"}));

	EXPECT_THROW(root.verifySubSections({"subElementNotFound"}), std::runtime_error);
	EXPECT_THROW(root.verifySubSections({}), std::runtime_error);
	EXPECT_THROW(root.verifySubSections({}, {"subElementNotFound"}), std::runtime_error);
}

TEST_F(XmlSection, verifyKeys) {
	EXPECT_NO_THROW(root.verifyKeys({"attributeName", "subElementValue"}));
	EXPECT_NO_THROW(root.verifyKeys({"attributeName", "subElementValue"}, {"optionalNotFound"}));
	EXPECT_NO_THROW(root.verifyKeys({}, {"attributeName", "subElementValue"}));
	EXPECT_NO_THROW(root.verifyKeys({}, {"attributeName", "subElementValue", "optionalNotFound"}));

	EXPECT_THROW(root.verifyKeys({"attributeNotFound"}), std::runtime_error);
	EXPECT_THROW(root.verifyKeys({}), std::runtime_error);
	EXPECT_THROW(root.verifyKeys({}, {"attributeNotFound"}), std::runtime_error);
}

TEST_F(XmlSection, subSectionNames) {
	EXPECT_EQ((std::vector<std::string>{"subElement", "subElementValue"}), root.subSectionNames());
}

TEST_F(XmlSection, keys) {
	EXPECT_EQ((std::vector<std::string>{"attributeName", "subElementValue"}), root.keys());
}

TEST_F(XmlSection, hasSubSection) {
	EXPECT_TRUE(root.hasSubSection("subElement"));
	EXPECT_TRUE(root.hasSubSection("subElementValue"));

	EXPECT_FALSE(root.hasSubSection("elementNotFound"));
	EXPECT_FALSE(root.hasSubSection("attributeName"));
}

TEST_F(XmlSection, hasKey) {
	EXPECT_TRUE(root.hasKey("attributeName"));

	EXPECT_FALSE(root.hasKey("attributeNotFound"));
}

TEST_F(XmlSection, subSection) {
	const auto subElement = root.subSection("subElement");
	EXPECT_EQ("subElement", subElement.name());

	EXPECT_THROW(root.subSection("sectionNameNotFound"), std::runtime_error);
}

TEST_F(XmlSection, subSections) {
	const auto sectionsRange = root.subSections();
	EXPECT_EQ("subElement", (*sectionsRange.begin()).name());

	const auto sections = std::vector<NAS2D::XmlSection>{sectionsRange.begin(), sectionsRange.end()};
	EXPECT_GE(sections.size(), 1);
}

TEST_F(XmlSection, subSectionsNamed) {
	const auto sectionsRange = root.subSections("subElement");
	EXPECT_EQ("subElement", (*sectionsRange.begin()).name());

	const auto sections = std::vector<NAS2D::XmlSection>{sectionsRange.begin(), sectionsRange.end()};
	EXPECT_GE(sections.size(), 1);
}

TEST_F(XmlSection, valueOrEmpty) {
	EXPECT_EQ(std::string{"attributeValue"}, root.valueOrEmpty("attributeName"));
	EXPECT_EQ(std::string{"Value"}, root.valueOrEmpty("subElementValue"));
	EXPECT_EQ(std::string{""}, root.valueOrEmpty("attributeNameNotFound"));
}

TEST_F(XmlSection, value) {
	EXPECT_EQ("attributeValue", root.value("attributeName"));
	EXPECT_EQ("Value", root.value("subElementValue"));
	EXPECT_THROW(root.value("attributeNameNotFound"), std::runtime_error);
}

TEST_F(XmlSection, valueWithDefault) {
	EXPECT_EQ("attributeValue", root.value("attributeName", std::string{""}));
	EXPECT_EQ("Value", root.value("subElementValue", std::string{""}));
	EXPECT_EQ("", root.value("attributeNameNotFound", std::string{""}));
}
