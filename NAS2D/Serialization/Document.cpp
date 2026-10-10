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
	if (!xmlElement)
	{
		throw std::runtime_error("XML document has no root element. Expected: '" + std::string{rootElementName} + "'");
	}
	if (xmlElement->value() != rootElementName)
	{
		throw std::runtime_error("XML document has unexpected root element. Expected: '" + std::string{rootElementName} + "' Actual: '" + xmlElement->value() + "'");
	}
	return ReaderObject{*xmlElement};
}


WriterObject Document::setRootObject(std::string_view rootElementName)
{
	auto* xmlElement = new Xml::XmlElement{rootElementName};
	mDocument->linkEndChild(xmlElement);
	return WriterObject{*xmlElement};
}
