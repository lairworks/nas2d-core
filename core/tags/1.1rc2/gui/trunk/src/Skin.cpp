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

#include "Skin.h"

#include "NAS2D/Singleton.h"
#include "NAS2D/Filesystem.h"

#include <string>
#include <sstream>


/**
 * Constructor.
 * 
 * Begins parsing for valid skin directories.
 */
Skin::Skin(const string& filePath):	Resource(filePath),
									mXmlDoc(new TiXmlDocument()),
									mSource(NULL),
									mSkinFont(NULL)
{
	load();
}


Skin::~Skin()
{
	// Delete Image resource if allocated.
	if(mSource)
		delete mSource;

	if(mXmlDoc)
		delete mXmlDoc;

	if(!mPartList.empty())
	{
		mPartList.clear();
	}
}


void Skin::load()
{
	if(!Singleton<Filesystem>::get().exists(mResourceName))
	{
		stringstream tmpStr;
		tmpStr << "Specified File '" << mResourceName << "' doesn't exist.";
		mErrorDescription = tmpStr.str();
	}
	else if(!readSkin(mResourceName))
	{
		stringstream tmpStr;
		tmpStr << "Error parsing '" << mResourceName << "': " << mErrorDescription << ".";
		mErrorDescription = tmpStr.str();
	}
	else
	{
		mErrorDescription = "";
		mIsLoaded = true;
	}
}


/**
 * Parses the skin.xml file within a directory.
 *
 * \todo	Pull this function apart so that related parsing logic
 *			is contained within separate functions.
 */
bool Skin::readSkin(const string& path)
{
	TiXmlElement  *rootNode;

	File xmlFile = Singleton<Filesystem>::get().getFile(path);

	// Load the XML document and handle any errors if occuring
	mXmlDoc->Parse(xmlFile.raw_bytes());
	if(mXmlDoc->Error())
	{
		stringstream tmpStr;
		tmpStr << "Skin file malformed. Error on Row " << mXmlDoc->ErrorRow() << ", Column " << mXmlDoc->ErrorCol() << ": " << mXmlDoc->ErrorDesc();
		mMessageList.push_back(tmpStr.str());
		return false;
	}
	else
	{
		// Ensure that we're working with an XML Document.
		if(mXmlDoc->ToDocument() == NULL)
		{
			stringstream tmpStr;
			tmpStr << "Unable to continue parsing Skin XML Definition. Unknown error: '" << mXmlDoc->ErrorDesc() << "'.";
			mMessageList.push_back(tmpStr.str());
			return false;
		} 
		else 
		{
			// Find root node.
			rootNode = mXmlDoc->FirstChildElement("skinset");
			if(rootNode == NULL)
			{
				mMessageList.push_back("Specified file doesn't contain '<skinset>' tag.");
				return false;
			}

			// Get Skin Name
			if(!getSkinName(rootNode))
				return false;

			// Find and load Source Image
			if(!getSourceImage(rootNode))
				return false;
			
			// Find and load the Skin Font
			if(!getSkinFont(rootNode))
			{
				// Do nothing
				//std::cout << "Unable to load skin defined font." << endl;
				//return false;
			}
		}

		// Add an initial default image.
		mPointerList[MOUSE_DEFAULT] = Image();

		// Parse through each 'control' tag as it appears.
		TiXmlNode *controlNode = 0;
		while(controlNode == rootNode->IterateChildren(controlNode))
		{
			if(controlNode->ValueStr() == "control")
				parseControlNode(controlNode);
			else if(controlNode->ValueStr() == "pointers")
				parseMousePointers(controlNode);
			else
				std::cout << "WARN: " << "Unexpected node found in '" << mResourceName << "' on row " << controlNode->Row() << endl;
		}

		return true;
	}
}


bool Skin::parseControlNode(TiXmlNode *node)
{
	// Grab the Control Type.
	const char *controlType = node->ToElement()->Attribute("type");
	if(controlType == NULL)
	{
		stringstream tmpStr;
		tmpStr << "Attribute 'type' is missing in tag '<control>' on row " << node->Row() << ".";
		mMessageList.push_back(tmpStr.str());
		return false;
	}

	std::vector<SkinPart*> partList;
	TextOffsets captionTextOffsets;		// This gets applied to all states and allows the 'captiontext'
										// tag to be defined anywhere whithin a parent 'control' tag.
	
	// Parse State tags
	TiXmlNode *childNode = 0;
	while(childNode == node->IterateChildren(childNode))
	{
		if(childNode->ValueStr() == "state")
		{
			SkinPart *tmpPart = parseStateNode(childNode);

			if(tmpPart)
				partList.push_back(tmpPart);
		}
		else if(childNode->ValueStr() == "captiontext")
			captionTextOffsets = parseCaptionTextNode(childNode);
		else
			std::cout << "WARN: " << "Unexpected node found in '" << mResourceName << "' on row " << node->Row() << endl;
	}

	// If any states were found for the Control, add them to the set.
	if(partList.empty())
	{
		std::cout << "ERR: " << "Control '" << controlType << "' doesn't define any states." << endl;
	}
	else
	{
		for(size_t i = 0; i < partList.size(); i++)
		{
			partList[i]->setCaptionOffsets(captionTextOffsets);
			mPartList.insert(PartList::value_type(toLowercase(controlType), *partList[i]));
		}
	}

	return true;
}


/**
 * Parses a 'state' node.
 */
