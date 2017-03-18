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
#include "NAS2D/Xml/XmlAttribute.h"

using namespace NAS2D::Xml;

const XmlAttribute* XmlAttribute::Next() const
{
	// We are using knowledge of the sentinel. The sentinel have a value or name.
	if (next->value.empty() && next->name.empty())
		return nullptr;

	return next;
}


const XmlAttribute* XmlAttribute::Previous() const
{
	// We are using knowledge of the sentinel. The sentinel have a value or name.
	if (prev->value.empty() && prev->name.empty())
		return nullptr;

	return prev;
}


void XmlAttribute::Print(std::string& buf, int /*depth*/) const
{
	std::string n, v;

	EncodeString(name, n);
	EncodeString(value, v);

	if (value.find('\"') == std::string::npos)
	{
		buf += n;
		buf += "=\"";
		buf += v;
		buf += "\"";
	}
	else
	{
		buf += n;
		buf += "='";
		buf += v;
		buf += "'";
	}
}


int XmlAttribute::QueryIntValue(int& ival) const
{
	try
	{
		ival = std::stoi(value);
	}
	catch (std::invalid_argument)
	{
		return TIXML_WRONG_TYPE;
	}

	return TIXML_SUCCESS;
}


int XmlAttribute::QueryDoubleValue(double& dval) const
{
	try
	{
		dval = std::stod(value);
	}
	catch (std::invalid_argument)
	{
		return TIXML_WRONG_TYPE;
	}

	return TIXML_SUCCESS;
}


void XmlAttribute::SetIntValue(int _value)
{
	SetValue(std::to_string(_value));
}


void XmlAttribute::SetDoubleValue(double _value)
{
	SetValue(std::to_string(_value));
}


int XmlAttribute::IntValue() const
{
	return std::stoi(value);
}


double  XmlAttribute::DoubleValue() const
{
	return std::stof(value);
}
