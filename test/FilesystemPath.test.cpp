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

TEST(FilesystemPath, constructionStringView) {
	const auto path = NAS2D::FilesystemPath{std::string_view{"a/b/c/"}};
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
	EXPECT_EQ(NAS2D::FilesystemPath{"a/b/c/d/"}, path / NAS2D::FilesystemPath{"d/"});
	EXPECT_EQ(NAS2D::FilesystemPath{"a/b/c/filename"}, path / NAS2D::FilesystemPath{"filename"});
}

TEST(FilesystemPath, isRelative) {
	EXPECT_TRUE(NAS2D::FilesystemPath{""}.isRelative());
	EXPECT_TRUE(NAS2D::FilesystemPath{"a/"}.isRelative());
	EXPECT_TRUE(NAS2D::FilesystemPath{"a/filename"}.isRelative());
}

TEST(FilesystemPath, componentCountRelative) {
	EXPECT_EQ(0u, NAS2D::FilesystemPath{""}.componentCount());
	EXPECT_EQ(1u, NAS2D::FilesystemPath{"a/"}.componentCount());
	EXPECT_EQ(2u, NAS2D::FilesystemPath{"a/b/"}.componentCount());
	EXPECT_EQ(3u, NAS2D::FilesystemPath{"a/b/c/"}.componentCount());
	EXPECT_EQ(4u, NAS2D::FilesystemPath{"a/b/c/filename"}.componentCount());
}

TEST(FilesystemPath, componentCountAbsolute) {
	EXPECT_EQ(1u, NAS2D::FilesystemPath{"/"}.componentCount());
	EXPECT_EQ(2u, NAS2D::FilesystemPath{"/a/"}.componentCount());
	EXPECT_EQ(3u, NAS2D::FilesystemPath{"/a/b/"}.componentCount());
	EXPECT_EQ(4u, NAS2D::FilesystemPath{"/a/b/c/"}.componentCount());
	EXPECT_EQ(5u, NAS2D::FilesystemPath{"/a/b/c/filename"}.componentCount());
}

TEST(FilesystemPath, absoluteRelative) {
	const auto path = NAS2D::FilesystemPath{"a"} / NAS2D::FilesystemPath{"filename"};
	const auto pathAbsolute = path.absolute();
	EXPECT_THAT(pathAbsolute, testing::EndsWith(path));
	EXPECT_NE(path, pathAbsolute);
}

TEST(FilesystemPath, absoluteAbsolute) {
	const auto path = NAS2D::FilesystemPath{"a/filename"}.absolute();
	EXPECT_EQ(path, path.absolute());
}

TEST(FilesystemPath, parentFoldersRelative) {
	const auto path = NAS2D::FilesystemPath{"a/b/c/"};
	EXPECT_EQ(NAS2D::FilesystemPath{"a/b/"}, path.parent());
	EXPECT_EQ(NAS2D::FilesystemPath{"a/"}, path.parent().parent());
	EXPECT_EQ(NAS2D::FilesystemPath{""}, path.parent().parent().parent());
}

TEST(FilesystemPath, parentFoldersAbsolute) {
	const auto path = NAS2D::FilesystemPath{"/a/b/c/"};
	EXPECT_EQ(NAS2D::FilesystemPath{"/a/b/"}, path.parent());
	EXPECT_EQ(NAS2D::FilesystemPath{"/a/"}, path.parent().parent());
	EXPECT_EQ(NAS2D::FilesystemPath{"/"}, path.parent().parent().parent());
}

TEST(FilesystemPath, parentFileRelative) {
	const auto path = NAS2D::FilesystemPath{"a/b/c/filename"};
	EXPECT_EQ(NAS2D::FilesystemPath{"a/b/c/"}, path.parent());
}

TEST(FilesystemPath, parentFileAbsolute) {
	const auto path = NAS2D::FilesystemPath{"/a/b/c/filename"};
	EXPECT_EQ(NAS2D::FilesystemPath{"/a/b/c/"}, path.parent());
}

TEST(FilesystemPath, stem) {
	const auto path = NAS2D::FilesystemPath{"path/to/filename.ext"};
	EXPECT_EQ(NAS2D::FilesystemPath{"filename"}, path.stem());
}

TEST(FilesystemPath, string) {
	const auto path = NAS2D::FilesystemPath{"a/b/c/"};
	EXPECT_EQ(std::string{"a/b/c/"}, path.string());
}

TEST(FilesystemPath, constCharStarConcat) {
	const auto path = NAS2D::FilesystemPath{"a/b/c/"};
	EXPECT_EQ(std::string{"Path is: a/b/c/"}, "Path is: " + path);
	EXPECT_EQ(std::string{"a/b/c/ was the path"}, path + " was the path");
}
