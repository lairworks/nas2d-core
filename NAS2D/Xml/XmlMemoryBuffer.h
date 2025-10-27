// ==================================================================================
// = NAS2D
// = Copyright © 2008 - 2020 New Age Software
// ==================================================================================
// = NAS2D is distributed under the terms of the zlib license. You are free to copy,
// = modify and distribute the software under the terms of the zlib license.
// =
// = Acknowledgment of your use of NAS2D is appreciated but is not required.
// ==================================================================================
// = Originally based on TinyXML. See Xml.h for additional details.
// ==================================================================================
#pragma once

#include "XmlVisitor.h"

#include <string>


namespace NAS2D
{
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
 * Utility<Filesystem>::get().writeFile(filePath, buff.buffer());
 * \endcode
 */
class XmlMemoryBuffer : public XmlVisitor
{
public:
	XmlMemoryBuffer();

	bool visitEnter(const XmlDocument&) override { return true; }
	bool visitExit(const XmlDocument&) override { return true; }

	bool visitEnter(const XmlElement& element, const XmlAttribute* firstAttribute) override;
	bool visitExit(const XmlElement& element) override;

	bool visit(const XmlText& text) override;
	bool visit(const XmlComment& comment) override;
	bool visit(const XmlUnknown& unknown) override;

	std::size_t size();

	const std::string& buffer();

private:
	std::string _buffer{};
	std::string _indent;
	std::string _lineBreak;
	int depth;
protected:
	int _padding{};
};

}
}
