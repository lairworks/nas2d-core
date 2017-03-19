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

#include "NAS2D/Xml/XmlNode.h"

namespace NAS2D {
namespace Xml {

/**
 * An XmlHandle is a class that wraps a node pointer with null checks; this is
 * an incredibly useful thing. Note that XmlHandle is not part of the TinyXml
 * DOM structure. It is a separate utility class.
 * 
 * Take an example:
 * 
 * \code{.xml}
 * <Document>
 *	<Element attributeA = "valueA">
 *		<Child attributeB = "value1" />
 *		<Child attributeB = "value2" />
 *	</Element>
 * </Document>
 * \endcode
 * 
 * Assuming you want the value of "attributeB" in the 2nd "Child" element, it's very
 * easy to write a *lot* of code that looks like:
 * 
 * \code{.cpp}
 * XmlElement* root = document.firstChildElement("Document");
 * if (root)
 * {
 *	XmlElement* element = root->firstChildElement("Element");
 *	if (element)
 * 	{
 * 		XmlElement* child = element->firstChildElement("Child");
 * 		if (child)
 * 		{
 * 			XmlElement* child2 = child->nextSiblingElement("Child");
 * 			if (child2)
 * 			{
 * 				// Finally do something useful.
 * 			}
 * 		}
 * 	}
 * }
 * \endcode
 *
 * And that doesn't even cover "else" cases. XmlHandle addresses the verbosity
 * of such code. An XmlHandle checks for null pointers so it is perfectly safe
 * and correct to use:
 *
 * \code{.cpp}
 * XmlHandle docHandle(&document);
 * XmlElement* child2 = docHandle.firstChild("Document").firstChild("Element").child("Child", 1).toElement();
 * if (child2)
 * {
 *	// do something usefull
 * }
 * \endcode
 *
 * Which is MUCH more concise and useful.
 *
 * It is also safe to copy handle.
 * \code{.cpp}
 * XmlHandle handleCopy = handle;
 * \endcode
 *
 * What they should not be used for is iteration:
 *
 * \code{.cpp}
 * int i = 0;
 * while (true)
 * {
 *	XmlElement* child = docHandle.firstChild("Document").firstChild("Element").child("Child", i).toElement();
 *	if (!child)
 * 		break;
 * 	// do something
 * 	++i;
 * }
 * \endcode
 *
 * It seems reasonable, but it is in fact two embedded \c while loops. The \c child method is
 * a linear walk to find the element, so this code would iterate much more than it needs
 * to. Instead, use:
 *
 * \code{.cpp}
 * XmlElement* child = docHandle.firstChild("Document").firstChild("Element").firstChild("Child").toElement();
 * for (child; child; child = child->nextSiblingElement())
 * {
 *	// do something
 * }
 * \endcode
*/
class XmlHandle
{
public:
	XmlHandle(XmlNode* _node) { this->node = _node; }
	XmlHandle(const XmlHandle& ref) { this->node = ref.node; }
	XmlHandle operator=(const XmlHandle& ref) { if (&ref != this) this->node = ref.node; return *this; }

	XmlHandle firstChild() const;
	XmlHandle firstChild(const std::string& value) const;
	XmlHandle firstChildElement() const;
	XmlHandle firstChildElement(const std::string& value) const;

	/**
	 * Return a handle to the "index" child with the given name. The first child is 0, the second 1, etc.
	 */
	XmlHandle child(const std::string& value, int index) const;

	/**
	 * Return a handle to the "index" child. The first child is 0, the second 1, etc.
	 */
	XmlHandle child(int index) const;

	/**
	 * Return a handle to the "index" child element with the given name. The first child element is 0, the second 1, etc.
	 * 
	 * \note	Only XmlElements are indexed. Other types are not counted.
	 */
	XmlHandle childElement(const std::string& value, int index) const;

	/**
	 * Return a handle to the "index" child element. The first child element is 0, the second 1, etc.
	 * 
	 * \note	Only XmlElements are indexed. Other types are not counted.
	 */
	XmlHandle childElement(int index) const;

	/**
	 * Return the handle as a XmlNode. This may return null.
	 */
	XmlNode* toNode() const { return node; }

	/**
	 * Return the handle as a XmlElement. This may return null.
	 */
	XmlElement* toElement() const { return ((node && node->toElement()) ? node->toElement() : nullptr); }
	
	/**
	 * Return the handle as a XmlText. This may return null.
	 */
	XmlText* toText() const { return ((node && node->toText()) ? node->toText() : nullptr); }
	
	/**
	 * Return the handle as a XmlUnknown. This may return null.
	 */
	XmlUnknown* toUnknown() const { return ((node && node->toUnknown()) ? node->toUnknown() : nullptr); }

private:
	XmlNode* node;
};

} // namespace Xml
} // namespace NAS2D
