// ==================================================================================
// = NAS2D
// = Copyright � 2008 - 2017 New Age Software
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
 * XmlElement* root = document.FirstChildElement( "Document" );
 * if (root)
 * {
 *	XmlElement* element = root->FirstChildElement( "Element" );
 *	if (element)
 * 	{
 * 		XmlElement* child = element->FirstChildElement( "Child" );
 * 		if ( child )
 * 		{
 * 			XmlElement* child2 = child->NextSiblingElement( "Child" );
 * 			if ( child2 )
 * 			{
 * 				// Finally do something useful.
 * 			}
 * 		}
 * 	}
 * }
 * \endcode
 *
 * And that doesn't even cover "else" cases. XmlHandle addresses the verbosity
 * of such code. A XmlHandle checks for nullpointers so it is perfectly safe
 * and correct to use:
 *
 * \code{.cpp}
 * XmlHandle docHandle( &document );
 * XmlElement* child2 = docHandle.FirstChild( "Document" ).FirstChild( "Element" ).Child( "Child", 1 ).ToElement();
 * if ( child2 )
 * {
 *	// do something usefull
 * }
 * \endcode
 *
 * Which is MUCH more concise and useful.
 *
 * It is also safe to copy handles - internally they are nothing more than node pointers.
 * \code{.cpp{
 * XmlHandle handleCopy = handle;
 * \endcode
 *
 * What they should not be used for is iteration:
 *
 * \code{.cpp}
 * int i = 0;
 * while (true)
 * {
 *	XmlElement* child = docHandle.FirstChild( "Document" ).FirstChild( "Element" ).Child( "Child", i ).ToElement();
 *	if (!child)
 * 		break;
 * 	// do something
 * 	++i;
 * }
 * \endcode
 *
 * It seems reasonable, but it is in fact two embedded \c while loops. The Child method is
 * a linear walk to find the element, so this code would iterate much more than it needs
 * to. Instead, prefer:
 *
 * \code{.cpp}
 * XmlElement* child = docHandle.FirstChild("Document").FirstChild("Element").FirstChild("Child").ToElement();
 * for (child; child; child = child->NextSiblingElement())
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

	XmlHandle FirstChild() const;
	XmlHandle FirstChild(const std::string& value) const;
	XmlHandle FirstChildElement() const;
	XmlHandle FirstChildElement(const std::string& value) const;

	/**
	 * Return a handle to the "index" child with the given name. The first child is 0, the second 1, etc.
	 */
	XmlHandle Child(const std::string& value, int index) const;

	/**
	 * Return a handle to the "index" child. The first child is 0, the second 1, etc.
	 */
	XmlHandle Child(int index) const;

	/**
	 * Return a handle to the "index" child element with the given name. The first child element is 0, the second 1, etc.
	 * 
	 * \note	Only XmlElements are indexed. Other types are not counted.
	 */
	XmlHandle ChildElement(const std::string& value, int index) const;

	/**
	 * Return a handle to the "index" child element. The first child element is 0, the second 1, etc.
	 * 
	 * \note	Only XmlElements are indexed. Other types are not counted.
	 */
	XmlHandle ChildElement(int index) const;

	/**
	 * Return the handle as a XmlNode. This may return null.
	 */
	XmlNode* ToNode() const { return node; }

	/**
	 * Return the handle as a XmlElement. This may return null.
	 */
	XmlElement* ToElement() const { return ((node && node->ToElement()) ? node->ToElement() : 0); }
	
	/**
	 * Return the handle as a XmlText. This may return null.
	 */
	XmlText* ToText() const { return ((node && node->ToText()) ? node->ToText() : 0); }
	
	/**
	 * Return the handle as a XmlUnknown. This may return null.
	 */
	XmlUnknown* ToUnknown() const { return ((node && node->ToUnknown()) ? node->ToUnknown() : 0); }

private:
	XmlNode* node;
};

} // namespace Xml
} // namespace NAS2D
