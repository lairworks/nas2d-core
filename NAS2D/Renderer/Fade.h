// ==================================================================================
// = NAS2D
// = Copyright © 2008 - 2020 New Age Software
// ==================================================================================
// = NAS2D is distributed under the terms of the zlib license. You are free to copy,
// = modify and distribute the software under the terms of the zlib license.
// =
// = Acknowledgment of your use of NAS2D is appreciated but is not required.
// ==================================================================================

#pragma once

#include "Color.h"
#include "../Duration.h"
#include "../Timer.h"
#include "../Signal/Delegate.h"


namespace NAS2D
{

	class Renderer;


	class Fade
	{
	public:
		using DelegateType = Delegate<void()>;


		explicit Fade(DelegateType onFadeComplete = {});
		explicit Fade(Color fadeColor, DelegateType onFadeComplete = {});

		void fadeIn(Duration fadeTime);
		void fadeOut(Duration fadeTime);

		bool isFading() const;
		bool isFaded() const;

		void update();
		void draw(Renderer& renderer) const;

	private:
		void setDuration(Duration newDuration);

		enum class FadeDirection
		{
			None,
			In,
			Out
		};

		Color mFadeColor;
		FadeDirection mDirection;
		Duration mDuration;
		Timer mFadeTimer;
		DelegateType mOnFadeComplete;
	};

}