SkinPart* Skin::parseStateNode(TiXmlNode *node)
{
	// Grab the Control Type.
	const char *stateName = node->ToElement()->Attribute("type");
	if(stateName == NULL)
	{
		stringstream tmpStr;
		tmpStr << "ERR: " << "Attribute 'type' is missing in tag '<state>' on row " << node->Row() << ".";
		mErrorDescription = tmpStr.str();
		return NULL;
	}

	ControlState controlState = enumerateState(stateName);

	// Get the number of regions defined in this state.
	int regionCount = countChildNodes(node, "region");

	// Check our count and set values accordingly or error out if there was a problem.
	// TODO: Can we stick this in a separate function to clean things up?
	SkinPartType partType = PART_MONAD;	// The kind of part this State is.
	if(regionCount == 1)
		partType = PART_MONAD;
	else if(regionCount == 3)
		partType = PART_TRIAD;
	else if(regionCount == 9)
		partType = PART_ENNEAD;
	else if(regionCount == 0)
	{
		if(countChildNodes(node, "colordef") > 0)
			partType = PART_COLORONLY;
	}
	else
	{
		stringstream tmpStr;
		tmpStr << "ERR: " << "Invalid number of regions in skin file for state " << stateName << endl;
		mErrorDescription = tmpStr.str();
		return NULL;
	}

	// Temporary SkinPart that we'll return if processing went well.
	SkinPart *newPart = new SkinPart(partType, controlState);

	// Start parsing the State's region nodes.
	TiXmlNode *stateChild = 0;
	while(stateChild == node->IterateChildren(stateChild))
	{
		if(stateChild->ValueStr() == "region")
		{
			parseRegionNode(stateChild, newPart);
		}
		else if(stateChild->ValueStr() == "colordef")
		{
			parseColordefNode(stateChild, newPart);
		}
		else
		{
			stringstream tmpStr;
			tmpStr << "WARN: " << "Unexpected tag '<" << stateChild->ValueStr() << ">' found in skin XML file on row " << stateChild->Row() << ".";
			std::cout << tmpStr << endl;
			mErrorDescription = tmpStr.str();
		}
	}

	return newPart;
}


/**
 * Translates a string into a ConstrolState enumeration.
 *
 * \param	state	A string defining a state. Case insensitive.
 *
 * \return	Returns a ControlState enumeration. \c STATE_UNDEFINED if
 *			if an unrecognized string is passed in.
 */
ControlState Skin::enumerateState(const string& state)
{
	ControlState returnState = STATE_UNDEFINED;

	std::string cmpStr = toLowercase(state);
	if(cmpStr == "normal")
		returnState = STATE_NORMAL;
	else if(cmpStr == "pressed")
		returnState = STATE_PRESSED;
	else if(cmpStr == "hover")
		returnState = STATE_HOVER;
	else if(cmpStr == "selected")
		returnState = STATE_SELECTED;
	else if(cmpStr == "disabled")
		returnState = STATE_DISABLED;
	else if(cmpStr == "nofocus")
		returnState = STATE_NOFOCUS;
	else
	{
		stringstream tmpStr;
		tmpStr << "WARN: " << "Unrecognized state found: " << state << endl;
		mErrorDescription = tmpStr.str();
	}

	return returnState;
}


/**
 * Parses a 'region' node.
 */
