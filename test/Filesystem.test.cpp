#include "NAS2D/Filesystem.h"
#include <gtest/gtest.h>
#include <gmock/gmock.h>


TEST(Filesystem, ConstructDestruct) {
	EXPECT_NO_THROW(NAS2D::Filesystem fs("", "NAS2DUnitTests", "LairWorks", "./"));
}

class FilesystemTest : public ::testing::Test {
  protected:
	FilesystemTest() :
		fs("", "NAS2DUnitTests", "LairWorks", "data/")
	{}

	NAS2D::Filesystem fs;
};


TEST_F(FilesystemTest, basePath) {
	// Result is a directory, and should end with a directory separator
	EXPECT_THAT(fs.basePath(), testing::EndsWith(fs.dirSeparator()));
}

TEST_F(FilesystemTest, prefPath) {
	// Result is a directory, and should end with a directory separator
	EXPECT_THAT(fs.prefPath(), testing::EndsWith(fs.dirSeparator()));
}

TEST_F(FilesystemTest, dataPath) {
	EXPECT_EQ("data/", fs.dataPath());
}

TEST_F(FilesystemTest, extension) {
	EXPECT_EQ(".txt", fs.extension("subdir/file.txt"));
	EXPECT_EQ(".txt", fs.extension("file.txt"));
	EXPECT_EQ(".reallyLongExtensionName", fs.extension("file.reallyLongExtensionName"));
	EXPECT_EQ(".a", fs.extension("file.a"));
	EXPECT_EQ(".file", fs.extension(".file"));
	EXPECT_EQ(".", fs.extension("file."));
	EXPECT_EQ("", fs.extension("file"));
}

TEST_F(FilesystemTest, workingPath) {
	EXPECT_EQ("data/", fs.workingPath("data/file.extension"));
	EXPECT_EQ("data/subfolder/", fs.workingPath("data/subfolder/file.extension"));
	EXPECT_EQ("anotherFolder/", fs.workingPath("anotherFolder/file.extension"));
	EXPECT_EQ("", fs.workingPath("file.extension"));
}

TEST_F(FilesystemTest, searchPath) {
	auto pathList = fs.searchPath();
	EXPECT_EQ(3u, pathList.size());
	EXPECT_THAT(pathList, Contains(testing::HasSubstr("NAS2DUnitTests")));
	EXPECT_THAT(pathList, Contains(testing::HasSubstr("data/")));
}

TEST_F(FilesystemTest, directoryList) {
	auto pathList = fs.directoryList("");
	EXPECT_LE(1u, pathList.size());
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

	EXPECT_NO_THROW(fs.write(file));
	EXPECT_TRUE(fs.exists(testFilename));

	// Try to overwrite file, with and without permission
	EXPECT_NO_THROW(fs.write(file));
	EXPECT_THROW(fs.write(file, false), std::runtime_error);

	const auto fileRead = fs.open(testFilename);
	EXPECT_EQ(testData, fileRead.bytes());

	EXPECT_NO_THROW(fs.del(testFilename));
	EXPECT_FALSE(fs.exists(testFilename));
	EXPECT_THROW(fs.del(testFilename), std::runtime_error);
}

TEST_F(FilesystemTest, isDirectoryMakeDirectory) {
	const std::string fileName = "file.txt";
	const std::string folderName = "subfolder/";

	EXPECT_TRUE(fs.exists(fileName));
	EXPECT_FALSE(fs.isDirectory(fileName));

	EXPECT_NO_THROW(fs.makeDirectory(folderName));
	EXPECT_TRUE(fs.exists(folderName));
	EXPECT_TRUE(fs.isDirectory(folderName));

	fs.del(folderName);
	EXPECT_FALSE(fs.exists(folderName));
	EXPECT_FALSE(fs.isDirectory(folderName));
}

TEST_F(FilesystemTest, mountUnmount) {
	const std::string extraMount = "extraData/";
	const std::string extraFile = "extraFile.txt";

	EXPECT_FALSE(fs.exists(extraFile));
	EXPECT_NO_THROW(fs.mount(extraMount));
	EXPECT_THAT(fs.searchPath(), Contains(testing::HasSubstr(extraMount)));
	EXPECT_TRUE(fs.exists(extraFile));

	EXPECT_NO_THROW(fs.unmount(extraMount));
	EXPECT_FALSE(fs.exists(extraFile));

	EXPECT_THROW(fs.mount("nonExistentPath/"), std::runtime_error);
}

TEST_F(FilesystemTest, dirSeparator) {
	// Varies by platform, so we can't know the exact value ("/", "\", ":")
	// New platforms may choose a new unique value
	// Some platforms may not even have a hierarchal filesystem ("")
	EXPECT_NO_THROW(fs.dirSeparator());
}
