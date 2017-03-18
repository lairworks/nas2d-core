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

TiXmlHandle TiXmlHandle::FirstChild() const
{
	if (node)
	{
		TiXmlNode* child = node->FirstChild();
		if (child)
			return TiXmlHandle(child);
	}
	return TiXmlHandle(nullptr);
}


TiXmlHandle TiXmlHandle::FirstChild(const std::string& value) const
{
	if (node)
	{
		TiXmlNode* child = node->FirstChild(value);
		if (child)
			return TiXmlHandle(child);
	}
	return TiXmlHandle(nullptr);
}


TiXmlHandle TiXmlHandle::FirstChildElement() const
{
	if (node)
	{
		TiXmlElement* child = node->FirstChildElement();
		if (child)
			return TiXmlHandle(child);
	}
	return TiXmlHandle(nullptr);
}


TiXmlHandle TiXmlHandle::FirstChildElement(const std::string& value) const
{
	if (node)
	{
		TiXmlElement* child = node->FirstChildElement(value);
		if (child)
			return TiXmlHandle(child);
	}

	return TiXmlHandle(nullptr);
}


TiXmlHandle TiXmlHandle::Child(int count) const
{
	if (node)
	{
		TiXmlNode* child = node->FirstChild();
		for (int i = 0; child && i < count; child = child->NextSibling(), ++i)
			; // Nothing
		if (child)
			return TiXmlHandle(child);
	}

	return TiXmlHandle(nullptr);
}


TiXmlHandle TiXmlHandle::Child(const std::string& value, int count) const
{
	if (node)
	{
		TiXmlNode* child = node->FirstChild(value);
		for (int i = 0; child && i < count; child = child->NextSibling(value), ++i)
			; // Nothing
		if (child)
			return TiXmlHandle(child);
	}

	return TiXmlHandle(nullptr);
}


TiXmlHandle TiXmlHandle::ChildElement(int count) const
{
	if (node)
	{
		TiXmlElement* child = node->FirstChildElement();
		for (int i = 0; child && i < count; child = child->NextSiblingElement(), ++i)
			; // Nothing

		if (child)
			return TiXmlHandle(child);
	}

	return TiXmlHandle(nullptr);
}


TiXmlHandle TiXmlHandle::ChildElement(const std::string& value, int count) const
{
	if (node)
	{
		TiXmlElement* child = node->FirstChildElement(value);
		for (int i = 0; child && i < count; child = child->NextSiblingElement(value), ++i)
			; // Nothing
		if (child)
			return TiXmlHandle(child);
	}

	return TiXmlHandle(nullptr);
}
