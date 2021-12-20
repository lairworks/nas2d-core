#include "XmlNamedSubSectionIterator.h"

#include "XmlSection.h"
#include "../Xml/XmlElement.h"

#include <utility>


using namespace NAS2D;


XmlNamedSubSectionIterator::XmlNamedSubSectionIterator(const Xml::XmlElement* currentElement, std::string name) :
	mXmlElement{currentElement},
	mName{std::move(name)}
{
}


XmlNamedSubSectionIterator& XmlNamedSubSectionIterator::operator++()
{
	mXmlElement = mXmlElement->nextSiblingElement(mName);
	return *this;
}


bool XmlNamedSubSectionIterator::operator!=(const XmlNamedSubSectionIterator& other)
{
	return mXmlElement != other.mXmlElement;
}


XmlSection XmlNamedSubSectionIterator::operator*() const
{
	return XmlSection{*mXmlElement};
}