bool Skin::parseRegionNode(TiXmlNode *node, SkinPart *part)
{
	int result = TIXML_SUCCESS;
	int xpos = 0, ypos = 0, width = 0, height = 0;

	// ===========================================
	// Region Type
	// ===========================================
	const char *regionType = node->ToElement()->Attribute("type");
	if(regionType == NULL)
	{
		stringstream tmpStr;
		tmpStr << "Attribute 'type' in tag '<region>' contains invalid data on row " << node->Row() << ".";
		mMessageList.push_back(tmpStr.str());
		return false;
	}
	else if(strlen(regionType) == 0)
	{
		stringstream tmpStr;
		tmpStr << "Attribute 'type' in tag '<region>' contains invalid data on row " << node->Row() << ".";
		mMessageList.push_back(tmpStr.str());
		return false;
	}

	// ===========================================
	// X-Position
	// ===========================================
	result = node->ToElement()->QueryIntAttribute("xpos", &xpos);
	if(result != TIXML_SUCCESS)
	{
		if(result == TIXML_NO_ATTRIBUTE)
		{
			stringstream tmpStr;
			tmpStr << "Tag '<region>' is missing attribute 'xpos' on row " << node->Row() << ".";
			mMessageList.push_back(tmpStr.str());
			return false;
		}
		else if(result == TIXML_WRONG_TYPE)
		{
			stringstream tmpStr;
			tmpStr << "Attribute 'xpos' in tag '<region>' contains invalid data on row " << node->Row() << ".";
			mMessageList.push_back(tmpStr.str());
			return false;
		}
		else
		{
			mErrorDescription = "Unable to continue parsing Skin XML Definition. Unknown error parsing 'xpos' attribute.";
			mMessageList.push_back(mErrorDescription);
			return false;
		}
	}

	// ===========================================
	// Y-Position
	// ===========================================
	result = node->ToElement()->QueryIntAttribute("ypos", &ypos);
	if(result != TIXML_SUCCESS)
	{
		if(result == TIXML_NO_ATTRIBUTE)
		{
			stringstream tmpStr;
			tmpStr << "Tag '<region>' is missing attribute 'ypos' on row " << node->Row() << ".";
			mMessageList.push_back(tmpStr.str());
			return false;
		}
		else if(result == TIXML_WRONG_TYPE)
		{
			stringstream tmpStr;
			tmpStr << "Attribute 'ypos' in tag '<region>' contains invalid data on row " << node->Row() << ".";
			mMessageList.push_back(tmpStr.str());
			return false;
		}
		else
		{
			mErrorDescription = "Unable to continue parsing Skin XML Definition. Unknown error parsing 'ypos' attribute.";
			mMessageList.push_back(mErrorDescription);
			return false;
		}
	}

	// ===========================================
	// Width
	// ===========================================
	result = node->ToElement()->QueryIntAttribute("width", &width);
	if(result != TIXML_SUCCESS)
	{
		if(result == TIXML_NO_ATTRIBUTE)
		{
			stringstream tmpStr;
			tmpStr << "Tag '<region>' is missing attribute 'width' on row " << node->Row() << ".";
			mMessageList.push_back(tmpStr.str());
			return false;
		}
		else if(result == TIXML_WRONG_TYPE)
		{
			stringstream tmpStr;
			tmpStr << "Attribute 'width' in tag '<region>' contains invalid data on row " << node->Row() << ".";
			mMessageList.push_back(tmpStr.str());
			return false;
		}
		else
		{
			mErrorDescription = "Unable to continue parsing Skin XML Definition. Unknown error parsing 'width' attribute.";
			mMessageList.push_back(mErrorDescription);
			return false;
		}
	}

	// ===========================================
	// Height
	// ===========================================
	result = node->ToElement()->QueryIntAttribute("height", &height);
	if(result != TIXML_SUCCESS)
	{
		if(result == TIXML_NO_ATTRIBUTE)
		{
			stringstream tmpStr;
			tmpStr << "Tag '<region>' is missing attribute 'height' on row " << node->Row() << ".";
			mMessageList.push_back(tmpStr.str());
			return false;
		}
		else if(result == TIXML_WRONG_TYPE)
		{
			stringstream tmpStr;
			tmpStr << "Attribute 'height' in tag '<region>' contains invalid data on row " << node->Row() << ".";
			mMessageList.push_back(tmpStr.str());
			return false;
		}
		else
		{
			mErrorDescription = "Unable to continue parsing Skin XML Definition. Unknown error while parsing 'height' attribute.";
			mMessageList.push_back(mErrorDescription);
			return false;
		}
	}
	
	// Create a new 'sub image' from the source skin image given the information we've just
	// parsed from the region's attributes and add it to the part's image list.
	Image regionImg(mSource, xpos, ypos, width, height);
	if(regionImg.isLoaded())
	{
		part->addImage(translateRegion(regionType), regionImg);
	}

	return true;
}


/**
 * Parses a 'colordef' node.
 */
bool Skin::parseColordefNode(TiXmlNode *node, SkinPart *part)
{
	int result = TIXML_SUCCESS;
	int red = 0, green = 0, blue = 0;

	// ===========================================
	// COLORDEF TYPE
	// ===========================================
	const char *colordefType = node->ToElement()->Attribute("type");
	if(colordefType == NULL)
	{
		stringstream tmpStr;
		tmpStr << "Tag '<colordef>' is missing attribute 'type' on row " << node->Row() << ".";
		mMessageList.push_back(tmpStr.str());
		return false;
	}
	else if(strlen(colordefType) == 0)
	{
		stringstream tmpStr;
		tmpStr << "Attribute 'type' in tag '<colordef>' contains invalid data on row " << node->Row() << ".";
		mMessageList.push_back(tmpStr.str());
		return false;
	}


	// ===========================================
	// RED
	// ===========================================
	result = node->ToElement()->QueryIntAttribute("red", &red);
	if(result != TIXML_SUCCESS)
	{
		if(result == TIXML_NO_ATTRIBUTE)
		{
			stringstream tmpStr;
			tmpStr << "Tag '<colordef>' is missing attribute 'red' on row " << node->Row() << ".";
			mMessageList.push_back(tmpStr.str());
			return false;
		}
		else if (result == TIXML_WRONG_TYPE)
		{
			stringstream tmpStr;
			tmpStr << "Attribute 'red' in tag '<colordef>' contains invalid data on row " << node->Row() << ".";
			mMessageList.push_back(tmpStr.str());
			return false;
		}
		else
		{
			mErrorDescription = "Unable to continue parsing Skin XML Definition. Unknown error parsing 'red' attribute.";
			mMessageList.push_back(mErrorDescription);
			return false;
		}
	}

	// ===========================================
	// GREEN
	// ===========================================
	result = node->ToElement()->QueryIntAttribute("green", &green);
	if(result != TIXML_SUCCESS)
	{
		if(result == TIXML_NO_ATTRIBUTE)
		{
			stringstream tmpStr;
			tmpStr << "Tag '<colordef>' is missing attribute 'green' on row " << node->Row() << ".";
			mMessageList.push_back(tmpStr.str());
			return false;
		}
		else if(result == TIXML_WRONG_TYPE)
		{
			stringstream tmpStr;
			tmpStr << "Attribute 'green' in tag '<colordef>' contains invalid data on row " << node->Row() << ".";
			mMessageList.push_back(tmpStr.str());
			return false;
		}
		else
		{
			mErrorDescription = "Unable to continue parsing Skin XML Definition. Unknown error parsing 'green' attribute.";
			mMessageList.push_back(mErrorDescription);
			return false;
		}
	}

	// ===========================================
	// BLUE
	// ===========================================
	result = node->ToElement()->QueryIntAttribute("blue", &blue);
	if(result != TIXML_SUCCESS)
	{
		if(result == TIXML_NO_ATTRIBUTE)
		{
			stringstream tmpStr;
			tmpStr << "Tag '<colordef>' is missing attribute 'blue' on row " << node->Row() << ".";
			mMessageList.push_back(tmpStr.str());
			return false;
		}
		else if(result == TIXML_WRONG_TYPE)
		{
			stringstream tmpStr;
			tmpStr << "Attribute 'blue' in tag '<colordef>' contains invalid data on row " << node->Row() << ".";
			mMessageList.push_back(tmpStr.str());
			return false;
		}
		else
		{
			mErrorDescription = "Unable to continue parsing Skin XML Definition. Unknown error parsing 'blue' attribute.";
			mMessageList.push_back(mErrorDescription);
			return false;
		}
	}

	if(toLowercase(colordefType) == "foreground")
		part->setForeground(red, green, blue);
	else if(toLowercase(colordefType) == "background")
		part->setBackground(red, green, blue);
	else if(toLowercase(colordefType) == "border")
		part->setBorder(red, green, blue);
	else
	{
		mErrorDescription = "Tag 'colordef' defines an unrecognized type.";
		mMessageList.push_back(mErrorDescription);
		return false;
	}

	return true;
}


