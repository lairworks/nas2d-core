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

#include "XmlNode.h"

#include <string>


namespace NAS2D
{
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
	XmlHandle();
	explicit XmlHandle(XmlNode* _node);
	XmlHandle(const XmlHandle& ref);
	XmlHandle& operator=(const XmlHandle& ref);

	XmlHandle firstChild() const;
	XmlHandle firstChild(const std::string& value) const;
	XmlHandle firstChildElement() const;
	XmlHandle firstChildElement(const std::string& value) const;

	XmlHandle child(const std::string& value, int index) const;
	XmlHandle child(int index) const;

	XmlHandle childElement(const std::string& value, int index) const;
	XmlHandle childElement(int index) const;

	// Cast to type functions.
	XmlNode* toNode() const;
	XmlElement* toElement() const;
	XmlText* toText() const;
	XmlUnknown* toUnknown() const;

private:
	XmlNode* node;
};

} // namespace Xml
} // namespace NAS2D
