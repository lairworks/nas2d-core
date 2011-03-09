// ==================================================================================
// = NAS2D
// = Copyright © 2008 New Age Software
// ==================================================================================
// = NAS2D is distributed under the terms of the zlib license. You are free to copy,
// = modify and distribute the software as long under the terms of the zlib license.
// = 
// = Acknowledgement of your use of NAS2D is appriciated but is not required.
// ==================================================================================

#include "NAS2D/Filesystem.h"

#include "NAS2D/Resources/Image.h"
#include "NAS2D/Renderer/Renderer.h"

#include <map>
#include <string>
#include <vector>

#ifndef _NAS_SPRITE_
#define _NAS_SPRITE_

#define SPRITE_VERSION "0.99"

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
	Sprite(const std::string& filePath);
	Sprite(const Sprite &sprite);
	~Sprite();

	Sprite& operator=(const Sprite &rhs);

	void play(const std::string& action);
	void pause(bool pause);

	void update(int x, int y);

	void setAlpha(int alpha) { mAlpha = clamp(alpha, 0, 255); }

	Image* getCurrentFrameImage();

	int getWidth();
	int getHeight();
	int getOriginX(int x);
	int getOriginY(int y);

	void debug();

private:
	/**
	 * \struct	spriteFrame
	 * \brief	Contains all information necessary for animation frames of a Sprite.
	 * \note	Opted to use a struct instead of a class as all members are publicly exposed.
	 */
	struct spriteFrame
	{
		spriteFrame(Image *src, int x, int y, int w, int h, int aX, int aY, int d);
		spriteFrame(const spriteFrame &spriteframe);

		spriteFrame& operator=(const spriteFrame &rhs);

		~spriteFrame();

		Image	*mFrameImage;

		int		mAnchorX, mAnchorY;
		unsigned int	mWidth, mHeight;
		unsigned int	mFrameDelay;
	};

	typedef std::vector<spriteFrame*>		FrameList;
	typedef std::map<string, FrameList>		ActionList;
	typedef std::map<string, Image*>		SheetContainer;

	bool parseXml(const std::string& filePath);
	bool parseImageSheets(TiXmlElement *root);
	bool parseActions(TiXmlElement *root);
	void parseFrames(TiXmlNode *node, const std::string& action);

	void addDefaultFrame(FrameList &frmList, unsigned int frmDelay = 25);
	void addDefaultAction();

	Timer				*mTimer;			/**< Internal time keeper. */

	TiXmlDocument		*mDocXml;			/**< XML Document Object. */

	SheetContainer		mSpriteSheets;		/**< Collection of sprite sheets. */
	ActionList			mActions;			/**< A list of Actions and their associated Frames. */

	std::string			mSpriteName;		/**< Name of this Sprite. */
	std::string			mErrorMessage;		/**< The last error that occured with this Sprite. */
	std::string			mCurrentAction;		/**< The current Action being performed. */

	unsigned int				mCurrentFrame;		/**< The current frame index in the current Action's frame list. */
	unsigned int				mLastFrameTick;		/**< The last tick in which the frame was updated. */

	int					mAlpha;				/**< Alpha value to draw the sprite. */

	bool				mPaused;			/**< Indicate whether or not the animation for this Sprite is paused. */
};

#endif