/**
 * Parses a 'captiontext' node.
 */
TextOffsets Skin::parseCaptionTextNode(TiXmlNode *node)
{
	int result = TIXML_SUCCESS;
	int leftoffset = 0, rightoffset = 0, topoffset = 0, bottomoffset = 0, minimumheight = 0;

	// ===========================================
	// CAPTIONTEXT ALIGNMENT
	// ===========================================
	const char *align = node->ToElement()->Attribute("align");
	if(align == NULL)
	{
		stringstream tmpStr;
		tmpStr << "Tag '<captiontext>' is missing attribute 'align' on row " << node->Row() << ".";
		mMessageList.push_back(tmpStr.str());
		return TextOffsets();
	}
	else if(strlen(align) == 0)
	{
		stringstream tmpStr;
		tmpStr << "Attribute 'type' in tag '<captiontext>' contains invalid data on row " << node->Row() << ".";
		mMessageList.push_back(tmpStr.str());
		return TextOffsets();
	}


	// ===========================================
	// LEFT OFFSET
	// ===========================================
	result = node->ToElement()->QueryIntAttribute("leftoffset", &leftoffset);
	if(result != TIXML_SUCCESS)
	{
		if(result == TIXML_NO_ATTRIBUTE)
		{
			stringstream tmpStr;
			tmpStr << "Tag '<captiontext>' is missing attribute 'leftoffset' on row " << node->Row() << ".";
			mMessageList.push_back(tmpStr.str());
			return TextOffsets();
		}
		else if (result == TIXML_WRONG_TYPE)
		{
			stringstream tmpStr;
			tmpStr << "Attribute 'leftoffset' in tag '<captiontext>' contains invalid data on row " << node->Row() << ".";
			mMessageList.push_back(tmpStr.str());
			return TextOffsets();
		}
		else
		{
			mErrorDescription = "Unable to continue parsing Skin XML Definition. Unknown error parsing 'leftoffset' attribute.";
			mMessageList.push_back(mErrorDescription);
			return TextOffsets();
		}
	}


	// ===========================================
	// RIGHT OFFSET
	// ===========================================
	result = node->ToElement()->QueryIntAttribute("rightoffset", &rightoffset);
	if(result != TIXML_SUCCESS)
	{
		if(result == TIXML_NO_ATTRIBUTE)
		{
			stringstream tmpStr;
			tmpStr << "Tag '<captiontext>' is missing attribute 'rightoffset' on row " << node->Row() << ".";
			mMessageList.push_back(tmpStr.str());
			return TextOffsets();
		}
		else if(result == TIXML_WRONG_TYPE)
		{
			stringstream tmpStr;
			tmpStr << "Attribute 'rightoffset' in tag '<captiontext>' contains invalid data on row " << node->Row() << ".";
			mMessageList.push_back(tmpStr.str());
			return TextOffsets();
		}
		else
		{
			mErrorDescription = "Unable to continue parsing Skin XML Definition. Unknown error parsing 'rightoffset' attribute.";
			mMessageList.push_back(mErrorDescription);
			return TextOffsets();
		}
	}


	// ===========================================
	// TOP OFFSET
	// ===========================================
	result = node->ToElement()->QueryIntAttribute("topoffset", &topoffset);
	if(result != TIXML_SUCCESS)
	{
		if(result == TIXML_NO_ATTRIBUTE)
		{
			stringstream tmpStr;
			tmpStr << "Tag '<captiontext>' is missing attribute 'topoffset' on row " << node->Row() << ".";
			mMessageList.push_back(tmpStr.str());
			return TextOffsets();
		}
		else if(result == TIXML_WRONG_TYPE)
		{
			stringstream tmpStr;
			tmpStr << "Attribute 'topoffset' in tag '<captiontext>' contains invalid data on row " << node->Row() << ".";
			mMessageList.push_back(tmpStr.str());
			return TextOffsets();
		}
		else
		{
			mErrorDescription = "Unable to continue parsing Skin XML Definition. Unknown error parsing 'topoffset' attribute.";
			mMessageList.push_back(mErrorDescription);
			return TextOffsets();
		}
	}


	// ===========================================
	// BOTTOM OFFSET
	// ===========================================
	result = node->ToElement()->QueryIntAttribute("bottomoffset", &bottomoffset);
	if(result != TIXML_SUCCESS)
	{
		if(result == TIXML_NO_ATTRIBUTE)
		{
			stringstream tmpStr;
			tmpStr << "Tag '<captiontext>' is missing attribute 'bottomoffset' on row " << node->Row() << ".";
			mMessageList.push_back(tmpStr.str());
			return TextOffsets();
		}
		else if(result == TIXML_WRONG_TYPE)
		{
			stringstream tmpStr;
			tmpStr << "Attribute 'bottomoffset' in tag '<captiontext>' contains invalid data on row " << node->Row() << ".";
			mMessageList.push_back(tmpStr.str());
			return TextOffsets();
		}
		else
		{
			mErrorDescription = "Unable to continue parsing Skin XML Definition. Unknown error parsing 'bottomoffset' attribute.";
			mMessageList.push_back(mErrorDescription);
			return TextOffsets();
		}
	}


	// ===========================================
	// MINIMUM HEIGHT
	// ===========================================
	result = node->ToElement()->QueryIntAttribute("minimumheight", &minimumheight);
	if(result != TIXML_SUCCESS)
	{
		if(result == TIXML_NO_ATTRIBUTE)
		{
			stringstream tmpStr;
			tmpStr << "Tag '<captiontext>' is missing attribute 'minimumheight' on row " << node->Row() << ".";
			mMessageList.push_back(tmpStr.str());
			return TextOffsets();
		}
		else if(result == TIXML_WRONG_TYPE)
		{
			stringstream tmpStr;
			tmpStr << "Attribute 'minimumheight' in tag '<captiontext>' contains invalid data on row " << node->Row() << ".";
			mMessageList.push_back(tmpStr.str());
			return TextOffsets();
		}
		else
		{
			mErrorDescription = "Unable to continue parsing Skin XML Definition. Unknown error parsing 'minimumheight' attribute.";
			mMessageList.push_back(mErrorDescription);
			return TextOffsets();
		}
	}

	TextOffsets rtrnOffsts;

	if(toLowercase(align) == "left")
		rtrnOffsts = TextOffsets(leftoffset, rightoffset, topoffset, bottomoffset, minimumheight, TextOffsets::TEXT_ALIGN_LEFT);
	else if(toLowercase(align) == "center")
		rtrnOffsts = TextOffsets(leftoffset, rightoffset, topoffset, bottomoffset, minimumheight, TextOffsets::TEXT_ALIGN_CENTER);
	else if(toLowercase(align) == "right")
		rtrnOffsts = TextOffsets(leftoffset, rightoffset, topoffset, bottomoffset, minimumheight, TextOffsets::TEXT_ALIGN_RIGHT);
	else
	{
		mErrorDescription = "Tag 'captiontext' defines an unrecognized alignment.";
		mMessageList.push_back(mErrorDescription);
		return TextOffsets();
	}

	return rtrnOffsts;
}


