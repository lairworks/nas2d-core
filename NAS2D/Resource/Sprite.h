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
#include "../Signal/Signal.h"
#include "../Timer.h"
#include "../Renderer/Color.h"

#include <vector>
#include <string>


namespace NAS2D {

class Sprite
{
public:
	using AnimationCompleteSignal = Signal<>;

	Sprite(const std::string& filePath, const std::string& initialAction);

	Vector<int> size() const;
	Point<int> origin(Point<int> point) const;

	std::vector<std::string> actions() const;

	void play(const std::string& action);
	void pause();
	void resume();

	void setFrame(std::size_t frameIndex);
	void incrementFrame();
	void decrementFrame();

	void update(Point<float> position);

	//TODO: Rename argument angleDegrees?
	//TODO: Rename function rotateDegrees?
	void rotation(float angle);
	float rotation() const;

	void alpha(uint8_t alpha);
	uint8_t alpha() const;
	void color(Color color);
	Color color() const;

	AnimationCompleteSignal::Source& animationCompleteSignalSource();

private:
	std::string mSpriteName;

	const AnimationSet* mAnimationSet;
	const std::vector<AnimationSet::Frame>* mCurrentAction{nullptr};
	std::size_t mCurrentFrame{0};

	bool mPaused{false};
	Timer mTimer;
	AnimationCompleteSignal mAnimationCompleteSignal;

	Color mColor{Color::Normal};
	//TODO: Rename orientationDegrees?
	float mRotationAngle{0.0f};
};

} // namespace
