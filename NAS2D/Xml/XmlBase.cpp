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
#include "Xml.h"

using namespace NAS2D::Xml;

bool XmlBase::condenseWhiteSpace = true;


XmlBase::~XmlBase()
{
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
	return location.row + 1;
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
	return location.column + 1;
}


/**
 * The world does not agree on whether white space should be kept or not. In order
 * to make everyone happy, these global, static functions are provided to set whether
 * or not TinyXml will condense all white space into a single space or not. The
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
