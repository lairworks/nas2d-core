#include "NAS2D/FilesystemPathParents.h"

#include <gtest/gtest.h>
#include <gmock/gmock.h>


TEST(FilesystemPathParents, sizeRelativeTrailingSlash) {
	EXPECT_EQ(1u, NAS2D::FilesystemPathParents{NAS2D::RealPath{""}}.size());
	EXPECT_EQ(2u, NAS2D::FilesystemPathParents{NAS2D::RealPath{"a/"}}.size());
	EXPECT_EQ(3u, NAS2D::FilesystemPathParents{NAS2D::RealPath{"a/b/"}}.size());
	EXPECT_EQ(4u, NAS2D::FilesystemPathParents{NAS2D::RealPath{"a/b/c/"}}.size());
}

TEST(FilesystemPathParents, sizeRelativeNoTrailingSlash) {
	EXPECT_EQ(1u, NAS2D::FilesystemPathParents{NAS2D::RealPath{""}}.size());
	EXPECT_EQ(2u, NAS2D::FilesystemPathParents{NAS2D::RealPath{"a"}}.size());
	EXPECT_EQ(3u, NAS2D::FilesystemPathParents{NAS2D::RealPath{"a/b"}}.size());
	EXPECT_EQ(4u, NAS2D::FilesystemPathParents{NAS2D::RealPath{"a/b/c"}}.size());
}

TEST(FilesystemPathParents, sizeAbsoluteTrailingSlash) {
	EXPECT_EQ(1u, NAS2D::FilesystemPathParents{NAS2D::RealPath{"/"}}.size());
	EXPECT_EQ(2u, NAS2D::FilesystemPathParents{NAS2D::RealPath{"/a/"}}.size());
	EXPECT_EQ(3u, NAS2D::FilesystemPathParents{NAS2D::RealPath{"/a/b/"}}.size());
	EXPECT_EQ(4u, NAS2D::FilesystemPathParents{NAS2D::RealPath{"/a/b/c/"}}.size());
}

TEST(FilesystemPathParents, sizeAbsoluteNoTrailingSlash) {
	EXPECT_EQ(1u, NAS2D::FilesystemPathParents{NAS2D::RealPath{"/"}}.size());
	EXPECT_EQ(2u, NAS2D::FilesystemPathParents{NAS2D::RealPath{"/a"}}.size());
	EXPECT_EQ(3u, NAS2D::FilesystemPathParents{NAS2D::RealPath{"/a/b"}}.size());
	EXPECT_EQ(4u, NAS2D::FilesystemPathParents{NAS2D::RealPath{"/a/b/c"}}.size());
}

TEST(FilesystemPathParents, iterationValuesRelative) {
	const auto parentPaths = NAS2D::FilesystemPathParents(NAS2D::RealPath{"a/b/c/"});
	const auto paths = std::vector(parentPaths.begin(), parentPaths.end());
	ASSERT_THAT(paths, testing::ElementsAre(NAS2D::RealPath{"a/b/c/"}, NAS2D::RealPath{"a/b/"}, NAS2D::RealPath{"a/"}, NAS2D::RealPath{""}));
}

TEST(FilesystemPathParents, iterationValuesAbsolute) {
	const auto parentPaths = NAS2D::FilesystemPathParents(NAS2D::RealPath{"/a/b/c/"});
	const auto paths = std::vector(parentPaths.begin(), parentPaths.end());
	ASSERT_THAT(paths, testing::ElementsAre(NAS2D::RealPath{"/a/b/c/"}, NAS2D::RealPath{"/a/b/"}, NAS2D::RealPath{"/a/"}, NAS2D::RealPath{"/"}));
}
