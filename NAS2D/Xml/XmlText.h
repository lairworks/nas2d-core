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

class XmlText : public XmlNode
{
	friend class XmlElement;
public:

	explicit XmlText(const std::string& initValue);

	XmlText(const XmlText& copy);
	XmlText& operator=(const XmlText& base);

	void write(std::string& buf, int depth) const override;

	bool CDATA() const { return cdata; }

	//TODO: Don't prefix function arguments with underscores
	void CDATA(bool _cdata) { cdata = _cdata; }

	const char* parse(const char* p, void* data) override;

	const XmlText* toText() const override { return this; }
	XmlText* toText() override { return this; }

	bool accept(void* visitor) const override;

protected:
	XmlNode* clone() const override;
	void copyTo(XmlText* target) const;

	bool blank() const;
	void streamIn(std::istream& in, std::string& tag) override;

private:
	bool cdata;
};

} // namespace Xml
} // namespace NAS2D
