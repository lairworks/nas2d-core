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
#include "XmlElement.h"

#include "XmlAttribute.h"
#include "XmlAttributeSet.h"
#include "XmlComment.h"
#include "XmlDocument.h"
#include "XmlText.h"
#include "XmlUnknown.h"

using namespace NAS2D::Xml;


XmlElement::XmlElement(const std::string& value) :
	XmlNode(XmlNode::NodeType::XML_ELEMENT)
{
	_value = value;
}


XmlElement::XmlElement(const XmlElement& copy) :
	XmlNode(XmlNode::NodeType::XML_ELEMENT)
{
	copy.copyTo(this);
}


XmlElement& XmlElement::operator=(const XmlElement& base)
{
	clearThis();
	base.copyTo(this);
	return *this;
}


XmlElement::~XmlElement()
{
	clearThis();
}


void XmlElement::clearThis()
{
	clear();
	while (attributeSet.first())
	{
		XmlAttribute* node = attributeSet.first();
		attributeSet.remove(node);
		delete node;
	}
}


/**
 * Gets the value of an attribute matching 'name'.
 *
 * \param name	Name of the attribute to find.
 *
 * \returns The value of the named attribute or an empty string if
 *			the attribute wasn't found.
 */
std::string XmlElement::attribute(const std::string& name) const
{
	const XmlAttribute* node = attributeSet.find(name);
	if (node)
	{
		return node->value();
	}

	return "";
}


/**
 * Sets a value to a named attribute.
 *
 * \note	If no named attribute is found, one is created.
 *
 * \param name	Name of the attribute to find.
 * \param i		\c int value to set to the attribute.
 */
void XmlElement::attribute(const std::string& name, int i)
{
	XmlAttribute* attrib = attributeSet.findOrCreate(name);

	if (attrib)
	{
		attrib->intValue(i);
	}
}


/**
 * Sets a value to a named attribute.
 *
 * \note	If no named attribute is found, one is created.
 *
 * \param name	Name of the attribute to find.
 * \param d		\c double value to set to the attribute.
 */
void XmlElement::attribute(const std::string& name, double d)
{
	XmlAttribute* attrib = attributeSet.findOrCreate(name);
	if (attrib)
	{
		attrib->doubleValue(d);
	}
}


/**
 * Sets a value to a named attribute.
 *
 * \note	If no named attribute is found, one is created.
 *
 * \param name	Name of the attribute to find.
 * \param s		String value to set to the attribute.
 */
void XmlElement::attribute(const std::string& name, const std::string& s)
{
	XmlAttribute* attrib = attributeSet.findOrCreate(name);
	if (attrib)
	{
		attrib->value(s);
	}
}


/**
 * Writes the element to a string buffer.
 *
 * \param buf	String buffer to write to.
 * \param depth	Indent depth to use when writing the element.
 */
void XmlElement::write(std::string& buf, int depth) const
{
	for (int i = 0; i < depth; ++i)
	{
		buf += "\t";
	}

	buf += "<" + _value;

	const XmlAttribute* attrib;
	for (attrib = attributeSet.first(); attrib; attrib = attrib->next())
	{
		buf += " ";
		attrib->write(buf, depth);
	}

	XmlNode* node;
	if (!_firstChild)
	{
		buf += " />";
	}
	else if (_firstChild == _lastChild && _firstChild->toText())
	{
		buf += ">";
		_firstChild->write(buf, depth + 1);
		buf += "</" + _value + ">";
	}
	else
	{
		buf += ">";

		for (node = _firstChild; node; node = node->nextSibling())
		{
			if (!node->toText()) { buf += "\n"; }

			node->write(buf, depth + 1);
		}

		buf += "\n";

		for (int i = 0; i < depth; ++i)
		{
			buf += "\t";
		}

		buf += "</" + _value + ">";
	}
}


void XmlElement::copyTo(XmlElement* target) const
{
	// superclass:
	XmlNode::copyTo(target);

	// Element class:
	// Clone the attributes, then clone the children.
	const XmlAttribute* attribute = nullptr;
	for (attribute = attributeSet.first(); attribute; attribute = attribute->next())
	{
		target->attribute(attribute->name(), attribute->value());
	}

	XmlNode* node = nullptr;
	for (node = _firstChild; node; node = node->nextSibling())
	{
		target->linkEndChild(node->clone());
	}
}


/**
 * Walk the XML tree visiting this node and all of its children.
 */
bool XmlElement::accept(void* visitor) const
{
	if (static_cast<XmlVisitor*>(visitor)->visitEnter(*this, attributeSet.first()))
	{
		for (const XmlNode* node = firstChild(); node; node = node->nextSibling())
		{
			if (!node->accept(visitor)) { break; }
		}
	}
	return static_cast<XmlVisitor*>(visitor)->visitExit(*this);
}


/**
 * Creates a new Element and returns it - the returned element is a copy.
 */
XmlNode* XmlElement::clone() const
{
	XmlElement* clone = new XmlElement(value());
	if (!clone)
	{
		return nullptr;
	}

	copyTo(clone);
	return clone;
}

std::string XmlElement::getText() const
{
	const XmlNode* child = this->firstChild();
	if (child)
	{
		const XmlText* childText = child->toText();
		if (childText)
		{
			return childText->value();
		}
	}

	return std::string();
}

/**
 * Deletes an attribute with the given name.
 *
 * \param name	Name of the attribute to delete.
 */
void XmlElement::removeAttribute(const std::string& name)
{
	XmlAttribute* node = attributeSet.find(name);
	if (node)
	{
		attributeSet.remove(node);
		delete node;
	}
}


/**
 * Get the first attribute of the XmlElement.
 *
 * \returns Pointer to an XmlAttribute or nullptr if the
 *			element has no attributes.
 */
const XmlAttribute* XmlElement::firstAttribute() const
{
	return attributeSet.first();
}


/**
 * Get the first attribute of the XmlElement.
 *
 * Non-const version of firstAttribute().
 *
 * \returns Pointer to an XmlAttribute or nullptr if the
 *			element has no attributes.
 */
XmlAttribute* XmlElement::firstAttribute()
{
	return attributeSet.first();
}


/**
 * Get the last attribute of the XmlElement.
 */
const XmlAttribute* XmlElement::lastAttribute() const
{
	return attributeSet.last();
}


/**
 * Get the last attribute of the XmlElement.
 */
XmlAttribute* XmlElement::lastAttribute()
{
	return attributeSet.last();
}