void Skin::parseMousePointers(TiXmlNode *node)
{
	// Parse through each 'pointer' tag as it appears.
	TiXmlNode *pointerNode = 0;
	while(pointerNode == node->IterateChildren(pointerNode))
	{
		if(pointerNode->ValueStr() == "pointer")
		{
			// POINTER STATE
			const char *state = pointerNode->ToElement()->Attribute("state");
			if(state == NULL)
			{
				stringstream tmpStr;
				tmpStr << "Attribute 'state' in '<pointer>' on row " << node->Row() << " is missing.";
				mMessageList.push_back(tmpStr.str());
				continue;
			}
			else if(strlen(state) == 0)
			{
				stringstream tmpStr;
				tmpStr << "Attribute 'state' in '<pointer>' on row " << node->Row() << " is empty.";
				mMessageList.push_back(tmpStr.str());
				continue;
			}

			// POINTER IMAGE
			const char *src = pointerNode->ToElement()->Attribute("src");
			if(src == NULL)
			{
				stringstream tmpStr;
				tmpStr << "Attribute 'src' in '<pointer>' on row " << node->Row() << " is missing.";
				mMessageList.push_back(tmpStr.str());
				continue;
			}
			else if(strlen(src) == 0)
			{
				stringstream tmpStr;
				tmpStr << "Attribute 'src' in '<pointer>' on row " << node->Row() << " is empty.";
				mMessageList.push_back(tmpStr.str());
				continue;
			}

			string tmpPath = Singleton<Filesystem>::get().getWorkingDir(mResourceName) + "/" + src;

			// Add a new pointer image to the pointer state.
			if(Singleton<Filesystem>::get().exists(tmpPath))
				mPointerList[translateState(state)] = Image(tmpPath);
			else
				std::cout << "WARN: " << "Pointer source image '" << src << "' on row " << pointerNode->Row() << " doesn't exist." << endl;


		}
		else
			std::cout << "WARN: " << "Unexpected tag found in '" << mResourceName << "' on row " << pointerNode->Row() << endl;
	}
}


/**
 * Translates a string value into a MousePointerState.
 * 
 * \return	A valid MousePointerState value. If the string passed in
 *			does not refer to a valid mouse state, MOUSE_DEFAULT is
 *			returned instead.
 */
