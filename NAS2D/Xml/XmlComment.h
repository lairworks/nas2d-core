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

#include <string>


namespace NAS2D
{
namespace Xml {

/**
 * A Comment in XML.
 */
class XmlComment : public XmlNode
{
public:
	XmlComment();

	explicit XmlComment(const std::string& commentValue);
	XmlComment(const XmlComment& copy);
	XmlComment& operator=(const XmlComment& base);

	XmlNode* clone() const override;

	void write(std::string& buf, int depth) const override;

	const char* parse(const char* p, void* data) override;

	const XmlComment* toComment() const override { return this; }
	XmlComment* toComment() override { return this; }

	bool accept(void* visitor) const override;

protected:
	void copyTo(XmlComment* target) const;
	void streamIn(std::istream& in, std::string& tag) override;

private:

};

} // namespace Xml
}
