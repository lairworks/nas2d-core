// ==================================================================================
// = NAS2D
// = Copyright © 2008 - 2017 New Age Software
// ==================================================================================
// = NAS2D is distributed under the terms of the zlib license. You are free to copy,
// = modify and distribute the software under the terms of the zlib license.
// = 
// = Acknowledgement of your use of NAS2D is appriciated but is not required.
// ==================================================================================

#pragma once

#include "NAS2D/Common.h"

#include "NAS2D/Xml/Xml.h"

namespace NAS2D {

class XmlAttributeParser
{
public:
	int intAttribute(Xml::TiXmlNode* node, const std::string& attribute);
	float floatAttribute(Xml::TiXmlNode* node, const std::string& attribute);
	double doubleAttribute(Xml::TiXmlNode* node, const std::string& attribute);
	std::string stringAttribute(Xml::TiXmlNode* node, const std::string& attribute);
};

} // namespace
