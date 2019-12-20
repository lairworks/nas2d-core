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
#pragma once

#include "XmlNode.h"

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

	virtual XmlNode* clone() const;

	virtual void write(std::string& buf, int depth) const;

	virtual const char* parse(const char* p, void* data);

	virtual const XmlComment* toComment() const { return this; }
	virtual XmlComment* toComment() { return this; }

	virtual bool accept(void* visitor) const;

protected:
	void copyTo(XmlComment* target) const;
	virtual void streamIn(std::istream& in, std::string& tag);

private:

};

} // namespace Xml
} // namespace NAS2D
