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
#include "XmlNode.h"
#include "XmlDocument.h"

using namespace NAS2D::Xml;

XmlNode::XmlNode() :
	XmlBase(),
	_parent(nullptr),
	_firstChild(nullptr),
	_lastChild(nullptr),
	_prev(nullptr),
	_next(nullptr),
	_type(NodeType::XML_UNKNOWN)
{}


/**
 * Construct a node with a given type.
 *
 * \param	type	Type of the node. See XmlNode::NodeType.
 */
XmlNode::XmlNode(NodeType type) :
	XmlBase(),
	_parent(nullptr),
	_firstChild(nullptr),
	_lastChild(nullptr),
	_prev(nullptr),
	_next(nullptr),
	_type(type)
{}


XmlNode::~XmlNode()
{
	XmlNode* node = _firstChild;
	XmlNode* temp = nullptr;

	while (node)
	{
		temp = node;
		node = node->_next;
		delete temp;
	}
}


/**
 * Copy to the allocated object. Shared functionality between Clone,
 * Copy constructor, and the copy assignment operator.
 */
void XmlNode::copyTo(XmlNode* target) const
{
	target->value(_value);
	target->location = location;
}


/**
 * Changes the value of the node.
 *
 * The type of node changes the meaning of its value:
 * \li <b>Document:</b> filename of the XML file.
 * \li <b>Element:</b> name of the Element.
 * \li <b>Comment:</b> the Comment text.
 * \li <b>Unknown:</b> the Tag contents.
 * \li <b>Text:</b> the Text string.
 *
 * \param	value	\c std::string containing the value to set.
 */
void XmlNode::value(const std::string& value)
{
	_value = value;
}


/**
 * Changes the value of the node.
 *
 * The type of node changes the meaning of its value:
 * \li <b>Document:</b> filename of the XML file.
 * \li <b>Element:</b> name of the Element.
 * \li <b>Comment:</b> the Comment text.
 * \li <b>Unknown:</b> the Tag contents.
 * \li <b>Text:</b> the Text string.
 *
 * \returns	The value of the node.
 */
const std::string& XmlNode::value() const
{
	return _value;
}


/**
 * Delete all the children of this node. Does not affect 'this'.
 */
void XmlNode::clear()
{
	XmlNode* node = _firstChild;
	XmlNode* temp = nullptr;

	while (node)
	{
		temp = node;
		node = node->_next;
		delete temp;
	}

	_firstChild = nullptr;
	_lastChild = nullptr;
}


/**
 * Add a new node related to this. Adds a child past the LastChild.
 *
 * \note The node to be added is passed by pointer, and will be henceforth owned
 *		 by the Xml library.
 *
 * \see insertEndChild
 */
XmlNode* XmlNode::linkEndChild(XmlNode* node)
{
	if (!node)
	{
		return nullptr;
	}
	else if (node->_parent != nullptr && node->_parent != this)
	{
		return nullptr;
	}
	else if (node->document() != nullptr && node->document() != this->document())
	{
		return nullptr;
	}

	if (node->type() == XmlNode::NodeType::XML_DOCUMENT)
	{
		delete node;
		if (document()) { document()->error(XmlErrorCode::XML_ERROR_DOCUMENT_TOP_ONLY, nullptr, nullptr); }
		return nullptr;
	}

	node->_parent = this;

	node->_prev = _lastChild;
	node->_next = nullptr;

	if (_lastChild) { _lastChild->_next = node; }
	else { _firstChild = node; } // it was an empty list.

	_lastChild = node;
	return node;
}


/**
 * Add a new node related to this. Adds a child past the LastChild.
 * Returns a pointer to the new object or NULL if an error occured.
 */
XmlNode* XmlNode::insertEndChild(const XmlNode& node)
{
	if (node.type() == XmlNode::NodeType::XML_DOCUMENT)
	{
		if (document()) { document()->error(XmlErrorCode::XML_ERROR_DOCUMENT_TOP_ONLY, nullptr, nullptr); }

		return nullptr;
	}

	XmlNode* n = node.clone();
	if (!n)
	{
		return nullptr;
	}

	return linkEndChild(n);
}


/**
 * Add a new node related to this. Adds a child before the specified child.
 *
 * \return A pointer to the new object or nullptr if an error occured.
 */
XmlNode* XmlNode::insertBeforeChild(XmlNode* beforeThis, const XmlNode& addThis)
{
	if (!beforeThis || beforeThis->_parent != this)
	{
		return nullptr;
	}

	if (addThis.type() == XmlNode::NodeType::XML_DOCUMENT)
	{
		if (document()) { document()->error(XmlErrorCode::XML_ERROR_DOCUMENT_TOP_ONLY, nullptr, nullptr); }
		return nullptr;
	}

	XmlNode* node = addThis.clone();
	if (!node) { return nullptr; }
	node->_parent = this;

	node->_next = beforeThis;
	node->_prev = beforeThis->_prev;
	if (beforeThis->_prev)
	{
		beforeThis->_prev->_next = node;
	}
	else
	{
		//assert(firstChild == beforeThis);
		_firstChild = node;
	}
	beforeThis->_prev = node;
	return node;
}


