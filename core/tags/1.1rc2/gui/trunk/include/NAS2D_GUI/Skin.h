// ==================================================================================
// = The Legend of Mazzeroth
// = Copyright © 2008 - 2010, New Age Software
// ==================================================================================
// = This file is part of The Legend of Mazzeroth.
// = 
// = The Legend of Mazzeroth is free software; you can redistribute it and/or modify
// = it under the terms of the GNU General Public License version 3 as published by
// = the Free Software Foundation.
// = 
// = The Legend of Mazzeroth is distributed in the hope that it will be useful,
// = but WITHOUT ANY WARRANTY; without even the implied warranty of
// = MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
// = GNU General Public License for more details.
// = 
// = You should have received a copy of the GNU General Public License
// = along with The Legend of Mazzeroth; if not, write to the Free Software
// = Foundation, Inc., 59 Temple Place, Suite 330, Boston, MA  02111-1307  USA
// ==================================================================================

#ifndef _LOM_GUI_SKIN_
#define _LOM_GUI_SKIN_

#include "NAS2D/Resources/Resource.h"
#include "NAS2D/Resources/Image.h"
#include "NAS2D/Resources/Font.h"

#include "NAS2D/Common.h"
#include "NAS2D/Exception.h"
#include "NAS2D/Filesystem.h"
#include "NAS2D/tinyxml/tinyxml.h"

#include <map>
#include <vector>


/**
 * \enum	SkinPartType
 * \brief	Type of part.
 *
 * The type indicates the number of images a particular part contains.
 *
 * <b>PART_MONAD</b> - A single image.
 * <b>PART_TRIAD</b> - A set of three images.
 * <b>PART_ENNEAD</b> - A set of nine images.
 */
enum SkinPartType
{
	PART_COLORONLY	= 0,
	PART_MONAD		= 1,
	PART_TRIAD		= 3,
	PART_ENNEAD		= 9
};


/**
 * \enum	ControlState
 * \brief	State the Control is in.
 *
 * This is for various states that a control might be in and is used
 * to identify which graphics to retrieve.
 */
enum ControlState
{
	STATE_UNDEFINED,
	STATE_NORMAL,
	STATE_HOVER,
	STATE_PRESSED,
	STATE_SELECTED,
	STATE_DISABLED,
	STATE_NOFOCUS
};


/**
 * \enum	MousePointerStyle
 * \brief	Style states for the mouse pointer.
 */
enum MousePointerState
{
	MOUSE_DEFAULT,
	MOUSE_NORMAL = MOUSE_DEFAULT,
	MOUSE_THINKING,
	MOUSE_RESIZE_NE,
	MOUSE_RESIZE_NW,
	MOUSE_RESIZE_ALL,
	MOUSE_HAND
};


/**
 * \enum	ImageRegion
 * \brief	Comment Me
 */
enum ImageRegion
{
	REGION_TOP_LEFT_CORNER,
	REGION_TOP_EDGE,
	REGION_TOP_RIGHT_CORNER,
	REGION_LEFT_EDGE,
	REGION_CENTER,
	REGION_RIGHT_EDGE,
	REGION_BOTTOM_LEFT_CORNER,
	REGION_BOTTOM_EDGE,
	REGION_BOTTOM_RIGHT_CORNER,
	REGION_UNKNOWN
};


/**
 * \struct	TextOffsets
 * \brief	Contains offset values for text.
 */
struct TextOffsets
{
	enum TextAlign
	{
		TEXT_ALIGN_LEFT,
		TEXT_ALIGN_CENTER,
		TEXT_ALIGN_RIGHT
	};

	TextOffsets():	left(0), right(0), top(0), bottom(0), min_height(0), alignment(TEXT_ALIGN_LEFT), blank(true)
	{}

	TextOffsets(int left, int right, int top, int bottom, int min_height, TextAlign alignment):	left(left),
																								right(right),
																								top(top),
																								bottom(bottom),
																								min_height(min_height),
																								alignment(alignment),
																								blank(false)
	{}

