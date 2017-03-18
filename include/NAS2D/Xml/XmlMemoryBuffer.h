// ==================================================================================
// = NAS2D
// = Copyright © 2008 - 2017 New Age Software
// ==================================================================================
// = NAS2D is distributed under the terms of the zlib license. You are free to copy,
// = modify and distribute the software under the terms of the zlib license.
// = 
// = Acknowledgement of your use of NAS2D is appriciated but is not required.
// ==================================================================================
// = Originally based on TinyXML. See Xml.h for additional details.
// ==================================================================================
#pragma once

#include "NAS2D/Xml/XmlVisitor.h"

#include <string>

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

	virtual bool VisitEnter(const XmlDocument& doc) { return true; }
	virtual bool VisitExit(const XmlDocument& doc) { return true; }

	virtual bool VisitEnter(const XmlElement& element, const XmlAttribute* firstAttribute);
	virtual bool VisitExit(const XmlElement& element);

	virtual bool Visit(const XmlText& text);
	virtual bool Visit(const XmlComment& comment);
	virtual bool Visit(const XmlUnknown& unknown);

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