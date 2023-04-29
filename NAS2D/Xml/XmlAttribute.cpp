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

using namespace NAS2D::Xml;

XmlAttribute::XmlAttribute() :
	XmlBase(),
	_document(nullptr),
	_name(),
	_value(),
	_prev(nullptr),
	_next(nullptr)
{}


/**
 * Constructs an XmlAttribute with a given name and value.
 *
 * \param	name	Name of the attribute.
 * \param	value	Value of the attribute.
 */
XmlAttribute::XmlAttribute(const std::string& name, std::string& value) :
	XmlBase(),
	_document(nullptr),
	_name(name),
	_value(value),
	_prev(nullptr),
	_next(nullptr)
{}


/**
 * Get the next sibling attribute.
 *
 * \returns	A const pointer to the next sibling attribute or nullptr if at the end.
 */
const XmlAttribute* XmlAttribute::next() const
{
	// We are using knowledge of the sentinel. The sentinel have a value or name.
	if (_next->_value.empty() && _next->_name.empty())
	{
		return nullptr;
	}

	return _next;
}


/**
 * Get the next sibling attribute.
 *
 * \returns	A pointer to the next sibling attribute or nullptr if at the end.
 */
XmlAttribute* XmlAttribute::next()
{
	return const_cast<XmlAttribute*>((const_cast<const XmlAttribute*>(this))->next());
}


/**
 * Get the previous sibling attribute.
 *
 * \returns	A const pointer to the next sibling attribute or nullptr if at the beginning.
 */
const XmlAttribute* XmlAttribute::previous() const
{
	// We are using knowledge of the sentinel. The sentinel have a value or name.
	if (_prev->_value.empty() && _prev->_name.empty())
	{
		return nullptr;
	}

	return _prev;
}


/**
 * Get the previous sibling attribute.
 *
 * \returns	A pointer to the next sibling attribute or nullptr if at the beginning.
 */
XmlAttribute* XmlAttribute::previous()
{
	return const_cast<XmlAttribute*>((const_cast<const XmlAttribute*>(this))->previous());
}


/**
 * \see XmlBase::write()
 */
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


/**
 * Set value to an \c int.
 *
 * \param	i	\c int value to use.
 */
void XmlAttribute::intValue(int i)
{
	value(std::to_string(i));
}


/**
 * Set value to a \c double.
 *
 * \param	d	\c double value to use.
 */
void XmlAttribute::doubleValue(double d)
{
	value(std::to_string(d));
}


/**
 * Gets the value of the attribute as an \c int.
 *
 * \note	This function does not attempt to verify that the value in
 *			the attribute is a valid numeric value that can be converted
 *			to a \c double.
 *
 * \returns	Returns the requested value as an \c int.
 *
 * \throws	Throws \c std::invalid_argument if the value is not a numeric value.
 */
int XmlAttribute::intValue() const
{
	return std::stoi(_value);
}


/**
 * Gets the value of the attribute as a \c double.
 *
 * \note	This function does not attempt to verify that the value in
 *			the attribute is a valid numeric value that can be converted
 *			to a \c double.
 *
 * \returns	Returns the requested value as a \c double.
 *
 * \throws	Throws \c std::invalid_argument if the value is not a numeric value.
 */
double XmlAttribute::doubleValue() const
{
	return std::stod(_value);
}


/**
 * Gets the value of the attribute as an \c int.
 *
 * Similar to XmlAttribute::intValue() but with richer
 * error checking.
 *
 * \param	i	Reference to an \c int to write the value to.
 *
 * \returns	An XmlAttribute::QueryResult value.
 */
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


/**
 * Gets the value of the attribute as a \c double.
 *
 * Similar to XmlAttribute::doubleValue() but with richer
 * error checking.
 *
 * \param	d	Reference to an \c double to write the value to.
 *
 * \returns	An XmlAttribute::QueryResult value.
 */
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


/**
 * Gets the name of the Attribute.
 *
 * \return	Returns a \c std::string containing the name of the attribute.
 */
const std::string& XmlAttribute::name() const
{
	return _name;
}


/**
 * Gets the value of the Attribute.
 *
 * \return	Returns a \c std::string containing the value of the attribute.
 *
 * \see	XmlAttribute::intValue(), XmlAttribute::doubleValue()
 */
const std::string& XmlAttribute::value() const
{
	return _value;
}


/**
 * Sets the name of the Attribute.
 *
 * \param name	A \c std::string containing the name to set.
 */
void XmlAttribute::name(const std::string& name)
{
	_name = name;
}


/**
 * Sets the value of the Attribute.
 *
 * \param value	A \c std::string containing the value to set.
 */
void XmlAttribute::value(const std::string& value)
{
	_value = value;
}
