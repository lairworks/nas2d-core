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

#include "AnimationSet.h"
#include "../Timer.h"
#include "../Math/Angle.h"
#include "../Renderer/Color.h"
#include "../Signal/Signal.h"
#include "../Signal/Delegate.h"

#include <vector>
#include <string>


namespace NAS2D
{
	/**
	 * Sprite resource.
	 *
	 * The Sprite Class is a self-contained group of Image resources that displays
	 * Images at a specified screen coordinate in sequence to display an animation.
	 */
	class Sprite
	{
	public:
		using AnimationCompleteSignal = Signal<>;

		Sprite(const std::string& filePath, const std::string& initialAction);
		Sprite(const AnimationSet& animationSet, const std::string& initialAction);
		Sprite(const std::string& filePath, const std::string& initialAction, Delegate<void()> animationCompleteHandler);
		Sprite(const AnimationSet& animationSet, const std::string& initialAction, Delegate<void()> animationCompleteHandler);
		Sprite(const Sprite&) = default;
		Sprite(Sprite&&) = default;

		Sprite& operator=(const Sprite&) = delete;
		Sprite& operator=(Sprite&&) = delete;

		Vector<int> size() const;
		Point<int> origin(Point<int> point) const;

		std::vector<std::string> actions() const;

		void play(const std::string& action);
		void pause();
		void resume();

		void setFrame(std::size_t frameIndex);

		void update();
		void draw(Point<float> position) const;

		void rotation(Angle angle);
		Angle rotation() const;

		void alpha(uint8_t alpha);
		uint8_t alpha() const;
		void color(Color color);
		Color color() const;

		AnimationCompleteSignal::Source& animationCompleteSignalSource();

	protected:
		unsigned int advanceByTimeDelta(unsigned int timeDelta);

	private:
		const AnimationSet& mAnimationSet;
		const std::vector<AnimationSet::Frame>* mCurrentAction{nullptr};
		std::size_t mCurrentFrame{0};

		bool mPaused{false};
		Timer mTimer{};
		AnimationCompleteSignal mAnimationCompleteSignal{};

		Color mTintColor{Color::Normal};
		Angle mRotationAngle = Angle::degrees(0.0f);
	};
} // namespace
