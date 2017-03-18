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
#include "NAS2D/Xml/XmlHandle.h"

#include "NAS2D/Xml/XmlElement.h"
#include "NAS2D/Xml/XmlText.h"
#include "NAS2D/Xml/XmlUnknown.h"

using namespace NAS2D::Xml;

XmlHandle XmlHandle::FirstChild() const
{
	if (node)
	{
		XmlNode* child = node->FirstChild();
		if (child)
			return XmlHandle(child);
	}
	return XmlHandle(nullptr);
}


XmlHandle XmlHandle::FirstChild(const std::string& value) const
{
	if (node)
	{
		XmlNode* child = node->FirstChild(value);
		if (child)
			return XmlHandle(child);
	}
	return XmlHandle(nullptr);
}


XmlHandle XmlHandle::FirstChildElement() const
{
	if (node)
	{
		XmlElement* child = node->FirstChildElement();
		if (child)
			return XmlHandle(child);
	}
	return XmlHandle(nullptr);
}


XmlHandle XmlHandle::FirstChildElement(const std::string& value) const
{
	if (node)
	{
		XmlElement* child = node->FirstChildElement(value);
		if (child)
			return XmlHandle(child);
	}

	return XmlHandle(nullptr);
}


XmlHandle XmlHandle::Child(int count) const
{
	if (node)
	{
		XmlNode* child = node->FirstChild();
		for (int i = 0; child && i < count; child = child->NextSibling(), ++i)
			; // Nothing
		if (child)
			return XmlHandle(child);
	}

	return XmlHandle(nullptr);
}


XmlHandle XmlHandle::Child(const std::string& value, int count) const
{
	if (node)
	{
		XmlNode* child = node->FirstChild(value);
		for (int i = 0; child && i < count; child = child->NextSibling(value), ++i)
			; // Nothing
		if (child)
			return XmlHandle(child);
	}

	return XmlHandle(nullptr);
}


XmlHandle XmlHandle::ChildElement(int count) const
{
	if (node)
	{
		XmlElement* child = node->FirstChildElement();
		for (int i = 0; child && i < count; child = child->NextSiblingElement(), ++i)
			; // Nothing

		if (child)
			return XmlHandle(child);
	}

	return XmlHandle(nullptr);
}


XmlHandle XmlHandle::ChildElement(const std::string& value, int count) const
{
	if (node)
	{
		XmlElement* child = node->FirstChildElement(value);
		for (int i = 0; child && i < count; child = child->NextSiblingElement(value), ++i)
			; // Nothing
		if (child)
			return XmlHandle(child);
	}

	return XmlHandle(nullptr);
}
