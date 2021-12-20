#include "XmlSubSectionRange.h"

#include "../Xml/XmlElement.h"


using namespace NAS2D;


XmlSubSectionRange::XmlSubSectionRange(const Xml::XmlElement& parentElement) :
	mParentElement{parentElement}
{
}


XmlSubSectionIterator XmlSubSectionRange::begin() const
{
	return XmlSubSectionIterator{mParentElement.firstChildElement()};
}


std::nullptr_t XmlSubSectionRange::end() const
{
	return nullptr;
}
