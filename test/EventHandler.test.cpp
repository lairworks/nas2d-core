#include "NAS2D/EventHandler.h"
#include "NAS2D/EnumKeyModifier.h"

#include <gtest/gtest.h>


TEST(EventHandler, shift) {
	EXPECT_TRUE(NAS2D::EventHandler::shift(NAS2D::KeyModifier::Shift));
	EXPECT_TRUE(NAS2D::EventHandler::shift(NAS2D::KeyModifier::ShiftLeft));
	EXPECT_TRUE(NAS2D::EventHandler::shift(NAS2D::KeyModifier::ShiftRight));
	EXPECT_TRUE(NAS2D::EventHandler::shift(NAS2D::KeyModifier::ShiftLeft | NAS2D::KeyModifier::CtrlLeft));
	EXPECT_FALSE(NAS2D::EventHandler::shift(NAS2D::KeyModifier::Caps));
	EXPECT_FALSE(NAS2D::EventHandler::shift(NAS2D::KeyModifier::Ctrl));
	EXPECT_FALSE(NAS2D::EventHandler::shift(NAS2D::KeyModifier::Alt));
}

TEST(EventHandler, numlock) {
	EXPECT_TRUE(NAS2D::EventHandler::numlock(NAS2D::KeyModifier::Num));
	EXPECT_TRUE(NAS2D::EventHandler::numlock(NAS2D::KeyModifier::Num | NAS2D::KeyModifier::CtrlLeft));
	EXPECT_FALSE(NAS2D::EventHandler::numlock(NAS2D::KeyModifier::Shift));
	EXPECT_FALSE(NAS2D::EventHandler::numlock(NAS2D::KeyModifier::Ctrl));
	EXPECT_FALSE(NAS2D::EventHandler::numlock(NAS2D::KeyModifier::Alt));
}

TEST(EventHandler, control) {
	EXPECT_TRUE(NAS2D::EventHandler::control(NAS2D::KeyModifier::Ctrl));
	EXPECT_TRUE(NAS2D::EventHandler::control(NAS2D::KeyModifier::Ctrl | NAS2D::KeyModifier::CtrlLeft));
	EXPECT_FALSE(NAS2D::EventHandler::control(NAS2D::KeyModifier::Shift));
	EXPECT_FALSE(NAS2D::EventHandler::control(NAS2D::KeyModifier::Alt));
}

TEST(EventHandler, alt) {
	EXPECT_TRUE(NAS2D::EventHandler::alt(NAS2D::KeyModifier::Alt));
	EXPECT_TRUE(NAS2D::EventHandler::alt(NAS2D::KeyModifier::Alt | NAS2D::KeyModifier::CtrlLeft));
	EXPECT_FALSE(NAS2D::EventHandler::alt(NAS2D::KeyModifier::Shift));
	EXPECT_FALSE(NAS2D::EventHandler::alt(NAS2D::KeyModifier::Ctrl));
}
