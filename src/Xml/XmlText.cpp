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
#include "NAS2D/Xml/XmlText.h"

using namespace NAS2D::Xml;

TiXmlText::TiXmlText(const std::string& initValue) : TiXmlNode(TiXmlNode::TINYXML_TEXT), cdata(false)
{
	SetValue(initValue);
}


TiXmlText::TiXmlText(const TiXmlText& copy) : TiXmlNode(TiXmlNode::TINYXML_TEXT)
{
	copy.CopyTo(this);
}


TiXmlText& TiXmlText::operator=(const TiXmlText& base)
{
	base.CopyTo(this);
	return *this;
}


void TiXmlText::Print(std::string& buf, int depth) const
{
	if (cdata)
	{
		buf += "\n";
		for (int i = 0; i < depth; i++)
			buf += "\t";

		buf += "<![CDATA[" + value + "]]>\n"; // unformatted output
	}
	else
	{
		std::string buffer;
		EncodeString(value, buffer);
		buf += buffer;
	}
}


void TiXmlText::CopyTo(TiXmlText* target) const
{
	TiXmlNode::CopyTo(target);
	target->cdata = cdata;
}


TiXmlNode* TiXmlText::Clone() const
{
	TiXmlText* clone = new TiXmlText("");
	CopyTo(clone);

	return clone;
}
