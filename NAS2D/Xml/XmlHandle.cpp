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

XmlHandle::XmlHandle(XmlNode* _node) : node(_node)
{}

XmlHandle::XmlHandle(const XmlHandle& ref) : node(ref.node)
{}


XmlHandle XmlHandle::operator=(const XmlHandle& ref)
{
	if (&ref != this)
	{
		this->node = ref.node;
	}

	return *this;
}

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

XmlNode* XmlHandle::toNode() const
{
	return node;
}

XmlElement* XmlHandle::toElement() const
{
	return ((node && node->toElement()) ? node->toElement() : nullptr);
}

XmlText* XmlHandle::toText() const
{
	return ((node && node->toText()) ? node->toText() : nullptr);
}

XmlUnknown* XmlHandle::toUnknown() const
{
	return ((node && node->toUnknown()) ? node->toUnknown() : nullptr);
}