/**
 * Add a new node related to this. Adds a child after the specified child.
 * Returns a pointer to the new object or nullptr if an error occured.
 */
XmlNode* XmlNode::insertAfterChild(XmlNode* afterThis, const XmlNode& addThis)
{
	if (!afterThis || afterThis->_parent != this)
	{
		return nullptr;
	}
	if (addThis.type() == XmlNode::NodeType::XML_DOCUMENT)
	{
		if (document()) { document()->error(XmlErrorCode::XML_ERROR_DOCUMENT_TOP_ONLY, nullptr, nullptr); }
		return nullptr;
	}

	XmlNode* node = addThis.clone();
	if (!node) { return nullptr; }

	node->_parent = this;

	node->_prev = afterThis;
	node->_next = afterThis->_next;
	if (afterThis->_next)
	{
		afterThis->_next->_prev = node;
	}
	else
	{
		//assert(lastChild == afterThis);
		_lastChild = node;
	}
	afterThis->_next = node;
	return node;
}


/**
 * Replace a child of this node.
 *
 * \return A pointer to the new object or \c nullptr if an error occured.
 */
XmlNode* XmlNode::replaceChild(XmlNode* replaceThis, const XmlNode& withThis)
{
	if (!replaceThis) { return nullptr; }

	if (replaceThis->_parent != this) { return nullptr; }

	if (withThis.toDocument())
	{
		// A document can never be a child. Thanks to Noam.
		XmlDocument* doc = document();
		if (doc) { doc->error(XmlErrorCode::XML_ERROR_DOCUMENT_TOP_ONLY, nullptr, nullptr); }
		return nullptr;
	}

	XmlNode* node = withThis.clone();
	if (!node) { return nullptr; }

	node->_next = replaceThis->_next;
	node->_prev = replaceThis->_prev;

	if (replaceThis->_next) { replaceThis->_next->_prev = node; }
	else { _lastChild = node; }

	if (replaceThis->_prev) { replaceThis->_prev->_next = node; }
	else { _firstChild = node; }

	delete replaceThis;
	node->_parent = this;
	return node;
}


/**
 * Delete a child of this node.
 */
bool XmlNode::removeChild(XmlNode* node)
{
	if (!node) { return false; }

	if (node->_parent != this)
	{
		//assert(0); // ??? Not really a great way to do this.
		return false;
	}

	if (node->_next) { node->_next->_prev = node->_prev; }
	else { _lastChild = node->_prev; }

	if (node->_prev) { node->_prev->_next = node->_next; }
	else { _firstChild = node->_next; }

	delete node;
	return true;
}


/**
 * Gets the first child of the node matching 'value'.
 *
 * \param	value	Value of the child to look for.
 *
 * \returns	The first child of the node with a matching value or
 *			\c nullptr if not available.
 *
 * \see See XmlNode::value() for possible meanings of 'value'.
 */
const XmlNode* XmlNode::firstChild(const std::string& value) const
{
	const XmlNode* node = nullptr;
	for (node = _firstChild; node; node = node->_next)
	{
		if (node->value() == value)
		{
			return node;
		}
	}
	return nullptr;
}


/**
 * The last child of this node matching 'value'.
 *
 * \param value Value of the node to look for.
 *
 * \returns Child node matching 'value' or \c nullptr if
 *			node isn't found.
 *
 * \see See XmlNode::value() for possible meanings of 'value'.
 */
const XmlNode* XmlNode::lastChild(const std::string& value) const
{
	const XmlNode* node = nullptr;
	for (node = _lastChild; node; node = node->_prev)
	{
		if (node->value() == value)
		{
			return node;
		}
	}
	return nullptr;
}


/**
 * The last child of this node matching 'value'.
 *
 * \note Non-const version. See XmlNode::lastChild(const std::string&)
 *
 * \param value Value of the node to look for.
 *
 * \returns Child node matching 'value' or \c nullptr if
 *			node isn't found.
 *
 * \see See XmlNode::value() for possible meanings of 'value'.
 */
XmlNode* XmlNode::lastChild(const std::string& value)
{
	return const_cast<XmlNode*> ((const_cast<const XmlNode*>(this))->lastChild(value));
}


