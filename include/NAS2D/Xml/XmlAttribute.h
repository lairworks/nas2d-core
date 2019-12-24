// ==================================================================================
// = NAS2D
// = Copyright © 2008 - 2019 New Age Software
// ==================================================================================
// = NAS2D is distributed under the terms of the zlib license. You are free to copy,
// = modify and distribute the software under the terms of the zlib license.
// =
// = Acknowledgement of your use of NAS2D is appriciated but is not required.
// ==================================================================================
// = Originally based on TinyXML. See Xml.h for additional details.
// ==================================================================================
#pragma once

#include "XmlBase.h"

#include <string>

namespace NAS2D {
namespace Xml {

/**
 * An XmlAttribute is a name-value pair. Elements have an arbitrary number of attributes,
 * each with a unique name.
 */
class XmlAttribute : public XmlBase
{
public:
	enum class QueryResult
	{
		XML_SUCCESS,
		XML_NO_ATTRIBUTE,
		XML_WRONG_TYPE
	};

public:
	XmlAttribute();
	XmlAttribute(const std::string& name, std::string& value);

	const std::string& name() const;
	const std::string& value() const;

	int intValue() const;
	void intValue(int i);

	double doubleValue() const;
	void doubleValue(double d);

	QueryResult queryIntValue(int& i) const;
	QueryResult queryDoubleValue(double& d) const;

	void name(const std::string& name);
	void value(const std::string& value);

	const XmlAttribute* next() const;
	XmlAttribute* next();

	const XmlAttribute* previous() const;
	XmlAttribute* previous();

	bool operator==(const XmlAttribute& rhs) const { return rhs._name == _name; }
	bool operator<(const XmlAttribute& rhs)	 const { return _name < rhs._name; }
	bool operator>(const XmlAttribute& rhs)  const { return _name > rhs._name; }

	bool operator==(const std::string& rhs) const { return rhs == _name; }

	/**
	 * Attribute parsing starts: first letter of the name
	 * returns: the next char after the value end quote
	 */
	virtual const char* parse(const char* p, void* data);

	virtual void write(std::string& buf, int depth) const;

protected:
	friend class XmlElement;
	// Set the document pointer so the attribute can report errors.
	void document(XmlDocument* doc) { _document = doc; }

private:
	friend class XmlAttributeSet;

	XmlAttribute(const XmlAttribute&); // Explicitly disallowed.
	void operator=(const XmlAttribute& base); // Explicitly disallowed.

private:
	XmlDocument*	_document;	/**< Comment Me. */
	std::string		_name;		/**< Comment Me. */
	std::string		_value;		/**< Comment Me. */
	XmlAttribute*	_prev;		/**< Comment Me. */
	XmlAttribute*	_next;		/**< Comment Me. */
};

} // namespace Xml
} // namespace NAS2D
