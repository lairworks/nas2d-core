#include "NAS2D/Xml/XmlDocument.h"

using namespace NAS2D::Xml;

// ==================================================================================
// = XmlDocument Implementation
// ==================================================================================
XmlDocument::XmlDocument() :	XmlNode(XmlNode::XML_DOCUMENT),
								_errorId(0),
								_tabsize(4),
								_error(false)							
{
	clearError();
}


XmlDocument::XmlDocument(const std::string& documentName) :	XmlNode(XmlNode::XML_DOCUMENT),
															_errorId(0),
															_tabsize(4),
															_error(false)
{
	_value = documentName;
	clearError();
}


XmlDocument::XmlDocument(const XmlDocument& copy) : XmlNode(XmlNode::XML_DOCUMENT)
{
	copy.copyTo(this);
}


XmlDocument& XmlDocument::operator=(const XmlDocument& copy)
{
	clear();
	copy.copyTo(this);
	return *this;
}


void XmlDocument::CopyTo(XmlDocument* target) const
{
	XmlNode::copyTo(target);

	target->_error = _error;
	target->_errorId = _errorId;
	target->_errorDesc = _errorDesc;
	target->_tabsize = _tabsize;
	target->_errorLocation = _errorLocation;

	XmlNode* node = nullptr;
	for (node = _firstChild; node; node = node->nextSibling())
		target->linkEndChild(node->clone());
}


XmlNode* XmlDocument::clone() const
{
	XmlDocument* clone = new XmlDocument();
	if (!clone)
		return nullptr;

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
bool XmlDocument::accept(XmlVisitor* visitor) const
{
	if (visitor->visitEnter(*this))
	{
		for (const XmlNode* node = firstChild(); node; node = node->nextSibling())
		{
			if (!node->accept(visitor))
				break;
		}
	}

	return visitor->visitExit(*this);
}


/**
 * If you have handled the error, it can be reset with this call. The error
 * state is automatically cleared if you parse a new XML block.
 */
void XmlDocument::clearError()
{
	_error = false;
	_errorId = 0;
	_errorDesc = "";
	_errorLocation(0, 0);
}
