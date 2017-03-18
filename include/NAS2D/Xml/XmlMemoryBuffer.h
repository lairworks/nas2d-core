#pragma once

#include "NAS2D/Xml/XmlVisitor.h"

namespace NAS2D {
namespace Xml {

/**
* Writes an XML Document to a memory buffer.
*
* Once you have a buffer containing the XML document, you can
* save it to disk using the Filesystem class.
*
* \code{.cpp}
* XmlDocument doc;
* XmlMemoryBuffer buff;
* doc.Accept(&buff);
*
* Utility<Filesystem>::get().write(File(buff.buffer(), mConfigPath));
* \endcode
*/
class XmlMemoryBuffer : public XmlVisitor
{
public:
	XmlMemoryBuffer() : depth(0), _indent("\t"), _lineBreak("\n") {}

	virtual bool VisitEnter(const TiXmlDocument& doc) { return true; }
	virtual bool VisitExit(const TiXmlDocument& doc) { return true; }

	virtual bool VisitEnter(const TiXmlElement& element, const TiXmlAttribute* firstAttribute);
	virtual bool VisitExit(const TiXmlElement& element);

	virtual bool Visit(const TiXmlText& text);
	virtual bool Visit(const TiXmlComment& comment);
	virtual bool Visit(const TiXmlUnknown& unknown);

	size_t size() { return _buffer.size(); }

	const std::string& buffer() { return _buffer; }

private:
	void indent() { for (int i = 0; i < depth; ++i) _buffer += _indent; }
	void line_break() { _buffer += _lineBreak; }

private:
	int depth;

	std::string _buffer;
	std::string _indent;
	std::string _lineBreak;
};

} // namespace Xml
} // namespace NAS2D