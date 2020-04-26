#include "NAS2D/ContainerUtils.h"

#include <gmock/gmock.h>
#include <gtest/gtest.h>

#include <vector>
#include <list>


TEST(Container, VectorSelfAdd) {
	const std::vector b{3, 4};

	{
		std::vector a{1, 2};
		EXPECT_EQ((std::vector{1, 2, 3, 4}), NAS2D::ContainerOperators::operator+=(a, b));
		EXPECT_EQ((std::vector{1, 2, 3, 4}), a);
	}

	{
		std::vector a{1, 2};
		using namespace NAS2D::ContainerOperators;
		EXPECT_EQ((std::vector{1, 2, 3, 4}), a += b);
		EXPECT_EQ((std::vector{1, 2, 3, 4}), a);
	}
}

TEST(Container, VectorAdd) {
	const std::vector a{1, 2};
	const std::vector b{3, 4};

	EXPECT_EQ((std::vector{1, 2, 3, 4}), NAS2D::ContainerOperators::operator+(a, b));

	{
		using namespace NAS2D::ContainerOperators;
		EXPECT_EQ((std::vector{1, 2, 3, 4}), a + b);
	}
}
