#include "NAS2D/FilesystemPathParents.h"

#include <gtest/gtest.h>
#include <gmock/gmock.h>


TEST(FilesystemPathParents, sizeRelativeTrailingSlash) {
	EXPECT_EQ(1, NAS2D::FilesystemPathParents{""}.size());
	EXPECT_EQ(2, NAS2D::FilesystemPathParents{"a/"}.size());
	EXPECT_EQ(3, NAS2D::FilesystemPathParents{"a/b/"}.size());
	EXPECT_EQ(4, NAS2D::FilesystemPathParents{"a/b/c/"}.size());
}

TEST(FilesystemPathParents, sizeRelativeNoTrailingSlash) {
	EXPECT_EQ(1, NAS2D::FilesystemPathParents{""}.size());
	EXPECT_EQ(2, NAS2D::FilesystemPathParents{"a"}.size());
	EXPECT_EQ(3, NAS2D::FilesystemPathParents{"a/b"}.size());
	EXPECT_EQ(4, NAS2D::FilesystemPathParents{"a/b/c"}.size());
}

TEST(FilesystemPathParents, sizeAbsoluteTrailingSlash) {
	EXPECT_EQ(1, NAS2D::FilesystemPathParents{"/"}.size());
	EXPECT_EQ(2, NAS2D::FilesystemPathParents{"/a/"}.size());
	EXPECT_EQ(3, NAS2D::FilesystemPathParents{"/a/b/"}.size());
	EXPECT_EQ(4, NAS2D::FilesystemPathParents{"/a/b/c/"}.size());
}

TEST(FilesystemPathParents, sizeAbsoluteNoTrailingSlash) {
	EXPECT_EQ(1, NAS2D::FilesystemPathParents{"/"}.size());
	EXPECT_EQ(2, NAS2D::FilesystemPathParents{"/a"}.size());
	EXPECT_EQ(3, NAS2D::FilesystemPathParents{"/a/b"}.size());
	EXPECT_EQ(4, NAS2D::FilesystemPathParents{"/a/b/c"}.size());
}

TEST(FilesystemPathParents, iterationValuesRelative) {
	const auto parentPaths = NAS2D::FilesystemPathParents("a/b/c/");
	const auto paths = std::vector(parentPaths.begin(), parentPaths.end());
	ASSERT_THAT(paths, testing::ElementsAre("a/b/c/", "a/b/", "a/", ""));
}

TEST(FilesystemPathParents, iterationValuesAbsolute) {
	const auto parentPaths = NAS2D::FilesystemPathParents("/a/b/c/");
	const auto paths = std::vector(parentPaths.begin(), parentPaths.end());
	ASSERT_THAT(paths, testing::ElementsAre("/a/b/c/", "/a/b/", "/a/", "/"));
}
