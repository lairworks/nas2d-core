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

class XmlUnknown : public XmlNode
{
public:
	XmlUnknown();
	~XmlUnknown() override;

	XmlUnknown(const XmlUnknown& copy);
	XmlUnknown& operator=(const XmlUnknown& copy);

	XmlNode* clone() const override;

	void write(std::string& buf, int depth) const override;

	const char* parse(const char* p, void* data) override;

	const XmlUnknown* toUnknown() const override { return this; }
	XmlUnknown* toUnknown() override { return this; }

	bool accept(void* visitor) const override;

protected:
	void copyTo(XmlUnknown* target) const;
	void streamIn(std::istream& in, std::string& tag) override;
};

} // namespace Xml
} // namespace NAS2D
