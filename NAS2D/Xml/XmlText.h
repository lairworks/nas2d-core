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
 * XML text.
 * A text node has two ways to output text: "normal" output and CDATA. It will default
 * to the mode it was parsed from the XML file.
 */
class XmlText : public XmlNode
{
	friend class XmlElement;
public:
	/**
	 * Constructor for text element. By default, it is treated as normal, encoded text.
	 * If you want it be output as a CDATA text element, call \c CDATA(true).
	 */
	explicit XmlText(const std::string& initValue);

	XmlText(const XmlText& copy);
	XmlText& operator=(const XmlText& base);

	void write(std::string& buf, int depth) const override;

	/**
	 * Queries whether this represents text using a CDATA section.
	 */
	bool CDATA() const { return cdata; }

	/**
	 * Turns on or off a CDATA representation of text.
	 */
	void CDATA(bool _cdata) { cdata = _cdata; }

	const char* parse(const char* p, void* data) override;

	const XmlText* toText() const override { return this; }
	XmlText* toText() override { return this; }

	bool accept(void* visitor) const override;

protected:
	///  [internal use] Creates a new Element and returns it.
	XmlNode* clone() const override;
	void copyTo(XmlText* target) const;

	bool blank() const; // returns true if all white space and new lines
	void streamIn(std::istream& in, std::string& tag) override;

private:
	bool cdata{}; // true if this should be input and output as a CDATA style text element
	char padding[7]{};
};

} // namespace Xml
}
