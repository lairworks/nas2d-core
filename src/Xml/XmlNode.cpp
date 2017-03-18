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

TiXmlNode::~TiXmlNode()
{
	TiXmlNode* node = firstChild;
	TiXmlNode* temp = nullptr;

	while (node)
	{
		temp = node;
		node = node->next;
		delete temp;
	}
}


void TiXmlNode::CopyTo(TiXmlNode* target) const
{
	target->SetValue(value);
	target->userData = userData;
	target->location = location;
}


void TiXmlNode::Clear()
{
	TiXmlNode* node = firstChild;
	TiXmlNode* temp = nullptr;

	while (node)
	{
		temp = node;
		node = node->next;
		delete temp;
	}

	firstChild = nullptr;
	lastChild = nullptr;
}


TiXmlNode* TiXmlNode::LinkEndChild(TiXmlNode* node)
{
	if (!node)
		throw std::invalid_argument("XmlNode::LinkEndChild: Null pointer passed as a parameter.");
	else if (node->parent == nullptr || node->parent == this || node->GetDocument() == nullptr || node->GetDocument() == this->GetDocument())
		throw std::runtime_error("XmlNode::LinkEndChild: Unrecoverable error."); // this is not a helpful message.

	if (node->Type() == TiXmlNode::TINYXML_DOCUMENT)
	{
		delete node;
		if (GetDocument())
			GetDocument()->SetError(TIXML_ERROR_DOCUMENT_TOP_ONLY, 0, 0, TIXML_ENCODING_UNKNOWN);
		return nullptr;
	}

	node->parent = this;

	node->prev = lastChild;
	node->next = nullptr;

	if (lastChild)
		lastChild->next = node;
	else
		firstChild = node;			// it was an empty list.

	lastChild = node;
	return node;
}


TiXmlNode* TiXmlNode::InsertEndChild(const TiXmlNode& addThis)
{
	if (addThis.Type() == TiXmlNode::TINYXML_DOCUMENT)
	{
		if (GetDocument())
			GetDocument()->SetError(TIXML_ERROR_DOCUMENT_TOP_ONLY, 0, 0, TIXML_ENCODING_UNKNOWN);
		return nullptr;
	}
	TiXmlNode* node = addThis.Clone();
	if (!node)
		return nullptr;

	return LinkEndChild(node);
}


TiXmlNode* TiXmlNode::InsertBeforeChild(TiXmlNode* beforeThis, const TiXmlNode& addThis)
{
	if (!beforeThis || beforeThis->parent != this)
		return nullptr;

	if (addThis.Type() == TiXmlNode::TINYXML_DOCUMENT)
	{
		if (GetDocument())
			GetDocument()->SetError(TIXML_ERROR_DOCUMENT_TOP_ONLY, 0, 0, TIXML_ENCODING_UNKNOWN);
		return nullptr;
	}

	TiXmlNode* node = addThis.Clone();
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


TiXmlNode* TiXmlNode::InsertAfterChild(TiXmlNode* afterThis, const TiXmlNode& addThis)
{
	if (!afterThis || afterThis->parent != this) {
		return nullptr;
	}
	if (addThis.Type() == TiXmlNode::TINYXML_DOCUMENT)
	{
		if (GetDocument())
			GetDocument()->SetError(TIXML_ERROR_DOCUMENT_TOP_ONLY, 0, 0, TIXML_ENCODING_UNKNOWN);
		return nullptr;
	}

	TiXmlNode* node = addThis.Clone();
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


TiXmlNode* TiXmlNode::ReplaceChild(TiXmlNode* replaceThis, const TiXmlNode& withThis)
{
	if (!replaceThis)
		return nullptr;

	if (replaceThis->parent != this)
		return nullptr;

	if (withThis.ToDocument())
	{
		// A document can never be a child.	Thanks to Noam.
		TiXmlDocument* document = GetDocument();
		if (document)
			document->SetError(TIXML_ERROR_DOCUMENT_TOP_ONLY, 0, 0, TIXML_ENCODING_UNKNOWN);
		return nullptr;
	}

	TiXmlNode* node = withThis.Clone();
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


bool TiXmlNode::RemoveChild(TiXmlNode* removeThis)
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

const TiXmlNode* TiXmlNode::FirstChild(const std::string& _value) const
{
	const TiXmlNode* node = nullptr;
	for (node = firstChild; node; node = node->next)
	{
		if (node->Value() == _value)
			return node;
	}
	return nullptr;
}


const TiXmlNode* TiXmlNode::LastChild(const std::string& _value) const
{
	const TiXmlNode* node = nullptr;
	for (node = lastChild; node; node = node->prev)
	{
		if (node->Value() == _value)
			return node;
	}
	return nullptr;
}


const TiXmlNode* TiXmlNode::IterateChildren(const TiXmlNode* previous) const
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


const TiXmlNode* TiXmlNode::IterateChildren(const std::string& val, const TiXmlNode* previous) const
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


const TiXmlNode* TiXmlNode::NextSibling(const std::string& _value) const
{
	const TiXmlNode* node = nullptr;
	for (node = next; node; node = node->next)
	{
		if (node->Value() == _value)
			return node;
	}
	return nullptr;
}


const TiXmlNode* TiXmlNode::PreviousSibling(const std::string& _value) const
{
	const TiXmlNode* node = nullptr;
	for (node = prev; node; node = node->prev)
	{
		if (node->Value() == _value)
			return node;
	}
	return nullptr;
}


const TiXmlElement* TiXmlNode::FirstChildElement() const
{
	const TiXmlNode* node = nullptr;

	for (node = FirstChild(); node; node = node->NextSibling())
	{
		if (node->ToElement())
			return node->ToElement();
	}
	return nullptr;
}


const TiXmlElement* TiXmlNode::FirstChildElement(const std::string& _value) const
{
	const TiXmlNode* node = nullptr;

	for (node = FirstChild(_value);	node; node = node->NextSibling(_value))
	{
		if (node->ToElement())
			return node->ToElement();
	}
	return nullptr;
}


const TiXmlElement* TiXmlNode::NextSiblingElement() const
{
	const TiXmlNode* node = nullptr;

	for (node = NextSibling(); node; node = node->NextSibling())
	{
		if (node->ToElement())
			return node->ToElement();
	}
	return nullptr;
}


const TiXmlElement* TiXmlNode::NextSiblingElement(const std::string&  _value) const
{
	const TiXmlNode* node = nullptr;

	for (node = NextSibling(_value); node; node = node->NextSibling(_value))
	{
		if (node->ToElement())
			return node->ToElement();
	}

	return nullptr;
}


const TiXmlDocument* TiXmlNode::GetDocument() const
{
	const TiXmlNode* node;

	for (node = this; node; node = node->parent)
	{
		if (node->ToDocument())
			return node->ToDocument();
	}
	return nullptr;
}

