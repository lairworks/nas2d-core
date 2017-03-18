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
#include "NAS2D/Xml/XmlVisitor.h"

namespace NAS2D {
namespace Xml {

/**
 * The parent class for everything in the DOM (Except for attributes).
 * 
 * Nodes have siblings, a parent, and children. A node can be in a document, or stand
 * on its own. The type of a XmlNode can be queried, and it can be cast to its more
 * defined type.
 */
class XmlNode : public XmlBase
{
	friend class XmlDocument;
	friend class XmlElement;

public:
	/**
	 * The types of XML nodes supported by TinyXml. (All the unsupported
	 * types are picked up by UNKNOWN.)
	 */
	enum NodeType
	{
		TINYXML_DOCUMENT,
		TINYXML_ELEMENT,
		TINYXML_COMMENT,
		TINYXML_UNKNOWN,
		TINYXML_TEXT,
		TINYXML_TYPECOUNT
	};

	XmlNode() : XmlBase(), parent(nullptr), type(TINYXML_UNKNOWN), firstChild(nullptr), lastChild(nullptr), prev(nullptr), next(nullptr) {}
	virtual ~XmlNode();

	/**
	 * The meaning of 'value' changes for the specific type of XmlNode.
	 * 
	 * @verbatim
	 * Document:	filename of the xml file
	 * Element:		name of the element
	 * Comment:		the comment text
	 * Unknown:		the tag contents
	 * Text:		the text string
	 * @endverbatim
	 * 
	 * The subclasses will wrap this function.
	*/
	const std::string& Value() const { return value; }

	/**
	 * Changes the value of the node. Defined as:
	 * @verbatim
	 * Document:	filename of the xml file
	 * Element:	name of the element
	 * Comment:	the comment text
	 * Unknown:	the tag contents
	 * Text:		the text string
	 * @endverbatim
	 */
	void SetValue(const std::string& _value) { value = _value; }

	/// Delete all the children of this node. Does not affect 'this'.
	void Clear();

	/**
	 * One step up in the DOM.
	 */
	XmlNode* Parent() { return parent; }
	const XmlNode* Parent() const { return parent; }

	/**
	 * The first child of this node. Will be \c nullptr if there are no children.
	 */
	const XmlNode* FirstChild()	const { return firstChild; }
	XmlNode* FirstChild() { return firstChild; }

	/**
	 * The first child of this node with the matching 'value'. Will be \c nullptr if none found.
	 */
	const XmlNode* FirstChild(const std::string& value) const;
	XmlNode* FirstChild(const std::string& _value) { return const_cast<XmlNode*> ((const_cast<const XmlNode*>(this))->FirstChild(_value)); }

	/**
	 * The last child of this node. Will be \c nullptr if there are no children.
	 */
	const XmlNode* LastChild() const { return lastChild; }
	XmlNode* LastChild() { return lastChild; }

	/**
	 * The last child of this node matching 'value'. Will be \c nullptr if there are no children.
	 */
	const XmlNode* LastChild(const std::string& value) const;
	XmlNode* LastChild(const std::string& _value) { return const_cast<XmlNode*> ((const_cast<const XmlNode*>(this))->LastChild(_value)); }

	/**
	 * An alternate way to walk the children of a node.
	 * 
	 * One way to iterate over nodes is:
	 * \code{.cpp}
	 * for(child = parent->FirstChild(); child; child = child->NextSibling())
	 * \endcode
	 * 
	 * IterateChildren does the same thing with the syntax:
	 * \code{.cpp}
	 * child = nullptr;
	 * while(child = parent->IterateChildren(child))
	 * \endcode
	 * 
	 * IterateChildren takes the previous child as input and finds the next one. If the previous
	 * child is \c nullptr, it returns thefirst. IterateChildren will return \c nullptr when done.
	 */
	const XmlNode* IterateChildren(const XmlNode* previous) const;
	XmlNode* IterateChildren(const XmlNode* previous) { return const_cast<XmlNode*>((const_cast<const XmlNode*>(this))->IterateChildren(previous)); }

	/**
	 * Search for children with a particular 'value'.
	 */
	const XmlNode* IterateChildren(const std::string& value, const XmlNode* previous) const;
	XmlNode* IterateChildren(const std::string& _value, const XmlNode* previous) { return const_cast<XmlNode*>((const_cast<const XmlNode*>(this))->IterateChildren(_value, previous)); }

	/**
	 * Add a new node related to this. Adds a child past the LastChild.
	 * Returns a pointer to the new object or NULL if an error occured.
	 */
	XmlNode* InsertEndChild(const XmlNode& addThis);


	/**
	 * Add a new node related to this. Adds a child past the LastChild.
	 * 
	 * \note The node to be added is passed by pointer, and will be henceforth owned
	 *		 by the Xml library.
	 * 
	 * \see InsertEndChild
	 */
	XmlNode* LinkEndChild(XmlNode* addThis);

	/**
	 * Add a new node related to this. Adds a child before the specified child.
	 * 
	 * \return A pointer to the new object or nullptr if an error occured.
	 */
	XmlNode* InsertBeforeChild(XmlNode* beforeThis, const XmlNode& addThis);

	/**
	 * Add a new node related to this. Adds a child after the specified child.
	 * Returns a pointer to the new object or nullptr if an error occured.
	 */
	XmlNode* InsertAfterChild(XmlNode* afterThis, const XmlNode& addThis);

	/**
	 * Replace a child of this node.
	 * 
	 * \return A pointer to the new object or \c nullptr if an error occured.
	 */
	XmlNode* ReplaceChild(XmlNode* replaceThis, const XmlNode& withThis);

