#include "Test3State.h"


const int	CAUSTICS_SPEED		= 55;
const int	TILE_SIZE			= 256;


#include <sstream>

using namespace std;

Test3State::Test3State():	mFont("fonts/tork.ttf", 25),
							mSmallFont("fonts/tork.ttf", 12),
							mMud("mud.png"),
							mCaustics("caustics_atlas.png"),
							mCaustics2("caustics_atlas_noalpha.png"),
							mCaustics3("caustics_atlas_2.png"),
							mCaustics4("caustics_atlas_2_noalpha.png"),
							mAlpha(true),
							mMultiply(false),
							mCausticsOnly(false),
							mAlpha2(false),
							mMultiply2(false),
							mCausticsOnly2(false)
{
}


void Test3State::initialize()
{
	Renderer& r = Utility<Renderer>::get();
	
	Utility<EventHandler>::get().keyDown().Connect(this, &Test3State::onKeyDown);
}


State* Test3State::update()
{
	Renderer& r = Utility<Renderer>::get();

	static int counter = 0;

	if(mCausticsTimer.accumulator() > CAUSTICS_SPEED)
	{
		mCausticsTimer.reset();
		counter++;
		if(counter >= 16)
			counter = 0;
	}

	for(size_t col = 0; col < divideUp(r.height(), TILE_SIZE); col++)
	{
		for(size_t row = 0; row < divideUp(r.width(), TILE_SIZE); row++)
		{
			r.drawImage(mMud, row * 256, col * 256);
		}
	}

	if(mMultiply || mMultiply2)
		glBlendFunc(GL_SRC_COLOR, GL_DST_COLOR);
	for(size_t col = 0; col < divideUp(r.height(), TILE_SIZE); col++)
	{
		for(size_t row = 0; row < divideUp(r.width(), TILE_SIZE); row++)
		{
			if(mAlpha)
				r.drawSubImage(mCaustics, row * TILE_SIZE, col * TILE_SIZE, (counter % 4) * TILE_SIZE, ((counter % 16) / 4) * TILE_SIZE, TILE_SIZE, TILE_SIZE);
			else if(mMultiply || mCausticsOnly)
				r.drawSubImage(mCaustics2, row * TILE_SIZE, col * TILE_SIZE, (counter % 4) * TILE_SIZE, ((counter % 16) / 4) * TILE_SIZE, TILE_SIZE, TILE_SIZE);
			else if(mAlpha2)
				r.drawSubImage(mCaustics3, row * TILE_SIZE, col * TILE_SIZE, (counter % 4) * TILE_SIZE, ((counter % 16) / 4) * TILE_SIZE, TILE_SIZE, TILE_SIZE);
			else if(mMultiply2 || mCausticsOnly2)
				r.drawSubImage(mCaustics4, row * TILE_SIZE, col * TILE_SIZE, (counter % 4) * TILE_SIZE, ((counter % 16) / 4) * TILE_SIZE, TILE_SIZE, TILE_SIZE);
		}
	}
	if(mMultiply || mMultiply2)
		glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);


	r.drawText(mFont, "Blending Demo", 10, 10, 255, 255, 255);

	if(mMultiply || mMultiply2)
		r.drawText(mSmallFont, "Multiply", r.width() - mSmallFont.width("Multiply") - 10, 10, 255, 255, 255);
	else if(mCausticsOnly || mCausticsOnly2)
		r.drawText(mSmallFont, "Caustics Only", r.width() - mSmallFont.width("Caustics Only") - 10, 10, 255, 255, 255);
	else
		r.drawText(mSmallFont, "Alpha", r.width() - mSmallFont.width("Alpha") - 10, 10, 255, 255, 255);

	std::stringstream str;
	str << "FPS: " << mFps.fps();
	r.drawText(mSmallFont, str.str(), r.width() - 65, r.height() - 15, 255, 255, 255);

	return this;
}



void Test3State::onKeyDown(KeyCode key, KeyModifier mod, bool repeat)
{
	if(repeat)
		return;

	if(key == KEY_ESCAPE)
		postQuitEvent();

	if(key == KEY_1)
	{
		mAlpha = true;
		mMultiply = false;
		mCausticsOnly = false;
		mAlpha2 = false;
		mMultiply2 = false;
		mCausticsOnly2 = false;
	}
	if(key == KEY_2)
	{
		mAlpha = false;
		mMultiply = true;
		mCausticsOnly = false;
		mAlpha2 = false;
		mMultiply2 = false;
		mCausticsOnly2 = false;

	}
	if(key == KEY_3)
	{
		mAlpha = false;
		mMultiply = false;
		mCausticsOnly = true;
		mAlpha2 = false;
		mMultiply2 = false;
		mCausticsOnly2 = false;

	}
	if(key == KEY_4)
	{
		mAlpha = false;
		mMultiply = false;
		mCausticsOnly = false;
		mAlpha2 = true;
		mMultiply2 = false;
		mCausticsOnly2 = false;
	}
	if(key == KEY_5)
	{
		mAlpha = false;
		mMultiply = false;
		mCausticsOnly = false;
		mAlpha2 = false;
		mMultiply2 = true;
		mCausticsOnly2 = false;
	}
	if(key == KEY_6)
	{
		mAlpha = false;
		mMultiply = false;
		mCausticsOnly = false;
		mAlpha2 = false;
		mMultiply2 = false;
		mCausticsOnly2 = true;
	}
}
