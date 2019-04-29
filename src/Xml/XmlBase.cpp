// ==================================================================================
// = NAS2D
// = Copyright © 2008 - 2019 New Age Software
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
	if (!XML_ERROR_TABLE.empty()) { return; }

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
 * Get the row of the node in the document.
 *
 * The values reflect the initial load. Once the document is modified programmatically
 * (by adding or changing nodes and attributes) the new values will NOT update to
 * reflect changes in the document.
 */
int XmlBase::row() const
{
	return location.first + 1;
}


/**
 * Get the column of the node in the document.
 *
 * The values reflect the initial load. Once the document is modified programmatically
 * (by adding or changing nodes and attributes) the new values will NOT update to
 * reflect changes in the document.
 */
int XmlBase::column() const
{
	return location.second + 1;
}


/**
 * The world does not agree on whether white space should be kept or not. In order
 * to make everyone happy, these global, static functions are provided to set whether
 * or not TinyXml will condense all white space	into a single space or not. The
 * default is to condense. Note changing this value is not thread safe.
 */
void XmlBase::whitespaceCondensed(bool condense)
{
	condenseWhiteSpace = condense;
}


/**
 * Return the current white space setting.
 */
bool XmlBase::whitespaceCondensed()
{
	return condenseWhiteSpace;
}
