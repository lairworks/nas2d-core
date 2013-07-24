// ==================================================================================
// = NAS2D
// = Copyright © 2008 - 2013 New Age Software
// ==================================================================================
// = NAS2D is distributed under the terms of the zlib license. You are free to copy,
// = modify and distribute the software under the terms of the zlib license.
// = 
// = Acknowledgement of your use of NAS2D is appriciated but is not required.
// ==================================================================================

#ifndef _LOM_XML_ATTRIBUTE_PARSER_
#define _LOM_XML_ATTRIBUTE_PARSER_

#include "Common.h"

#include "tinyxml/tinyxml.h"


class XmlAttributeParser
{
public:
	int intAttribute(TiXmlNode* node, const std::string& attribute);
	float floatAttribute(TiXmlNode* node, const std::string& attribute);
	double doubleAttribute(TiXmlNode* node, const std::string& attribute);
	std::string stringAttribute(TiXmlNode* node, const std::string& attribute);

	const StringList& getMessages() const { return mMessages; }

private:
	StringList		mMessages;
};



#endif