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

bool XmlMemoryBuffer::visitEnter(const XmlElement& element, const XmlAttribute* firstAttribute)
{
	indent();
	_buffer += "<";
	_buffer += element.value();

	for (const XmlAttribute* attrib = firstAttribute; attrib; attrib = attrib->next())
	{
		_buffer += " ";
		attrib->write(_buffer, 0);
	}

	if (!element.firstChild())
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


bool XmlMemoryBuffer::visitExit(const XmlElement& element)
{
	--depth;
	if (!element.firstChild())
		;
	else
	{
		indent();
		_buffer += "</";
		_buffer += element.value();
		_buffer += ">";
		line_break();
	}
	return true;
}


bool XmlMemoryBuffer::visit(const XmlText& text)
{
	if (text.CDATA())
	{
		indent();
		_buffer += "<![CDATA[";
		_buffer += text.value();
		_buffer += "]]>";
		line_break();
	}
	else
	{
		indent();
		std::string str;
		_buffer += text.value();
		line_break();
	}
	return true;
}


bool XmlMemoryBuffer::visit(const XmlComment& comment)
{
	indent();
	_buffer += "<!--";
	_buffer += comment.value();
	_buffer += "-->";
	line_break();
	return true;
}


bool XmlMemoryBuffer::visit(const XmlUnknown& unknown)
{
	indent();
	_buffer += "<";
	_buffer += unknown.value();
	_buffer += ">";
	line_break();
	return true;
}
