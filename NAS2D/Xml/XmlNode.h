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

namespace NAS2D {
namespace Xml {

class XmlNode : public XmlBase
{
public:
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


	virtual const XmlDocument* toDocument() const { return nullptr; }
	virtual const XmlElement* toElement() const { return nullptr; }
	virtual const XmlComment* toComment() const { return nullptr; }
	virtual const XmlUnknown* toUnknown() const { return nullptr; }
	virtual const XmlText* toText() const { return nullptr; }

	virtual XmlDocument* toDocument() { return nullptr; }
	virtual XmlElement* toElement() { return nullptr; }
	virtual XmlComment* toComment() { return nullptr; }
	virtual XmlUnknown* toUnknown() { return nullptr; }
	virtual XmlText* toText() { return nullptr; }

	virtual XmlNode* clone() const = 0;
	virtual bool accept(void* visitor) const = 0;
	virtual void streamIn(std::istream& in, std::string& tag) = 0;

protected:
	explicit XmlNode(NodeType _type);

	void copyTo(XmlNode* target) const;
	XmlNode* identify(const char* start);

	XmlNode* _parent;
	NodeType _type;

	XmlNode* _firstChild;
	XmlNode* _lastChild;

	std::string _value;

	XmlNode* _prev;
	XmlNode* _next;

private:
	friend class XmlDocument;
	friend class XmlElement;
};


} // namespace Xml
} // namespace NAS2D
