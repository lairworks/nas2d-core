#pragma once

#include "../Duration.h"
#include "../Math/Vector.h"
#include "../Math/Rectangle.h"


namespace NAS2D
{
	class Image;


	struct AnimationFrame
	{
		const Image& image;
		Rectangle<int> bounds;
		Vector<int> anchorOffset;
		Duration frameDelay;


		bool isStopFrame() const;
	};
}
