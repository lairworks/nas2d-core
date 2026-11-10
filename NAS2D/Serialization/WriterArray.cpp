#include "WriterArray.h"

#include "WriterObject.h"

#include "../Xml/XmlElement.h"


using namespace NAS2D;


WriterArray::WriterArray(Xml::XmlElement& parentElement, std::string_view arrayElementName) :
	mParentElement{parentElement},
	mArrayElementName{arrayElementName}
{
}


WriterObject WriterArray::object()
{
	auto* newObjectElement = new Xml::XmlElement{mArrayElementName};
	mParentElement.linkEndChild(newObjectElement);
	return WriterObject{*newObjectElement};
}
