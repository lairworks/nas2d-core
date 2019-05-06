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
