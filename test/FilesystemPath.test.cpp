#include "NAS2D/FilesystemPath.h"

#include <gtest/gtest.h>
#include <gmock/gmock.h>


TEST(FilesystemPath, constructionDefault) {
	const auto path = NAS2D::FilesystemPath{};
	EXPECT_EQ("", path.string());
}

TEST(FilesystemPath, constructionConstCharStar) {
	const auto path = NAS2D::FilesystemPath{"a/b/c/"};
	EXPECT_EQ("a/b/c/", path.string());
}

TEST(FilesystemPath, constructionString) {
	const auto path = NAS2D::FilesystemPath{std::string{"a/b/c/"}};
	EXPECT_EQ("a/b/c/", path.string());
}

TEST(FilesystemPath, operatorString) {
	const auto path = NAS2D::FilesystemPath{std::string{"a/b/c/"}};
	EXPECT_EQ(std::string{"a/b/c/"}, std::string{path});
}

TEST(FilesystemPath, operatorEqual) {
	const auto path = NAS2D::FilesystemPath{"a/b/c/"};
	EXPECT_EQ(NAS2D::FilesystemPath{"a/b/c/"}, path);
}

TEST(FilesystemPath, operatorLess) {
	const auto path = NAS2D::FilesystemPath{"a/b/c/"};
	EXPECT_LT(NAS2D::FilesystemPath{"a/b/b/"}, path);
}

TEST(FilesystemPath, operatorSlash) {
	const auto path = NAS2D::FilesystemPath{"a/b/c/"};
	EXPECT_EQ(NAS2D::FilesystemPath{"a/b/c/d/"}, path / "d/");
	EXPECT_EQ(NAS2D::FilesystemPath{"a/b/c/filename"}, path / "filename");
}

TEST(FilesystemPath, stem) {
	const auto path = NAS2D::FilesystemPath{"path/to/filename.ext"};
	EXPECT_EQ(NAS2D::FilesystemPath{"filename"}, path.stem());
}

TEST(FilesystemPath, string) {
	const auto path = NAS2D::FilesystemPath{"a/b/c/"};
	EXPECT_EQ(std::string{"a/b/c/"}, path.string());
}
