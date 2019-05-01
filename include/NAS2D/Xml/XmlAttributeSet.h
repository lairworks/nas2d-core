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

#include "NAS2D/Xml/XmlAttribute.h"

namespace NAS2D {
namespace Xml {

/**
 * \class XmlAttributeSet
 * \brief Used to manage a group of attributes.
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
	~XmlAttributeSet();

	void add(XmlAttribute* attribute);
	void remove(XmlAttribute* attribute);

	const XmlAttribute* first()	const { return (sentinel._next == &sentinel) ? nullptr : sentinel._next; }
	XmlAttribute* first() { return (sentinel._next == &sentinel) ? nullptr : sentinel._next; }
	const XmlAttribute* last() const { return (sentinel._prev == &sentinel) ? nullptr : sentinel._prev; }
	XmlAttribute* last() { return (sentinel._prev == &sentinel) ? nullptr : sentinel._prev; }

	XmlAttribute* find(const std::string& _name) const;
	XmlAttribute* findOrCreate(const std::string& _name);


private:
	XmlAttributeSet(const XmlAttributeSet&); // Explicitly disallowed.
	void operator=(const XmlAttributeSet&); // Explicitly disallowed.

	XmlAttribute sentinel;	/**< Comment me. */
};

} // namespace Xml
} // namespace NAS2D
