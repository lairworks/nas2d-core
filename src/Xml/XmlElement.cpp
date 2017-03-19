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
#include "NAS2D/Xml/XmlElement.h"

#include "NAS2D/Xml/XmlAttribute.h"
#include "NAS2D/Xml/XmlAttributeSet.h"
#include "NAS2D/Xml/XmlComment.h"
#include "NAS2D/Xml/XmlDocument.h"
#include "NAS2D/Xml/XmlElement.h"
#include "NAS2D/Xml/XmlText.h"
#include "NAS2D/Xml/XmlUnknown.h"

using namespace NAS2D::Xml;

const std::string NAS2D_EMPTY_STR = "";

XmlElement::XmlElement(const std::string& value) : XmlNode(XmlNode::XML_ELEMENT)
{
	_value = value;
}


XmlElement::XmlElement(const XmlElement& copy) : XmlNode(XmlNode::XML_ELEMENT)
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


std::string XmlElement::attribute(const std::string& name) const
{
	const XmlAttribute* node = attributeSet.find(name);
	if (node)
		return node->value();
	return "";
}


std::string XmlElement::attribute(const std::string& name, int& i) const
{
	const XmlAttribute* attrib = attributeSet.find(name);
	std::string result;

	if (attrib)
	{
		result = attrib->value();
		if (i)
		{
			attrib->queryIntValue(i);
		}
	}
	return result;
}


std::string XmlElement::attribute(const std::string& name, double& d) const
{
	const XmlAttribute* attrib = attributeSet.find(name);
	std::string result;

	if (attrib)
	{
		result = attrib->value();
		if (d)
		{
			attrib->queryDoubleValue(d);
		}
	}
	return result;
}


XmlAttribute::QueryResult XmlElement::queryIntAttribute(const std::string& name, int& ival) const
{
	const XmlAttribute* attrib = attributeSet.find(name);
	if (!attrib)
		return XmlAttribute::XML_NO_ATTRIBUTE;

	return attrib->queryIntValue(ival);
}


XmlAttribute::QueryResult XmlElement::queryDoubleAttribute(const std::string& name, double& dval) const
{
	const XmlAttribute* attrib = attributeSet.find(name);
	if (!attrib)
		return XmlAttribute::XML_NO_ATTRIBUTE;

	return attrib->queryDoubleValue(dval);
}


void XmlElement::setAttribute(const std::string& name, int val)
{
	XmlAttribute* attrib = attributeSet.findOrCreate(name);

	if (attrib)
		attrib->intValue(val);
}


void XmlElement::setDoubleAttribute(const std::string& name, double val)
{
	XmlAttribute* attrib = attributeSet.findOrCreate(name);
	if (attrib)
		attrib->doubleValue(val);
}


void XmlElement::setAttribute(const std::string& cname, const std::string& cvalue)
{
	XmlAttribute* attrib = attributeSet.findOrCreate(cname);
	if (attrib)
	{
		attrib->value(cvalue);
	}
}


void XmlElement::write(std::string& buf, int depth) const
{
	for (int i = 0; i < depth; ++i)
		buf += "\t";

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
			if (!node->toText())
				buf += "\n";

			node->write(buf, depth + 1);
		}

		buf += "\n";

		for (int i = 0; i < depth; ++i)
			buf += "\t";

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
		target->setAttribute(attribute->name(), attribute->value());
	}

	XmlNode* node = nullptr;
	for (node = _firstChild; node; node = node->nextSibling())
	{
		target->linkEndChild(node->clone());
	}
}

bool XmlElement::accept(XmlVisitor* visitor) const
{
	if (visitor->visitEnter(*this, attributeSet.first()))
	{
		for (const XmlNode* node = firstChild(); node; node = node->nextSibling())
		{
			if (!node->accept(visitor))
				break;
		}
	}
	return visitor->visitExit(*this);
}


XmlNode* XmlElement::clone() const
{
	XmlElement* clone = new XmlElement(value());
	if (!clone)
		return nullptr;

	copyTo(clone);
	return clone;
}


const std::string& XmlElement::text() const
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
	return NAS2D_EMPTY_STR;
}


void XmlElement::removeAttribute(const std::string& name)
{
	XmlAttribute* node = attributeSet.find(name);
	if (node)
	{
		attributeSet.remove(node);
		delete node;
	}
}


const XmlAttribute* XmlElement::firstAttribute() const
{
	return attributeSet.first();
}


XmlAttribute* XmlElement::firstAttribute()
{
	return attributeSet.first();
}


const XmlAttribute* XmlElement::lastAttribute() const
{
	return attributeSet.last();
}


XmlAttribute* XmlElement::lastAttribute()
{
	return attributeSet.last();
}


XmlAttribute::QueryResult XmlElement::queryFloatAttribute(const std::string& name, float& _value) const
{
	double d = 0;
	auto result = queryDoubleAttribute(name, d);
	if (result == XmlAttribute::XML_SUCCESS)
		_value = static_cast<float>(d);

	return result;
}


XmlAttribute::QueryResult XmlElement::queryStringAttribute(const std::string& name, std::string& _value) const
{
	std::string str = attribute(name);
	if (!str.empty())
	{
		_value = str;
		return XmlAttribute::XML_SUCCESS;
	}
	return XmlAttribute::XML_NO_ATTRIBUTE;
}
