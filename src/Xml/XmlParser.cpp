// ==================================================================================
// = www.sourceforge.net/projects/tinyxml
// = Original code by Lee Thomason (www.grinninglizard.com)
// ==================================================================================
// = This software is provided 'as-is', without any express or implied warranty. In
// = no event will the authors be held liable for any damages arising from the use of
// = this software.
// = 
// = Permission is granted to anyone to use this software for any purpose, including
// = commercial applications, and to alter it and redistribute it freely, subject to
// = the following restrictions:
// = 
// = 1. The origin of this software must not be misrepresented; you must not claim
// = that you wrote the original software. If you use this software in a product, an
// = acknowledgment in the product documentation would be appreciated but is not
// = required.
// = 
// = 2. Altered source versions must be plainly marked as such, and must not be
// = misrepresented as being the original software.
// = 
// = 3. This notice may not be removed or altered from any source distribution.
// ==================================================================================
#include "NAS2D/XML/Xml.h"

#include <assert.h>
#include <ctype.h>
#include <stddef.h>

#include <locale>

//#define DEBUG_PARSER
#if defined( DEBUG_PARSER )
#	if defined( DEBUG ) && defined( _MSC_VER )
#		include <windows.h>
#		define TIXML_LOG OutputDebugString
#	else
#		define TIXML_LOG printf
#	endif
#endif

extern std::vector<std::string> XML_ERROR_TABLE;

