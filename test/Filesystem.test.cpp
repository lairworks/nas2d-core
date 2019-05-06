#include "NAS2D/Filesystem.h"
#include <gtest/gtest.h>
#include <gmock/gmock.h>


TEST(Filesystem, ConstructDestruct) {
	// Check if object can be safely constructed and destructed
	EXPECT_NO_THROW(NAS2D::Filesystem fs);
}

TEST(Filesystem, Init) {
	NAS2D::Filesystem fs;
	EXPECT_NO_THROW(fs.init("", "NAS2DUnitTests", "LairWorks", "./"));
}


class FilesystemTest : public ::testing::Test {
protected:
	void SetUp() override {
		fs.init("", "NAS2DUnitTests", "LairWorks", "data/");
	}

	NAS2D::Filesystem fs;
};


TEST_F(FilesystemTest, dataPath) {
	EXPECT_EQ("data/", fs.dataPath());
}

TEST_F(FilesystemTest, workingPath) {
	EXPECT_EQ("data/", fs.workingPath("data/file.extension"));
	EXPECT_EQ("data/subfolder/", fs.workingPath("data/subfolder/file.extension"));
	EXPECT_EQ("anotherFolder/", fs.workingPath("anotherFolder/file.extension"));
	EXPECT_EQ("", fs.workingPath("file.extension"));
}

TEST_F(FilesystemTest, searchPath) {
	auto pathList = fs.searchPath();
	EXPECT_EQ(3, pathList.size());
	EXPECT_THAT(pathList, Contains(testing::HasSubstr("NAS2DUnitTests/")));
	EXPECT_THAT(pathList, Contains(testing::HasSubstr("data/")));
}

TEST_F(FilesystemTest, directoryList) {
	auto pathList = fs.directoryList("");
	EXPECT_LE(1, pathList.size());
	EXPECT_THAT(pathList, Contains(testing::StrEq("file.txt")));
}

TEST_F(FilesystemTest, exists) {
	EXPECT_TRUE(fs.exists("file.txt"));
}

TEST_F(FilesystemTest, open) {
	const auto file = fs.open("file.txt");
	EXPECT_EQ("Test data\n", file.bytes());
}

// Test a few related methods. Some don't test well standalone.
TEST_F(FilesystemTest, writeReadDeleteExists) {
	const std::string testFilename = "TestFile.txt";
	const std::string testData = "Test file contents";
	const auto file = NAS2D::File(testData, testFilename);

	EXPECT_TRUE(fs.write(file));
	EXPECT_TRUE(fs.exists(testFilename));

	// Try to overwrite file, with and without permission
	EXPECT_TRUE(fs.write(file));
	EXPECT_FALSE(fs.write(file, false));

	const auto fileRead = fs.open(testFilename);
	EXPECT_EQ(testData, fileRead.bytes());

	fs.del(testFilename);
	EXPECT_FALSE(fs.exists(testFilename));
}
