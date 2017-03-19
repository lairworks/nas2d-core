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
	XmlComment();

	XmlComment(const std::string& _value);
	XmlComment(const XmlComment& copy);
	XmlComment& operator=(const XmlComment& base);

	virtual ~XmlComment() {}

	/// Returns a copy of this Comment.
	virtual XmlNode* clone() const;

	virtual void write(std::string& buf, int depth) const;

	/**
	 * Attribtue parsing starts: at the ! of the !--
	 * 					 returns: next char past '>'
	 */
	virtual const char* parse(const char* p, TiXmlParsingData* data);

	virtual const XmlComment* toComment() const { return this; }
	virtual XmlComment* toComment() { return this; }

	virtual bool accept(XmlVisitor* visitor) const;

protected:
	void copyTo(XmlComment* target) const;
	virtual void streamIn(std::istream& in, std::string& tag);

private:

};

} // namespace Xml
} // namespace NAS2D
