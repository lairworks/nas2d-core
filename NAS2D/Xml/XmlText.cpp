// ==================================================================================
// = NAS2D
// = Copyright © 2008 - 2020 New Age Software
// ==================================================================================
// = NAS2D is distributed under the terms of the zlib license. You are free to copy,
// = modify and distribute the software under the terms of the zlib license.
// =
// = Acknowledgment of your use of NAS2D is appreciated but is not required.
// ==================================================================================
// = Originally based on TinyXML. See Xml.h for additional details.
// ==================================================================================
#include "XmlText.h"

using namespace NAS2D::Xml;

XmlText::XmlText(const std::string& initValue) :
	XmlNode(XmlNode::NodeType::XML_TEXT),
	cdata(false)
{
	value(initValue);
}


XmlText::XmlText(const XmlText& copy) :
	XmlNode(XmlNode::NodeType::XML_TEXT)
{
	copy.copyTo(this);
}


XmlText& XmlText::operator=(const XmlText& base)
{
	base.copyTo(this);
	return *this;
}


void XmlText::write(std::string& buf, int depth) const
{
	if (cdata)
	{
		buf += "\n";
		for (int i = 0; i < depth; i++)
		{
			buf += "\t";
		}

		buf += "<![CDATA[" + value() + "]]>\n"; // unformatted output
	}
	else
	{
		buf += value();
	}
}


void XmlText::copyTo(XmlText* target) const
{
	XmlNode::copyTo(target);
	target->cdata = cdata;
}


XmlNode* XmlText::clone() const
{
	XmlText* clone = new XmlText("");
	copyTo(clone);

	return clone;
}


bool XmlText::blank() const
{
	for (unsigned i = 0; i < _value.length(); ++i)
	{
		if (!white_space(_value[i]))
		{
			return false;
		}
	}

	return true;
}


/**
 * Walk the XML tree visiting this node and all of its children.
 */
bool XmlText::accept(void* visitor) const
{
	return static_cast<XmlVisitor*>(visitor)->visit(*this);
}
