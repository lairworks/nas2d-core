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

int XmlBase::row() const
{
	return location.row + 1;
}

int XmlBase::column() const
{
	return location.column + 1;
}

void XmlBase::whitespaceCondensed(bool condense)
{
	condenseWhiteSpace = condense;
}

bool XmlBase::whitespaceCondensed()
{
	return condenseWhiteSpace;
}
