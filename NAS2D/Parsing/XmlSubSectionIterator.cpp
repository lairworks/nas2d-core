#include "XmlSubSectionIterator.h"

#include "XmlSection.h"
#include "../Xml/XmlElement.h"


using namespace NAS2D;


XmlSubSectionIterator::XmlSubSectionIterator(const Xml::XmlElement& currentElement) :
	mXmlElement{&currentElement}
{
}


XmlSubSectionIterator& XmlSubSectionIterator::operator++()
{
	mXmlElement = mXmlElement->nextSiblingElement();
	return *this;
}


bool XmlSubSectionIterator::operator!=(std::nullptr_t)
{
	return mXmlElement != nullptr;
}


XmlSection XmlSubSectionIterator::operator*() const
{
	return XmlSection{*mXmlElement};
}
