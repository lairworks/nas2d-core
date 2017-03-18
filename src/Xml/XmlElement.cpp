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

TiXmlElement::TiXmlElement(const std::string& _value) : TiXmlNode(TiXmlNode::TINYXML_ELEMENT)
{
	value = _value;
}


TiXmlElement::TiXmlElement(const TiXmlElement& copy) : TiXmlNode(TiXmlNode::TINYXML_ELEMENT)
{
	copy.CopyTo(this);
}


TiXmlElement& TiXmlElement::operator=(const TiXmlElement& base)
{
	ClearThis();
	base.CopyTo(this);
	return *this;
}


TiXmlElement::~TiXmlElement()
{
	ClearThis();
}


void TiXmlElement::ClearThis()
{
	Clear();
	while (attributeSet.First())
	{
		TiXmlAttribute* node = attributeSet.First();
		attributeSet.Remove(node);
		delete node;
	}
}


std::string TiXmlElement::Attribute(const std::string& name) const
{
	const TiXmlAttribute* node = attributeSet.Find(name);
	if (node)
		return node->Value();
	return "";
}


std::string TiXmlElement::Attribute(const std::string& name, int& i) const
{
	const TiXmlAttribute* attrib = attributeSet.Find(name);
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


std::string TiXmlElement::Attribute(const std::string& name, double& d) const
{
	const TiXmlAttribute* attrib = attributeSet.Find(name);
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


int TiXmlElement::QueryIntAttribute(const std::string& name, int& ival) const
{
	const TiXmlAttribute* attrib = attributeSet.Find(name);
	if (!attrib)
		return TIXML_NO_ATTRIBUTE;

	return attrib->QueryIntValue(ival);
}


int TiXmlElement::QueryDoubleAttribute(const std::string& name, double& dval) const
{
	const TiXmlAttribute* attrib = attributeSet.Find(name);
	if (!attrib)
		return TIXML_NO_ATTRIBUTE;

	return attrib->QueryDoubleValue(dval);
}


void TiXmlElement::SetAttribute(const std::string& name, int val)
{
	TiXmlAttribute* attrib = attributeSet.FindOrCreate(name);

	if (attrib)
		attrib->SetIntValue(val);
}


void TiXmlElement::SetDoubleAttribute(const std::string& name, double val)
{
	TiXmlAttribute* attrib = attributeSet.FindOrCreate(name);
	if (attrib)
		attrib->SetDoubleValue(val);
}


void TiXmlElement::SetAttribute(const std::string& cname, const std::string& cvalue)
{
	TiXmlAttribute* attrib = attributeSet.FindOrCreate(cname);
	if (attrib)
	{
		attrib->SetValue(cvalue);
	}
}


void TiXmlElement::Print(std::string& buf, int depth) const
{
	for (int i = 0; i < depth; ++i)
		buf += "\t";

	buf += "<" + value;

	const TiXmlAttribute* attrib;
	for (attrib = attributeSet.First(); attrib; attrib = attrib->Next())
	{
		buf += " ";
		attrib->Print(buf, depth);
	}

	TiXmlNode* node;
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


void TiXmlElement::CopyTo(TiXmlElement* target) const
{
	// superclass:
	TiXmlNode::CopyTo(target);

	// Element class: 
	// Clone the attributes, then clone the children.
	const TiXmlAttribute* attribute = nullptr;
	for (attribute = attributeSet.First(); attribute; attribute = attribute->Next())
	{
		target->SetAttribute(attribute->Name(), attribute->Value());
	}

	TiXmlNode* node = nullptr;
	for (node = firstChild; node; node = node->NextSibling())
	{
		target->LinkEndChild(node->Clone());
	}
}

bool TiXmlElement::Accept(XmlVisitor* visitor) const
{
	if (visitor->VisitEnter(*this, attributeSet.First()))
	{
		for (const TiXmlNode* node = FirstChild(); node; node = node->NextSibling())
		{
			if (!node->Accept(visitor))
				break;
		}
	}
	return visitor->VisitExit(*this);
}


TiXmlNode* TiXmlElement::Clone() const
{
	TiXmlElement* clone = new TiXmlElement(Value());
	if (!clone)
		return nullptr;

	CopyTo(clone);
	return clone;
}


const std::string& TiXmlElement::GetText() const
{
	const TiXmlNode* child = this->FirstChild();
	if (child)
	{
		const TiXmlText* childText = child->ToText();
		if (childText)
		{
			return childText->Value();
		}
	}
	return NAS2D_EMPTY_STR;
}


void TiXmlElement::RemoveAttribute(const std::string& name)
{
	TiXmlAttribute* node = attributeSet.Find(name);
	if (node)
	{
		attributeSet.Remove(node);
		delete node;
	}
}


const TiXmlAttribute* TiXmlElement::FirstAttribute() const
{
	return attributeSet.First();
}


TiXmlAttribute* TiXmlElement::FirstAttribute()
{
	return attributeSet.First();
}


const TiXmlAttribute* TiXmlElement::LastAttribute() const
{
	return attributeSet.Last();
}


TiXmlAttribute* TiXmlElement::LastAttribute()
{
	return attributeSet.Last();
}


int TiXmlElement::QueryFloatAttribute(const std::string& name, float& _value) const
{
	double d = 0;
	int result = QueryDoubleAttribute(name, d);
	if (result == TIXML_SUCCESS)
		_value = static_cast<float>(d);

	return result;
}


int TiXmlElement::QueryStringAttribute(const std::string& name, std::string& _value) const
{
	std::string str = Attribute(name);
	if (!str.empty())
	{
		_value = str;
		return TIXML_SUCCESS;
	}
	return TIXML_NO_ATTRIBUTE;
}
