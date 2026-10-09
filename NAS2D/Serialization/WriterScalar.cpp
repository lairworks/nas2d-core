#include "WriterScalar.h"

#include "../Xml/XmlElement.h"


using namespace NAS2D;


WriterScalar::WriterScalar(Xml::XmlElement& parentElement, std::string_view attributeName) :
	mParentElement{parentElement},
	mAttributeName{attributeName}
{
}


void WriterScalar::write(std::string_view value)
{
	mParentElement.attribute(mAttributeName, value);
}
