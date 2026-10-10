#include "NAS2D/Serialization/Serialize.h"

#include <gtest/gtest.h>


using namespace NAS2D;


namespace {
	struct TestObjectData {
	};
}


template<>
TestObjectData NAS2D::readType<TestObjectData>(NAS2D::ReaderObject&&) {
	return {};
}


TEST(Serialization, DocumentEmptyError) {
	EXPECT_THROW(NAS2D::readFileData<TestObjectData>("", ""), std::runtime_error);
}

TEST(Serialization, DocumentEmptyTagNameError) {
	EXPECT_THROW(NAS2D::readFileData<TestObjectData>("</>", ""), std::runtime_error);
}

TEST(Serialization, DocumentRootNoMatch) {
	EXPECT_THROW(NAS2D::readFileData<TestObjectData>("<unexpectedRoot/>", "root"), std::runtime_error);
}

TEST(Serialization, DocumentRootMatch) {
	EXPECT_NO_THROW(NAS2D::readFileData<TestObjectData>("<root/>", "root"));
}
