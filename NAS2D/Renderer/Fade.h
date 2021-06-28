// ==================================================================================
// = NAS2D
// = Copyright � 2008 - 2020 New Age Software
// ==================================================================================
// = NAS2D is distributed under the terms of the zlib license. You are free to copy,
// = modify and distribute the software under the terms of the zlib license.
// =
// = Acknowledgment of your use of NAS2D is appreciated but is not required.
// ==================================================================================
#pragma once

#include "Color.h"
#include "../Timer.h"
#include "../Signal/Signal.h"


namespace NAS2D {

class Renderer;


class Fade
{
public:
	Fade(Color fadeColor = Color::Black);

	SignalSource<>& fadeComplete();

	void fadeIn(unsigned int durationInMilliseconds);
	void fadeOut(unsigned int durationInMilliseconds);

	bool isFading() const;
	bool isFaded() const;

	void update();
	void draw(Renderer& renderer) const;

private:
	void setDuration(unsigned int durationInMilliseconds);

	enum class FadeDirection
	{
		None,
		In,
		Out
	};

	Color mFadeColor{Color::Black};
	FadeDirection mDirection{FadeDirection::None};
	unsigned int mDuration{0};
	Timer mFadeTimer;
	Signal<> mFadeComplete;
};

}
