#pragma once

#include "NAS2D/Xml/XmlNode.h"

namespace NAS2D {
namespace Xml {

/** Always the top level node. A document binds together all the
	XML pieces. It can be saved, loaded, and printed to the screen.
	The 'value' of a document node is the xml file name.
*/
class XmlDocument : public XmlNode
{
public:
	XmlDocument();
	XmlDocument(const std::string& documentName);

	XmlDocument(const XmlDocument& copy);
	XmlDocument& operator=(const XmlDocument& copy);

	virtual ~XmlDocument() {}

	virtual const char* parse(const char* p, TiXmlParsingData* data = nullptr);

	const XmlElement* rootElement() const;
	XmlElement* rootElement();

	bool error() const;
	const std::string& errorDesc() const;
	int errorId() const;
	int errorRow() const;
	int errorCol() const;

	void clearError();

	virtual const XmlDocument* toDocument() const { return this; }
	virtual XmlDocument* toDocument() { return this; }

	virtual bool accept(XmlVisitor* content) const;
	virtual void write(std::string& buf, int depth = 0) const;

public:
	void error(XmlErrorCode err, const char* errorLocation, TiXmlParsingData* prevData);

protected:
	virtual XmlNode* clone() const;
	virtual void streamIn(std::istream& in, std::string& tag);

private:
	void copyTo(XmlDocument* target) const;

private:
	int				_errorId;

	bool			_error;

	std::string		_errorDesc;

	XmlCursor		_errorLocation;
};

} // namespace Xml
} // namespace NAS2D
