#include "NAS2D/Serialization/ReaderScalar.h"

#include "NAS2D/Serialization/Document.h"
#include "NAS2D/Serialization/ReaderObject.h"

#include <gtest/gtest.h>

#include <string_view>


using namespace NAS2D;


TEST(Serialization, ScalarReadString) {
	EXPECT_EQ("value", NAS2D::Document("<root field=\"value\" />").getRootObject("root").scalar("field").read<std::string_view>());
}
