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

#include "XmlAttribute.h"
#include <stdexcept>

using namespace std;
using namespace NAS2D::Xml;

XmlAttribute::XmlAttribute() :
	XmlBase(),
	_document(nullptr),
	_prev(nullptr),
	_next(nullptr)
{}

XmlAttribute::XmlAttribute(const std::string &name, std::string &value) :
	XmlBase(),
	_document(nullptr),
	_name(name),
	_value(value),
	_prev(nullptr),
	_next(nullptr)
{}

const XmlAttribute* XmlAttribute::next() const
{
	if (_next->_value.empty() && _next->_name.empty())
	{
		return nullptr;
	}

	return _next;
}

XmlAttribute* XmlAttribute::next()
{
	return const_cast<XmlAttribute*>((const_cast<const XmlAttribute*>(this))->next());
}

const XmlAttribute* XmlAttribute::previous() const
{
	if (_prev->_value.empty() && _prev->_name.empty())
	{
		return nullptr;
	}

	return _prev;
}

XmlAttribute* XmlAttribute::previous()
{
	return const_cast<XmlAttribute*>((const_cast<const XmlAttribute*>(this))->previous());
}

void XmlAttribute::write(std::string& buf, int) const
{
	if (_value.find('\"') == std::string::npos)
	{
		buf += _name;
		buf += "=\"";
		buf += _value;
		buf += "\"";
	}
	else
	{
		buf += _name;
		buf += "='";
		buf += _value;
		buf += "'";
	}
}

void XmlAttribute::intValue(int i)
{
	value(std::to_string(i));
}

void XmlAttribute::doubleValue(double d)
{
	value(std::to_string(d));
}

int XmlAttribute::intValue() const
{
	return std::stoi(_value);
}

double XmlAttribute::doubleValue() const
{
	return std::stod(_value);
}

XmlAttribute::QueryResult XmlAttribute::queryIntValue(int& i) const
{
	try
	{
		i = std::stoi(_value);
	}
	catch (const std::invalid_argument&)
	{
		return QueryResult::XML_WRONG_TYPE;
	}

	return QueryResult::XML_SUCCESS;
}

XmlAttribute::QueryResult XmlAttribute::queryDoubleValue(double& d) const
{
	try
	{
		d = std::stod(_value);
	}
	catch (const std::invalid_argument&)
	{
		return QueryResult::XML_WRONG_TYPE;
	}

	return QueryResult::XML_SUCCESS;
}

const std::string& XmlAttribute::name() const
{
	return _name;
}

const std::string& XmlAttribute::value() const
{
	return _value;
}

void XmlAttribute::name(const std::string& name)
{
	_name = name;
}

void XmlAttribute::value(const std::string& value)
{
	_value = value;
}
