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
	using Callback = NAS2D::Signals::Signal0<>;	/**< Signal used when action animations complete. */

	Sprite();
	explicit Sprite(const std::string& filePath);
	Sprite(const Sprite& sprite) = default;
	Sprite& operator=(const Sprite& rhs) = default;
	~Sprite() = default;

	void play(const std::string& action);
	void pause();
	void resume();

	void skip(int frames);

	void update(float x, float y);

	void rotation(float angle);
	float rotation() const;

	/**
	 * Sets the alpha value for the Sprite.
	 *
	 * \param	alpha	Alpha value to set between 0 - 255.
	 */
	void alpha(uint8_t alpha) { mColor.alpha(alpha); }

	/**
	 * Gets the alpha value for the Sprite.
	 */
	uint8_t alpha() const { return mColor.alpha(); }

	/**
	 * Sets the color of the Sprite.
	 *
	 * \param	color	Reference to a Color object.
	 */
	void color(const Color& color) { mColor = color; }

	/**
	 * Sets the color of the Sprite.
	 *
	 * \param	red		Red value to set between 0 - 255.
	 * \param	green	Green value to set between 0 - 255.
	 * \param	blue	Blue value to set between 0 - 255.
	 * \param	alpha	Alpha value to set between 0 - 255.
	 */
	void color(uint8_t red, uint8_t green, uint8_t blue, uint8_t alpha) { color(Color(red, green, blue, alpha)); }

	/**
	 * Gets the color of the Sprite.
	 */
	const Color& color() const { return mColor; }

	int width();
	int height();
	int originX(int x);
	int originY(int y);

	/**
	 * Returns a reference to the frame listener signal slot.
	 */
	Callback& frameCallback() { return mFrameCallback; }

	StringList actions();

	void incrementFrame();
	void decrementFrame();

protected:
	/// Gets the name of the Sprite. \note Internal use only.
	const std::string& name() const { return mSpriteName; }

private:
	/**
	 * \class	spriteFrame
	 * \brief	Contains
	 */
	class SpriteFrame
	{
	public:
		SpriteFrame(const std::string& sheetId, int x, int y, int w, int h, int aX, int aY, int d);
		SpriteFrame(const SpriteFrame &spriteframe) = default;
		SpriteFrame& operator=(const SpriteFrame& rhs) = default;
		~SpriteFrame() = default;

		const std::string& sheetId() const { return mSheetId; }

		int anchorX() const { return mAnchorX; }
		int anchorY() const { return mAnchorY; }

		int width() const { return mRect.width(); }
		int height() const { return mRect.height(); }
		int x() const { return mRect.x(); }
		int y() const { return mRect.y(); }

		int frameDelay() const { return mFrameDelay; }

	private:
		std::string mSheetId;
		int mFrameDelay;
		int mAnchorX;
		int mAnchorY;
		Rectangle_2d mRect;
	};

private:
	using FrameList = std::vector<SpriteFrame>;
	using ActionList = std::map<std::string, FrameList>;
	using SheetList = std::map<std::string, Image>;

	void processXml(const std::string& filePath);
	void processImageSheets(void* root);
	void addImageSheet(const std::string& id, const std::string& src, void* node);

	void processActions(void* root);
	void processFrames(const std::string& action, void* node);

	bool validateSheetId(const std::string& sheetId, int row);

	void addDefaultAction();

private:
	static inline const std::string DEFAULT_ACTION{"default"};

	Timer mTimer; /**< Internal time keeper. */

	SheetList mImageSheets; /**< Imagesheets */
	ActionList mActions; /**< A list of Actions and their associated Frames. */

	std::string mSpriteName{"Default Constructed"}; /**< Name of this Sprite. */
	std::string mCurrentAction{DEFAULT_ACTION}; /**< The current Action being performed. */

	size_t mCurrentFrame{0}; /**< The current frame index in the current Action's frame list. */

	Callback mFrameCallback; /**< Callback to signal a listener whenever an animation sequence completes. */

	Color mColor{Color::Normal}; /**< Color value to use for drawing the sprite. */

	float mRotationAngle{0.0f}; /**< Angle of rotation in degrees. */

	bool mPaused{false}; /**< Indicate whether or not the animation for this Sprite is paused. */
};

} // namespace
