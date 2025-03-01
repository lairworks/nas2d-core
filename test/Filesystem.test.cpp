#include "NAS2D/Filesystem.h"

#include <gtest/gtest.h>
#include <gmock/gmock.h>


TEST(FilesystemStatic, dirSeparator) {
	// Varies by platform, so we can't know the exact value ("/", "\", ":")
	// New platforms may choose a new unique value
	// Some platforms may not even have a hierarchal filesystem ("")
	EXPECT_NO_THROW(NAS2D::Filesystem::dirSeparator());
}

TEST(FilesystemStatic, parentPath) {
	EXPECT_EQ("", NAS2D::Filesystem::parentPath(""));
	EXPECT_EQ("", NAS2D::Filesystem::parentPath("file.extension"));
	EXPECT_EQ("/", NAS2D::Filesystem::parentPath("/"));
	EXPECT_EQ("data/", NAS2D::Filesystem::parentPath("data/"));
	EXPECT_EQ("data/", NAS2D::Filesystem::parentPath("data/file.extension"));
	EXPECT_EQ("data/subfolder/", NAS2D::Filesystem::parentPath("data/subfolder/file.extension"));
	EXPECT_EQ("anotherFolder/", NAS2D::Filesystem::parentPath("anotherFolder/file.extension"));
}

TEST(FilesystemStatic, extension) {
	EXPECT_EQ("", NAS2D::Filesystem::extension(""));
	EXPECT_EQ("", NAS2D::Filesystem::extension("file"));
	EXPECT_EQ("", NAS2D::Filesystem::extension("subdir/file"));
	EXPECT_EQ("", NAS2D::Filesystem::extension("subdir.ext/file"));
	EXPECT_EQ(".", NAS2D::Filesystem::extension("file."));
	EXPECT_EQ("", NAS2D::Filesystem::extension(".file"));

	EXPECT_EQ(".a", NAS2D::Filesystem::extension("file.a"));
	EXPECT_EQ(".txt", NAS2D::Filesystem::extension("file.txt"));
	EXPECT_EQ(".txt", NAS2D::Filesystem::extension("subdir/file.txt"));
	EXPECT_EQ(".reallyLongExtensionName", NAS2D::Filesystem::extension("file.reallyLongExtensionName"));
}


namespace {
	class Filesystem : public ::testing::Test {
	protected:
		static constexpr auto AppName = "NAS2DUnitTests";
		static constexpr auto OrganizationName = "LairWorks";

		Filesystem() :
			fs(AppName, OrganizationName)
		{
			fs.mount(fs.basePath());
			fs.mount("data/");
			fs.mountReadWrite(fs.prefPath());
		}

		NAS2D::Filesystem fs;
	};
}


constexpr auto EndsWithDirSeparator = []() {
	return testing::Truly([](const auto& path) { return path.string().ends_with(NAS2D::Filesystem::dirSeparator()); });
};


constexpr auto HasPartialPath = [](const auto& subString) {
	return testing::Truly([&](const auto& path) { return path.string().find(subString) != std::string::npos; });
};


TEST_F(Filesystem, basePath) {
	// Result is a directory, and should end with a directory separator
	EXPECT_THAT(fs.basePath(), EndsWithDirSeparator());
}

TEST_F(Filesystem, prefPath) {
	// Result is a directory, and should end with a directory separator
	EXPECT_THAT(fs.prefPath(), EndsWithDirSeparator());
	EXPECT_THAT(fs.prefPath(), HasPartialPath(AppName));
}

TEST_F(Filesystem, searchPath) {
	auto pathList = fs.searchPath();
	EXPECT_EQ(3u, pathList.size());
	EXPECT_THAT(pathList, Contains(HasPartialPath("NAS2DUnitTests")));
	EXPECT_THAT(pathList, Contains(HasPartialPath("data/")));
}

TEST_F(Filesystem, directoryList) {
	{
		auto pathList = fs.directoryList("");
		EXPECT_LE(1u, pathList.size());
		EXPECT_THAT(pathList, testing::Contains(std::filesystem::path{"file.txt"}));
	}

	{
		auto pathList = fs.directoryList("", "txt");
		EXPECT_LE(1u, pathList.size());
		EXPECT_THAT(pathList, testing::Contains(std::filesystem::path{"file.txt"}));
	}
}

TEST_F(Filesystem, exists) {
	EXPECT_TRUE(fs.exists("file.txt"));
}

TEST_F(Filesystem, read) {
	const auto data = fs.readFile("file.txt");
	EXPECT_THAT(data, testing::StartsWith("Test data"));

	EXPECT_THROW(fs.readFile("FileDoesNotExist.txt"), std::runtime_error);
}

// Test a few related methods. Some don't test well standalone.
TEST_F(Filesystem, writeReadDeleteExists) {
	const std::string testFilename = "TestFile.txt";
	const std::string testData = "Test file contents";

	EXPECT_NO_THROW(fs.writeFile(testFilename, testData));
	EXPECT_TRUE(fs.exists(testFilename));

	// Try to overwrite file, with and without permission
	EXPECT_NO_THROW(fs.writeFile(testFilename, testData));
	EXPECT_THROW(fs.writeFile(testFilename, testData, NAS2D::Filesystem::WriteFlags::NoOverwrite), std::runtime_error);

	const auto data = fs.readFile(testFilename);
	EXPECT_EQ(testData, data);

	EXPECT_NO_THROW(fs.del(testFilename));
	EXPECT_FALSE(fs.exists(testFilename));
	EXPECT_THROW(fs.del(testFilename), std::runtime_error);
}

TEST_F(Filesystem, isDirectoryMakeDirectory) {
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

TEST_F(Filesystem, mountUnmount) {
	const std::string extraMount = "data/extraData/";
	const std::string extraFile = "extraFile.txt";

	EXPECT_FALSE(fs.exists(extraFile));
	EXPECT_NO_THROW(fs.mount(extraMount));
	EXPECT_THAT(fs.searchPath(), Contains(HasPartialPath(extraMount)));
	EXPECT_TRUE(fs.exists(extraFile));

	EXPECT_NO_THROW(fs.unmount(extraMount));
	EXPECT_FALSE(fs.exists(extraFile));

	EXPECT_THROW(fs.mount("nonExistentPath/"), std::runtime_error);
}
