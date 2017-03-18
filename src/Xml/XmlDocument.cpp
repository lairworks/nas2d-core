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
	value = documentName;
	ClearError();
}


XmlDocument::XmlDocument(const XmlDocument& copy) : XmlNode(XmlNode::XML_DOCUMENT)
{
	copy.CopyTo(this);
}


XmlDocument& XmlDocument::operator=(const XmlDocument& copy)
{
	Clear();
	copy.CopyTo(this);
	return *this;
}


void XmlDocument::CopyTo(XmlDocument* target) const
{
	XmlNode::CopyTo(target);

	target->error = error;
	target->errorId = errorId;
	target->errorDesc = errorDesc;
	target->tabsize = tabsize;
	target->errorLocation = errorLocation;
	target->useMicrosoftBOM = useMicrosoftBOM;

	XmlNode* node = nullptr;
	for (node = firstChild; node; node = node->NextSibling())
		target->LinkEndChild(node->Clone());
}


XmlNode* XmlDocument::Clone() const
{
	XmlDocument* clone = new XmlDocument();
	if (!clone)
		return nullptr;

	CopyTo(clone);
	return clone;
}


void XmlDocument::Print(std::string& buf, int depth) const
{
	for (const XmlNode* node = FirstChild(); node; node = node->NextSibling())
	{
		node->Print(buf, depth);
		buf += "\n";
	}
}


bool XmlDocument::Accept(XmlVisitor* visitor) const
{
	if (visitor->VisitEnter(*this))
	{
		for (const XmlNode* node = FirstChild(); node; node = node->NextSibling())
		{
			if (!node->Accept(visitor))
				break;
		}
	}

	return visitor->VisitExit(*this);
}
