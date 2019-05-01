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

namespace NAS2D {
namespace Xml {

class XmlAttribute;
class XmlComment;
class XmlDocument;
class XmlElement;
class XmlText;
class XmlUnknown;

/**
 * Implements the interface to the "Visitor pattern" (see the Accept() method.) If you call
 * the accept() method, it requires being passed an XmlVisitor class to handle callbacks.
 * For nodes that contain other nodes (Document, Element) you will get called with a
 * VisitEnter/VisitExit pair. Nodes that are always leaves are simply called with Visit().
 *
 * If you return 'true' from a Visit method, recursive parsing will continue. If you return
 * false, <b>no children of this node or its sibilings</b> will be visited.
 *
 * All flavors of Visit methods have a default implementation that returns 'true' (continue
 * visiting). You need to only override methods that are interesting to you.
 *
 * Generally accept() is called on the XmlDocument, although all nodes suppert Visiting.
 *
 * \see XmlNode::accept()
 */
class XmlVisitor
{
public:
	virtual ~XmlVisitor() {}

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
