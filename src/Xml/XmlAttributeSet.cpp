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
#include "NAS2D/Xml/XmlAttributeSet.h"

using namespace NAS2D::Xml;

XmlAttributeSet::XmlAttributeSet()
{
	sentinel.next = &sentinel;
	sentinel.prev = &sentinel;
}


XmlAttributeSet::~XmlAttributeSet()
{
	//assert(sentinel.next == &sentinel);
	//assert(sentinel.prev == &sentinel);
}


void XmlAttributeSet::Add(XmlAttribute* addMe)
{
	//assert(!Find(addMe->Name()));	// Shouldn't be multiply adding to the set.

	addMe->next = &sentinel;
	addMe->prev = sentinel.prev;

	sentinel.prev->next = addMe;
	sentinel.prev = addMe;
}


void XmlAttributeSet::Remove(XmlAttribute* removeMe)
{
	XmlAttribute* node;

	for (node = sentinel.next; node != &sentinel; node = node->next)
	{
		if (node == removeMe)
		{
			node->prev->next = node->next;
			node->next->prev = node->prev;
			node->next = 0;
			node->prev = 0;
			return;
		}
	}
	//assert(0);		// we tried to remove a non-linked attribute.
}


XmlAttribute* XmlAttributeSet::Find(const std::string& name) const
{
	for (XmlAttribute* node = sentinel.next; node != &sentinel; node = node->next)
	{
		if (node->name == name)
			return node;
	}
	return nullptr;
}


XmlAttribute* XmlAttributeSet::FindOrCreate(const std::string& _name)
{
	XmlAttribute* attrib = Find(_name);
	if (!attrib) {
		attrib = new XmlAttribute();
		Add(attrib);
		attrib->SetName(_name);
	}
	return attrib;
}
