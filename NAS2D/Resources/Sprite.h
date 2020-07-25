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
#include <string>
#include <vector>

namespace NAS2D {

extern const std::string SPRITE_VERSION;

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

	Sprite();
	explicit Sprite(const std::string& filePath);
	Sprite(const Sprite& sprite) = default;
	Sprite& operator=(const Sprite& rhs) = default;
	~Sprite() = default;

	void play(const std::string& action);
	void pause();
	void resume();

	void setFrame(std::size_t frameIndex);

	void update(Point<float> position);
	void update(float x, float y);

	void rotation(float angle);
	float rotation() const;

	/**
	 * Sets the alpha value for the Sprite.
	 *
	 * \param	alpha	Alpha value to set between 0 - 255.
	 */
	void alpha(uint8_t alpha) { mColor.alpha = alpha; }

	/**
	 * Gets the alpha value for the Sprite.
	 */
	uint8_t alpha() const { return mColor.alpha; }

	/**
	 * Sets the color tint of the Sprite.
	 */
	void color(const Color& color) { mColor = color; }

	/**
	 * Gets the color tint of the Sprite.
	 */
	const Color& color() const { return mColor; }

	Vector<int> size() const;
	Point<int> origin(Point<int> point) const;

	Callback& frameCallback() { return mAnimationCompleteCallback; }

	StringList actions() const;

	void incrementFrame();
	void decrementFrame();

protected:
	const std::string& name() const { return mSpriteName; }

private:
	struct SpriteFrame
	{
		std::string sheetId;
		Rectangle<int> bounds;
		Vector<int> anchorOffset;
		unsigned int frameDelay;
	};


	using FrameList = std::vector<SpriteFrame>;

	void processXml(const std::string& filePath);
	void processImageSheets(const void* root);
	void addImageSheet(const std::string& id, const std::string& src, const void* node);

	void processActions(const void* root);
	void processFrames(const std::string& action, const void* node);

	void addDefaultAction();


	static inline const std::string DEFAULT_ACTION{"default"};

	std::map<std::string, Image> mImageSheets;
	std::map<std::string, FrameList> mActions;

	std::string mSpriteName{"Default Constructed"};
	std::string mCurrentAction{DEFAULT_ACTION};

	bool mPaused{false};
	Timer mTimer;
	std::size_t mCurrentFrame{0};
	Callback mAnimationCompleteCallback;

	Color mColor{Color::Normal}; /**< Color tint to use for drawing the sprite. */
	float mRotationAngle{0.0f}; /**< Angle of rotation in degrees. */
};

} // namespace
