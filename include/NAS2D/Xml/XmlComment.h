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
#pragma once

#include "NAS2D/Xml/XmlNode.h"

namespace NAS2D {
namespace Xml {

/**
 * A Comment in XML.
 */
class XmlComment : public XmlNode
{
public:
	XmlComment() : XmlNode(XmlNode::TINYXML_COMMENT) {}

	XmlComment(const std::string& _value) : XmlNode(XmlNode::TINYXML_COMMENT) { SetValue(_value); }
	XmlComment(const XmlComment& copy) { copy.CopyTo(this); }
	XmlComment& operator=(const XmlComment& base);

	virtual ~XmlComment() {}

	/// Returns a copy of this Comment.
	virtual XmlNode* Clone() const;

	virtual void Print(std::string& buf, int depth) const;

	/**
	 * Attribtue parsing starts: at the ! of the !--
	 * 					 returns: next char past '>'
	 */
	virtual const char* Parse(const char* p, TiXmlParsingData* data, XmlEncoding encoding);

	virtual const XmlComment* ToComment() const { return this; }
	virtual XmlComment* ToComment() { return this; }

	/**
	 * Walk the XML tree visiting this node and all of its children.
	 */
	virtual bool Accept(XmlVisitor* visitor) const { return visitor->Visit(*this); }

protected:
	void CopyTo(XmlComment* target) const { XmlNode::CopyTo(target); }
	virtual void StreamIn(std::istream& in, std::string& tag);

private:

};

} // namespace Xml
} // namespace NAS2D
