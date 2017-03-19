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
 * contain other elements, text, comments, and unknowns. Elements also contain an
 * arbitrary number of attributes.
 */
class XmlElement : public XmlNode
{
public:
	XmlElement(const std::string& _value);
	XmlElement(const XmlElement& copy);

	XmlElement& operator=(const XmlElement& base);

	virtual ~XmlElement();

	/**
	 * Given an attribute name, Attribute() returns the value for the attribute
	 * of that name, or null if none exists.
	 */
	std::string attribute(const std::string& name) const;

	/**
	 * Given an attribute name, Attribute() returns the value for the attribute
	 * of that name, or null if none exists. If the attribute exists and can be
	 * converted to an integer,	the integer value will be put in the return
	 * 'i', if 'i' is non-null.
	 */
	std::string attribute(const std::string& name, int& i) const;

	/**
	 * Given an attribute name, Attribute() returns the value for the attribute
	 * of that name, or null if none exists. If the attribute exists and can be
	 * converted to an double, the double value will be put in the return 'd',
	 * if 'd' is non-null.
	 */
	std::string attribute(const std::string& name, double& d) const;

	/**
	 * QueryIntAttribute examines the attribute - it is an alternative to the
	 * Attribute() method with richer error checking. If the attribute is an
	 * integer, it is stored in 'value' and	the call returns XML_SUCCESS. If
	 * it is not an integer, it returns XML_WRONG_TYPE. If the attribute does
	 * not exist, then XML_NO_ATTRIBUTE is returned.
	 */
	XmlAttribute::QueryResult queryIntAttribute(const std::string& name, int& _value) const;

	/**
	 * QueryDoubleAttribute examines the attribute.
	 * 
	 * \see QueryIntAttribute()
	 */
	XmlAttribute::QueryResult queryDoubleAttribute(const std::string& name, double& _value) const;

	/**
	 * QueryFloatAttribute examines the attribute.
	 * 
	 * \see QueryIntAttribute()
	 */
	XmlAttribute::QueryResult queryFloatAttribute(const std::string& name, float& _value) const;

	/**
	 * QueryStringAttribute examines the attribute.
	 * 
	 * \see QueryIntAttribute()
	 */
	XmlAttribute::QueryResult queryStringAttribute(const std::string& name, std::string& _value) const;


	/**
	 * Sets an attribute of name to a given value. The attribute
	 * will be created if it does not exist, or changed if it does.
	 */
	void setAttribute(const std::string& name, const std::string& _value);

	/**
	 * Sets an attribute of name to a given value. The attribute
	 * will be created if it does not exist, or changed if it does.
	 */
	void setAttribute(const std::string& name, int value);

	/**
	 * Sets an attribute of name to a given value. The attribute
	 * will be created if it does not exist, or changed if it does.
	 */
	void setDoubleAttribute(const std::string& name, double value);

	/**
	 * Deletes an attribute with the given name.
	 */
	void removeAttribute(const std::string& name);


	/**
	 * Get the first attribute of the XmlElement.
	 */
	const XmlAttribute* firstAttribute() const;
	XmlAttribute* firstAttribute();

	/**
	 * Get the last attribute of the XmlElement.
	 */
	const XmlAttribute* lastAttribute() const;
	XmlAttribute* lastAttribute();

	/**
	 * Convenience function for easy access to the text inside an element. Although easy
	 * and concise, GetText() is limited compared to getting the XmlText child and 
	 * accessing it directly.
	 * 
	 * If the first child of 'this' is a XmlText, the GetText() returns the character
	 * string of the Text node, else null is returned.
	 * 
	 * This is a convenient method for getting the text of simple contained text:
	 * \code{.xml}
	 * <foo>This is text</foo>
	 * \endcode
	 * 
	 * \code {.cpp}
	 * std::string str = fooElement->GetText();
	 * \endcode
	 * 
	 * \c str will contain "This is text".
	 * 
	 * \note This function can be misleading. If the element \c foo was created from
	 * this XML:
	 * \code{.xml}
	 * <foo><b>This is text</b></foo>
	 * \endcode
	 * Then \c str would be empty. The first child node isn't a text node, it is
	 * another element. From this XML:
	 * \code{.xml}
	 * <foo>This is <b>text</b></foo>
	 * \endcode
	 * \c GetText() will return "This is ".
	 * 
	 * \warning	GetText() accesses a child node - don't become confused with the
	 *			similarly named XmlHandle::Text() and XmlNode::ToText() which
	 *			are safe type casts on the referenced node.
	 */
	const std::string& text() const;

	/**
	 * Creates a new Element and returns it - the returned element is a copy.
	 */
	virtual XmlNode* clone() const;
	
	virtual void write(std::string& buf, int depth) const;

	/**
	 * Attribtue parsing starts: next char past '<'
	 * returns: next char past '>'
	 */
	virtual const char* parse(const char* p, TiXmlParsingData* data);

	virtual const XmlElement* toElement() const { return this; }
	virtual XmlElement* toElement() { return this; }

	/**
	 * Walk the XML tree visiting this node and all of its children.
	 */
	virtual bool accept(XmlVisitor* visitor) const;

protected:
	void copyTo(XmlElement* target) const;
	void clearThis();	// like clear, but initializes 'this' object as well

	// Used to be public [internal use]
	virtual void streamIn(std::istream& in, std::string& tag);

	/*	[internal use]
		Reads the "value" of the element -- another element, or text.
		This should terminate with the current end tag.
	*/
	const char* readValue(const char* in, TiXmlParsingData* prevData);

private:
	XmlAttributeSet attributeSet;
};	
	
} // namespace Xml
} // namespace NAS2D
