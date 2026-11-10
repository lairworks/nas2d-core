#include "NAS2D/Serialization/Document.h"

#include "NAS2D/Serialization/ReaderObject.h"

#include <gtest/gtest.h>


using namespace NAS2D;


TEST(Serialization, DocumentEmptyError) {
	EXPECT_NO_THROW(NAS2D::Document(""));
}

TEST(Serialization, DocumentEmptyTagNameError) {
	EXPECT_NO_THROW(NAS2D::Document("</>"));
}

TEST(Serialization, DocumentRootNoMatch) {
	EXPECT_THROW(NAS2D::Document("<unexpectedRoot/>").getRootObject("root"), std::runtime_error);
}

TEST(Serialization, DocumentRootMatch) {
	EXPECT_NO_THROW(NAS2D::Document("<root/>").getRootObject("root"));
}
