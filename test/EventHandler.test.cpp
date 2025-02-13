#include "NAS2D/EventHandler.h"
#include "NAS2D/EventHandlerKeyModifier.h"

#include <gtest/gtest.h>


TEST(EventHandler, shift) {
	EXPECT_TRUE(NAS2D::EventHandler::shift(NAS2D::EventHandler::KeyModifier::Shift));
	EXPECT_TRUE(NAS2D::EventHandler::shift(NAS2D::EventHandler::KeyModifier::ShiftLeft));
	EXPECT_TRUE(NAS2D::EventHandler::shift(NAS2D::EventHandler::KeyModifier::ShiftRight));
	EXPECT_TRUE(NAS2D::EventHandler::shift(NAS2D::EventHandler::KeyModifier::ShiftLeft | NAS2D::EventHandler::KeyModifier::CtrlLeft));
	EXPECT_FALSE(NAS2D::EventHandler::shift(NAS2D::EventHandler::KeyModifier::Caps));
	EXPECT_FALSE(NAS2D::EventHandler::shift(NAS2D::EventHandler::KeyModifier::Ctrl));
	EXPECT_FALSE(NAS2D::EventHandler::shift(NAS2D::EventHandler::KeyModifier::Alt));
}

TEST(EventHandler, numlock) {
	EXPECT_TRUE(NAS2D::EventHandler::numlock(NAS2D::EventHandler::KeyModifier::Num));
	EXPECT_TRUE(NAS2D::EventHandler::numlock(NAS2D::EventHandler::KeyModifier::Num | NAS2D::EventHandler::KeyModifier::CtrlLeft));
	EXPECT_FALSE(NAS2D::EventHandler::numlock(NAS2D::EventHandler::KeyModifier::Shift));
	EXPECT_FALSE(NAS2D::EventHandler::numlock(NAS2D::EventHandler::KeyModifier::Ctrl));
	EXPECT_FALSE(NAS2D::EventHandler::numlock(NAS2D::EventHandler::KeyModifier::Alt));
}

TEST(EventHandler, control) {
	EXPECT_TRUE(NAS2D::EventHandler::control(NAS2D::EventHandler::KeyModifier::Ctrl));
	EXPECT_TRUE(NAS2D::EventHandler::control(NAS2D::EventHandler::KeyModifier::Ctrl | NAS2D::EventHandler::KeyModifier::CtrlLeft));
	EXPECT_FALSE(NAS2D::EventHandler::control(NAS2D::EventHandler::KeyModifier::Shift));
	EXPECT_FALSE(NAS2D::EventHandler::control(NAS2D::EventHandler::KeyModifier::Alt));
}

TEST(EventHandler, alt) {
	EXPECT_TRUE(NAS2D::EventHandler::alt(NAS2D::EventHandler::KeyModifier::Alt));
	EXPECT_TRUE(NAS2D::EventHandler::alt(NAS2D::EventHandler::KeyModifier::Alt | NAS2D::EventHandler::KeyModifier::CtrlLeft));
	EXPECT_FALSE(NAS2D::EventHandler::alt(NAS2D::EventHandler::KeyModifier::Shift));
	EXPECT_FALSE(NAS2D::EventHandler::alt(NAS2D::EventHandler::KeyModifier::Ctrl));
}
