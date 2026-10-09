#include "Document.h"

#include "ReaderObject.h"
#include "WriterObject.h"

#include "../Xml/XmlDocument.h"
#include "../Xml/XmlElement.h"

#include <stdexcept>


using namespace NAS2D;


Document::Document() :
	mDocument{new Xml::XmlDocument}
{
}

Document::Document(std::string_view data) :
	Document{}
{
	mDocument->parse(data.data());
}

Document::~Document()
{
	delete mDocument;
}


std::string Document::asString() const
{
	std::string result;
	mDocument->write(result);
	return result;
}


ReaderObject Document::getRootObject(std::string_view rootElementName) const
{
	const auto* xmlElement = mDocument->rootElement();
	if (!xmlElement || xmlElement->value() != rootElementName)
	{
		throw std::runtime_error("Expected root XML element not found: " + std::string{rootElementName});
	}
	return ReaderObject{*xmlElement};
}


WriterObject Document::setRootObject(std::string_view rootElementName)
{
	auto* xmlElement = new Xml::XmlElement{rootElementName};
	mDocument->linkEndChild(xmlElement);
	return WriterObject{*xmlElement};
}
