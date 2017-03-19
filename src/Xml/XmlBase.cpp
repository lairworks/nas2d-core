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
#include "NAS2D/Xml/Xml.h"

using namespace NAS2D::Xml;

bool XmlBase::condenseWhiteSpace = true;

std::vector<std::string> XML_ERROR_TABLE;

void fillErrorTable()
{
	if (!XML_ERROR_TABLE.empty())
		return;

	XML_ERROR_TABLE.push_back("No error");
	XML_ERROR_TABLE.push_back("Unspecified Error");
	XML_ERROR_TABLE.push_back("Error parsing Element.");
	XML_ERROR_TABLE.push_back("Failed to read Element name.");
	XML_ERROR_TABLE.push_back("Error reading Element value.");
	XML_ERROR_TABLE.push_back("Error reading Attributes.");
	XML_ERROR_TABLE.push_back("Error: Empty tag.");
	XML_ERROR_TABLE.push_back("Error reading end tag.");
	XML_ERROR_TABLE.push_back("Error parsing Unknown.");
	XML_ERROR_TABLE.push_back("Error parsing Comment.");
	XML_ERROR_TABLE.push_back("Error parsing Declaration.");
	XML_ERROR_TABLE.push_back("Error: Document empty.");
	XML_ERROR_TABLE.push_back("Error: Unexpected EOF found in input stream.");
	XML_ERROR_TABLE.push_back("Error parsing CDATA.");
	XML_ERROR_TABLE.push_back("Error adding XmlDocument to document: XmlDocument can only be at the root.");
}


/**
 * Return the position, in the original source file, of this node or attribute.
 *
 * Generally, the row and column value will be set when the XmlDocument::Load(),
 * XmlDocument::LoadFile(), or any XmlNode::Parse() is called. It will NOT be set
 * when the DOM was created from operator>>.
 *
 * The values reflect the initial load. Once the DOM is modified programmatically
 * (by adding or changing nodes and attributes) the new values will NOT update to
 * reflect changes in the document.
 *
 * There is a minor performance cost to computing the row and column. Computation
 * can be disabled if XmlDocument::SetTabSize() is called with 0 as the value.
 *
 * \see XmlDocument::SetTabSize()
 */
int XmlBase::row() const
{
	return location.row + 1;
}

int XmlBase::column() const { return location.col + 1; }	///< See Row()

