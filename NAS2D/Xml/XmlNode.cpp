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
	_type(NodeType::XML_UNKNOWN),
	_firstChild(nullptr),
	_lastChild(nullptr),
	_prev(nullptr),
	_next(nullptr)
{}

XmlNode::XmlNode(NodeType type) :
	XmlBase(),
	_parent(nullptr),
	_type(type),
	_firstChild(nullptr),
	_lastChild(nullptr),
	_prev(nullptr),
	_next(nullptr)
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

void XmlNode::copyTo(XmlNode* target) const
{
	target->value(_value);
	target->location = location;
}

void XmlNode::value(const std::string& value)
{
	_value = value;
}

const std::string& XmlNode::value() const
{
	return _value;
}

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
	else { _firstChild = node; }

	_lastChild = node;
	return node;
}

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
		_firstChild = node;
	}
	beforeThis->_prev = node;
	return node;
}

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
		_lastChild = node;
	}
	afterThis->_next = node;
	return node;
}

XmlNode* XmlNode::replaceChild(XmlNode* replaceThis, const XmlNode& withThis)
{
	if (!replaceThis) { return nullptr; }

	if (replaceThis->_parent != this) { return nullptr; }

	if (withThis.toDocument())
	{
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

bool XmlNode::removeChild(XmlNode* node)
{
	if (!node) { return false; }

	if (node->_parent != this)
	{
		return false;
	}

	if (node->_next) { node->_next->_prev = node->_prev; }
	else { _lastChild = node->_prev; }

	if (node->_prev) { node->_prev->_next = node->_next; }
	else { _firstChild = node->_next; }

	delete node;
	return true;
}


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

XmlNode* XmlNode::lastChild(const std::string& value)
{
	return const_cast<XmlNode*> ((const_cast<const XmlNode*>(this))->lastChild(value));
}

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

XmlNode* XmlNode::iterateChildren(const XmlNode* previous)
{
	return const_cast<XmlNode*>((const_cast<const XmlNode*>(this))->iterateChildren(previous));
}

const XmlNode* XmlNode::iterateChildren(const std::string& value, const XmlNode* previous) const
{
	if (!previous)
	{
		return firstChild(value);
	}
	else
	{
		return previous->nextSibling(value);
	}
}

XmlNode* XmlNode::iterateChildren(const std::string& value, const XmlNode* previous)
{
	return const_cast<XmlNode*>((const_cast<const XmlNode*>(this))->iterateChildren(value, previous));
}

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

XmlNode* XmlNode::nextSibling(const std::string& next)
{
	return const_cast<XmlNode*>((const_cast<const XmlNode*>(this))->nextSibling(next));
}

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

XmlNode* XmlNode::previousSibling(const std::string& prev)
{
	return const_cast<XmlNode*>((const_cast<const XmlNode*>(this))->previousSibling(prev));
}

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

XmlElement* XmlNode::firstChildElement()
{
	return const_cast<XmlElement*>((const_cast<const XmlNode*>(this))->firstChildElement());
}

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

XmlElement* XmlNode::firstChildElement(const std::string& value)
{
	return const_cast<XmlElement*>((const_cast<const XmlNode*>(this))->firstChildElement(value));
}

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

XmlElement* XmlNode::nextSiblingElement()
{
	return const_cast<XmlElement*>((const_cast<const XmlNode*>(this))->nextSiblingElement());
}

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

XmlElement* XmlNode::nextSiblingElement(const std::string& next)
{
	return const_cast<XmlElement*>((const_cast<const XmlNode*>(this))->nextSiblingElement(next));
}

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

XmlDocument* XmlNode::document()
{
	return const_cast<XmlDocument*>((const_cast<const XmlNode*>(this))->document());
}

XmlNode* XmlNode::parent()
{
	return _parent;
}

const XmlNode* XmlNode::parent() const
{
	return _parent;
}

const XmlNode* XmlNode::firstChild() const
{
	return _firstChild;
}

XmlNode* XmlNode::firstChild()
{
	return _firstChild;
}

XmlNode* XmlNode::firstChild(const std::string& value)
{
	return const_cast<XmlNode*> ((const_cast<const XmlNode*>(this))->firstChild(value));
}

const XmlNode* XmlNode::lastChild() const
{
	return _lastChild;
}

XmlNode* XmlNode::lastChild()
{
	return _lastChild;
}

const XmlNode* XmlNode::previousSibling() const
{
	return _prev;
}

XmlNode* XmlNode::previousSibling()
{
	return _prev;
}

const XmlNode* XmlNode::nextSibling() const
{
	return _next;
}

XmlNode* XmlNode::nextSibling()
{
	return _next;
}

XmlNode::NodeType XmlNode::type() const
{
	return _type;
}

bool XmlNode::hasChildren() const
{
	return _firstChild != nullptr;
}