	int left, right, top, bottom, min_height;
	TextAlign alignment;
	bool blank;
};

/**
 * \class	SkinPart
 * \brief	Contains images used for drawing skin parts.
 *
 * SkinPart contains all of the information and handling functions
 * to properly draw different skin parts.
 */
class SkinPart
{
public:
	SkinPart(SkinPartType type, ControlState state = STATE_NORMAL);
	~SkinPart();

	ImageList& getImageList();
	ControlState getState() const;
	SkinPartType getPartType() const;

	Color_4ub getForeground() const;
	Color_4ub getBackground() const;
	Color_4ub getBorder() const;

	const TextOffsets& getCaptionOffsets() const { return mCaptionOffsets; }

	int getMinimumWidth() const;
	int getMinimumHeight() const;

protected:
	friend class Skin;
	void addImage(ImageRegion region, Image &image);
	
	void setForeground(int r, int g, int b, int a = 255);
	void setBackground(int r, int g, int b, int a = 255);
	void setBorder(int r, int g, int b, int a = 255);

	void setCaptionOffsets(const TextOffsets& tO);

private:
	ImageList		mImageList;	/**< A list of Images. Used for every other part type. */
	SkinPartType	mPartType;	/**< The type of part this is. */

	ControlState	mState;		/**< The state of a Control that this part represents. */

	TextOffsets		mCaptionOffsets;

	Color_4ub		mBg;
	Color_4ub		mFg;
	Color_4ub		mBr;
};


/**
 * \class Skin
 * \brief Skin Resource
 *
 * Parses and interprets Skin XML Definitions and builds a set of SkinParts
 * that can then be used to draw GUI Elements.
 */
class Skin : public Resource
{
public:
	Skin(const std::string& filePath);
	~Skin();

	bool hasPart(const std::string& partName) const;

	SkinPart* getPart(const std::string& name, ControlState state = STATE_NORMAL);

	/**
	 * Gets the name of the Skin.
	 *
	 * \note	This is not the same as the Resource object's
	 *			\c getName() function.
	 */
	const string& getSkinName() const { return mSkinName; }
	
	Font* getSkinFont() { return mSkinFont; }

	Image& getMousePointer(MousePointerState state);

private:

	// Control and its associated Skin graphics.
	typedef std::multimap<string, SkinPart> PartList;
	typedef std::map<MousePointerState, Image> MousePointerList;

	void load();

	bool readSkin(const std::string& path);

	bool getSourceImage(TiXmlNode *node);
	bool getSkinName(TiXmlNode *node);
	bool getSkinFont(TiXmlNode *node);

	bool parseControlNode(TiXmlNode *node);
	SkinPart* parseStateNode(TiXmlNode *node);
	
	bool parseRegionNode(TiXmlNode *node, SkinPart *part);
	bool parseColordefNode(TiXmlNode *node, SkinPart *part);
	TextOffsets parseCaptionTextNode(TiXmlNode *node);

	void parseMousePointers(TiXmlNode *node);
	MousePointerState translateState(const std::string& state);

	ControlState enumerateState(const string& state);

	int countChildNodes(TiXmlNode *node, const std::string& filter = "");

	ImageRegion translateRegion(const std::string& region) const;
	const std::string translateState(ControlState state) const;

	TiXmlDocument		*mXmlDoc;		/**< XML Document. */

	Image				*mSource;		/**< Image containing all part graphics. */

	PartList			mPartList;		/**< List of Controls and their various associated parts. */

	MousePointerList	mPointerList;	/**< Set of mouse pointers. */

	std::string			mSkinName;		/**< Name of the Skin. */
	StringList			mMessageList;	/**< List of messages, error and otherwise. */
	
	Font				*mSkinFont;		/**< Font used for the skin. */
};

#endif