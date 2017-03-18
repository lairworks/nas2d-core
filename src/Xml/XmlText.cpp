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

XmlText::XmlText(const std::string& initValue) : XmlNode(XmlNode::XML_TEXT), cdata(false)
{
	SetValue(initValue);
}


XmlText::XmlText(const XmlText& copy) : XmlNode(XmlNode::XML_TEXT)
{
	copy.CopyTo(this);
}


XmlText& XmlText::operator=(const XmlText& base)
{
	base.CopyTo(this);
	return *this;
}


void XmlText::Print(std::string& buf, int depth) const
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
		buf += value;
	}
}


void XmlText::CopyTo(XmlText* target) const
{
	XmlNode::CopyTo(target);
	target->cdata = cdata;
}


XmlNode* XmlText::Clone() const
{
	XmlText* clone = new XmlText("");
	CopyTo(clone);

	return clone;
}
