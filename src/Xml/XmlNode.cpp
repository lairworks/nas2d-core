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
#include "NAS2D/Xml/XmlNode.h"
#include "NAS2D/Xml/XmlDocument.h"

#include "NAS2D/Exception.h"

using namespace NAS2D::Xml;

XmlNode::~XmlNode()
{
	XmlNode* node = firstChild;
	XmlNode* temp = nullptr;

	while (node)
	{
		temp = node;
		node = node->next;
		delete temp;
	}
}


void XmlNode::CopyTo(XmlNode* target) const
{
	target->SetValue(value);
	target->userData = userData;
	target->location = location;
}


void XmlNode::Clear()
{
	XmlNode* node = firstChild;
	XmlNode* temp = nullptr;

	while (node)
	{
		temp = node;
		node = node->next;
		delete temp;
	}

	firstChild = nullptr;
	lastChild = nullptr;
}


XmlNode* XmlNode::LinkEndChild(XmlNode* node)
{
	if (!node)
		return nullptr;
	else if (node->parent != nullptr && node->parent != this)
		return nullptr;
	else if (node->GetDocument() != nullptr && node->GetDocument() != this->GetDocument())
		return nullptr;

	if (node->Type() == XmlNode::XML_DOCUMENT)
	{
		delete node;
		if (GetDocument())
			GetDocument()->SetError(XML_ERROR_DOCUMENT_TOP_ONLY, 0, 0);
		return nullptr;
	}

	node->parent = this;

	node->prev = lastChild;
	node->next = nullptr;

	if (lastChild)
		lastChild->next = node;
	else
		firstChild = node; // it was an empty list.

	lastChild = node;
	return node;
}


XmlNode* XmlNode::InsertEndChild(const XmlNode& addThis)
{
	if (addThis.Type() == XmlNode::XML_DOCUMENT)
	{
		if (GetDocument())
			GetDocument()->SetError(XML_ERROR_DOCUMENT_TOP_ONLY, 0, 0);
		return nullptr;
	}
	XmlNode* node = addThis.Clone();
	if (!node)
		return nullptr;

	return LinkEndChild(node);
}


XmlNode* XmlNode::InsertBeforeChild(XmlNode* beforeThis, const XmlNode& addThis)
{
	if (!beforeThis || beforeThis->parent != this)
		return nullptr;

	if (addThis.Type() == XmlNode::XML_DOCUMENT)
	{
		if (GetDocument())
			GetDocument()->SetError(XML_ERROR_DOCUMENT_TOP_ONLY, 0, 0);
		return nullptr;
	}

	XmlNode* node = addThis.Clone();
	if (!node)
		return nullptr;
	node->parent = this;

	node->next = beforeThis;
	node->prev = beforeThis->prev;
	if (beforeThis->prev)
	{
		beforeThis->prev->next = node;
	}
	else
	{
		//assert(firstChild == beforeThis);
		firstChild = node;
	}
	beforeThis->prev = node;
	return node;
}


XmlNode* XmlNode::InsertAfterChild(XmlNode* afterThis, const XmlNode& addThis)
{
	if (!afterThis || afterThis->parent != this) {
		return nullptr;
	}
	if (addThis.Type() == XmlNode::XML_DOCUMENT)
	{
		if (GetDocument())
			GetDocument()->SetError(XML_ERROR_DOCUMENT_TOP_ONLY, 0, 0);
		return nullptr;
	}

	XmlNode* node = addThis.Clone();
	if (!node)
		return nullptr;
	node->parent = this;

	node->prev = afterThis;
	node->next = afterThis->next;
	if (afterThis->next)
	{
		afterThis->next->prev = node;
	}
	else
	{
		//assert(lastChild == afterThis);
		lastChild = node;
	}
	afterThis->next = node;
	return node;
}


