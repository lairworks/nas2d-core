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
class TiXmlNode : public TiXmlBase
{
	friend class TiXmlDocument;
	friend class TiXmlElement;

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
		TINYXML_DECLARATION,
		TINYXML_TYPECOUNT
	};

	TiXmlNode() : TiXmlBase(), parent(nullptr), type(TINYXML_UNKNOWN), firstChild(nullptr), lastChild(nullptr), prev(nullptr), next(nullptr) {}
	virtual ~TiXmlNode();

	/**
	 * The meaning of 'value' changes for the specific type of TiXmlNode.
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
	TiXmlNode* Parent() { return parent; }
	const TiXmlNode* Parent() const { return parent; }

	/**
	 * The first child of this node. Will be \c nullptr if there are no children.
	 */
	const TiXmlNode* FirstChild()	const { return firstChild; }
	TiXmlNode* FirstChild() { return firstChild; }

	/**
	 * The first child of this node with the matching 'value'. Will be \c nullptr if none found.
	 */
	const TiXmlNode* FirstChild(const std::string& value) const;
	TiXmlNode* FirstChild(const std::string& _value) { return const_cast<TiXmlNode*> ((const_cast<const TiXmlNode*>(this))->FirstChild(_value)); }

	/**
	 * The last child of this node. Will be \c nullptr if there are no children.
	 */
	const TiXmlNode* LastChild() const { return lastChild; }
	TiXmlNode* LastChild() { return lastChild; }

	/**
	 * The last child of this node matching 'value'. Will be \c nullptr if there are no children.
	 */
	const TiXmlNode* LastChild(const std::string& value) const;
	TiXmlNode* LastChild(const std::string& _value) { return const_cast<TiXmlNode*> ((const_cast<const TiXmlNode*>(this))->LastChild(_value)); }

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
	const TiXmlNode* IterateChildren(const TiXmlNode* previous) const;
	TiXmlNode* IterateChildren(const TiXmlNode* previous) { return const_cast<TiXmlNode*>((const_cast<const TiXmlNode*>(this))->IterateChildren(previous)); }

	/**
	 * Search for children with a particular 'value'.
	 */
	const TiXmlNode* IterateChildren(const std::string& value, const TiXmlNode* previous) const;
	TiXmlNode* IterateChildren(const std::string& _value, const TiXmlNode* previous) { return const_cast<TiXmlNode*>((const_cast<const TiXmlNode*>(this))->IterateChildren(_value, previous)); }

	/**
	 * Add a new node related to this. Adds a child past the LastChild.
	 * Returns a pointer to the new object or NULL if an error occured.
	 */
	TiXmlNode* InsertEndChild(const TiXmlNode& addThis);


	/**
	 * Add a new node related to this. Adds a child past the LastChild.
	 * 
	 * \note The node to be added is passed by pointer, and will be henceforth owned
	 *		 (and deleted) by TinyXML. This method is efficient and avoids an extra
	 *		 copy, but should be used with care as it uses a different memory model
	 *		 than the other insert functions.
	 * 
	 * \see InsertEndChild
	 */
	TiXmlNode* LinkEndChild(TiXmlNode* addThis);

	/**
	 * Add a new node related to this. Adds a child before the specified child.
	 * Returns a pointer to the new object or nullptr if an error occured.
	 */
	TiXmlNode* InsertBeforeChild(TiXmlNode* beforeThis, const TiXmlNode& addThis);

	/**
	 * Add a new node related to this. Adds a child after the specified child.
	 * Returns a pointer to the new object or nullptr if an error occured.
	 */
	TiXmlNode* InsertAfterChild(TiXmlNode* afterThis, const TiXmlNode& addThis);

	/**
	 * Replace a child of this node.
	 * 
	 * \return A pointer to the new object or \c nullptr if an error occured.
	 */
	TiXmlNode* ReplaceChild(TiXmlNode* replaceThis, const TiXmlNode& withThis);

	/**
	 * Delete a child of this node.
	 */
	bool RemoveChild(TiXmlNode* removeThis);

	/**
	 * Navigate to a sibling node.
	 */
	const TiXmlNode* PreviousSibling() const { return prev; }
	TiXmlNode* PreviousSibling() { return prev; }

	/**
	 * Navigate to a sibling node.
	 */
	const TiXmlNode* PreviousSibling(const std::string&) const;
	TiXmlNode* PreviousSibling(const std::string& _prev) { return const_cast<TiXmlNode*>((const_cast<const TiXmlNode*>(this))->PreviousSibling(_prev)); }

	/**
	 * Navigate to a sibling node.
	 */
	const TiXmlNode* NextSibling() const { return next; }
	TiXmlNode* NextSibling() { return next; }

	/**
	 * Navigate to a sibling node with a given value.
	 */
	const TiXmlNode* NextSibling(const std::string&) const;
	TiXmlNode* NextSibling(const std::string& _next) { return const_cast<TiXmlNode*>((const_cast<const TiXmlNode*>(this))->NextSibling(_next)); }

	/**
	 * Convenience function to get through elements.
	 * Calls NextSibling and ToElement. Will skip all non-Element nodes.
	 * 
	 * \return \c nullptr if there is not another element.
	 */
	const TiXmlElement* NextSiblingElement() const;
	TiXmlElement* NextSiblingElement() { return const_cast<TiXmlElement*>((const_cast<const TiXmlNode*>(this))->NextSiblingElement()); }

	/**
	 * Convenience function to get through elements.
	 * Calls NextSibling and ToElement. Will skip all non-Element nodes.
	 * 
	 * \return \c nullptr if there is not another element.
	 */
	const TiXmlElement* NextSiblingElement(const std::string&) const;
	TiXmlElement* NextSiblingElement(const std::string& _next) { return const_cast<TiXmlElement*>((const_cast<const TiXmlNode*>(this))->NextSiblingElement(_next)); }

	/**
	 * Convenience function to get through elements.
	 */
	const TiXmlElement* FirstChildElement()	const;
	TiXmlElement* FirstChildElement() { return const_cast<TiXmlElement*>((const_cast<const TiXmlNode*>(this))->FirstChildElement()); }

	/**
	 * Convenience function to get through elements.
	 */
	const TiXmlElement* FirstChildElement(const std::string& _value) const;
	TiXmlElement* FirstChildElement(const std::string& _value) { return const_cast<TiXmlElement*>((const_cast<const TiXmlNode*>(this))->FirstChildElement(_value)); }

	/**
	 * Query the type (as an enumerated value, above) of this node. The possible
	 * types are: TINYXML_DOCUMENT, TINYXML_ELEMENT, TINYXML_COMMENT, TINYXML_UNKNOWN,
	 * TINYXML_TEXT, and TINYXML_DECLARATION.
	 */
	int Type() const { return type; }

	/**
	 * Return a pointer to the Document this node lives in. Returns null if not in a document.
	 */
	const TiXmlDocument* GetDocument() const;
	TiXmlDocument* GetDocument() { return const_cast<TiXmlDocument*>((const_cast<const TiXmlNode*>(this))->GetDocument()); }

	/**
	 * Returns true if this node has no children.
	 */
	bool NoChildren() const { return firstChild == nullptr; }

	virtual const TiXmlDocument* ToDocument() const { return nullptr; }
	virtual const TiXmlElement* ToElement() const { return nullptr; }
	virtual const TiXmlComment* ToComment() const { return nullptr; }
	virtual const TiXmlUnknown* ToUnknown() const { return nullptr; }
	virtual const TiXmlText* ToText() const { return nullptr; }
	virtual const TiXmlDeclaration* ToDeclaration() const { return nullptr; }

	virtual TiXmlDocument* ToDocument() { return nullptr; }
	virtual TiXmlElement* ToElement() { return nullptr; }
	virtual TiXmlComment* ToComment() { return nullptr; }
	virtual TiXmlUnknown* ToUnknown() { return nullptr; }
	virtual TiXmlText* ToText() { return nullptr; }
	virtual TiXmlDeclaration* ToDeclaration() { return nullptr; }

	/**
	 * Create an exact duplicate of this node and return it. The memory must be deleted by the caller.
	 */
	virtual TiXmlNode* Clone() const = 0;

	/**
	 * Accept a hierchical visit the nodes in the TinyXML DOM. Every node in the  XML tree will be
	 * conditionally visited and the host will be called back via the TiXmlVisitor interface.
	 */
	virtual bool Accept(XmlVisitor* visitor) const = 0;

	// The real work of the input operator.
	virtual void StreamIn(std::istream& in, std::string& tag) = 0;

protected:
	TiXmlNode(NodeType _type) : TiXmlBase(), parent(nullptr), type(_type), firstChild(nullptr), lastChild(nullptr), prev(nullptr), next(nullptr) {}

	/**
	 * Copy to the allocated object. Shared functionality between Clone,
	 * Copy constructor, and the copy assignment operator.
	 */
	void CopyTo(TiXmlNode* target) const;

	/**
	 * Figure out what is at *p, and parse it. Returns null if it is not an xml node.
	 */
	TiXmlNode* Identify(const char* start, TiXmlEncoding encoding);

	TiXmlNode*		parent;			/**< Parent of the XmlNode. */
	NodeType		type;			/**< Type of the XmlNode. */

	TiXmlNode*		firstChild;		/**< First child of the XmlNode. */
	TiXmlNode*		lastChild;		/**< Last child of the XmlNode. */

	std::string		value;			/**< Value of the XmlNode. */

	TiXmlNode*		prev;			/**< Previous XmlNode. */
	TiXmlNode*		next;			/**< Next XmlNode. */

private:
	TiXmlNode(const TiXmlNode&); // Explicitly disallowed.
	void operator=(const TiXmlNode& base); // Explicitly disallowed.
};


} // namespace Xml
} // namespace NAS2D
