#include "NAS2D/Filesystem.h"

#include <gtest/gtest.h>
#include <gmock/gmock.h>


TEST(Filesystem, ConstructDestruct) {
	EXPECT_NO_THROW(NAS2D::Filesystem fs("", "NAS2DUnitTests", "LairWorks"));
}

class FilesystemTest : public ::testing::Test {
  protected:

	static constexpr auto AppName = "NAS2DUnitTests";
	static constexpr auto OrganizationName = "LairWorks";

	FilesystemTest() :
		fs("", AppName, OrganizationName)
	{
		fs.mount(fs.basePath());
		fs.mount("data/");
		fs.mountReadWrite(fs.prefPath());
	}

	NAS2D::Filesystem fs;
};


TEST_F(FilesystemTest, basePath) {
	// Result is a directory, and should end with a directory separator
	EXPECT_THAT(fs.basePath(), testing::EndsWith(fs.dirSeparator()));
}

TEST_F(FilesystemTest, prefPath) {
	// Result is a directory, and should end with a directory separator
	EXPECT_THAT(fs.prefPath(), testing::EndsWith(fs.dirSeparator()));
	EXPECT_THAT(fs.prefPath(), testing::HasSubstr(AppName));
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

TEST_F(FilesystemTest, read) {
	const auto data = fs.read("file.txt");
	EXPECT_EQ("Test data\n", data);
}

// Test a few related methods. Some don't test well standalone.
TEST_F(FilesystemTest, writeReadDeleteExists) {
	const std::string testFilename = "TestFile.txt";
	const std::string testData = "Test file contents";

	EXPECT_NO_THROW(fs.write(testFilename, testData));
	EXPECT_TRUE(fs.exists(testFilename));

	// Try to overwrite file, with and without permission
	EXPECT_NO_THROW(fs.write(testFilename, testData));
	EXPECT_THROW(fs.write(testFilename, testData, NAS2D::Filesystem::WriteFlags::NoOverwrite), std::runtime_error);

	const auto data = fs.read(testFilename);
	EXPECT_EQ(testData, data);

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
	const std::string extraMount = "data/extraData/";
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

TEST_F(FilesystemTest, parentPath) {
	EXPECT_EQ("", fs.parentPath(""));
	EXPECT_EQ("", fs.parentPath("file.extension"));
	EXPECT_EQ("/", fs.parentPath("/"));
	EXPECT_EQ("data/", fs.parentPath("data/"));
	EXPECT_EQ("data/", fs.parentPath("data/file.extension"));
	EXPECT_EQ("data/subfolder/", fs.parentPath("data/subfolder/file.extension"));
	EXPECT_EQ("anotherFolder/", fs.parentPath("anotherFolder/file.extension"));
}

TEST_F(FilesystemTest, extension) {
	EXPECT_EQ("", fs.extension("file"));
	EXPECT_EQ(".", fs.extension("file."));
	EXPECT_EQ(".file", fs.extension(".file"));

	EXPECT_EQ(".a", fs.extension("file.a"));
	EXPECT_EQ(".txt", fs.extension("file.txt"));
	EXPECT_EQ(".txt", fs.extension("subdir/file.txt"));
	EXPECT_EQ(".reallyLongExtensionName", fs.extension("file.reallyLongExtensionName"));
}
