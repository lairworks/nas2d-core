#include "WriterObject.h"

#include "../Xml/XmlNode.h"
#include "../Xml/XmlElement.h"


using namespace NAS2D;


WriterObject::WriterObject(Xml::XmlElement& element) :
	mElement{element}
{
}


WriterScalar WriterObject::scalar(std::string_view name)
{
	return WriterScalar{mElement, name};
}


WriterObject WriterObject::object(std::string_view name)
{
	auto* newObjectElement = new Xml::XmlElement{name};
	mElement.linkEndChild(newObjectElement);
	return WriterObject{*newObjectElement};
}


WriterArray WriterObject::array(std::string_view name)
{
	return WriterArray{mElement, name};
}
