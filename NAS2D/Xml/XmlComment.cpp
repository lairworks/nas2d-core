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
#include "XmlComment.h"

using namespace NAS2D::Xml;

XmlComment::XmlComment() : XmlNode(XmlNode::NodeType::XML_COMMENT)
{}

XmlComment::XmlComment(const std::string& commentValue) : XmlNode(XmlNode::NodeType::XML_COMMENT)
{
	value(commentValue);
}


XmlComment::XmlComment(const XmlComment& copy) : XmlNode(XmlNode::NodeType::XML_COMMENT)
{
	copy.copyTo(this);
}


XmlComment& XmlComment::operator=(const XmlComment& base)
{
	clear();
	base.copyTo(this);
	return *this;
}

void XmlComment::write(std::string& buf, int depth) const
{
	for (int i = 0; i < depth; i++)
	{
		buf += "\t";
	}

	buf += "<!--" + _value + "-->";
}

XmlNode* XmlComment::clone() const
{
	XmlComment* clone = new XmlComment();
	if (!clone) { return nullptr; }

	copyTo(clone);
	return clone;
}

bool XmlComment::accept(void* visitor) const
{
	return static_cast<XmlVisitor*>(visitor)->visit(*this);
}


void XmlComment::copyTo(XmlComment* target) const
{
	XmlNode::copyTo(target);
}
