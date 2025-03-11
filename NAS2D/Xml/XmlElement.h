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

#include "XmlAttribute.h"
#include "XmlAttributeSet.h"
#include "XmlNode.h"

#include <string>


namespace NAS2D
{
namespace Xml {

/**
 * The XmlElement is a container class. It has a value, the element name, and can
 * contain other elements, text, comments, and unknowns. Elements may also contain
 * an arbitrary number of attributes.
 */
class XmlElement : public XmlNode
{
public:
	explicit XmlElement(const std::string& _value);
	XmlElement(const XmlElement& copy);
	~XmlElement() override;

	XmlElement& operator=(const XmlElement& base);

	void removeAttribute(const std::string& name);

	std::string attribute(const std::string& name) const;
	void attribute(const std::string& name, int i);
	void attribute(const std::string& name, double d);
	void attribute(const std::string& name, const std::string& s);

	const XmlAttribute* firstAttribute() const;
	XmlAttribute* firstAttribute();

	const XmlAttribute* lastAttribute() const;
	XmlAttribute* lastAttribute();

	XmlNode* clone() const override;

	// Convenience function for easy access to the text inside an element
	std::string getText() const;

	void write(std::string& buf, int depth) const override;

	const char* parse(const char* p, void* data) override;

	const XmlElement* toElement() const override { return this; }
	XmlElement* toElement() override { return this; }

	bool accept(void* visitor) const override;

protected:
	void copyTo(XmlElement* target) const;
	void clearThis(); // like clear, but initializes 'this' object as well
	void streamIn(std::istream& in, std::string& tag) override;
	const char* readValue(const char* in, void* prevData);

private:
	XmlAttributeSet attributeSet{};
};

} // namespace Xml
} // namespace NAS2D
