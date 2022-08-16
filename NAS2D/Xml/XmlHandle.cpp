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
#include "XmlHandle.h"

#include "XmlElement.h"
#include "XmlText.h"
#include "XmlUnknown.h"

using namespace NAS2D::Xml;

XmlHandle::XmlHandle() : node(nullptr)
{}


/**
 * \param _node	Pointer to an XmlNode.
 *
 * \note	XmlHandle does not take ownership of the pointer.
 */
XmlHandle::XmlHandle(XmlNode* _node) : node(_node)
{}


/**
 * \param ref	XmlHandle to copy.
 */
XmlHandle::XmlHandle(const XmlHandle& ref) : node(ref.node)
{}


XmlHandle& XmlHandle::operator=(const XmlHandle& ref)
{
	if (&ref != this)
	{
		this->node = ref.node;
	}

	return *this;
}


/**
 * Get the first child node.
 *
 * \returns	A handle to the first child node or nullptr
 *			if there are no children.
 */
XmlHandle XmlHandle::firstChild() const
{
	if (node)
	{
		XmlNode* child = node->firstChild();
		if (child)
		{
			return XmlHandle(child);
		}
	}
	return XmlHandle(nullptr);
}


/**
 * Get the first child node matching 'value'.
 *
 * \param value	Value of the child node to find.
 *
 * \returns	A handle to the first child node matching 'value. Will
 *			return nullptr if there are no children or if no
 *			children match 'value'.
 *
 * \see See XmlNode::value() for possible meanings of 'value'.
 */
XmlHandle XmlHandle::firstChild(const std::string& value) const
{
	if (node)
	{
		XmlNode* child = node->firstChild(value);
		if (child)
		{
			return XmlHandle(child);
		}
	}
	return XmlHandle(nullptr);
}


/**
 * Get the first child element.
 *
 * \returns	A handle to the first child element or nullptr
 *			if there are no children.
 */
XmlHandle XmlHandle::firstChildElement() const
{
	if (node)
	{
		XmlElement* child = node->firstChildElement();
		if (child)
		{
			return XmlHandle(child);
		}
	}
	return XmlHandle(nullptr);
}


/**
 * Get the first child element matching 'value'.
 *
 * \param value	Value of the child element to find.
 *
 * \returns	A handle to the first child element matching 'value. Will
 *			return nullptr if there are no children or if no
 *			children match 'value'.
 *
 * \see See XmlNode::value() for possible meanings of 'value'.
 */
XmlHandle XmlHandle::firstChildElement(const std::string& value) const
{
	if (node)
	{
		XmlElement* child = node->firstChildElement(value);
		if (child)
		{
			return XmlHandle(child);
		}
	}

	return XmlHandle(nullptr);
}


/**
 * Get a handle to child in a specific index order.
 *
 * For example, assuming you have the following XML:
 *
 * \code{.xml}
 * <document>
 *	<element_a />
 *	<element_b />
 *	<element_c />
 * </document>
 * \endcode
 *
 * Using '1' for the \c index paramter will return a handle to
 * an XmlNode with the value "element_b".
 *
 * \param index	Index position of the child.
 *
 * \return	Returns a handle to a child node or nullptr
 *			if there are no children or 'count' is past
 *			the number of children contained in the node.
 */
XmlHandle XmlHandle::child(int index) const
{
	if (node)
	{
		XmlNode* child = node->firstChild();
		for (int i = 0; child && i < index; child = child->nextSibling(), ++i) {} // Nothing
		if (child)
		{
			return XmlHandle(child);
		}
	}

	return XmlHandle(nullptr);
}


