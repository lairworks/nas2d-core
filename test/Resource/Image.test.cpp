#include "NAS2D/Resource/Image.h"

#include <gtest/gtest.h>


TEST(Image, size) {
	{
		uint32_t buffer[1 * 1]{};
		const auto image = NAS2D::Image{&buffer, 4, {1, 1}};
		EXPECT_EQ((NAS2D::Vector{1, 1}), image.size());
	}
	{
		uint32_t buffer[2 * 1]{};
		const auto image = NAS2D::Image{&buffer, 4, {2, 1}};
		EXPECT_EQ((NAS2D::Vector{2, 1}), image.size());
	}
	{
		uint32_t buffer[1 * 2]{};
		const auto image = NAS2D::Image{&buffer, 4, {1, 2}};
		EXPECT_EQ((NAS2D::Vector{1, 2}), image.size());
	}
}
