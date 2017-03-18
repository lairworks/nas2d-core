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

#include "NAS2D/Xml/XmlBase.h"

#include <string>

namespace NAS2D {
namespace Xml {

enum
{
	TIXML_SUCCESS,
	TIXML_NO_ATTRIBUTE,
	TIXML_WRONG_TYPE
};

/**
 * An attribute is a name-value pair. Elements have an arbitrary number of attributes,
 * each with a unique name.
 * 
 * \note	The attributes are not TiXmlNodes, since they are not part of the TinyXML
 *			document object model. There are other suggested ways to look at this problem.
 */
class TiXmlAttribute : public TiXmlBase
{
	friend class TiXmlAttributeSet;

public:
	TiXmlAttribute() : TiXmlBase(), document(nullptr), prev(nullptr), next(nullptr) {}

	/// Construct an attribute with a name and value.
	TiXmlAttribute(const std::string& _name, std::string& _value) : TiXmlBase(), document(nullptr), name(_name), value(_value), prev(nullptr), next(nullptr) {}

	/**
	 * Name of the Attribute.
	 */
	const std::string&	Name()  const { return name; }

	/**
	 * Value of the Attribute.
	 */
	const std::string&	Value() const { return value; }

	int IntValue() const;					///< Return the value of this attribute, converted to an integer.
	double DoubleValue() const;				///< Return the value of this attribute, converted to a double.

	int QueryIntValue(int& _value) const;
	int QueryDoubleValue(double& _value) const;

	void SetName(const std::string& _name) { name = _name; }		///< Set the name of this attribute.
	void SetValue(const std::string& _value) { value = _value; }	///< Set the value.

	void SetIntValue(int _value);									///< Set the value from an integer.
	void SetDoubleValue(double _value);								///< Set the value from a double.

	/// Get the next sibling attribute in the DOM. Returns nullptr at end.
	const TiXmlAttribute* Next() const;
	TiXmlAttribute* Next() { return const_cast<TiXmlAttribute*>((const_cast<const TiXmlAttribute*>(this))->Next()); }

	/// Get the previous sibling attribute in the DOM. Returns nullptr at beginning.
	const TiXmlAttribute* Previous() const;
	TiXmlAttribute* Previous() { return const_cast<TiXmlAttribute*>((const_cast<const TiXmlAttribute*>(this))->Previous()); }

	bool operator==(const TiXmlAttribute& rhs) const { return rhs.name == name; }
	bool operator<(const TiXmlAttribute& rhs)	 const { return name < rhs.name; }
	bool operator>(const TiXmlAttribute& rhs)  const { return name > rhs.name; }

	/**
	 * Attribute parsing starts: first letter of the name
	 * returns: the next char after the value end quote
	 */
	virtual const char* Parse(const char* p, TiXmlParsingData* data, TiXmlEncoding encoding);

	// Prints this Attribute to a FILE stream.
	virtual void Print(std::string& buf, int depth) const;

	// [internal use]
	// Set the document pointer so the attribute can report errors.
	void SetDocument(TiXmlDocument* doc) { document = doc; }

private:
	TiXmlAttribute(const TiXmlAttribute&); // Explicitly disallowed.
	void operator=(const TiXmlAttribute& base); // Explicitly disallowed.

	TiXmlDocument*	document;	/**< Comment Me. */
	std::string		name;		/**< Comment Me. */
	std::string		value;		/**< Comment Me. */
	TiXmlAttribute*	prev;		/**< Comment Me. */
	TiXmlAttribute*	next;		/**< Comment Me. */
};

} // namespace Xml
} // namespace NAS2D
