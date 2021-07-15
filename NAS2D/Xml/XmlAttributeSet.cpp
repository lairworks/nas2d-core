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
#include "XmlAttributeSet.h"

using namespace NAS2D::Xml;


XmlAttributeSet::XmlAttributeSet()
{
	sentinel._next = &sentinel;
	sentinel._prev = &sentinel;
}


XmlAttributeSet::~XmlAttributeSet()
{}


/**
 * Adds an attribute to the set.
 */
void XmlAttributeSet::add(XmlAttribute* attribute)
{
	attribute->_next = &sentinel;
	attribute->_prev = sentinel._prev;

	sentinel._prev->_next = attribute;
	sentinel._prev = attribute;
}


/**
 * Removes an attribute from the set.
 */
void XmlAttributeSet::remove(XmlAttribute* attribute)
{
	XmlAttribute* node;

	for (node = sentinel._next; node != &sentinel; node = node->_next)
	{
		if (node == attribute)
		{
			node->_prev->_next = node->_next;
			node->_next->_prev = node->_prev;
			node->_next = nullptr;
			node->_prev = nullptr;
			return;
		}
	}
	//assert(0); // we tried to remove a non-linked attribute.
}


XmlAttribute* XmlAttributeSet::find(const std::string& name) const
{
	for (XmlAttribute* node = sentinel._next; node != &sentinel; node = node->_next)
	{
		if (node->_name == name)
		{
			return node;
		}
	}
	return nullptr;
}


XmlAttribute* XmlAttributeSet::findOrCreate(const std::string& _name)
{
	XmlAttribute* attrib = find(_name);
	if (!attrib)
	{
		attrib = new XmlAttribute();
		add(attrib);
		attrib->name(_name);
	}

	return attrib;
}
