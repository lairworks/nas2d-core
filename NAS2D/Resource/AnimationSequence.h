#pragma once

#include <vector>


namespace NAS2D
{
	struct AnimationFrame;


	class AnimationSequence
	{
	public:
		AnimationSequence(std::vector<AnimationFrame> frames);

		bool empty() const;
		std::size_t frameCount() const;
		const AnimationFrame& frame(std::size_t index) const;

	private:
		std::vector<AnimationFrame> mFrames;
	};
}
