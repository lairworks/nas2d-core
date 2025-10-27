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
#pragma once

#include "XmlAttribute.h"

#include <string>


namespace NAS2D::Xml
{

/**
 * Used to manage a group of attributes.
 *
 * It is only used internally, both by the ELEMENT and the DECLARATION.
 *
 * The set can be changed transparent to the Element and Declaration
 * classes that use it, but NOT transparent to the Attribute
 * which has to implement a next() and previous() method. Which makes
 * it a bit problematic and prevents the use of STL.
 */
class XmlAttributeSet
{
public:
	XmlAttributeSet();
	XmlAttributeSet(const XmlAttributeSet&) = delete;
	void operator=(const XmlAttributeSet&) = delete;
	~XmlAttributeSet();

	void add(XmlAttribute* attribute);
	void remove(XmlAttribute* attribute);

	const XmlAttribute* first() const { return (sentinel._next == &sentinel) ? nullptr : sentinel._next; }
	XmlAttribute* first() { return (sentinel._next == &sentinel) ? nullptr : sentinel._next; }
	const XmlAttribute* last() const { return (sentinel._prev == &sentinel) ? nullptr : sentinel._prev; }
	XmlAttribute* last() { return (sentinel._prev == &sentinel) ? nullptr : sentinel._prev; }

	XmlAttribute* find(const std::string& _name) const;
	XmlAttribute* findOrCreate(const std::string& _name);

private:
	XmlAttribute sentinel{};
};

}