/**
 * Get a handle to child 'value' in a specific index order.
 *
 * For example, assuming you have the following XML:
 *
 * \code{.xml}
 * <document>
 *	<element_a />
 *	<element_b />
 *	<element_a />
 *	<element_c />
 * </document>
 * \endcode
 *
 * Using parameters 'element_a' '1' for the \c index paramter will
 * return a handle to the second node 'element_a'.
 *
 * \param value Value of the node to look for.
 * \param index	Index position of the child.
 *
 * \return	Returns a handle to a child node or nullptr
 *			if there are no children or 'count' is past
 *			the number of children contained in the node.
 *
 * \see See XmlNode::value() for possible meanings of 'value'.
 */
XmlHandle XmlHandle::child(const std::string& value, int index) const
{
	if (node)
	{
		XmlNode* child = node->firstChild(value);
		for (int i = 0; child && i < index; child = child->nextSibling(value), ++i) {}
		if (child)
		{
			return XmlHandle(child);
		}
	}

	return XmlHandle(nullptr);
}


/**
 * Get a handle to child in a specific index order.
 *
 * For example, assuming you have the following XML:
 *
 * \code{.xml}
 * <document>
 *	<element_a />
 *	<!-- This is a comment -->
 *	<element_b />
 * </document>
 * \endcode
 *
 * Using '1' for the \c index paramter will return a handle to
 * an XmlNode with the value "element_b".
 *
 * \param index	Index position of the child.
 *
 * \return	Returns a handle to a child node or nullptr
 *			if there are no children or 'count' is past
 *			the number of children contained in the node.
 *
 * \note	Only Elements are indexed. Other types are ignored.
 */
XmlHandle XmlHandle::childElement(int index) const
{
	if (node)
	{
		XmlElement* child = node->firstChildElement();
		for (int i = 0; child && i < index; child = child->nextSiblingElement(), ++i) {}

		if (child)
		{
			return XmlHandle(child);
		}
	}

	return XmlHandle(nullptr);
}


/**
 * Get a handle to child 'value' in a specific index order.
 *
 * For example, assuming you have the following XML:
 *
 * \code{.xml}
 * <document>
 *	<element_a />
 *	<element_b />
 *	<element_a />
 *	<element_c />
 * </document>
 * \endcode
 *
 * Using parameters 'element_a' '1' for the \c index paramter will
 * return a handle to the second node 'element_a'.
 *
 * \param value Value of the node to look for.
 * \param count	Index position of the child.
 *
 * \return	Returns a handle to a child node or nullptr
 *			if there are no children or 'count' is past
 *			the number of children contained in the node.
 *
 * \note Only Elements are indexed. Other types are ignored.
 *
 * \see See XmlNode::value() for possible meanings of 'value'.
 */
XmlHandle XmlHandle::childElement(const std::string& value, int count) const
{
	if (node)
	{
		XmlElement* child = node->firstChildElement(value);
		for (int i = 0; child && i < count; child = child->nextSiblingElement(value), ++i) {}

		if (child)
		{
			return XmlHandle(child);
		}
	}

	return XmlHandle(nullptr);
}


/**
 * Get a handle as an XmlNode.
 *
 * \returns	A handle to an XmlNode or nullptr if
 *			not a node.
 */
XmlNode* XmlHandle::toNode() const
{
	return node;
}


/**
 * Get a handle as an XmlElement.
 *
 * \returns	A handle to an XmlElement or nullptr if
 *			not an Element.
 */
XmlElement* XmlHandle::toElement() const
{
	return ((node && node->toElement()) ? node->toElement() : nullptr);
}


/**
 * Get a handle as an XmlText.
 *
 * \returns	A handle to an XmlText or nullptr if
 *			not a text element.
 */
XmlText* XmlHandle::toText() const
{
	return ((node && node->toText()) ? node->toText() : nullptr);
}


/**
 * Get a handle as an XmlUnknown.
 *
 * \returns	A handle to an XmlUnknown or nullptr if
 *			not an unknown element.
 */
XmlUnknown* XmlHandle::toUnknown() const
{
	return ((node && node->toUnknown()) ? node->toUnknown() : nullptr);
}
