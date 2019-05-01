// ==================================================================================
// = NAS2D
// = Copyright © 2008 - 2019 New Age Software
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
	XmlMemoryBuffer();

	virtual bool visitEnter(const XmlDocument& doc) { return true; }
	virtual bool visitExit(const XmlDocument& doc) { return true; }

	virtual bool visitEnter(const XmlElement& element, const XmlAttribute* firstAttribute);
	virtual bool visitExit(const XmlElement& element);

	virtual bool visit(const XmlText& text);
	virtual bool visit(const XmlComment& comment);
	virtual bool visit(const XmlUnknown& unknown);

	size_t size();

	const std::string& buffer();

private:
	int depth;

	std::string _buffer;
	std::string _indent;
	std::string _lineBreak;
};

} // namespace Xml
} // namespace NAS2D
