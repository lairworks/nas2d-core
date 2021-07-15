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
#include "XmlDocument.h"

using namespace NAS2D::Xml;

XmlDocument::XmlDocument() :
	XmlNode(XmlNode::NodeType::XML_DOCUMENT),
	_errorId(XmlErrorCode::XML_NO_ERROR),
	_error(false)
{
	clearError();
}


XmlDocument::XmlDocument(const std::string& documentName) :
	XmlNode(XmlNode::NodeType::XML_DOCUMENT),
	_errorId(XmlErrorCode::XML_NO_ERROR),
	_error(false)
{
	_value = documentName;
	clearError();
}


/**
 * \param copy XmlDocument to copy.
 */
XmlDocument::XmlDocument(const XmlDocument& copy) :
	XmlNode(XmlNode::NodeType::XML_DOCUMENT)
{
	copy.copyTo(this);
}


XmlDocument& XmlDocument::operator=(const XmlDocument& copy)
{
	clear();
	copy.copyTo(this);
	return *this;
}


void XmlDocument::copyTo(XmlDocument* target) const
{
	XmlNode::copyTo(target);

	target->_error = _error;
	target->_errorId = _errorId;
	target->_errorDesc = _errorDesc;
	target->_errorLocation = _errorLocation;

	XmlNode* node = nullptr;
	for (node = _firstChild; node; node = node->nextSibling())
	{
		target->linkEndChild(node->clone());
	}
}


XmlNode* XmlDocument::clone() const
{
	XmlDocument* clone = new XmlDocument();
	if (!clone) { return nullptr; }

	copyTo(clone);
	return clone;
}


void XmlDocument::write(std::string& buf, int depth) const
{
	for (const XmlNode* node = firstChild(); node; node = node->nextSibling())
	{
		node->write(buf, depth);
		buf += "\n";
	}
}


/**
 * Walk the XML tree visiting this node and all of its children.
 */
bool XmlDocument::accept(void* visitor) const
{
	if (static_cast<XmlVisitor*>(visitor)->visitEnter(*this))
	{
		for (const XmlNode* node = firstChild(); node; node = node->nextSibling())
		{
			if (!node->accept(visitor))
			{
				break;
			}
		}
	}

	return static_cast<XmlVisitor*>(visitor)->visitExit(*this);
}


/**
 * If you have handled the error, it can be reset with this call. The error
 * state is automatically cleared if you parse a new XML block.
 */
void XmlDocument::clearError()
{
	_error = false;
	_errorId = XmlErrorCode::XML_NO_ERROR;
	_errorDesc = "";
	_errorLocation = XmlBase::ParseLocation{};
}


/**
 * If an error occurs, Error will be set to true. Also,
 * 	- The ErrorId() will contain the integer identifier of the error (not generally useful)
 * 	- The ErrorDesc() method will return the name of the error. (very useful)
 * 	- The ErrorRow() and ErrorCol() will return the location of the error (if known)
 */
bool XmlDocument::error() const
{
	return _error;
}


/**
 * Get the root element -- the only top level element -- of the document. In well formed XML,
 * there should only be one. TinyXml is tolerant of multiple elements at the document level.
 */
const XmlElement* XmlDocument::rootElement() const
{
	return firstChildElement();
}


/**
 * Get the root element -- the only top level element -- of the document. In well formed XML,
 * there should only be one. TinyXml is tolerant of multiple elements at the document level.
 */
XmlElement* XmlDocument::rootElement()
{
	return firstChildElement();
}


/**
 * Gets the error descrition.
 */
const std::string& XmlDocument::errorDesc() const
{
	return _errorDesc;
}


/**
 * Generally, you probably want the error string ( ErrorDesc() ). But if you
 * prefer the ErrorId, this function will fetch it.
 */
XmlBase::XmlErrorCode XmlDocument::errorId() const
{
	return _errorId;
}


/**
 * Returns the location (if known) of the error. The first column is column 1, and the first
 * row is row 1. A value of 0 means the row and column wasn't applicable (memory errors, for
 * example, have no row/column) or the parser lost the error. (An error in the error
 * reporting, in that case.)
 *
 * @sa SetTabSize, Row, Column
 */
int XmlDocument::errorRow() const
{
	return _errorLocation.row + 1;
}


int XmlDocument::errorCol() const
{
	return _errorLocation.column + 1;
}
