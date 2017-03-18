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

XmlElement::XmlElement(const std::string& _value) : XmlNode(XmlNode::XML_ELEMENT)
{
	value = _value;
}


XmlElement::XmlElement(const XmlElement& copy) : XmlNode(XmlNode::XML_ELEMENT)
{
	copy.CopyTo(this);
}


XmlElement& XmlElement::operator=(const XmlElement& base)
{
	ClearThis();
	base.CopyTo(this);
	return *this;
}


XmlElement::~XmlElement()
{
	ClearThis();
}


void XmlElement::ClearThis()
{
	Clear();
	while (attributeSet.First())
	{
		XmlAttribute* node = attributeSet.First();
		attributeSet.Remove(node);
		delete node;
	}
}


std::string XmlElement::Attribute(const std::string& name) const
{
	const XmlAttribute* node = attributeSet.Find(name);
	if (node)
		return node->Value();
	return "";
}


std::string XmlElement::Attribute(const std::string& name, int& i) const
{
	const XmlAttribute* attrib = attributeSet.Find(name);
	std::string result;

	if (attrib)
	{
		result = attrib->Value();
		if (i)
		{
			attrib->QueryIntValue(i);
		}
	}
	return result;
}


std::string XmlElement::Attribute(const std::string& name, double& d) const
{
	const XmlAttribute* attrib = attributeSet.Find(name);
	std::string result;

	if (attrib)
	{
		result = attrib->Value();
		if (d)
		{
			attrib->QueryDoubleValue(d);
		}
	}
	return result;
}


int XmlElement::QueryIntAttribute(const std::string& name, int& ival) const
{
	const XmlAttribute* attrib = attributeSet.Find(name);
	if (!attrib)
		return XML_NO_ATTRIBUTE;

	return attrib->QueryIntValue(ival);
}


int XmlElement::QueryDoubleAttribute(const std::string& name, double& dval) const
{
	const XmlAttribute* attrib = attributeSet.Find(name);
	if (!attrib)
		return XML_NO_ATTRIBUTE;

	return attrib->QueryDoubleValue(dval);
}


void XmlElement::SetAttribute(const std::string& name, int val)
{
	XmlAttribute* attrib = attributeSet.FindOrCreate(name);

	if (attrib)
		attrib->SetIntValue(val);
}


void XmlElement::SetDoubleAttribute(const std::string& name, double val)
{
	XmlAttribute* attrib = attributeSet.FindOrCreate(name);
	if (attrib)
		attrib->SetDoubleValue(val);
}


void XmlElement::SetAttribute(const std::string& cname, const std::string& cvalue)
{
	XmlAttribute* attrib = attributeSet.FindOrCreate(cname);
	if (attrib)
	{
		attrib->SetValue(cvalue);
	}
}


void XmlElement::Print(std::string& buf, int depth) const
{
	for (int i = 0; i < depth; ++i)
		buf += "\t";

	buf += "<" + value;

	const XmlAttribute* attrib;
	for (attrib = attributeSet.First(); attrib; attrib = attrib->Next())
	{
		buf += " ";
		attrib->Print(buf, depth);
	}

	XmlNode* node;
	if (!firstChild)
	{
		buf += " />";
	}
	else if (firstChild == lastChild && firstChild->ToText())
	{
		buf += ">";
		firstChild->Print(buf, depth + 1);
		buf += "</" + value + ">";
	}
	else
	{
		buf += ">";

		for (node = firstChild; node; node = node->NextSibling())
		{
			if (!node->ToText())
				buf += "\n";

			node->Print(buf, depth + 1);
		}

		buf += "\n";

		for (int i = 0; i < depth; ++i)
			buf += "\t";

		buf += "</" + value + ">";
	}
}


void XmlElement::CopyTo(XmlElement* target) const
{
	// superclass:
	XmlNode::CopyTo(target);

	// Element class: 
	// Clone the attributes, then clone the children.
	const XmlAttribute* attribute = nullptr;
	for (attribute = attributeSet.First(); attribute; attribute = attribute->Next())
	{
		target->SetAttribute(attribute->Name(), attribute->Value());
	}

	XmlNode* node = nullptr;
	for (node = firstChild; node; node = node->NextSibling())
	{
		target->LinkEndChild(node->Clone());
	}
}

bool XmlElement::Accept(XmlVisitor* visitor) const
{
	if (visitor->VisitEnter(*this, attributeSet.First()))
	{
		for (const XmlNode* node = FirstChild(); node; node = node->NextSibling())
		{
			if (!node->Accept(visitor))
				break;
		}
	}
	return visitor->VisitExit(*this);
}


XmlNode* XmlElement::Clone() const
{
	XmlElement* clone = new XmlElement(Value());
	if (!clone)
		return nullptr;

	CopyTo(clone);
	return clone;
}


const std::string& XmlElement::GetText() const
{
	const XmlNode* child = this->FirstChild();
	if (child)
	{
		const XmlText* childText = child->ToText();
		if (childText)
		{
			return childText->Value();
		}
	}
	return NAS2D_EMPTY_STR;
}


void XmlElement::RemoveAttribute(const std::string& name)
{
	XmlAttribute* node = attributeSet.Find(name);
	if (node)
	{
		attributeSet.Remove(node);
		delete node;
	}
}


const XmlAttribute* XmlElement::FirstAttribute() const
{
	return attributeSet.First();
}


XmlAttribute* XmlElement::FirstAttribute()
{
	return attributeSet.First();
}


const XmlAttribute* XmlElement::LastAttribute() const
{
	return attributeSet.Last();
}


XmlAttribute* XmlElement::LastAttribute()
{
	return attributeSet.Last();
}


int XmlElement::QueryFloatAttribute(const std::string& name, float& _value) const
{
	double d = 0;
	int result = QueryDoubleAttribute(name, d);
	if (result == XML_SUCCESS)
		_value = static_cast<float>(d);

	return result;
}


int XmlElement::QueryStringAttribute(const std::string& name, std::string& _value) const
{
	std::string str = Attribute(name);
	if (!str.empty())
	{
		_value = str;
		return XML_SUCCESS;
	}
	return XML_NO_ATTRIBUTE;
}
