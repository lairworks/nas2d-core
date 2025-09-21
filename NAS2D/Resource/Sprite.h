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

#include "../Timer.h"
#include "../Renderer/Color.h"

#include <vector>
#include <string>


namespace NAS2D
{
	struct Duration;
	class AnimationSequence;
	class AnimationSet;
	class Angle;
	template <typename BaseType> struct Point;
	template <typename BaseType> struct Vector;


	/**
	 * Sprite represents an object with a set of animation sequences that can be
	 * selected, animated, and displayed at a specified screen coordinate.
	 */
	class Sprite
	{
	public:
		Sprite(const AnimationSet& animationSet, const std::string& initialAction);
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
		bool isPaused() const;

		void setFrame(std::size_t frameIndex);

		void update();
		void draw(Point<float> position) const;
		void draw(Point<float> position, Angle rotation) const;

		void alpha(uint8_t alpha);
		uint8_t alpha() const;
		void color(Color color);
		Color color() const;

	protected:
		Duration advanceByTimeDelta(Duration timeDelta);

	private:
		const AnimationSet& mAnimationSet;
		const AnimationSequence* mCurrentAction;
		std::size_t mCurrentFrame;

		bool mPaused;
		Timer mTimer;

		Color mTintColor;
	};
} // namespace