/**
 * An alternate way to walk the children of a node.
 *
 * One way to iterate over nodes is:
 * \code{.cpp}
 * for (child = parent->firstChild(); child; child = child->nextSibling())
 * \endcode
 *
 * IterateChildren does the same thing with the syntax:
 * \code{.cpp}
 * child = nullptr;
 * while (child = parent->iterateChildren(child))
 * \endcode
 *
 * \param	previous	Pointer to the previous child of the node and finds
 *						the next one. If the previous child is \c nullptr,
 *						this function returns the first child.
 *
 * \returns	Pointer to the next child of the node. Will return \c nullptr
 *			when there are no children left.
 */
const XmlNode* XmlNode::iterateChildren(const XmlNode* previous) const
{
	if (!previous)
	{
		return firstChild();
	}
	else
	{
		//assert(previous->parent == this);
		return previous->nextSibling();
	}
}


/**
 * Non-const version of XmlNode::iterateChildren(const XmlNode*).
 *
 * \param	previous	Pointer to the previous child of the node and finds
 *						the next one. If the previous child is \c nullptr,
 *						this function returns the first child.
 *
 * \returns	Pointer to the next child of the node. Will return \c nullptr
 *			when there are no children left.
 *
 * \see See XmlNode::iterateChildren(const XmlNode*) const
 */
XmlNode* XmlNode::iterateChildren(const XmlNode* previous)
{
	return const_cast<XmlNode*>((const_cast<const XmlNode*>(this))->iterateChildren(previous));
}


/**
 * Search for children with a particular 'value'.
 *
 * \param value		Value of the node to search against.
 * \param previous	Pointer to the previous child of the node and finds
 *					the next one. If the previous child is \c nullptr,
 *					this function returns the first child.
 *
 * \returns	Pointer to the next child of the node. Will return \c nullptr
 *			when there are no children left.
 */
const XmlNode* XmlNode::iterateChildren(const std::string& value, const XmlNode* previous) const
{
	if (!previous)
	{
		return firstChild(value);
	}
	else
	{
		//assert(previous->parent == this);
		return previous->nextSibling(value);
	}
}


/**
 * Non-const version of XmlNode::iterateChildren(const std::string&, const XmlNode*) const.
 *
 * \param value		Value of the node to search against.
 * \param previous	Pointer to the previous child of the node and finds
 *					the next one. If the previous child is \c nullptr,
 *					this function returns the first child.
 *
 * \returns	Pointer to the next child of the node. Will return \c nullptr
 *			when there are no children left.
 *
 * \see See XmlNode::iterateChildren(const std::string&, const XmlNode*) const
 */
XmlNode* XmlNode::iterateChildren(const std::string& value, const XmlNode* previous)
{
	return const_cast<XmlNode*>((const_cast<const XmlNode*>(this))->iterateChildren(value, previous));
}


/**
 * Navigate to a sibling node with a given 'value'.
 *
 * \see See XmlNode::value() for possible meanings of 'value'.
 */
const XmlNode* XmlNode::nextSibling(const std::string& value) const
{
	const XmlNode* node = nullptr;
	for (node = _next; node; node = node->_next)
	{
		if (node->value() == value)
		{
			return node;
		}
	}
	return nullptr;
}


/**
 * Navigate to a sibling node with a given value.
 */
XmlNode* XmlNode::nextSibling(const std::string& next)
{
	return const_cast<XmlNode*>((const_cast<const XmlNode*>(this))->nextSibling(next));
}


/**
 * Navigate to a sibling node.
 */
const XmlNode* XmlNode::previousSibling(const std::string& value) const
{
	const XmlNode* node = nullptr;
	for (node = _prev; node; node = node->_prev)
	{
		if (node->value() == value)
		{
			return node;
		}
	}
	return nullptr;
}


/**
 * Navigate to a sibling node.
 */
XmlNode* XmlNode::previousSibling(const std::string& prev)
{
	return const_cast<XmlNode*>((const_cast<const XmlNode*>(this))->previousSibling(prev));
}


/**
 * Convenience function to get through elements.
 */
const XmlElement* XmlNode::firstChildElement() const
{
	const XmlNode* node = nullptr;

	for (node = firstChild(); node; node = node->nextSibling())
	{
		if (node->toElement())
		{
			return node->toElement();
		}
	}
	return nullptr;
}


/**
 * Convenience function to get through elements.
 */
XmlElement* XmlNode::firstChildElement()
{
	return const_cast<XmlElement*>((const_cast<const XmlNode*>(this))->firstChildElement());
}


/**
 * Convenience function to get through elements.
 */
const XmlElement* XmlNode::firstChildElement(const std::string& value) const
{
	const XmlNode* node = nullptr;

	for (node = firstChild(value); node; node = node->nextSibling(value))
	{
		if (node->toElement())
		{
			return node->toElement();
		}
	}
	return nullptr;
}


