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
class TiXmlComment : public TiXmlNode
{
public:
	TiXmlComment() : TiXmlNode(TiXmlNode::TINYXML_COMMENT) {}

	TiXmlComment(const std::string& _value) : TiXmlNode(TiXmlNode::TINYXML_COMMENT) { SetValue(_value); }
	TiXmlComment(const TiXmlComment& copy) { copy.CopyTo(this); }
	TiXmlComment& operator=(const TiXmlComment& base);

	virtual ~TiXmlComment() {}

	/// Returns a copy of this Comment.
	virtual TiXmlNode* Clone() const;

	virtual void Print(std::string& buf, int depth) const;

	/**
	 * Attribtue parsing starts: at the ! of the !--
	 * 					 returns: next char past '>'
	 */
	virtual const char* Parse(const char* p, TiXmlParsingData* data, TiXmlEncoding encoding);

	virtual const TiXmlComment* ToComment() const { return this; }
	virtual TiXmlComment* ToComment() { return this; }

	/**
	 * Walk the XML tree visiting this node and all of its children.
	 */
	virtual bool Accept(XmlVisitor* visitor) const { return visitor->Visit(*this); }

protected:
	void CopyTo(TiXmlComment* target) const { TiXmlNode::CopyTo(target); }
	virtual void StreamIn(std::istream& in, std::string& tag);

private:

};

} // namespace Xml
} // namespace NAS2D
