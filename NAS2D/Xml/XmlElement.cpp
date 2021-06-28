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
#include "XmlElement.h"
#include "XmlText.h"
#include "XmlUnknown.h"

using namespace NAS2D::Xml;

const std::string NAS2D_EMPTY_STR = "";

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

std::string XmlElement::attribute(const std::string& name) const
{
	const XmlAttribute* node = attributeSet.find(name);
	if (node)
	{
		return node->value();
	}

	return "";
}

void XmlElement::attribute(const std::string& name, int i)
{
	XmlAttribute* attrib = attributeSet.findOrCreate(name);

	if (attrib)
	{
		attrib->intValue(i);
	}
}

void XmlElement::attribute(const std::string& name, double d)
{
	XmlAttribute* attrib = attributeSet.findOrCreate(name);
	if (attrib)
	{
		attrib->doubleValue(d);
	}
}

void XmlElement::attribute(const std::string& name, const std::string& s)
{
	XmlAttribute* attrib = attributeSet.findOrCreate(name);
	if (attrib)
	{
		attrib->value(s);
	}
}

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
	XmlNode::copyTo(target);

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
