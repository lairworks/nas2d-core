#include "XmlNamedSubSectionRange.h"

#include "../Xml/XmlElement.h"

#include <utility>


using namespace NAS2D;


XmlNamedSubSectionRange::XmlNamedSubSectionRange(const Xml::XmlElement& parentElement, std::string name) :
	mParentElement{parentElement},
	mName{std::move(name)}
{
}


XmlNamedSubSectionIterator XmlNamedSubSectionRange::begin() const
{
	return XmlNamedSubSectionIterator{mParentElement.firstChildElement(mName), mName};
}


std::nullptr_t XmlNamedSubSectionRange::end() const
{
	return nullptr;
}
