#include "ReaderArray.h"

#include "ReaderObject.h"

#include "../Xml/XmlElement.h"


using namespace NAS2D;


ReaderArray::ReaderArray(const Xml::XmlElement& parentElement, std::string_view arrayElementName) :
	mParentElement{parentElement},
	mArrayElementName{arrayElementName},
	mCurrentElement{mParentElement.firstChildElement(mArrayElementName)}
{
}


bool ReaderArray::hasNext() const
{
	return mCurrentElement;
}


ReaderObject ReaderArray::object()
{
	if (!hasNext())
	{
		throw std::runtime_error("No more elements to read");
	}

	const auto* current = mCurrentElement;
	mCurrentElement = mCurrentElement->nextSiblingElement(mArrayElementName);

	return ReaderObject{*current};
}
