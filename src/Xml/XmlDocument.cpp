#include "NAS2D/Xml/XmlDocument.h"

using namespace NAS2D::Xml;

// ==================================================================================
// = XmlDocument Implementation
// ==================================================================================
TiXmlDocument::TiXmlDocument() :	TiXmlNode(TiXmlNode::TINYXML_DOCUMENT),
									errorId(0),
									tabsize(4),
									error(false),
									useMicrosoftBOM(false)
{
	ClearError();
}


TiXmlDocument::TiXmlDocument(const std::string& documentName) : TiXmlNode(TiXmlNode::TINYXML_DOCUMENT),
																errorId(0),
																tabsize(4),
																error(false),
																useMicrosoftBOM(false)
{
	value = documentName;
	ClearError();
}


TiXmlDocument::TiXmlDocument(const TiXmlDocument& copy) : TiXmlNode(TiXmlNode::TINYXML_DOCUMENT)
{
	copy.CopyTo(this);
}


TiXmlDocument& TiXmlDocument::operator=(const TiXmlDocument& copy)
{
	Clear();
	copy.CopyTo(this);
	return *this;
}


void TiXmlDocument::CopyTo(TiXmlDocument* target) const
{
	TiXmlNode::CopyTo(target);

	target->error = error;
	target->errorId = errorId;
	target->errorDesc = errorDesc;
	target->tabsize = tabsize;
	target->errorLocation = errorLocation;
	target->useMicrosoftBOM = useMicrosoftBOM;

	TiXmlNode* node = nullptr;
	for (node = firstChild; node; node = node->NextSibling())
		target->LinkEndChild(node->Clone());
}


TiXmlNode* TiXmlDocument::Clone() const
{
	TiXmlDocument* clone = new TiXmlDocument();
	if (!clone)
		return nullptr;

	CopyTo(clone);
	return clone;
}


void TiXmlDocument::Print(std::string& buf, int depth) const
{
	for (const TiXmlNode* node = FirstChild(); node; node = node->NextSibling())
	{
		node->Print(buf, depth);
		buf += "\n";
	}
}


bool TiXmlDocument::Accept(XmlVisitor* visitor) const
{
	if (visitor->VisitEnter(*this))
	{
		for (const TiXmlNode* node = FirstChild(); node; node = node->NextSibling())
		{
			if (!node->Accept(visitor))
				break;
		}
	}

	return visitor->VisitExit(*this);
}
