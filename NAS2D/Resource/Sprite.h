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
	struct AnimationFrame;
	class AnimationSet;
	class Angle;
	template <typename BaseType> struct Point;
	template <typename BaseType> struct Vector;


	/**
	 * Sprite resource.
	 *
	 * The Sprite Class is a self-contained group of Image resources that displays
	 * Images at a specified screen coordinate in sequence to display an animation.
	 */
	class Sprite
	{
	public:
		Sprite(const std::string& filePath, const std::string& initialAction);
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
		const std::vector<AnimationFrame>* mCurrentAction{nullptr};
		std::size_t mCurrentFrame{0};

		bool mPaused{false};
		Timer mTimer{};

		Color mTintColor{Color::Normal};
	};
} // namespace