MousePointerState Skin::translateState(const std::string& state)
{
	if(toLowercase(state) == "normal")
		return MOUSE_NORMAL;
	else if(toLowercase(state) == "thinking")
		return MOUSE_THINKING;
	else if(toLowercase(state) == "resize_ne")
		return MOUSE_RESIZE_NE;
	else if(toLowercase(state) == "resize_nw")
		return MOUSE_RESIZE_NW;
	else if(toLowercase(state) == "resize_all")
		return MOUSE_RESIZE_ALL;
	else if(toLowercase(state) == "hand")
		return MOUSE_HAND;
	else
	{
		if(toLowercase(state) != "default")
			std::cout << "WARN: " << "Unknown mouse state defined in skin '" << getName() << "'." << endl;

		return MOUSE_DEFAULT;
	}
}


/**
 * Searches a specified XML node for a 'name' attribute and then sets the
 * internal Skin name.
 */
bool Skin::getSkinName(TiXmlNode *node)
{
	const char *name = node->ToElement()->Attribute("name");
	if(name == NULL)
	{
		stringstream tmpStr;
		tmpStr << "Attribute 'name' in '<skinset>' on row " << node->Row() << " is missing.";
		mMessageList.push_back(tmpStr.str());
		return false;
	}
	else if(strlen(name) == 0)
	{
		stringstream tmpStr;
		tmpStr << "Attribute 'name' in '<skinset>' on row " << node->Row() << " is empty.";
		mMessageList.push_back(tmpStr.str());
		return false;
	}
	else
		mSkinName = name;

	return true;
}


/**
 * Searches a specified XML node for an 'image' attribute and then attempts
 * to load the specified image file.
 */
bool Skin::getSourceImage(TiXmlNode *node)
{
	const char *imagePath = node->ToElement()->Attribute("image");
	if(imagePath == NULL)
	{
		stringstream tmpStr;
		tmpStr << "Attribute 'image' in '<skinset>' on row " << node->Row() << " is missing.";
		mMessageList.push_back(tmpStr.str());
		return false;
	}
	else if(strlen(imagePath) == 0)
	{
		stringstream tmpStr;
		tmpStr << "Attribute 'image' in '<skinset>' on row " << node->Row() << " is empty.";
		mMessageList.push_back(tmpStr.str());
		return false;
	}
	else
	{
		// Get the working directory of the Skin Definition File and load specified images from it.
		string tmpPath = Singleton<Filesystem>::get().getWorkingDir(mResourceName) + "/" + imagePath;

		if(!Singleton<Filesystem>::get().exists(tmpPath))
		{
			stringstream tmpStr;
			tmpStr << "Image file '" << imagePath << "' could not be found." << endl;
			mMessageList.push_back(tmpStr.str());
			return false;
		}
		else
			mSource = new Image(tmpPath);
	}

	return true;
}


/**
 * Searches a specified XML node for an 'image' attribute and then attempts
 * to load the specified image file.
 */
bool Skin::getSkinFont(TiXmlNode *node)
{
	const char *fontPath = node->ToElement()->Attribute("font");
	if(fontPath == NULL)
	{
		stringstream tmpStr;
		tmpStr << "Attribute 'font' in '<skinset>' on row " << node->Row() << " is missing.";
		mMessageList.push_back(tmpStr.str());
		return false;
	}
	else if(strlen(fontPath) == 0)
	{
		stringstream tmpStr;
		tmpStr << "Attribute 'font' in '<skinset>' on row " << node->Row() << " is empty.";
		mMessageList.push_back(tmpStr.str());
		return false;
	}
	else
	{
		// Get the working directory of the Skin Definition File and load specified fonts from it.
		string tmpPath = "fonts/" + string(fontPath);
		if(!Singleton<Filesystem>::get().exists(tmpPath))
		{
			stringstream tmpStr;
			tmpStr << "Font file '" << fontPath << "' could not be found." << endl;
			mMessageList.push_back(tmpStr.str());
			return false;
		}
		else
			mSkinFont = new Font(tmpPath);
	}
	
	return true;
}


/**
 * Gets a references to an Image that represents the requested mouse state.
 *
 * \param	state	A valid MousePointerState value.
 *
 * \note	If the requested mouse state is unavailable, a pointer
 *			equivalent to MOUSE_DFEFAULT will be returned.
 */
Image& Skin::getMousePointer(MousePointerState state)
{
	MousePointerList::iterator it = mPointerList.find(state);

	if(it == mPointerList.end())
		return mPointerList[MOUSE_DEFAULT];

	return mPointerList[state];
}


/**
 * Gets a part.
 *
 * \param	type	A string defining a Control Type (e.g., Window, Button, TextBox, etc.).
 * \param	state	A ControlState representing a desired state.
 *
 * \return	Returns a pointer to a SkinPart (owned by Skin). NULL if requested
 *			part was not found.
 */
SkinPart* Skin::getPart(const string& name, ControlState state)
{
	pair<PartList::iterator, PartList::iterator> partListIt;

	partListIt = mPartList.equal_range(toLowercase(name));

	for(PartList::iterator it = partListIt.first; it != partListIt.second; ++it)  
	{  
		if(it->second.getState() == state)
			return &it->second;
	}

	std::cout << "WARN: " << "Requested part '" << name << "' and state '" << translateState(state) << "' was not found in skin." << endl;

	return NULL;
}


/**
 * Indicates whether the specified part is available.
 *
 * \param	partName	A string specifying the desired part.
 *
 * \return	\c True if specified part is found. \c False otherwise.
 */
