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

#include "XmlBase.h"
#include "XmlVisitor.h"

#include <string>


namespace NAS2D::Xml
{

/**
 * The parent class for everything in the DOM (Except for attributes).
 *
 * Nodes have siblings, a parent, and children. A node can be in a document, or stand
 * on its own. The type of a XmlNode can be queried, and it can be cast to its more
 * defined type.
 */
class XmlNode : public XmlBase
{
public:
	/**
	 * The types of XML nodes supported by TinyXml. (All the unsupported
	 * types are picked up by UNKNOWN.)
	 */
	enum class NodeType
	{
		XML_DOCUMENT,
		XML_ELEMENT,
		XML_COMMENT,
		XML_UNKNOWN,
		XML_TEXT,
		XML_TYPECOUNT
	};

	XmlNode();
	XmlNode(const XmlNode&) = delete;
	void operator=(const XmlNode& base) = delete;
	~XmlNode() override;

	const std::string& value() const;
	void value(const std::string& value);

	void clear();

	XmlNode::NodeType type() const;

	bool hasChildren() const;

	XmlNode* parent();
	const XmlNode* parent() const;

	const XmlNode* firstChild() const;
	XmlNode* firstChild();
	const XmlNode* firstChild(const std::string& value) const;
	XmlNode* firstChild(const std::string& value);

	const XmlNode* lastChild() const;
	XmlNode* lastChild();
	const XmlNode* lastChild(const std::string& value) const;
	XmlNode* lastChild(const std::string& _value);

	const XmlNode* iterateChildren(const XmlNode* previous) const;
	XmlNode* iterateChildren(const XmlNode* previous);

	const XmlNode* iterateChildren(const std::string& value, const XmlNode* previous) const;
	XmlNode* iterateChildren(const std::string& value, const XmlNode* previous);

	XmlNode* insertEndChild(const XmlNode& addThis);
	XmlNode* linkEndChild(XmlNode* addThis);
	XmlNode* insertBeforeChild(XmlNode* beforeThis, const XmlNode& addThis);
	XmlNode* insertAfterChild(XmlNode* afterThis, const XmlNode& addThis);
	XmlNode* replaceChild(XmlNode* replaceThis, const XmlNode& withThis);

	bool removeChild(XmlNode* node);

	const XmlNode* previousSibling() const;
	XmlNode* previousSibling();

	const XmlNode* previousSibling(const std::string&) const;
	XmlNode* previousSibling(const std::string& prev);

	const XmlNode* nextSibling() const;
	XmlNode* nextSibling();

	const XmlNode* nextSibling(const std::string&) const;
	XmlNode* nextSibling(const std::string& next);

	const XmlElement* nextSiblingElement() const;
	XmlElement* nextSiblingElement();

	const XmlElement* nextSiblingElement(const std::string&) const;
	XmlElement* nextSiblingElement(const std::string& next);

	const XmlElement* firstChildElement() const;
	XmlElement* firstChildElement();

	const XmlElement* firstChildElement(const std::string& value) const;
	XmlElement* firstChildElement(const std::string& value);

	const XmlDocument* document() const;
	XmlDocument* document();

	/// Cast the XmlNode to a more derived type.
	virtual const XmlDocument* toDocument() const { return nullptr; }
	/// Cast the XmlNode to a more derived type.
	virtual const XmlElement* toElement() const { return nullptr; }
	/// Cast the XmlNode to a more derived type.
	virtual const XmlComment* toComment() const { return nullptr; }
	/// Cast the XmlNode to a more derived type.
	virtual const XmlUnknown* toUnknown() const { return nullptr; }
	/// Cast the XmlNode to a more derived type.
	virtual const XmlText* toText() const { return nullptr; }

	/// Cast the XmlNode to a more derived type.
	virtual XmlDocument* toDocument() { return nullptr; }
	/// Cast the XmlNode to a more derived type.
	virtual XmlElement* toElement() { return nullptr; }
	/// Cast the XmlNode to a more derived type.
	virtual XmlComment* toComment() { return nullptr; }
	/// Cast the XmlNode to a more derived type.
	virtual XmlUnknown* toUnknown() { return nullptr; }
	/// Cast the XmlNode to a more derived type.
	virtual XmlText* toText() { return nullptr; }

	/**
	 * Create an exact duplicate of this node and return it. The memory must be deleted by the caller.
	 */
	virtual XmlNode* clone() const = 0;

	/**
	 * Accept a hierchical visit the nodes in the TinyXML DOM. Every node in the  XML tree will be
	 * conditionally visited and the host will be called back via the TiXmlVisitor interface.
	 */
	virtual bool accept(void* visitor) const = 0;

	// The real work of the input operator.
	virtual void streamIn(std::istream& in, std::string& tag) = 0;

protected:
	explicit XmlNode(NodeType _type);

	void copyTo(XmlNode* target) const;
	XmlNode* identify(const char* start);

	XmlNode* _parent;
	XmlNode* _firstChild;
	XmlNode* _lastChild;
	XmlNode* _prev;
	XmlNode* _next;
	std::string _value{};
	NodeType _type;
	int _padding{};

private:
	friend class XmlDocument;
	friend class XmlElement;
};


}