/**
 * Convenience function to get through elements.
 */
XmlElement* XmlNode::firstChildElement(const std::string& value)
{
	return const_cast<XmlElement*>((const_cast<const XmlNode*>(this))->firstChildElement(value));
}


/**
 * Convenience function to get through elements.
 * Calls NextSibling and ToElement. Will skip all non-Element nodes.
 *
 * \return \c nullptr if there is not another element.
 */
const XmlElement* XmlNode::nextSiblingElement() const
{
	const XmlNode* node = nullptr;

	for (node = nextSibling(); node; node = node->nextSibling())
	{
		if (node->toElement())
		{
			return node->toElement();
		}
	}
	return nullptr;
}


/**
 * Convenience function to get through elements.
 * Calls NextSibling and ToElement. Will skip all non-Element nodes.
 *
 * \return \c nullptr if there is not another element.
 */
XmlElement* XmlNode::nextSiblingElement()
{
	return const_cast<XmlElement*>((const_cast<const XmlNode*>(this))->nextSiblingElement());
}


/**
 * Convenience function to get through elements.
 * Calls NextSibling and ToElement. Will skip all non-Element nodes.
 *
 * \return \c nullptr if there is not another element.
 */
const XmlElement* XmlNode::nextSiblingElement(const std::string& value) const
{
	const XmlNode* node = nullptr;

	for (node = nextSibling(value); node; node = node->nextSibling(value))
	{
		if (node->toElement())
		{
			return node->toElement();
		}
	}

	return nullptr;
}


/**
 * Convenience function to get through elements.
 * Calls NextSibling and ToElement. Will skip all non-Element nodes.
 *
 * \return \c nullptr if there is not another element.
 */
XmlElement* XmlNode::nextSiblingElement(const std::string& next)
{
	return const_cast<XmlElement*>((const_cast<const XmlNode*>(this))->nextSiblingElement(next));
}


/**
 * Get a const pointer to the Document this node lives in. Returns nullptr if not in a document.
 */
const XmlDocument* XmlNode::document() const
{
	const XmlNode* node;

	for (node = this; node; node = node->_parent)
	{
		if (node->toDocument())
		{
			return node->toDocument();
		}
	}
	return nullptr;
}


/**
 * Get a pointer to the Document this node lives in. Returns nullptr if not in a document.
 */
XmlDocument* XmlNode::document()
{
	return const_cast<XmlDocument*>((const_cast<const XmlNode*>(this))->document());
}


/**
 * Gets the parent of the node.
 *
 * \returns	Pointer to the node's parent. \c nullptr if no parent.
 */
XmlNode* XmlNode::parent()
{
	return _parent;
}


/**
 * Gets the parent of the node.
 *
 * \returns	Pointer to the node's parent. \c nullptr if no parent.
 */
const XmlNode* XmlNode::parent() const
{
	return _parent;
}


/**
 * Get the first child of the node.
 *
 * \returns	Pointer to the first child of the node. \c nullptr if no parent.
 */
const XmlNode* XmlNode::firstChild() const
{
	return _firstChild;
}


/**
 * Get the first child of the node.
 *
 * \returns	Pointer to the first child of the node. \c nullptr if no parent.
 */
XmlNode* XmlNode::firstChild()
{
	return _firstChild;
}


/**
 * The first child of this node with the matching 'value'. Will be \c nullptr if none found.
 */
XmlNode* XmlNode::firstChild(const std::string& value)
{
	return const_cast<XmlNode*> ((const_cast<const XmlNode*>(this))->firstChild(value));
}


/**
 * Gets the last child of this node.
 *
 * \returns The last child of the node or \c nullptr if
 *			there are no children.
 */
const XmlNode* XmlNode::lastChild() const
{
	return _lastChild;
}


/**
 * Gets the last child of this node.
 *
 * \returns The last child of the node or \c nullptr if
 *			there are no children.
 */
XmlNode* XmlNode::lastChild()
{
	return _lastChild;
}


/**
 * Navigate to a sibling node.
 */
const XmlNode* XmlNode::previousSibling() const
{
	return _prev;
}


/**
 * Navigate to a sibling node.
 */
XmlNode* XmlNode::previousSibling()
{
	return _prev;
}


/**
 * Navigate to a sibling node.
 */
const XmlNode* XmlNode::nextSibling() const
{
	return _next;
}


/**
 * Navigate to a sibling node.
 */
XmlNode* XmlNode::nextSibling()
{
	return _next;
}


/**
 * Query the type of the node.
 *
 * \see XmlNode::NodeType
 */
XmlNode::NodeType XmlNode::type() const
{
	return _type;
}


/**
 * Returns true if this node has children.
 */
bool XmlNode::hasChildren() const
{
	return _firstChild != nullptr;
}
