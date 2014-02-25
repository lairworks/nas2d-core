#include "Test2State.h"


Test2State::Test2State():	mFont("fonts/Fresca-Regular.ttf"),
							mBackgroundImage(),
							mPointer("graphics/pointer.png")
{}


Test2State::~Test2State()
{}

void Test2State::initialize()
{
	Utility<EventHandler>::get().mouseMotion().Connect(this, &Test2State::onMouseMove);
	Utility<EventHandler>::get().mouseButtonDown().Connect(this, &Test2State::onMouseDown);
	Utility<EventHandler>::get().keyDown().Connect(this, &Test2State::onKeyDown);
}


State* Test2State::update()
{
	return this;
}


void Test2State::onKeyDown(KeyCode key, KeyModifier mod, bool repeat)
{
}


void Test2State::onMouseMove(int x, int y, int relX, int relY)
{
}


void Test2State::onMouseDown(MouseButton button, int x, int y)
{
}
