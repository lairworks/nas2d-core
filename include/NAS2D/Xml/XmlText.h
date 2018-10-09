// ==================================================================================
// = NAS2D
// = Copyright © 2008 - 2018 New Age Software
// ==================================================================================
// = NAS2D is distributed under the terms of the zlib license. You are free to copy,
// = modify and distribute the software under the terms of the zlib license.
// = 
// = Acknowledgement of your use of NAS2D is appriciated but is not required.
// ==================================================================================
// = Originally based on TinyXML. See Xml.h for additional details.
// ==================================================================================
#pragma once

#include "NAS2D/Xml/XmlNode.h"

namespace NAS2D {
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
	 * If you want it be output as a CDATA text	element, call \c CDATA(true).
	 */
	XmlText(const std::string& initValue);
	virtual ~XmlText() {}

	XmlText(const XmlText& copy);
	XmlText& operator=(const XmlText& base);

	virtual void write(std::string& buf, int depth) const;

	/**
	 * Queries whether this represents text using a CDATA section.
	 */
	bool CDATA() const { return cdata; }

	/**
	 * Turns on or off a CDATA representation of text.
	 */
	void CDATA(bool _cdata) { cdata = _cdata; }

	virtual const char* parse(const char* p, void* data);

	virtual const XmlText* toText() const { return this; }
	virtual XmlText* toText() { return this; }

	virtual bool accept(void* visitor) const;

protected:
	///  [internal use] Creates a new Element and returns it.
	virtual XmlNode* clone() const;
	void copyTo(XmlText* target) const;

	bool blank() const;	// returns true if all white space and new lines
	virtual void streamIn(std::istream& in, std::string& tag);

private:
	bool cdata;			// true if this should be input and output as a CDATA style text element
};

} // namespace Xml
} // namespace NAS2D
