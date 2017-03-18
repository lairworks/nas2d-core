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
#include "NAS2D/Xml/XmlMemoryBuffer.h"

#include "NAS2D/Xml/XmlAttribute.h"
#include "NAS2D/Xml/XmlAttributeSet.h"
#include "NAS2D/Xml/XmlComment.h"
#include "NAS2D/Xml/XmlDocument.h"
#include "NAS2D/Xml/XmlElement.h"
#include "NAS2D/Xml/XmlText.h"
#include "NAS2D/Xml/XmlUnknown.h"

using namespace NAS2D::Xml;

bool XmlMemoryBuffer::VisitEnter(const TiXmlElement& element, const TiXmlAttribute* firstAttribute)
{
	indent();
	_buffer += "<";
	_buffer += element.Value();

	for (const TiXmlAttribute* attrib = firstAttribute; attrib; attrib = attrib->Next())
	{
		_buffer += " ";
		attrib->Print(_buffer, 0);
	}

	if (!element.FirstChild())
	{
		_buffer += " />";
		line_break();
	}
	else
	{
		_buffer += ">";
		line_break();
}
	++depth;
	return true;
}


bool XmlMemoryBuffer::VisitExit(const TiXmlElement& element)
{
	--depth;
	if (!element.FirstChild())
		;
	else
	{
		indent();
		_buffer += "</";
		_buffer += element.Value();
		_buffer += ">";
		line_break();
	}
	return true;
}


bool XmlMemoryBuffer::Visit(const TiXmlText& text)
{
	if (text.CDATA())
	{
		indent();
		_buffer += "<![CDATA[";
		_buffer += text.Value();
		_buffer += "]]>";
		line_break();
	}
	else
	{
		indent();
		std::string str;
		TiXmlBase::EncodeString(text.Value(), str);
		_buffer += str;
		line_break();
	}
	return true;
}


bool XmlMemoryBuffer::Visit(const TiXmlComment& comment)
{
	indent();
	_buffer += "<!--";
	_buffer += comment.Value();
	_buffer += "-->";
	line_break();
	return true;
}


bool XmlMemoryBuffer::Visit(const TiXmlUnknown& unknown)
{
	indent();
	_buffer += "<";
	_buffer += unknown.Value();
	_buffer += ">";
	line_break();
	return true;
}