	/**
	 * Delete a child of this node.
	 */
	bool RemoveChild(XmlNode* removeThis);

	/**
	 * Navigate to a sibling node.
	 */
	const XmlNode* PreviousSibling() const { return prev; }
	XmlNode* PreviousSibling() { return prev; }

	/**
	 * Navigate to a sibling node.
	 */
	const XmlNode* PreviousSibling(const std::string&) const;
	XmlNode* PreviousSibling(const std::string& _prev) { return const_cast<XmlNode*>((const_cast<const XmlNode*>(this))->PreviousSibling(_prev)); }

	/**
	 * Navigate to a sibling node.
	 */
	const XmlNode* NextSibling() const { return next; }
	XmlNode* NextSibling() { return next; }

	/**
	 * Navigate to a sibling node with a given value.
	 */
	const XmlNode* NextSibling(const std::string&) const;
	XmlNode* NextSibling(const std::string& _next) { return const_cast<XmlNode*>((const_cast<const XmlNode*>(this))->NextSibling(_next)); }

	/**
	 * Convenience function to get through elements.
	 * Calls NextSibling and ToElement. Will skip all non-Element nodes.
	 * 
	 * \return \c nullptr if there is not another element.
	 */
	const XmlElement* NextSiblingElement() const;
	XmlElement* NextSiblingElement() { return const_cast<XmlElement*>((const_cast<const XmlNode*>(this))->NextSiblingElement()); }

	/**
	 * Convenience function to get through elements.
	 * Calls NextSibling and ToElement. Will skip all non-Element nodes.
	 * 
	 * \return \c nullptr if there is not another element.
	 */
	const XmlElement* NextSiblingElement(const std::string&) const;
	XmlElement* NextSiblingElement(const std::string& _next) { return const_cast<XmlElement*>((const_cast<const XmlNode*>(this))->NextSiblingElement(_next)); }

	/**
	 * Convenience function to get through elements.
	 */
	const XmlElement* FirstChildElement()	const;
	XmlElement* FirstChildElement() { return const_cast<XmlElement*>((const_cast<const XmlNode*>(this))->FirstChildElement()); }

	/**
	 * Convenience function to get through elements.
	 */
	const XmlElement* FirstChildElement(const std::string& _value) const;
	XmlElement* FirstChildElement(const std::string& _value) { return const_cast<XmlElement*>((const_cast<const XmlNode*>(this))->FirstChildElement(_value)); }

	/**
	 * Query the type (as an enumerated value, above) of this node. The possible
	 * types are: TINYXML_DOCUMENT, TINYXML_ELEMENT, TINYXML_COMMENT, TINYXML_UNKNOWN,
	 * TINYXML_TEXT, and TINYXML_DECLARATION.
	 */
	int Type() const { return type; }

	/**
	 * Return a pointer to the Document this node lives in. Returns null if not in a document.
	 */
	const XmlDocument* GetDocument() const;
	XmlDocument* GetDocument() { return const_cast<XmlDocument*>((const_cast<const XmlNode*>(this))->GetDocument()); }

	/**
	 * Returns true if this node has no children.
	 */
	bool NoChildren() const { return firstChild == nullptr; }

	virtual const XmlDocument* ToDocument() const { return nullptr; }
	virtual const XmlElement* ToElement() const { return nullptr; }
	virtual const XmlComment* ToComment() const { return nullptr; }
	virtual const XmlUnknown* ToUnknown() const { return nullptr; }
	virtual const XmlText* ToText() const { return nullptr; }

	virtual XmlDocument* ToDocument() { return nullptr; }
	virtual XmlElement* ToElement() { return nullptr; }
	virtual XmlComment* ToComment() { return nullptr; }
	virtual XmlUnknown* ToUnknown() { return nullptr; }
	virtual XmlText* ToText() { return nullptr; }

	/**
	 * Create an exact duplicate of this node and return it. The memory must be deleted by the caller.
	 */
	virtual XmlNode* Clone() const = 0;

	/**
	 * Accept a hierchical visit the nodes in the TinyXML DOM. Every node in the  XML tree will be
	 * conditionally visited and the host will be called back via the TiXmlVisitor interface.
	 */
	virtual bool Accept(XmlVisitor* visitor) const = 0;

	// The real work of the input operator.
	virtual void StreamIn(std::istream& in, std::string& tag) = 0;

protected:
	XmlNode(NodeType _type) : XmlBase(), parent(nullptr), type(_type), firstChild(nullptr), lastChild(nullptr), prev(nullptr), next(nullptr) {}

	/**
	 * Copy to the allocated object. Shared functionality between Clone,
	 * Copy constructor, and the copy assignment operator.
	 */
	void CopyTo(XmlNode* target) const;

	/**
	 * Figure out what is at *p, and parse it. Returns null if it is not an xml node.
	 */
	XmlNode* Identify(const char* start, XmlEncoding encoding);

	XmlNode*		parent;			/**< Parent of the XmlNode. */
	NodeType		type;			/**< Type of the XmlNode. */

	XmlNode*		firstChild;		/**< First child of the XmlNode. */
	XmlNode*		lastChild;		/**< Last child of the XmlNode. */

	std::string		value;			/**< Value of the XmlNode. */

	XmlNode*		prev;			/**< Previous XmlNode. */
	XmlNode*		next;			/**< Next XmlNode. */

private:
	XmlNode(const XmlNode&); // Explicitly disallowed.
	void operator=(const XmlNode& base); // Explicitly disallowed.
};


} // namespace Xml
} // namespace NAS2D
