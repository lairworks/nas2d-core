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

#include "NAS2D/Xml/XmlAttribute.h"

namespace NAS2D {
namespace Xml {

/*	A class used to manage a group of attributes.
It is only used internally, both by the ELEMENT and the DECLARATION.
	
The set can be changed transparent to the Element and Declaration
classes that use it, but NOT transparent to the Attribute
which has to implement a next() and previous() method. Which makes
it a bit problematic and prevents the use of STL.

This version is implemented with circular lists because:
- I like circular lists
- it demonstrates some independence from the (typical) doubly linked list.
*/
class TiXmlAttributeSet
{
public:
	TiXmlAttributeSet();
	~TiXmlAttributeSet();

	void Add(TiXmlAttribute* attribute);
	void Remove(TiXmlAttribute* attribute);

	const TiXmlAttribute* First()	const { return (sentinel.next == &sentinel) ? 0 : sentinel.next; }
	TiXmlAttribute* First() { return (sentinel.next == &sentinel) ? 0 : sentinel.next; }
	const TiXmlAttribute* Last() const { return (sentinel.prev == &sentinel) ? 0 : sentinel.prev; }
	TiXmlAttribute* Last() { return (sentinel.prev == &sentinel) ? 0 : sentinel.prev; }

	TiXmlAttribute*	Find(const std::string& _name) const;
	TiXmlAttribute* FindOrCreate(const std::string& _name);


private:
	TiXmlAttributeSet(const TiXmlAttributeSet&); // Explicitly disallowed.
	void operator=(const TiXmlAttributeSet&); // Explicitly disallowed.

	TiXmlAttribute sentinel;	/**< Comment me. */
};

} // namespace Xml
} // namespace NAS2D
