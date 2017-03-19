#include "NAS2D/Xml/XmlDocument.h"

using namespace NAS2D::Xml;

// ==================================================================================
// = XmlDocument Implementation
// ==================================================================================
XmlDocument::XmlDocument() :	XmlNode(XmlNode::XML_DOCUMENT),
								errorId(0),
								tabsize(4),
								error(false),
								useMicrosoftBOM(false)
{
	ClearError();
}


XmlDocument::XmlDocument(const std::string& documentName) :	XmlNode(XmlNode::XML_DOCUMENT),
															errorId(0),
															tabsize(4),
															error(false),
															useMicrosoftBOM(false)
{
	_value = documentName;
	ClearError();
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

	target->error = error;
	target->errorId = errorId;
	target->errorDesc = errorDesc;
	target->tabsize = tabsize;
	target->errorLocation = errorLocation;
	target->useMicrosoftBOM = useMicrosoftBOM;

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
