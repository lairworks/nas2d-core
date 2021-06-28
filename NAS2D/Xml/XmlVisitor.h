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

namespace NAS2D {
namespace Xml {

class XmlAttribute;
class XmlComment;
class XmlDocument;
class XmlElement;
class XmlText;
class XmlUnknown;

class XmlVisitor
{
public:
	virtual ~XmlVisitor() = default;

	virtual bool visitEnter(const XmlDocument&) { return true; }
	virtual bool visitExit(const XmlDocument&) { return true; }

	virtual bool visitEnter(const XmlElement&, const XmlAttribute*) { return true; }
	virtual bool visitExit(const XmlElement&) { return true; }

	virtual bool visit(const XmlText&) { return true; }
	virtual bool visit(const XmlComment&) { return true; }
	virtual bool visit(const XmlUnknown&) { return true; }
};

} // namespace Xml
} // namespace NAS2D