XmlNode* XmlNode::ReplaceChild(XmlNode* replaceThis, const XmlNode& withThis)
{
	if (!replaceThis)
		return nullptr;

	if (replaceThis->parent != this)
		return nullptr;

	if (withThis.ToDocument())
	{
		// A document can never be a child.	Thanks to Noam.
		XmlDocument* document = GetDocument();
		if (document)
			document->SetError(XML_ERROR_DOCUMENT_TOP_ONLY, 0, 0);
		return nullptr;
	}

	XmlNode* node = withThis.Clone();
	if (!node)
		return nullptr;

	node->next = replaceThis->next;
	node->prev = replaceThis->prev;

	if (replaceThis->next)
		replaceThis->next->prev = node;
	else
		lastChild = node;

	if (replaceThis->prev)
		replaceThis->prev->next = node;
	else
		firstChild = node;

	delete replaceThis;
	node->parent = this;
	return node;
}


bool XmlNode::RemoveChild(XmlNode* removeThis)
{
	if (!removeThis)
		return false;

	if (removeThis->parent != this)
	{
		//assert(0); // ??? Not really a great way to do this.
		return false;
	}

	if (removeThis->next)
		removeThis->next->prev = removeThis->prev;
	else
		lastChild = removeThis->prev;

	if (removeThis->prev)
		removeThis->prev->next = removeThis->next;
	else
		firstChild = removeThis->next;

	delete removeThis;
	return true;
}

const XmlNode* XmlNode::FirstChild(const std::string& _value) const
{
	const XmlNode* node = nullptr;
	for (node = firstChild; node; node = node->next)
	{
		if (node->Value() == _value)
			return node;
	}
	return nullptr;
}


const XmlNode* XmlNode::LastChild(const std::string& _value) const
{
	const XmlNode* node = nullptr;
	for (node = lastChild; node; node = node->prev)
	{
		if (node->Value() == _value)
			return node;
	}
	return nullptr;
}


const XmlNode* XmlNode::IterateChildren(const XmlNode* previous) const
{
	if (!previous)
	{
		return FirstChild();
	}
	else
	{
		//assert(previous->parent == this);
		return previous->NextSibling();
	}
}


const XmlNode* XmlNode::IterateChildren(const std::string& val, const XmlNode* previous) const
{
	if (!previous)
	{
		return FirstChild(val);
	}
	else
	{
		//assert(previous->parent == this);
		return previous->NextSibling(val);
	}
}


const XmlNode* XmlNode::NextSibling(const std::string& _value) const
{
	const XmlNode* node = nullptr;
	for (node = next; node; node = node->next)
	{
		if (node->Value() == _value)
			return node;
	}
	return nullptr;
}


const XmlNode* XmlNode::PreviousSibling(const std::string& _value) const
{
	const XmlNode* node = nullptr;
	for (node = prev; node; node = node->prev)
	{
		if (node->Value() == _value)
			return node;
	}
	return nullptr;
}


const XmlElement* XmlNode::FirstChildElement() const
{
	const XmlNode* node = nullptr;

	for (node = FirstChild(); node; node = node->NextSibling())
	{
		if (node->ToElement())
			return node->ToElement();
	}
	return nullptr;
}


const XmlElement* XmlNode::FirstChildElement(const std::string& _value) const
{
	const XmlNode* node = nullptr;

	for (node = FirstChild(_value);	node; node = node->NextSibling(_value))
	{
		if (node->ToElement())
			return node->ToElement();
	}
	return nullptr;
}


const XmlElement* XmlNode::NextSiblingElement() const
{
	const XmlNode* node = nullptr;

	for (node = NextSibling(); node; node = node->NextSibling())
	{
		if (node->ToElement())
			return node->ToElement();
	}
	return nullptr;
}


const XmlElement* XmlNode::NextSiblingElement(const std::string&  _value) const
{
	const XmlNode* node = nullptr;

	for (node = NextSibling(_value); node; node = node->NextSibling(_value))
	{
		if (node->ToElement())
			return node->ToElement();
	}

	return nullptr;
}


const XmlDocument* XmlNode::GetDocument() const
{
	const XmlNode* node;

	for (node = this; node; node = node->parent)
	{
		if (node->ToDocument())
			return node->ToDocument();
	}
	return nullptr;
}

