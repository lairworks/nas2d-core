#include "AnimationFrame.h"

using namespace NAS2D;


bool AnimationFrame::isStopFrame() const
{
	// We want to redefine the sentinel value for stop frames from -1 to 0
	// Neither value makes sense as a delay, though the field is unsigned
	// Using 0 would also help simplify some code, so makes the most sense
	// Temporarily handle -1 for backwards compatibility during transition
	return (frameDelay.milliseconds == 0) || (frameDelay.milliseconds == unsigned(-1));
}
