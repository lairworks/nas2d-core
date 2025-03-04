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
#include "XmlMemoryBuffer.h"

#include "XmlAttribute.h"
#include "XmlAttributeSet.h"
#include "XmlComment.h"
#include "XmlDocument.h"
#include "XmlElement.h"
#include "XmlText.h"
#include "XmlUnknown.h"

using namespace NAS2D::Xml;


inline void indent(int depth, const std::string& indent, std::string& buffer)
{
	for (int i = 0; i < depth; ++i)
	{
		buffer += indent;
	}
}


inline void line_break(const std::string& linebreak, std::string& buffer)
{
	buffer += linebreak;
}


XmlMemoryBuffer::XmlMemoryBuffer() : _indent("\t"), _lineBreak("\n"), depth(0)
{}


bool XmlMemoryBuffer::visitEnter(const XmlElement& element, const XmlAttribute* firstAttribute)
{
	indent(depth, _indent, _buffer);
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
		line_break(_lineBreak, _buffer);
	}
	else
	{
		_buffer += ">";
		line_break(_lineBreak, _buffer);
	}
	++depth;
	return true;
}


bool XmlMemoryBuffer::visitExit(const XmlElement& element)
{
	--depth;
	if (element.firstChild())
	{
		indent(depth, _indent, _buffer);
		_buffer += "</";
		_buffer += element.value();
		_buffer += ">";
		line_break(_lineBreak, _buffer);
	}
	return true;
}


bool XmlMemoryBuffer::visit(const XmlText& text)
{
	if (text.CDATA())
	{
		indent(depth, _indent, _buffer);
		_buffer += "<![CDATA[";
		_buffer += text.value();
		_buffer += "]]>";
		line_break(_lineBreak, _buffer);
	}
	else
	{
		indent(depth, _indent, _buffer);
		std::string str;
		_buffer += text.value();
		line_break(_lineBreak, _buffer);
	}
	return true;
}


bool XmlMemoryBuffer::visit(const XmlComment& comment)
{
	indent(depth, _indent, _buffer);
	_buffer += "<!--";
	_buffer += comment.value();
	_buffer += "-->";
	line_break(_lineBreak, _buffer);
	return true;
}


bool XmlMemoryBuffer::visit(const XmlUnknown& unknown)
{
	indent(depth, _indent, _buffer);
	_buffer += "<";
	_buffer += unknown.value();
	_buffer += ">";
	line_break(_lineBreak, _buffer);
	return true;
}


std::size_t XmlMemoryBuffer::size()
{
	return _buffer.size();
}


const std::string& XmlMemoryBuffer::buffer()
{
	return _buffer;
}
