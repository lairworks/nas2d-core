#include "Test4State.h"

#include <math.h>
#include <sstream>


using namespace std;
using namespace NAS2D;


double			VU_LEFT_VALUE = 0.0f;
double			VU_RIGHT_VALUE = 0.0f;

const double	MAX_REFERENCE = 0.707 * 32767;
double			m_0dbRef = MAX_REFERENCE;


/**
 * VERY crude VU meter. Would like to get a proper spectrum analyzer going here.
 */
void compute_dB(int chan, void *stream, int len, void *udata)
{
	
	// The meter's range in decibels
	const static double MAX_DB = 0.0f;
	const static double MIN_DB = -45.0f;

	// Base 0dB reference on CURRENT VOLUME LEVEL
	m_0dbRef = MAX_REFERENCE * Utility<Configuration>::get().audioMusicVolume() / 100; 


	long long int l_avg = 0;
	long long int r_avg = 0;

	for(int i = 0; i < len / 2; i += 2)
	{
		l_avg += abs(*((int*)stream + i));
		r_avg += abs(*((int*)stream + i + 1));
	}

	l_avg /= len / 2;
	l_avg /= Utility<Configuration>::get().audioMixRate();

	r_avg /= len / 2;
	r_avg /= Utility<Configuration>::get().audioMixRate();


	// Convert to DECIBELS based on the CURRENT 0dB REFERENCE
	double l_dB = 0.0f;
	double r_dB = 0.0f;

	if(m_0dbRef > 0)
	{
		l_dB = 20 * log10(l_avg / m_0dbRef);
		r_dB = 20 * log10(r_avg / m_0dbRef);
	}
	else
	{
		l_dB = -1000;
		r_dB = -1000;
	}


	// Convert DECIBELS to SLIDER POSITION
	VU_LEFT_VALUE = 100.0f * (l_dB - MIN_DB) / (MAX_DB - MIN_DB);
	VU_RIGHT_VALUE = 100.0f * (r_dB - MIN_DB) / (MAX_DB - MIN_DB);
}


Test4State::Test4State():	mFont("fonts/tork.ttf", 25),
							mSmallFont("fonts/tork.ttf", 12),
							mBackgroundImg("gypsy_camp_0.png"),
							mModule1("music/nerve_intro.xm"),
							mModule2("music/nerve_intro2.xm"),
							mCurrentSelection(1)
{}


void Test4State::initialize()
{
	Mix_RegisterEffect(MIX_CHANNEL_POST, compute_dB, NULL, NULL);

	Utility<EventHandler>::get().keyDown().Connect(this, &Test4State::onKeyDown);
	Utility<Mixer>::get().playMusic(mModule1);

	mLeftVuRect.x(Utility<Renderer>::get().screenCenterX() - 25);
	mLeftVuRect.y(25);
	mLeftVuRect.w(40);
	mLeftVuRect.h(Utility<Renderer>::get().height() - 50);

	mRightVuRect.x(Utility<Renderer>::get().screenCenterX() + 25);
	mRightVuRect.y(25);
	mRightVuRect.w(40);
	mRightVuRect.h(Utility<Renderer>::get().height() - 50);

	mTimer.reset();
}


State* Test4State::update()
{
	static stringstream str;
	str.str("");


	Renderer& r = Utility<Renderer>::get();

	r.drawImageStretched(mBackgroundImg, 0, 0, r.width(), r.height());

	mLeftVuRect.h((VU_LEFT_VALUE * (Utility<Renderer>::get().height() - 50)) / 100);
	mLeftVuRect.y((Utility<Renderer>::get().height() - 25) - mLeftVuRect.h());

	mRightVuRect.h((VU_RIGHT_VALUE * (Utility<Renderer>::get().height() - 50)) / 100);
	mRightVuRect.y((Utility<Renderer>::get().height() - 25) - mRightVuRect.h());

	r.drawBoxFilled(mLeftVuRect, 200, 200, 0, 180);
	r.drawBoxFilled(mRightVuRect, 200, 200, 0, 180);

	r.drawText(mFont, "Mixer Test", 10, 10, 255, 255, 255);

	return this;
}


void Test4State::onKeyDown(KeyCode key, KeyModifier mod, bool repeat)
{
	if(repeat)
		return;

	switch(key)
	{
		case KEY_ESCAPE:
			postQuitEvent();
			break;

		case KEY_1:
			if(mCurrentSelection != 1)
			{
				mCurrentSelection = 1;
				Utility<Mixer>::get().playMusic(mModule1);
			}
			break;

		case KEY_2:
			if(mCurrentSelection != 2)
			{
				mCurrentSelection = 2;
				Utility<Mixer>::get().playMusic(mModule2);
			}
			break;

		default:
			break;
	}

	if(key == KEY_ESCAPE)
		postQuitEvent();
}