bool Skin::hasPart(const string& partName) const
{
	if(mPartList.count(toLowercase(partName)) > 0)
		return true;

	return false;
}

/**
 * Counts the number of child nodes in a given TiXmlNode
 *
 * \param	node	Node to search children of.
 * \param	filter	Filter to only count nodes matching this name. Optional.
 *
 * \return	Number of children in \c node. If \c filter is specified, number
 *			of children in \c node matching filter. 0 if no children are
 *			present	or no children match \c filter.
 */
int Skin::countChildNodes(TiXmlNode *node, const string& filter)
{
	int regionCount = 0;			// Number of region nodes that are children of the State
	TiXmlNode *regionNode = 0;	// Child node of State

	// Alert user if a NULL node was passed.
	assert(node);
	if(!node)
	{
		std::cout << "WARN: " << "Skin::countChildNodes(): NULL node." << endl;
		return 0;
	}

	// Count the number of child nodes so we know what kind of part this is (monad, triad or ennead)
	if(!filter.empty())
	{
		while(regionNode == node->IterateChildren(regionNode))
		{
			// Ignore any node that's not a region.
			if(regionNode->ValueStr() == filter)
				++regionCount;
		}
	}
	else
	{
		while(regionNode == node->IterateChildren(regionNode))
		{
			// Ignore any node that's not a region.
			if(regionNode->ValueStr() == "region")
				++regionCount;
		}
	}

	return regionCount;
}


/**
 * Translates a string to an appropriate ImageRegion value.
 */
ImageRegion Skin::translateRegion(const string& region) const
{
	// Normalize our string to eliminate typo errors.
	string cmpStr = toLowercase(region);

	if(cmpStr == "top-left-corner")
		return REGION_TOP_LEFT_CORNER;
	else if(cmpStr == "top-edge")
		return REGION_TOP_EDGE;
	else if(cmpStr == "top-right-corner")
		return REGION_TOP_RIGHT_CORNER;
	else if(cmpStr == "left-edge")
		return REGION_LEFT_EDGE;
	else if(cmpStr == "bg-quad")
		return REGION_CENTER;
	else if(cmpStr == "right-edge")
		return REGION_RIGHT_EDGE;
	else if(cmpStr == "bottom-left-corner")
		return REGION_BOTTOM_LEFT_CORNER;
	else if(cmpStr == "bottom-edge")
		return REGION_BOTTOM_EDGE;
	else if(cmpStr == "bottom-right-corner")
		return REGION_BOTTOM_RIGHT_CORNER;
	else
		return REGION_UNKNOWN;
}

/**
 * Translates a ControlState to a human readable string.
 */
const std::string Skin::translateState(ControlState state) const
{
	if(state == STATE_NORMAL)
		return "STATE_NORMAL";
	else if(state == STATE_HOVER)
		return "STATE_HOVER";
	else if(state == STATE_PRESSED)
		return "STATE_PRESSED";
	else if(state == STATE_SELECTED)
		return "STATE_SELECTED";
	else if(state == STATE_DISABLED)
		return "STATE_DISABLED";
	else if(state == STATE_NOFOCUS)
		return "STATE_NOFOCUS";
	else
		return "STATE_UNKNOWN";
}

// ==================================================================================
// = SkinPart Class
// ==================================================================================

/**
 * Constructor.
 *
 * \param	type	Type of part this is. Valid values are \c PART_MONAD,
 *					\c PART_TRIAD and \c PART_ENNEAD.
 */
SkinPart::SkinPart(SkinPartType type, ControlState state):	mPartType(type),
															mState(state),
															mBg(Color_4ub(0, 0, 0, 255)),
															mFg(Color_4ub(0, 0, 0, 255)),
															mBr(Color_4ub(0, 0, 0, 255))
{
	switch(mPartType)
	{
		case PART_COLORONLY:
			break;
		case PART_MONAD:
			mImageList.resize(1);
			break;
		case PART_TRIAD:
			mImageList.resize(3);
			break;
		case PART_ENNEAD:
			mImageList.resize(9);
			break;
		default:
			throw Exception(9701, "GUI Skin", "Attempted to initialize skin with an invalid part type.");
			break;
	}

	mImageList.assign(mImageList.size(), Image());
}


/**
 * Cleans up all Image objects associated with this part.
 */
SkinPart::~SkinPart()
{
//	for(size_t i = 0; i < mImageList.size(); i++)
//		delete *(mImageList[i];
}


/**
 * Adds an image to a specified region.
 *
 * Which region an image is associated to is dependant on the type of part this is.
 *
 * Monad parts only have a single image which is drawn across an entire Gui.
 * Control. Triad parts have three parts; the left, right and center images
 * which are drawn accordingly. Finally, ennead types includ nine parts, one
 * for each corner of a square, for each edge of a square and for the center
 * of a square.
 *
 * \note	Once an Image is added, it is owned by, managed and handled by
 *			SkinPart. When SkinPart is destroyed it also frees any memory
 *			allocated for the Image.
 */
