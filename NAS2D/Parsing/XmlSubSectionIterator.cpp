#include "XmlSubSectionIterator.h"

#include "XmlSection.h"
#include "../Xml/XmlElement.h"


using namespace NAS2D;


XmlSubSectionIterator::XmlSubSectionIterator(const Xml::XmlElement* currentElement) :
	mXmlElement{currentElement}
{
}


XmlSubSectionIterator& XmlSubSectionIterator::operator++()
{
	mXmlElement = mXmlElement->nextSiblingElement();
	return *this;
}


bool XmlSubSectionIterator::operator!=(const XmlSubSectionIterator& other)
{
	return mXmlElement != other.mXmlElement;
}


XmlSection XmlSubSectionIterator::operator*() const
{
	return XmlSection{*mXmlElement};
}