namespace NAS2D {
namespace Xml {

// Note tha "PutString" hardcodes the same list. This
// is less flexible than it appears. Changing the entries
// or order will break putstring.	
XmlBase::Entity XmlBase::entity[ XmlBase::NUM_ENTITY ] = 
{
	{ "&amp;",  5, '&' },
	{ "&lt;",   4, '<' },
	{ "&gt;",   4, '>' },
	{ "&quot;", 6, '\"' },
	{ "&apos;", 6, '\'' }
};


int XmlBase::IsAlpha(unsigned char anyByte)
{
	std::locale loc;

	if (anyByte < 127)
		return std::isalpha(anyByte, loc);
	else
		return 1;	// What else to do? The unicode set is huge...get the english ones right.
}


int XmlBase::IsAlphaNum(unsigned char anyByte)
{
	std::locale loc;

	if (anyByte < 127)
		return std::isalnum(anyByte, loc);
	else
		return 1;	// What else to do? The unicode set is huge...get the english ones right.
}


/**
 * Document this class.
 */
class TiXmlParsingData
{
	friend class XmlDocument;
public:
	void stamp(const char* now);
	const XmlCursor& Cursor() const { return _cursor; }

private:
	TiXmlParsingData(const char* start, int _tabsize, int row, int col)
	{
		assert(start);
		_stamp = start;
		_tabsize = _tabsize;
		_cursor.row = row;
		_cursor.col = col;
	}

private:
	XmlCursor		_cursor;
	const char*		_stamp;
	int				_tabsize;
};


void TiXmlParsingData::stamp(const char* now)
{
	assert(now);

	// Do nothing if the tabsize is 0.
	if (_tabsize < 1)
		return;

	// Get the current row, column.
	int row = _cursor.row;
	int col = _cursor.col;
	const char* p = _stamp;
	assert(p);

	while (p < now)
	{
		// Treat p as unsigned, so we have a happy compiler.
		const unsigned char* pU = (const unsigned char*)p;

		// Code contributed by Fletcher Dunn: (modified by lee)
		switch (*pU)
		{
		case 0:
			// We *should* never get here, but in case we do, don't
			// advance past the terminating null character, ever
			return;

		case '\r':
			// bump down to the next line
			++row;
			col = 0;
			// Eat the character
			++p;

			// Check for \r\n sequence, and treat this as a single character
			if (*p == '\n')
				++p;
			break;

		case '\n':
			// bump down to the next line
			++row;
			col = 0;

			// Eat the character
			++p;

			// Check for \n\r sequence, and treat this as a single character.
			if (*p == '\r')
				++p;
			break;

		case '\t':
			// Eat the character
			++p;

			// Skip to next tab stop
			col = (col / _tabsize + 1) * _tabsize;
			break;

		default:
			++p;
			++col;
		}
	}
	_cursor.row = row;
	_cursor.col = col;
	assert(_cursor.row >= -1);
	assert(_cursor.col >= -1);
	_stamp = p;
	assert(_stamp);
}


const char* XmlBase::SkipWhiteSpace(const char* p)
{
	if (!p || !*p)
		return nullptr;

	while (*p && white_space(*p))
		++p;

	return p;
}


bool XmlBase::StreamWhiteSpace(std::istream& in, std::string& tag)
{
	for (;;)
	{
		if (!in.good()) return false;

		int c = in.peek();
		// At this scope, we can't get to a document. So fail silently.
		if (!white_space(c) || c <= 0)
			return true;

		tag += static_cast<char>(in.get());
	}
}


bool XmlBase::StreamTo(std::istream& in, int character, std::string& tag)
{
	while (in.good())
	{
		int c = in.peek();
		if (c == character)
			return true;
		if (c <= 0)		// Silent failure: can't get document at this scope
			return false;

		in.get();
		tag += static_cast<char>(c);
	}
	return false;
}


// One of TinyXML's more performance demanding functions. Try to keep the memory overhead down. The
// "assign" optimization removes over 10% of the execution time.
//
const char* XmlBase::ReadName(const char* p, std::string& name)
{
	name.clear();
	assert(p);

	// Names start with letters or underscores. Of course, in unicode,
	// tinyxml has no idea what a letter *is*. The algorithm is generous.
	//
	// After that, they can be letters, underscores, numbers, hyphens,
	// or colons. (Colons are valid ony for namespaces, but tinyxml can't
	// tell namespaces from names.)
	if (p && *p && (IsAlpha((unsigned char)*p) || *p == '_'))
	{
		const char* start = p;
		while (p && *p && (IsAlphaNum((unsigned char)*p)
					|| *p == '_'
					|| *p == '-'
					|| *p == '.'
					|| *p == ':'))
		{
			//(*name) += *p; // expensive
			++p;
		}
		if (p - start > 0)
		{
			name.assign(start, p - start);
		}
		return p;
	}
	return nullptr;
}

const char* XmlBase::GetEntity(const char* p, char* value, int* length)
{
	// Presume an entity, and pull it out.
	std::string ent;
	int i;
	*length = 0;

	if (*(p + 1) && *(p + 1) == '#' && *(p + 2))
	{
		unsigned long ucs = 0;
		ptrdiff_t delta = 0;
		unsigned mult = 1;

		if (*(p + 2) == 'x')
		{
			// Hexadecimal.
			if (!*(p + 3)) return nullptr;

			const char* q = p + 3;
			q = strchr(q, ';');

			if (!q || !*q) return nullptr;

			delta = q - p;
			--q;

			while (*q != 'x')
			{
				if (*q >= '0' && *q <= '9')
					ucs += mult * (*q - '0');
				else if (*q >= 'a' && *q <= 'f')
					ucs += mult * (*q - 'a' + 10);
				else if (*q >= 'A' && *q <= 'F')
					ucs += mult * (*q - 'A' + 10);
				else
					return nullptr;
				mult *= 16;
				--q;
			}
		}
		else
		{
			// Decimal.
			if (!*(p + 2)) return nullptr;

			const char* q = p + 2;
			q = strchr(q, ';');

			if (!q || !*q) return nullptr;

			delta = q - p;
			--q;

			while (*q != '#')
			{
				if (*q >= '0' && *q <= '9')
					ucs += mult * (*q - '0');
				else
					return nullptr;
				mult *= 10;
				--q;
			}
		}

		*value = (char)ucs;
		*length = 1;

		return p + delta + 1;
	}

	// Now try to match it.
	for (i = 0; i < NUM_ENTITY; ++i)
	{
		if (strncmp(entity[i].str, p, entity[i].strLength) == 0)
		{
			assert(strlen(entity[i].str) == entity[i].strLength);
			*value = entity[i].chr;
			*length = 1;
			return (p + entity[i].strLength);
		}
	}

	// So it wasn't an entity, its unrecognized, or something like that.
	*value = *p;	// Don't put back the last one, since we return it!

	return p + 1;
}


const char* XmlBase::GetChar(const char* p, char* _value, int* length)
{
	assert(p);

	*length = 1;

	if (*length == 1)
	{
		if (*p == '&')
			return GetEntity(p, _value, length);
		*_value = *p;
		return p + 1;
	}
	else if (*length)
	{
		//strncpy( _value, p, *length );	// lots of compilers don't like this function (unsafe),
		// and the null terminator isn't needed
		for (int i = 0; p[i] && i < *length; ++i) {
			_value[i] = p[i];
		}
		return p + (*length);
	}
	else
	{
		// Not valid text.
		return nullptr;
	}
}


bool XmlBase::StringEqual(const char* p, const char* tag, bool ignoreCase)
{
	assert(p);
	assert(tag);
	if (!p || !*p)
	{
		assert(0);
		return false;
	}

	const char* q = p;

	if (ignoreCase)
	{
		while (*q && *tag && ToLower(*q) == ToLower(*tag))
		{
			++q;
			++tag;
		}

		if (*tag == 0)
			return true;
	}
	else
	{
		while (*q && *tag && *q == *tag)
		{
			++q;
			++tag;
		}

		if (*tag == 0)		// Have we found the end of the tag, and everything equal?
			return true;
	}
	return false;
}

const char* XmlBase::ReadText(const char* p, std::string* text, bool trimWhiteSpace, const char* endTag, bool caseInsensitive)
{
	*text = ""; // certain tags always keep whitespace
	if (!trimWhiteSpace	|| !condenseWhiteSpace)	// if true, whitespace is always kept
	{
		// Keep all the white space.
		while (p && *p && !StringEqual(p, endTag, caseInsensitive))
		{
			int len;
			char cArr[4] = { 0, 0, 0, 0 };
			p = GetChar(p, cArr, &len);
			text->append(cArr, len);
		}
	}
	else
	{
		bool whitespace = false;

		// Remove leading white space:
		p = SkipWhiteSpace(p);
		while (p && *p && !StringEqual(p, endTag, caseInsensitive))
		{
			if (*p == '\r' || *p == '\n')
			{
				whitespace = true;
				++p;
			}
			else if (white_space(*p))
			{
				whitespace = true;
				++p;
			}
			else
			{
				// If we've found whitespace, add it before the
				// new character. Any whitespace just becomes a space.
				if (whitespace)
				{
					(*text) += ' ';
					whitespace = false;
				}
				int len;
				char cArr[4] = { 0, 0, 0, 0 };
				p = GetChar(p, cArr, &len);
				if (len == 1)
					(*text) += cArr[0];	// more efficient
				else
					text->append(cArr, len);
			}
		}
	}
	if (p && *p)
		p += strlen(endTag);
	return (p && *p) ? p : 0;
}


void XmlDocument::StreamIn(std::istream& in, std::string& tag)
{
	// The basic issue with a document is that we don't know what we're
	// streaming. Read something presumed to be a tag (and hope), then
	// identify it, and call the appropriate stream method on the tag.
	//
	// This "pre-streaming" will never read the closing ">" so the
	// sub-tag can orient itself.

	if (!StreamTo(in, '<', tag))
	{
		SetError(XML_ERROR_PARSING_EMPTY, 0, 0);
		return;
	}

	while (in.good())
	{
		int tagIndex = (int)tag.length();
		while (in.good() && in.peek() != '>')
		{
			int c = in.get();
			if (c <= 0)
			{
				SetError(XML_ERROR_EMBEDDED_NULL, 0, 0);
				break;
			}
			tag += static_cast<char>(c);
		}

		if (in.good())
		{
			// We now have something we presume to be a node of  some sort.
			// Identify it, and call the node to continue streaming.
			XmlNode* node = Identify(tag.c_str() + tagIndex);

			if (node)
			{
				node->StreamIn(in, tag);
				bool isElement = node->ToElement() != 0;
				delete node;
				node = 0;

				// If this is the root element, we're done. Parsing will be
				// done by the >> operator.
				if (isElement)
					return;
			}
			else
			{
				SetError(XML_ERROR, 0, 0);
				return;
			}
		}
	}

	// We should have returned sooner.
	SetError(XML_ERROR, 0, 0);
}


const char* XmlDocument::Parse(const char* p, TiXmlParsingData* prevData)
{
	ClearError();

	// Parse away, at the document level. Since a document contains nothing but
	// other tags, most of what happens here is skipping white space.
	if (!p || !*p)
	{
		SetError(XML_ERROR_DOCUMENT_EMPTY, 0, 0);
		return nullptr;
	}

	// Note that, for a document, this needs to come before the while space skip,
	// so that parsing starts from the pointer we are given.
	location.clear();
	if (prevData)
	{
		location.row = prevData->_cursor.row;
		location.col = prevData->_cursor.col;
	}
	else
	{
		location.row = 0;
		location.col = 0;
	}
	TiXmlParsingData data(p, TabSize(), location.row, location.col);
	location = data.Cursor();

	p = SkipWhiteSpace(p);
	if (!p)
	{
		SetError(XML_ERROR_DOCUMENT_EMPTY, 0, 0);
		return nullptr;
	}

	while (p && *p)
	{
		XmlNode* node = Identify(p);
		if (node)
		{
			p = node->Parse(p, &data);
			LinkEndChild(node);
		}
		else
		{
			break;
		}

		p = SkipWhiteSpace(p);
	}

	// Was this empty?
	if (!firstChild)
	{
		SetError(XML_ERROR_DOCUMENT_EMPTY, 0, 0);
		return nullptr;
	}

	// All is well.
	return p;
}

void XmlDocument::SetError(XmlErrorCode err, const char* pError, TiXmlParsingData* data)
{
	// The first error in a chain is more accurate - don't set again!
	if (error)
		return;

	assert(err > 0 && err < XML_ERROR_STRING_COUNT);
	error = true;
	errorId = err;
	errorDesc = XML_ERROR_TABLE[errorId];

	errorLocation.clear();
	if (pError && data)
	{
		data->stamp(pError);
		errorLocation = data->Cursor();
	}
}


XmlNode* XmlNode::Identify(const char* p)
{
	XmlNode* returnNode = 0;

	p = SkipWhiteSpace(p);
	if (!p || !*p || *p != '<')
		return nullptr;

	p = SkipWhiteSpace(p);

	if (!p || !*p)
		return nullptr;


	// What is this thing? 
	// - Elements start with a letter or underscore, but xml is reserved.
	// - Comments: <!--
	// - Decleration: <?xml
	// - Everthing else is unknown to tinyxml.
	//

	// Holy crap there are MUCH better ways to do this.
	const char* commentHeader = { "<!--" };
	const char* dtdHeader = { "<!" };
	const char* cdataHeader = { "<![CDATA[" };

	if (StringEqual(p, commentHeader, false))
	{
		#ifdef DEBUG_PARSER
		TIXML_LOG("XML parsing Comment\n");
		#endif
		returnNode = new XmlComment();
	}
	else if (StringEqual(p, cdataHeader, false))
	{
		#ifdef DEBUG_PARSER
		TIXML_LOG("XML parsing CDATA\n");
		#endif
		XmlText* text = new XmlText("");
		text->CDATA(true);
		returnNode = text;
	}
	else if (StringEqual(p, dtdHeader, false))
	{
		#ifdef DEBUG_PARSER
		TIXML_LOG("XML parsing Unknown(1)\n");
		#endif
		returnNode = new XmlUnknown();
	}
	else if (IsAlpha(*(p + 1)) || *(p + 1) == '_')
	{
		#ifdef DEBUG_PARSER
		TIXML_LOG("XML parsing Element\n");
		#endif
		returnNode = new XmlElement("");
	}
	else
	{
		#ifdef DEBUG_PARSER
		TIXML_LOG("XML parsing Unknown(2)\n");
		#endif
		returnNode = new XmlUnknown();
	}

	if (returnNode)
	{
		// Set the parent, so it can report errors
		returnNode->parent = this;
	}
	return returnNode;
}

void XmlElement::StreamIn(std::istream & in, std::string & tag)
{
	// We're called with some amount of pre-parsing. That is, some of "this"
	// element is in "tag". Go ahead and stream to the closing ">"
	while (in.good())
	{
		int c = in.get();
		if (c <= 0)
		{
			XmlDocument* document = GetDocument();
			if (document)
				document->SetError(XML_ERROR_EMBEDDED_NULL, 0, 0);
			return;
		}
		tag += static_cast<char>(c);

		if (c == '>')
			break;
	}

	if (tag.length() < 3) return;

	// Okay...if we are a "/>" tag, then we're done. We've read a complete tag.
	// If not, identify and stream.
	if (tag.at(tag.length() - 1) == '>' && tag.at(tag.length() - 2) == '/')
	{
		// All good!
		return;
	}
	else if (tag.at(tag.length() - 1) == '>')
	{
		// There is more. Could be:
		//		text
		//		cdata text (which looks like another node)
		//		closing tag
		//		another node.
		for (;;)
		{
			StreamWhiteSpace(in, tag);

			// Do we have text?
			if (in.good() && in.peek() != '<')
			{
				// Yep, text.
				XmlText text("");
				text.StreamIn(in, tag);

				// What follows text is a closing tag or another node.
				// Go around again and figure it out.
				continue;
			}

			// We now have either a closing tag...or another node.
			// We should be at a "<", regardless.
			if (!in.good()) return;
			assert(in.peek() == '<');
			int tagIndex = (int)tag.length();

			bool closingTag = false;
			bool firstCharFound = false;

			for (;;)
			{
				if (!in.good())
					return;

				int c = in.peek();
				if (c <= 0)
				{
					XmlDocument* document = GetDocument();
					if (document)
						document->SetError(XML_ERROR_EMBEDDED_NULL, 0, 0);
					return;
				}

				if (c == '>')
					break;

				tag += static_cast<char>(c);
				in.get();

				// Early out if we find the CDATA id.
				if (c == '[' && tag.size() >= 9)
				{
					size_t len = tag.size();
					const char* start = tag.c_str() + len - 9;
					if (strcmp(start, "<![CDATA[") == 0)
					{
						assert(!closingTag);
						break;
					}
				}

				if (!firstCharFound && c != '<' && !white_space(c))
				{
					firstCharFound = true;
					if (c == '/')
						closingTag = true;
				}
			}
			// If it was a closing tag, then read in the closing '>' to clean up the input stream.
			// If it was not, the streaming will be done by the tag.
			if (closingTag)
			{
				if (!in.good())
					return;

				int c = in.get();
				if (c <= 0)
				{
					XmlDocument* document = GetDocument();
					if (document)
						document->SetError(XML_ERROR_EMBEDDED_NULL, 0, 0);
					return;
				}
				assert(c == '>');
				tag += static_cast<char>(c);

				// We are done, once we've found our closing tag.
				return;
			}
			else
			{
				// If not a closing tag, id it, and stream.
				const char* tagloc = tag.c_str() + tagIndex;
				XmlNode* node = Identify(tagloc);
				if (!node)
					return;
				node->StreamIn(in, tag);
				delete node;
				node = nullptr;

				// No return: go around from the beginning: text, closing tag, or node.
			}
		}
	}
}

const char* XmlElement::Parse(const char* p, TiXmlParsingData* data)
{
	p = SkipWhiteSpace(p);
	XmlDocument* document = GetDocument();

	if (!p || !*p)
	{
		if (document) document->SetError(XML_ERROR_PARSING_ELEMENT, 0, 0);
		return nullptr;
	}

	if (data)
	{
		data->stamp(p);
		location = data->Cursor();
	}

	if (*p != '<')
	{
		if (document) document->SetError(XML_ERROR_PARSING_ELEMENT, p, data);
		return nullptr;
	}

	p = SkipWhiteSpace(p + 1);

	// Read the name.
	const char* pErr = p;

	p = ReadName(p, value);
	if (!p || !*p)
	{
		if (document)	document->SetError(XML_ERROR_FAILED_TO_READ_ELEMENT_NAME, pErr, data);
		return nullptr;
	}

	std::string endTag("</");
	endTag += value;

	// Check for and read attributes. Also look for an empty
	// tag or an end tag.
	while (p && *p)
	{
		pErr = p;
		p = SkipWhiteSpace(p);
		if (!p || !*p)
		{
			if (document) document->SetError(XML_ERROR_READING_ATTRIBUTES, pErr, data);
			return nullptr;
		}
		if (*p == '/')
		{
			++p;
			// Empty tag.
			if (*p != '>')
			{
				if (document) document->SetError(XML_ERROR_PARSING_EMPTY, p, data);
				return nullptr;
			}
			return (p + 1);
		}
		else if (*p == '>')
		{
			// Done with attributes (if there were any.) Read the value -- which
			// can include other elements -- read the end tag, and return.
			++p;
			p = ReadValue(p, data); // Note this is an Element method, and will set the error if one happens.
			if (!p || !*p)
			{
				// We were looking for the end tag, but found nothing.
				// Fix for [ 1663758 ] Failure to report error on bad XML
				if (document) document->SetError(XML_ERROR_READING_END_TAG, p, data);
				return nullptr;
			}

			// We should find the end tag now
			// note that:
			// </foo > and
			// </foo> 
			// are both valid end tags.
			if (StringEqual(p, endTag.c_str(), false))
			{
				p += endTag.length();
				p = SkipWhiteSpace(p);
				if (p && *p && *p == '>')
				{
					++p;
					return p;
				}
				if (document) document->SetError(XML_ERROR_READING_END_TAG, p, data);
				return nullptr;
			}
			else
			{
				if (document) document->SetError(XML_ERROR_READING_END_TAG, p, data);
				return nullptr;
			}
		}
		else
		{
			// Try to read an attribute:
			XmlAttribute* attrib = new XmlAttribute();
			if (!attrib)
			{
				return nullptr;
			}

			attrib->SetDocument(document);
			pErr = p;
			p = attrib->Parse(p, data);

			if (!p || !*p)
			{
				if (document) document->SetError(XML_ERROR_PARSING_ELEMENT, pErr, data);
				delete attrib;
				return nullptr;
			}

			// Handle the strange case of double attributes:
			XmlAttribute* node = attributeSet.Find(attrib->Name());

			if (node)
			{
				if (document) document->SetError(XML_ERROR_PARSING_ELEMENT, pErr, data);
				delete attrib;
				return nullptr;
			}

			attributeSet.Add(attrib);
		}
	}
	return p;
}


const char* XmlElement::ReadValue(const char* p, TiXmlParsingData* data)
{
	XmlDocument* document = GetDocument();

	// Read in text and elements in any order.
	const char* pWithWhiteSpace = p;
	p = SkipWhiteSpace(p);

	while (p && *p)
	{
		if (*p != '<')
		{
			// Take what we have, make a text element.
			XmlText* textNode = new XmlText("");

			if (!textNode)
			{
				return nullptr;
			}

			if (XmlBase::IsWhiteSpaceCondensed())
			{
				p = textNode->Parse(p, data);
			}
			else
			{
				// Special case: we want to keep the white space
				// so that leading spaces aren't removed.
				p = textNode->Parse(pWithWhiteSpace, data);
			}

			if (!textNode->Blank())
				LinkEndChild(textNode);
			else
				delete textNode;
		}
		else
		{
			// We hit a '<'
			// Have we hit a new element or an end tag? This could also be an
			// XmlText in the "CDATA" style.
			if (StringEqual(p, "</", false))
			{
				return p;
			}
			else
			{
				XmlNode* node = Identify(p);
				if (node)
				{
					p = node->Parse(p, data);
					LinkEndChild(node);
				}
				else
				{
					return nullptr;
				}
			}
		}
		pWithWhiteSpace = p;
		p = SkipWhiteSpace(p);
	}

	if (!p)
	{
		if (document) document->SetError(XML_ERROR_READING_ELEMENT_VALUE, 0, 0);
	}
	return p;
}


void XmlUnknown::StreamIn(std::istream& in, std::string& tag)
{
	while (in.good())
	{
		int c = in.get();
		if (c <= 0)
		{
			XmlDocument* document = GetDocument();
			if (document)
				document->SetError(XML_ERROR_EMBEDDED_NULL, 0, 0);
			return;
		}
		tag += static_cast<char>(c);

		if (c == '>')
			return;
	}
}


const char* XmlUnknown::Parse(const char* p, TiXmlParsingData* data)
{
	XmlDocument* document = GetDocument();
	p = SkipWhiteSpace(p);

	if (data)
	{
		data->stamp(p);
		location = data->Cursor();
	}

	if (!p || !*p || *p != '<')
	{
		if (document) document->SetError(XML_ERROR_PARSING_UNKNOWN, p, data);
		return nullptr;
	}

	++p;
	value = "";

	while (p && *p && *p != '>')
	{
		value += *p;
		++p;
	}

	if (!p)
	{
		if (document)
			document->SetError(XML_ERROR_PARSING_UNKNOWN, 0, 0);
	}
	if (p && *p == '>')
		return p + 1;

	return p;
}

void XmlComment::StreamIn(std::istream& in, std::string& tag)
{
	while (in.good())
	{
		int c = in.get();
		if (c <= 0)
		{
			XmlDocument* document = GetDocument();
			if (document)
				document->SetError(XML_ERROR_EMBEDDED_NULL, 0, 0);
			return;
		}

		tag += static_cast<char>(c);

		if (c == '>'
			&& tag.at(tag.length() - 2) == '-'
			&& tag.at(tag.length() - 3) == '-')
		{
			// All is well.
			return;
		}
	}
}


const char* XmlComment::Parse(const char* p, TiXmlParsingData* data)
{
	XmlDocument* document = GetDocument();
	value = "";

	p = SkipWhiteSpace(p);

	if (data)
	{
		data->stamp(p);
		location = data->Cursor();
	}
	const char* startTag = "<!--";
	const char* endTag = "-->";

	if (!StringEqual(p, startTag, false))
	{
		if (document)
			document->SetError(XML_ERROR_PARSING_COMMENT, p, data);
		return nullptr;
	}
	p += strlen(startTag);

	// [ 1475201 ] TinyXML parses entities in comments
	// Oops - ReadText doesn't work, because we don't want to parse the entities.
	// p = ReadText( p, &value, false, endTag, false, encoding );
	//
	// from the XML spec:
	/*
	 [Definition: Comments may appear anywhere in a document outside other markup; in addition,
				  they may appear within the document type declaration at places allowed by the grammar.
				  They are not part of the document's character data; an XML processor MAY, but need not,
				  make it possible for an application to retrieve the text of comments. For compatibility,
				  the string "--" (double-hyphen) MUST NOT occur within comments.] Parameter entity
				  references MUST NOT be recognized within comments.

				  An example of a comment:

				  <!-- declarations for <head> & <body> -->
	*/

	value = "";
	// Keep all the white space.
	while (p && *p && !StringEqual(p, endTag, false))
	{
		value.append(p, 1);
		++p;
	}
	if (p && *p)
		p += strlen(endTag);

	return p;
}


const char* XmlAttribute::Parse(const char* p, TiXmlParsingData* data)
{
	p = SkipWhiteSpace(p);
	if (!p || !*p) return nullptr;

	if (data)
	{
		data->stamp(p);
		location = data->Cursor();
	}
	// Read the name, the '=' and the value.
	const char* pErr = p;
	p = ReadName(p, name);
	if (!p || !*p)
	{
		if (document) document->SetError(XML_ERROR_READING_ATTRIBUTES, pErr, data);
		return nullptr;
	}
	p = SkipWhiteSpace(p);
	if (!p || !*p || *p != '=')
	{
		if (document) document->SetError(XML_ERROR_READING_ATTRIBUTES, p, data);
		return nullptr;
	}

	++p;	// skip '='
	p = SkipWhiteSpace(p);
	if (!p || !*p)
	{
		if (document) document->SetError(XML_ERROR_READING_ATTRIBUTES, p, data);
		return nullptr;
	}

	const char* end;
	const char SINGLE_QUOTE = '\'';
	const char DOUBLE_QUOTE = '\"';

	if (*p == SINGLE_QUOTE)
	{
		++p;
		end = "\'";		// single quote in string
		p = ReadText(p, &value, false, end, false);
	}
	else if (*p == DOUBLE_QUOTE)
	{
		++p;
		end = "\"";		// double quote in string
		p = ReadText(p, &value, false, end, false);
	}
	else
	{
		// All attribute values should be in single or double quotes.
		// But this is such a common error that the parser will try
		// its best, even without them.
		value = "";
		while (p && *p					// existence
			&& !white_space(*p)			// whitespace
			&& *p != '/' && *p != '>')	// tag end
		{
			if (*p == SINGLE_QUOTE || *p == DOUBLE_QUOTE)
			{
				// [ 1451649 ] Attribute values with trailing quotes not handled correctly
				// We did not have an opening quote but seem to have a 
				// closing one. Give up and throw an error.
				if (document) document->SetError(XML_ERROR_READING_ATTRIBUTES, p, data);
				return nullptr;
			}
			value += *p;
			++p;
		}
	}
	return p;
}

void XmlText::StreamIn(std::istream& in, std::string& tag)
{
	while (in.good())
	{
		int c = in.peek();
		if (!cdata && (c == '<'))
		{
			return;
		}
		if (c <= 0)
		{
			XmlDocument* document = GetDocument();
			if (document)
				document->SetError(XML_ERROR_EMBEDDED_NULL, 0, 0);

			return;
		}

		tag += static_cast<char>(c);
		in.get();	// "commits" the peek made above

		if (cdata && c == '>' && tag.size() >= 3)
		{
			size_t len = tag.size();
			if (tag[len - 2] == ']' && tag[len - 3] == ']')
			{
				// terminator of cdata.
				return;
			}
		}
	}
}

const char* XmlText::Parse(const char* p, TiXmlParsingData* data)
{
	value = "";
	XmlDocument* document = GetDocument();

	if (data)
	{
		data->stamp(p);
		location = data->Cursor();
	}

	const char* const startTag = "<![CDATA[";
	const char* const endTag = "]]>";

	if (cdata || StringEqual(p, startTag, false))
	{
		cdata = true;

		if (!StringEqual(p, startTag, false))
		{
			if (document)
				document->SetError(XML_ERROR_PARSING_CDATA, p, data);
			return nullptr;
		}
		p += strlen(startTag);

		// Keep all the white space, ignore the encoding, etc.
		while (p && *p && !StringEqual(p, endTag, false))
		{
			value += *p;
			++p;
		}

		std::string dummy;
		p = ReadText(p, &dummy, false, endTag, false);
		return p;
	}
	else
	{
		bool ignoreWhite = true;

		const char* end = "<";
		p = ReadText(p, &value, ignoreWhite, end, false);
		if (p && *p)
			return p - 1;	// don't truncate the '<'
		return nullptr;
	}
}


bool XmlText::Blank() const
{
	for (unsigned i = 0; i < value.length(); ++i)
		if (!white_space(value[i]))
			return false;

	return true;
}

} // namespace Xml
} // namespace NAS2D
