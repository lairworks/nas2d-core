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
#include "NAS2D/Xml/XmlComment.h"

using namespace NAS2D::Xml;

TiXmlComment& TiXmlComment::operator=(const TiXmlComment& base)
{
	Clear();
	base.CopyTo(this);
	return *this;
}


void TiXmlComment::Print(std::string& buf, int depth) const
{
	for (int i = 0; i < depth; i++)
		buf += "\t";

	buf += "<!--" + value + "-->";
}


TiXmlNode* TiXmlComment::Clone() const
{
	TiXmlComment* clone = new TiXmlComment();

	if (!clone)
		return nullptr;

	CopyTo(clone);
	return clone;
}
