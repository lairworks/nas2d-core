// ==================================================================================
// = NAS2D
// = Copyright © 2008 - 2019 New Age Software
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

/**
 * Default c'tor.
 */
XmlComment::XmlComment() : XmlNode(XmlNode::XML_COMMENT)
{}


/**
 * C'tor.
 *
 * Constructs an XML Comment with a value.
 *
 * \param	_value	Reference to a \c std::string with the value to use for the comment.
 */
XmlComment::XmlComment(const std::string& _value) : XmlNode(XmlNode::XML_COMMENT)
{
	value(_value);
}


/**
 * Copy c'tor.
 */
XmlComment::XmlComment(const XmlComment& copy)
{
	copy.copyTo(this);
}


/**
 * Copy assignment operator.
 */
XmlComment& XmlComment::operator=(const XmlComment& base)
{
	clear();
	base.copyTo(this);
	return *this;
}


/**
 * \see XmlBase::write()
 */
void XmlComment::write(std::string& buf, int depth) const
{
	for (int i = 0; i < depth; i++)
	{
		buf += "\t";
	}

	buf += "<!--" + _value + "-->";
}


/**
 * Creates a clone of the Comment.
 *
 * \returns	A pointer to an XmlNode.
 */
XmlNode* XmlComment::clone() const
{
	XmlComment* clone = new XmlComment();
	if (!clone) { return nullptr; }

	copyTo(clone);
	return clone;
}


/**
 * Walk the XML tree visiting this node and all of its children.
 */
bool XmlComment::accept(void* visitor) const
{
	return static_cast<XmlVisitor*>(visitor)->visit(*this);
}


void XmlComment::copyTo(XmlComment* target) const
{
	XmlNode::copyTo(target);
}
