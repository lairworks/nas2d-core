#include "NAS2D/Resource/Font.h"

#include <gtest/gtest.h>


TEST(Font, null) {
	EXPECT_NO_THROW(NAS2D::Font::null());
}

TEST(Font, nullSafelyUsable) {
	const auto font = NAS2D::Font::null();

	EXPECT_EQ((NAS2D::Vector{0, 0}), font.glyphCellSize());
	EXPECT_EQ((NAS2D::Vector{0, 0}), font.size(""));
	EXPECT_EQ(0, font.width(""));
	EXPECT_EQ(0, font.height());
	EXPECT_EQ(0, font.ascent());
	EXPECT_EQ(0u, font.ptSize());

	const auto& metrics = font.metrics();
	EXPECT_EQ(0u, metrics.size());

	EXPECT_EQ(0u, font.textureId());
}

TEST(Font, nullWidthNonEmptyString) {
	const auto font = NAS2D::Font::null();
	EXPECT_EQ(0, font.width("ABCDEFGHIJKLMNOPQRSTUVWXYZ"));
}

TEST(Font, nullWidthChar) {
	const auto font = NAS2D::Font::null();
	EXPECT_EQ(0, font.width('A'));
}

TEST(Font, nullWidthBoundedSubstringLength) {
	const auto font = NAS2D::Font::null();
	EXPECT_EQ(0, font.widthBoundedSubstringLength("", 0));
	EXPECT_EQ(0, font.widthBoundedSubstringLength("A", 0));
}
