// ==================================================================================
// = NAS2D
// = Copyright © 2008 - 2017 New Age Software
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
 * Any tag that's unrecogmized is saved as an unknown. It is a tag of text,
 * but should not be modified. It will be written back to the XML, unchanged,
 * when the file is saved.
 * 
 * Declarations and DTD tags get thrown into XmlUnknown's.
 */
class XmlUnknown : public XmlNode
{
public:
	XmlUnknown();
	virtual ~XmlUnknown();

	XmlUnknown(const XmlUnknown& copy);
	XmlUnknown& operator=(const XmlUnknown& copy);

	virtual XmlNode* Clone() const;
	
	/**
	 * Print the Unknown to a buffer.
	 */
	virtual void Print(std::string& buf, int depth) const;

	virtual const char* Parse(const char* p, TiXmlParsingData* data, XmlEncoding encoding);

	virtual const XmlUnknown* ToUnknown() const { return this; }
	virtual XmlUnknown* ToUnknown() { return this; }

	/**
	 * Walk the XML tree visiting this node and all of its children.
	 */
	virtual bool Accept(XmlVisitor* visitor) const;

protected:
	void CopyTo(XmlUnknown* target) const;
	virtual void StreamIn(std::istream& in, std::string& tag);
};

} // namespace Xml
} // namespace NAS2D
