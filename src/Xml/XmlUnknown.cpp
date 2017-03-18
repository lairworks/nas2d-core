// ==================================================================================
// = NAS2D
// = Copyright © 2008 - 2017 New Age Software
// ==================================================================================
// = NAS2D is distributed under the terms of the zlib license. You are free to copy,
// = modify and distribute the software under the terms of the zlib license.
// = 
// = Acknowledgement of your use of NAS2D is appriciated but is not required.
// ==================================================================================
// = Originally based on TinyXML. See Xml.h for additional details.
// ==================================================================================
#include "NAS2D/Xml/XmlUnknown.h"

using namespace NAS2D::Xml;

TiXmlUnknown::TiXmlUnknown() : TiXmlNode(TiXmlNode::TINYXML_UNKNOWN)
{}


TiXmlUnknown::~TiXmlUnknown()
{}


TiXmlUnknown::TiXmlUnknown(const TiXmlUnknown& copy) : TiXmlNode(TiXmlNode::TINYXML_UNKNOWN)
{
	copy.CopyTo(this);
}


TiXmlUnknown& TiXmlUnknown::operator=(const TiXmlUnknown& copy)
{
	copy.CopyTo(this);
	return *this;
}


void TiXmlUnknown::Print(std::string& buf, int depth) const
{
	for (int i = 0; i < depth; i++)
		buf += "\t";

	buf += "<" + value + ">";
}


TiXmlNode* TiXmlUnknown::Clone() const
{
	TiXmlUnknown* clone = new TiXmlUnknown();

	if (!clone)
		return nullptr;

	CopyTo(clone);
	return clone;
}


bool TiXmlUnknown::Accept(XmlVisitor* visitor) const
{
	return visitor->Visit(*this);
}


void TiXmlUnknown::CopyTo(TiXmlUnknown* target) const
{
	TiXmlNode::CopyTo(target);
}
