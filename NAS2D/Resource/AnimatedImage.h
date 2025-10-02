#pragma once

#include <cstddef>


namespace NAS2D
{
	struct Color;
	struct AnimationFrame;
	class AnimationSequence;
	class Renderer;
	class Angle;
	template <typename BaseType> struct Point;


	class AnimatedImage
	{
	public:
		explicit AnimatedImage(const AnimationSequence& animationSequence);

		const AnimationSequence& sequence() const;
		const AnimationFrame& frame() const;

		std::size_t frameCount() const;
		std::size_t frameIndex() const;

		void setFrame(std::size_t frameIndex);
		void advanceFrame();

		void draw(Renderer& renderer, Point<int> position) const;
		void draw(Renderer& renderer, Point<int> position, Color tintColor) const;
		void draw(Renderer& renderer, Point<int> position, Color tintColor, Angle rotation) const;

	private:
		const AnimationSequence* mAnimationSequence;
		std::size_t mFrameIndex;
	};
}
