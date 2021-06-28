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

#include "XmlNode.h"

namespace NAS2D {
namespace Xml {

class XmlHandle
{
public:
	XmlHandle();
	explicit XmlHandle(XmlNode* _node);
	XmlHandle(const XmlHandle& ref);
	XmlHandle operator=(const XmlHandle& ref);

	XmlHandle firstChild() const;
	XmlHandle firstChild(const std::string& value) const;
	XmlHandle firstChildElement() const;
	XmlHandle firstChildElement(const std::string& value) const;

	XmlHandle child(const std::string& value, int index) const;
	XmlHandle child(int index) const;

	XmlHandle childElement(const std::string& value, int index) const;
	XmlHandle childElement(int index) const;

	XmlNode* toNode() const;
	XmlElement* toElement() const;
	XmlText* toText() const;
	XmlUnknown* toUnknown() const;

private:
	XmlNode* node;
};

} // namespace Xml
} // namespace NAS2D
