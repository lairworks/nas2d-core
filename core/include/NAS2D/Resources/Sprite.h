// ==================================================================================
// = NAS2D
// = Copyright © 2008 - 2012 New Age Software
// ==================================================================================
// = NAS2D is distributed under the terms of the zlib license. You are free to copy,
// = modify and distribute the software under the terms of the zlib license.
// = 
// = Acknowledgement of your use of NAS2D is appriciated but is not required.
// ==================================================================================

#include "NAS2D/Filesystem.h"
#include "NAS2D/sigslot.h"

#include "NAS2D/Resources/Image.h"
#include "NAS2D/Renderer/Renderer.h"

#include <map>
#include <string>
#include <vector>

#ifndef _NAS_SPRITE_
#define _NAS_SPRITE_

extern const std::string SPRITE_VERSION;

/**
 * \class Sprite
 * \brief Sprite Resource.
 *
 * The Sprite Class is a self-contained group of Image Resource's that displays
 * Image's at a specified screen coordinate in sequence to display an animation.
 */
class Sprite: public sigslot::has_slots<>
{
public:
	typedef sigslot::signal0<> Callback;

	Sprite();
	Sprite(const std::string& filePath);
	Sprite(const Sprite &sprite);
	
	/**
	 * D'tor
	 */
	~Sprite() {}

	Sprite& operator=(const Sprite &rhs);

	void play(const std::string& action);
	void pause();
	void resume();

	void update(int x, int y);

	void rotation(float angle);
	float rotation() const;

	void alpha(int alpha) { mAlpha = clamp(alpha, 0, 255); }

	int width();
	int height();
	int originX(int x);
	int originY(int y);

	void debug();

	/**
	 * Returns a reference to the frame listener signal slot.
	 */
	Callback& frameCallback() { return mFrameCallback; }

protected:
	const std::string& name() const { return mSpriteName; }

private:

	/**
	 * \class	spriteFrame
	 * \brief	Contains 
	 */
	struct SpriteFrame
	{
	public:
		SpriteFrame(const std::string& sheetId, int x, int y, int w, int h, int aX, int aY, int d);
		SpriteFrame(const SpriteFrame &spriteframe);

		SpriteFrame& operator=(const SpriteFrame &rhs);

		~SpriteFrame() {}

		const std::string& sheetId() const { return mSheetId; }
		
		int anchorX() const { return mAnchorX; }
		int anchorY() const { return mAnchorY; }
		
		int width() const { return mRect.w; }
		int height() const { return mRect.h; }
		int x() const { return mRect.x; }
		int y() const { return mRect.y; }

		int frameDelay() const { return mFrameDelay; }

	private:

		string			mSheetId;

		int				mFrameDelay;
		int				mAnchorX, mAnchorY;

		Rectangle_2d	mRect;
	};

	typedef std::vector<SpriteFrame>	FrameList;
	typedef std::map<string, FrameList>	ActionList;
	typedef std::map<string, Image>		SheetList;

	void parseXml(const std::string& filePath);
	void parseImageSheets(TiXmlElement *root);
	void addImageSheet(const std::string& id, const std::string& src, TiXmlNode* node);

	void parseActions(TiXmlElement *root);
	void parseFrames(const std::string& action, TiXmlNode *node);

	bool validateSheetId(const std::string& sheetId, int row);

	void addDefaultAction();

	Timer				mTimer;				/**< Internal time keeper. */

	SheetList			mImageSheets;		/**< Imagesheets */
	ActionList			mActions;			/**< A list of Actions and their associated Frames. */

	std::string			mSpriteName;		/**< Name of this Sprite. */
	std::string			mCurrentAction;		/**< The current Action being performed. */

	unsigned int		mCurrentFrame;		/**< The current frame index in the current Action's frame list. */
	unsigned int		mLastFrameTick;		/**< The last tick in which the frame was updated. */

	Callback			mFrameCallback;		/**< Callback to signal a listener whenever an animation sequence completes. */

	int					mAlpha;				/**< Alpha value to draw the sprite. */

	float				mRotationAngle;		/**< Angle of rotation in degrees. */

	bool				mPaused;			/**< Indicate whether or not the animation for this Sprite is paused. */
};

#endif