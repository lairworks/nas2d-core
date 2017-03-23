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

#include "NAS2D/Xml/XmlAttribute.h"
#include "NAS2D/Xml/XmlAttributeSet.h"
#include "NAS2D/Xml/XmlNode.h"

namespace NAS2D {
namespace Xml {

/**
 * The XmlElement is a container class. It has a value, the element name, and can
 * contain other elements, text, comments, and unknowns. Elements may also contain
 * an arbitrary number of attributes.
 */
class XmlElement : public XmlNode
{
public:
	XmlElement(const std::string& _value);
	XmlElement(const XmlElement& copy);
	virtual ~XmlElement();

	XmlElement& operator=(const XmlElement& base);

	void removeAttribute(const std::string& name);

	std::string attribute(const std::string& name) const;
	void attribute(const std::string& name, int i);
	void attribute(const std::string& name, double d);
	void attribute(const std::string& name, const std::string& s);

	const XmlAttribute* firstAttribute() const;
	XmlAttribute* firstAttribute();

	const XmlAttribute* lastAttribute() const;
	XmlAttribute* lastAttribute();

	virtual XmlNode* clone() const;
	
	virtual void write(std::string& buf, int depth) const;

	virtual const char* parse(const char* p, TiXmlParsingData* data);

	virtual const XmlElement* toElement() const { return this; }
	virtual XmlElement* toElement() { return this; }

	virtual bool accept(void* visitor) const;

protected:
	void copyTo(XmlElement* target) const;
	void clearThis();	// like clear, but initializes 'this' object as well
	virtual void streamIn(std::istream& in, std::string& tag);
	const char* readValue(const char* in, TiXmlParsingData* prevData);

private:
	XmlAttributeSet attributeSet;
};	
	
} // namespace Xml
} // namespace NAS2D
