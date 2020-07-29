// ==================================================================================
// = NAS2D
// = Copyright © 2008 - 2019 New Age Software
// ==================================================================================
// = NAS2D is distributed under the terms of the zlib license. You are free to copy,
// = modify and distribute the software under the terms of the zlib license.
// =
// = Acknowledgement of your use of NAS2D is appriciated but is not required.
// ==================================================================================

#pragma once

#include "Image.h"
#include "../Signal.h"
#include "../Timer.h"
#include "../Utility.h"
#include "../Filesystem.h"
#include "../Renderer/Color.h"
#include "../Renderer/Rectangle.h"
#include "../Renderer/Renderer.h"

#include <map>
#include <string_view>
#include <string>
#include <vector>


namespace NAS2D {

constexpr std::string_view SPRITE_VERSION{"0.99"};


/**
 * \class Sprite
 * \brief Sprite Resource.
 *
 * The Sprite Class is a self-contained group of Image Resource's that displays
 * Image's at a specified screen coordinate in sequence to display an animation.
 */
class Sprite
{
public:
	using Callback = Signals::Signal<>; /**< Signal used when action animations complete. */

	struct SpriteFrame
	{
		const Image& image;
		Rectangle<int> bounds;
		Vector<int> anchorOffset;
		unsigned int frameDelay;
	};

	struct SpriteAnimations
	{
		std::map<std::string, Image> imageSheets;
		std::map<std::string, std::vector<SpriteFrame>> actions;
	};


	Sprite(const std::string& filePath, const std::string& initialAction);
	Sprite(const Sprite& sprite) = delete;
	Sprite(Sprite&& sprite) = default;
	Sprite& operator=(const Sprite& rhs) = delete;
	Sprite& operator=(Sprite&& rhs) = default;
	~Sprite() = default;

	Vector<int> size() const;
	Point<int> origin(Point<int> point) const;

	StringList actions() const;

	void play(const std::string& action);
	void pause();
	void resume();

	void setFrame(std::size_t frameIndex);
	void incrementFrame();
	void decrementFrame();

	void update(Point<float> position);

	void rotation(float angle);
	float rotation() const;

	void alpha(uint8_t alpha);
	uint8_t alpha() const;
	void color(const Color& color);
	const Color& color() const;

	Callback& frameCallback();

private:
	std::string mSpriteName;

	SpriteAnimations mSpriteAnimations;

	std::vector<SpriteFrame>* mCurrentAction{nullptr};
	std::size_t mCurrentFrame{0};

	bool mPaused{false};
	Timer mTimer;
	Callback mAnimationCompleteCallback;

	Color mColor{Color::Normal}; /**< Color tint to use for drawing the sprite. */
	float mRotationAngle{0.0f}; /**< Angle of rotation in degrees. */
};

} // namespace