void SkinPart::addImage(ImageRegion region, Image &image)
{
	// Handle how images are added to the ImageList.
	switch(mPartType)
	{
		// Coloronly Controls don't take any images at all so tell whoever is attempting
		// to add one to go fuck themselves.
		case PART_COLORONLY:
			throw Exception(9705, "GUI Skin", "Attempting to add an image to a PART_COLORONLY part type.");
			break;
		// Monad controls only use a single image so ignore the region definition and
		// simply add it to the Image list.
		case PART_MONAD:
			mImageList[0] = image;
			break;

		// Triad controls use three images so put them where they belong.
		case PART_TRIAD:
			// TODO: This is a very ugly test. Is there a more sane way to do this?
			if(region != REGION_LEFT_EDGE || region != REGION_RIGHT_EDGE || region != REGION_CENTER)
			{
				std::cout << "WARN: " << "Attempting to add an invalid region type to a PART_TRIAD." << endl;
				return;
			}
			if(region == REGION_LEFT_EDGE)
				mImageList[0] = image;
			else if(region == REGION_RIGHT_EDGE)
				mImageList[2] = image;
			else if(region == REGION_CENTER)
				mImageList[1] = image;
			else
			{
				// NOTE: This is here purely for debugging purposes to make it loud and clear that we
				// have an error. Comment out when ready for public release.
				throw Exception(9703, "Skin Part", "Attempting to add an unknown region to skin part.");
			}
			break;

		// Ennead controls have 9 separate parts and are interpreted as a 3x3 grid by the renderer.
		case PART_ENNEAD:
			if(region == REGION_TOP_LEFT_CORNER)
				mImageList[0] = image;
			else if(region == REGION_TOP_EDGE)
				mImageList[1] = image;
			else if(region == REGION_TOP_RIGHT_CORNER)
				mImageList[2] = image;
			else if(region == REGION_LEFT_EDGE)
				mImageList[3] = image;
			else if(region == REGION_CENTER)
				mImageList[4] = image;
			else if(region == REGION_RIGHT_EDGE)
				mImageList[5] = image;
			else if(region == REGION_BOTTOM_LEFT_CORNER)
				mImageList[6] = image;
			else if(region == REGION_BOTTOM_EDGE)
				mImageList[7] = image;
			else if(region == REGION_BOTTOM_RIGHT_CORNER)
				mImageList[8] = image;
			else
			{
				// NOTE: This is here purely for debugging purposes to make it loud and clear that we have
				// an error (most likely typo in XML file). Comment out when ready for public release.
				throw Exception(9704, "Skin Part", "Attempting to add an unknown region to skin part.");
			}
			break;
	}
}


/**
 * Returns a reference to the internal ImageList.
 */
ImageList& SkinPart::getImageList()
{
	return mImageList;
}


/**
 * Returns the state of a Control that this this part represents.
 */
ControlState SkinPart::getState() const
{
	return mState;
}


/**
 * Returns the type of part this is.
 *
 * See SkinPartType for additional details.
 */
SkinPartType SkinPart::getPartType() const
{
	return mPartType;
}


/**
 * SGet foreground color.
 */
void SkinPart::setForeground(int r, int g, int b, int a)
{
	mFg = Color_4ub(r, g, b, a);
}


/**
 * Set background color.
 */
void SkinPart::setBackground(int r, int g, int b, int a)
{
	mBg = Color_4ub(r, g, b, a);
}


/**
 * Set border color.
 */
void SkinPart::setBorder(int r, int g, int b, int a)
{
	mBr = Color_4ub(r, g, b, a);
}


/**
 * Set border color.
 */
void SkinPart::setCaptionOffsets(const TextOffsets& tO)
{
	mCaptionOffsets = tO;
}


/**
 * Get foreground color.
 */
Color_4ub SkinPart::getForeground() const
{
	return mFg;
}


/**
 * Get foreground color.
 */
Color_4ub SkinPart::getBackground() const
{
	return mBg;
}


/**
 * Get foreground color.
 */
Color_4ub SkinPart::getBorder() const
{
	return mFg;
}


/**
 * Gets the minimum width this part can be to draw properly.
 *
 * \return	0 if this is a PART_COLORONLY part.
 */
int SkinPart::getMinimumWidth() const
{
	// Ugly and hideous despite being effective. Should probably
	// find a better way to do this.
	if(mPartType == PART_COLORONLY)
		return 0;
	else if(mPartType == PART_MONAD)
		return mImageList[0].getWidth();
	else if(mPartType == PART_TRIAD)
		return mImageList[0].getWidth() + mImageList[1].getWidth() + mImageList[2].getWidth();
	else if(mPartType == PART_ENNEAD)
		return	mImageList[0].getWidth() + mImageList[1].getWidth() + mImageList[2].getWidth();
	else
		// If we're here than we've got a bug that needs to get checked out.
		throw Exception(9704, "SkinPart::getMinimumWidth()", "SkinPart is defined with an invalid PartType!");

	return 0;
}


/**
 * Gets the minimum height this part can be to draw properly.
 *
 * \return	0 if this is a PART_COLORONLY part.
 */
int SkinPart::getMinimumHeight() const
{
	// Just as bad as its conterpart. Should probably
	// find a better way to do this.
	if(mPartType == PART_COLORONLY)
		return 0;
	else if(mPartType == PART_MONAD)
		return mImageList[0].getHeight();
	else if(mPartType == PART_TRIAD)
		return mImageList[0].getHeight();
	else if(mPartType == PART_ENNEAD)
		return	mImageList[0].getHeight() + mImageList[3].getHeight() + mImageList[6].getHeight();
	else
		// If we're here than we've got a bug that needs to get checked out.
		throw Exception(9704, "SkinPart::getMinimumHeight()", "SkinPart is defined with an invalid PartType!");
	return 0;
}
