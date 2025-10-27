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
#include "Xml.h"

#include <cassert>
#include <cstddef>
#include <cctype>
#include <fstream>
#include <cstring>
#include <string>
#include <array>


//#define DEBUG_PARSER
#if defined( DEBUG_PARSER )
#	if defined( DEBUG ) && defined( _MSC_VER )
#		include <windows.h>
#		define TIXML_LOG OutputDebugString
#	else
#		define TIXML_LOG printf
#	endif
#endif

namespace
{
	const auto XML_ERROR_TABLE = std::array{
		"No error",
		"Unspecified Error",
		"Error parsing Element.",
		"Failed to read Element name.",
		"Error reading Element value.",
		"Error reading Attributes.",
		"Error: Empty tag.",
		"Error reading end tag.",
		"Error parsing Unknown.",
		"Error parsing Comment.",
		"Error parsing Declaration.",
		"Error: Document empty.",
		"Error: Unexpected EOF found in input stream.",
		"Error parsing CDATA.",
		"Error adding XmlDocument to document: XmlDocument can only be at the root.",
	};

	struct Entity
	{
		const char* str;
		unsigned int strLength;
		char chr;
	};

	enum
	{
		NUM_ENTITY = 5,
		MAX_ENTITY_LENGTH = 6

	};

	Entity entity[NUM_ENTITY] =
	{
		{ "&amp;",  5, '&' },
		{ "&lt;",   4, '<' },
		{ "&gt;",   4, '>' },
		{ "&quot;", 6, '\"' },
		{ "&apos;", 6, '\'' }
	};
}


namespace NAS2D
{
namespace Xml {

int XmlBase::isAlpha(unsigned char anyByte)
{
	if (anyByte < 127)
	{
		return isalpha(anyByte);
	}
	else
	{
		return 1; // What else to do? The unicode set is huge...get the english ones right.
	}
}


int XmlBase::isAlphaNum(unsigned char anyByte)
{
	if (anyByte < 127)
	{
		return isalnum(anyByte);
	}
	else
	{
		return 1; // What else to do? The unicode set is huge...get the english ones right.
	}
}


/**
 * Document this class.
 */
class XmlParsingData
{
	friend class XmlDocument;
public:
	void stamp(const char* now);
	const XmlBase::ParseLocation& cursor() const { return _cursor; }

private:
	XmlParsingData(const char* start, const XmlBase::ParseLocation& cursor) : _stamp(start), _cursor(cursor)
	{
		assert(start);
	}

private:
	const char* _stamp;
	XmlBase::ParseLocation _cursor;
};


void XmlParsingData::stamp(const char* now)
{
	assert(now);

	// Get the current row, column.
	auto row = _cursor.row;
	auto col = _cursor.column;
	const char* p = _stamp;
	assert(p);

	while (p < now)
	{
		// Treat p as unsigned, so we have a happy compiler.
		const unsigned char* pU = reinterpret_cast<const unsigned char*>(p);

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
			if (*p == '\n') { ++p; }
			break;

		case '\n':
			// bump down to the next line
			++row;
			col = 0;

			// Eat the character
			++p;

			// Check for \n\r sequence, and treat this as a single character.
			if (*p == '\r') { ++p; }

			break;

		case '\t':
			// Eat the character
			++p;
			break;

		default:
			++p;
			++col;
		}
	}

	_cursor = XmlBase::ParseLocation{row, col};

	assert(_cursor.row >= -1);
	assert(_cursor.column >= -1);

	_stamp = p;
	assert(_stamp);
}


const char* XmlBase::skipWhiteSpace(const char* p)
{
	if (!p || !*p)
	{
		return nullptr;
	}

	while (*p && white_space(*p))
	{
		++p;
	}

	return p;
}


bool XmlBase::streamWhiteSpace(std::istream& in, std::string& tag)
{
	for (;;)
	{
		if (!in.good()) { return false; }

		int c = in.peek();
		// At this scope, we can't get to a document. So fail silently.
		if (!white_space(c) || c <= 0)
		{
			return true;
		}

		tag += static_cast<char>(in.get());
	}
}


bool XmlBase::streamTo(std::istream& in, int character, std::string& tag)
{
	while (in.good())
	{
		int c = in.peek();
		if (c == character)
		{
			return true;
		}

		if (c <= 0) // Silent failure: can't get document at this scope
		{
			return false;
		}

		in.get();
		tag += static_cast<char>(c);
	}

	return false;
}


/**
 * Reads an XML name into the string provided. Returns a pointer just past
 * the last character of the name, or null if the function encounters an error.
 */
const char* XmlBase::readName(const char* p, std::string& name)
{
	// One of TinyXML's more performance demanding functions. Try to keep the memory overhead down. The
	// "assign" optimization removes over 10% of the execution time. <-- I want to evaluate that statement.

	name.clear();
	assert(p);

	// Names start with letters or underscores. Of course, in unicode,
	// tinyxml has no idea what a letter *is*. The algorithm is generous.
	//
	// After that, they can be letters, underscores, numbers, hyphens,
	// or colons. (Colons are valid ony for namespaces, but tinyxml can't
	// tell namespaces from names.)
	if (p && *p && (isAlpha(static_cast<unsigned char>(*p)) || *p == '_'))
	{
		const char* start = p;
		while (p && *p && (isAlphaNum(static_cast<unsigned char>(*p))
			|| *p == '_'
			|| *p == '-'
			|| *p == '.'
			|| *p == ':'))
		{
			++p;
		}

		if (p - start > 0)
		{
			name.assign(start, p);
		}

		return p;
	}

	return nullptr;
}


/**
 * If an entity has been found, transform it into a character.
 */
const char* XmlBase::getEntity(const char* p, char* value, std::size_t* length)
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
			if (!*(p + 3)) { return nullptr; }

			const char* q = p + 3;
			q = strchr(q, ';');

			if (!q || !*q) { return nullptr; }

			delta = q - p;
			--q;

			while (*q != 'x')
			{
				if (*q >= '0' && *q <= '9') { ucs += mult * static_cast<unsigned>(*q - '0'); }
				else if (*q >= 'a' && *q <= 'f') { ucs += mult * static_cast<unsigned>(*q - 'a' + 10); }
				else if (*q >= 'A' && *q <= 'F') { ucs += mult * static_cast<unsigned>(*q - 'A' + 10); }
				else { return nullptr; }

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

			if (!q || !*q) { return nullptr; }

			delta = q - p;
			--q;

			while (*q != '#')
			{
				if (*q >= '0' && *q <= '9')
				{
					ucs += mult * static_cast<unsigned>(*q - '0');
				}
				else
				{
					return nullptr;
				}

				mult *= 10;
				--q;
			}
		}

		*value = static_cast<char>(ucs);
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
	*value = *p; // Don't put back the last one, since we return it!

	return p + 1;
}


/**
 * Get a character, while interpreting entities. The length can be from 0 to 4 bytes.
 */
const char* XmlBase::getChar(const char* p, char* _value, std::size_t* length)
{
	assert(p);

	*length = 1;

	if (*length == 1)
	{
		if (*p == '&') { return getEntity(p, _value, length); }
		*_value = *p;
		return p + 1;
	}
	else if (*length)
	{
		//strncpy( _value, p, *length ); // lots of compilers don't like this function (unsafe),
		// and the null terminator isn't needed
		for (std::size_t i = 0; i < *length && p[i]; ++i)
		{
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


/**
 * Return true if the next characters in the stream are any of the endTag sequences.
 * Ignore case only works for english, and should only be relied on when comparing
 * to English words: StringEqual( p, "version", true ) is fine.
 */
bool XmlBase::stringEqual(const char* p, const char* tag, bool ignoreCase)
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
		while (*q && *tag && toLower(*q) == toLower(*tag))
		{
			++q;
			++tag;
		}

		if (*tag == 0) { return true; }
	}
	else
	{
		while (*q && *tag && *q == *tag)
		{
			++q;
			++tag;
		}

		if (*tag == 0) // Have we found the end of the tag, and everything equal?
		{
			return true;
		}
	}

	return false;
}


/**
 * Reads text. Returns a pointer past the given end tag. Wickedly complex options, but it
 * keeps the (sensitive) code in one place.
 *
 * \param p				Where to start
 * \param text				The string read
 * \param trimWhiteSpace	Wheather to keep the white space
 * \param endTag			What ends this text
 * \param caseInsensitive		Whether to ignore case in the end tag
 */
const char* XmlBase::readText(const char* p, std::string* text, bool trimWhiteSpace, const char* endTag, bool caseInsensitive)
{
	*text = ""; // certain tags always keep whitespace
	if (!trimWhiteSpace || !condenseWhiteSpace) // if true, whitespace is always kept
	{
		// Keep all the white space.
		while (p && *p && !stringEqual(p, endTag, caseInsensitive))
		{
			std::size_t len;
			char cArr[4] = { 0, 0, 0, 0 };
			p = getChar(p, cArr, &len);
			text->append(cArr, len);
		}
	}
	else
	{
		bool whitespace = false;

		// Remove leading white space:
		p = skipWhiteSpace(p);
		while (p && *p && !stringEqual(p, endTag, caseInsensitive))
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
				std::size_t len;
				char cArr[4] = { 0, 0, 0, 0 };
				p = getChar(p, cArr, &len);
				if (len == 1)
				{
					(*text) += cArr[0]; // more efficient
				}
				else
				{
					text->append(cArr, len);
				}
			}
		}
	}

	if (p && *p)
	{
		p += strlen(endTag);
	}

	return (p && *p) ? p : nullptr;
}


void XmlDocument::streamIn(std::istream& in, std::string& tag)
{
	// The basic issue with a document is that we don't know what we're
	// streaming. Read something presumed to be a tag (and hope), then
	// identify it, and call the appropriate stream method on the tag.
	//
	// This "pre-streaming" will never read the closing ">" so the
	// sub-tag can orient itself.
	if (!streamTo(in, '<', tag))
	{
		error(XmlErrorCode::XML_ERROR_PARSING_EMPTY, nullptr, nullptr);
		return;
	}

	while (in.good())
	{
		int tagIndex = static_cast<int>(tag.length());
		while (in.good() && in.peek() != '>')
		{
			int c = in.get();
			if (c <= 0)
			{
				error(XmlErrorCode::XML_ERROR_EMBEDDED_NULL, nullptr, nullptr);
				break;
			}
			tag += static_cast<char>(c);
		}

		if (in.good())
		{
			// We now have something we presume to be a node of some sort.
			// Identify it, and call the node to continue streaming.
			XmlNode* node = identify(tag.c_str() + tagIndex);

			if (node)
			{
				node->streamIn(in, tag);
				bool isElement = node->toElement() != nullptr;
				delete node;
				node = nullptr;

				// If this is the root element, we're done. Parsing will be
				// done by the >> operator.
				if (isElement) { return; }
			}
			else
			{
				error(XmlErrorCode::XML_ERROR, nullptr, nullptr);
				return;
			}
		}
	}

	// We should have returned sooner.
	error(XmlErrorCode::XML_ERROR, nullptr, nullptr);
}


const char* XmlDocument::parse(const char* p, void* prevData)
{
	clearError();

	// Parse away, at the document level. Since a document contains nothing but
	// other tags, most of what happens here is skipping white space.
	if (!p || !*p)
	{
		error(XmlErrorCode::XML_ERROR_DOCUMENT_EMPTY, nullptr, nullptr);
		return nullptr;
	}

	location = ParseLocation{};
	// Note that, for a document, this needs to come before the while space skip,
	// so that parsing starts from the pointer we are given.
	if (prevData)
	{
		location = static_cast<XmlParsingData*>(prevData)->_cursor;
	}

	XmlParsingData data(p, location);
	location = data.cursor();

	p = skipWhiteSpace(p);
	if (!p)
	{
		error(XmlErrorCode::XML_ERROR_DOCUMENT_EMPTY, nullptr, nullptr);
		return nullptr;
	}

	while (p && *p)
	{
		XmlNode* node = identify(p);
		if (node)
		{
			p = node->parse(p, &data);
			linkEndChild(node);
		}
		else
		{
			break;
		}

		p = skipWhiteSpace(p);
	}

	// Was this empty?
	if (!_firstChild)
	{
		error(XmlErrorCode::XML_ERROR_DOCUMENT_EMPTY, nullptr, nullptr);
		return nullptr;
	}

	// All is well.
	return p;
}

void XmlDocument::error(XmlErrorCode err, const char* pError, void* data)
{
	// The first error in a chain is more accurate - don't set again!
	if (_error)
	{
		return;
	}

	assert(XmlErrorCode::XML_NO_ERROR < err && err < XmlErrorCode::XML_ERROR_STRING_COUNT);
	_error = true;
	_errorId = err;
	_errorDesc = XML_ERROR_TABLE[static_cast<std::underlying_type_t<XmlErrorCode>>(_errorId)];

	_errorLocation = XmlBase::ParseLocation{};

	if (pError && data)
	{
		static_cast<XmlParsingData*>(data)->stamp(pError);
		_errorLocation = static_cast<XmlParsingData*>(data)->cursor();
	}
}


/**
 * Figure out what is at *p, and parse it. Returns null if it is not an xml node.
 */
XmlNode* XmlNode::identify(const char* p)
{
	XmlNode* returnNode = nullptr;

	p = skipWhiteSpace(p);
	if (!p || !*p || *p != '<')
	{
		return nullptr;
	}

	p = skipWhiteSpace(p);

	if (!p || !*p)
	{
		return nullptr;
	}


	// What is this thing?
	// - Elements start with a letter or underscore, but xml is reserved.
	// - Comments: <!--
	// - Decleration: <?xml
	// - Everthing else is unknown to tinyxml.
	const char* commentHeader = { "<!--" };
	const char* dtdHeader = { "<!" };
	const char* cdataHeader = { "<![CDATA[" };

	if (stringEqual(p, commentHeader, false))
	{
		#ifdef DEBUG_PARSER
		TIXML_LOG("XML parsing Comment\n");
		#endif
		returnNode = new XmlComment();
	}
	else if (stringEqual(p, cdataHeader, false))
	{
		#ifdef DEBUG_PARSER
		TIXML_LOG("XML parsing CDATA\n");
		#endif
		XmlText* text = new XmlText("");
		text->CDATA(true);
		returnNode = text;
	}
	else if (stringEqual(p, dtdHeader, false))
	{
		#ifdef DEBUG_PARSER
		TIXML_LOG("XML parsing Unknown(1)\n");
		#endif
		returnNode = new XmlUnknown();
	}
	else if (isAlpha(static_cast<unsigned char>(*(p + 1))) || *(p + 1) == '_')
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
		returnNode->_parent = this;
	}
	return returnNode;
}


void XmlElement::streamIn(std::istream & in, std::string & tag)
{
	// We're called with some amount of pre-parsing. That is, some of "this"
	// element is in "tag". Go ahead and stream to the closing ">"
	while (in.good())
	{
		int c = in.get();
		if (c <= 0)
		{
			XmlDocument* doc = document();
			if (doc) { doc->error(XmlErrorCode::XML_ERROR_EMBEDDED_NULL, nullptr, nullptr); }
			return;
		}

		tag += static_cast<char>(c);

		if (c == '>')
		{
			break;
		}
	}

	if (tag.length() < 3) { return; }

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
			streamWhiteSpace(in, tag);

			// Do we have text?
			if (in.good() && in.peek() != '<')
			{
				// Yep, text.
				XmlText text("");
				text.streamIn(in, tag);

				// What follows text is a closing tag or another node.
				// Go around again and figure it out.
				continue;
			}

			// We now have either a closing tag...or another node.
			// We should be at a "<", regardless.
			if (!in.good())
			{
				return;
			}

			assert(in.peek() == '<');

			int tagIndex = static_cast<int>(tag.length());

			bool closingTag = false;
			bool firstCharFound = false;

			for (;;)
			{
				if (!in.good())
				{
					return;
				}

				int c = in.peek();
				if (c <= 0)
				{
					XmlDocument* doc = document();
					if (doc) { doc->error(XmlErrorCode::XML_ERROR_EMBEDDED_NULL, nullptr, nullptr); }
					return;
				}

				if (c == '>')
				{
					break;
				}

				tag += static_cast<char>(c);
				in.get();

				// Early out if we find the CDATA id.
				if (c == '[' && tag.size() >= 9)
				{
					std::size_t len = tag.size();
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
					{
						closingTag = true;
					}
				}
			}
			// If it was a closing tag, then read in the closing '>' to clean up the input stream.
			// If it was not, the streaming will be done by the tag.
			if (closingTag)
			{
				if (!in.good())
				{
					return;
				}

				int c = in.get();
				if (c <= 0)
				{
					XmlDocument* doc = document();
					if (doc) { doc->error(XmlErrorCode::XML_ERROR_EMBEDDED_NULL, nullptr, nullptr); }
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
				XmlNode* node = identify(tagloc);
				if (!node) { return; }

				node->streamIn(in, tag);
				delete node;
				node = nullptr;

				// No return: go around from the beginning: text, closing tag, or node.
			}
		}
	}
}


/**
 * Attribtue parsing starts: next char past '<'
 * returns: next char past '>'
 */
const char* XmlElement::parse(const char* p, void* data)
{
	p = skipWhiteSpace(p);
	XmlDocument* doc = document();

	if (!p || !*p)
	{
		if (doc) { doc->error(XmlErrorCode::XML_ERROR_PARSING_ELEMENT, nullptr, nullptr); }
		return nullptr;
	}

	if (data)
	{
		static_cast<XmlParsingData*>(data)->stamp(p);
		location = static_cast<XmlParsingData*>(data)->cursor();
	}

	if (*p != '<')
	{
		if (doc) { doc->error(XmlErrorCode::XML_ERROR_PARSING_ELEMENT, p, data); }
		return nullptr;
	}

	p = skipWhiteSpace(p + 1);

	// Read the name.
	const char* pErr = p;

	p = readName(p, _value);
	if (!p || !*p)
	{
		if (doc) { doc->error(XmlErrorCode::XML_ERROR_FAILED_TO_READ_ELEMENT_NAME, pErr, data); }
		return nullptr;
	}

	std::string endTag("</");
	endTag += _value;

	// Check for and read attributes. Also look for an empty
	// tag or an end tag.
	while (p && *p)
	{
		pErr = p;
		p = skipWhiteSpace(p);
		if (!p || !*p)
		{
			if (doc) { doc->error(XmlErrorCode::XML_ERROR_READING_ATTRIBUTES, pErr, data); }
			return nullptr;
		}
		if (*p == '/')
		{
			++p;
			// Empty tag.
			if (*p != '>')
			{
				if (doc) { doc->error(XmlErrorCode::XML_ERROR_PARSING_EMPTY, p, data); }
				return nullptr;
			}
			return (p + 1);
		}
		else if (*p == '>')
		{
			// Done with attributes (if there were any.) Read the value -- which
			// can include other elements -- read the end tag, and return.
			++p;
			p = readValue(p, data); // Note this is an Element method, and will set the error if one happens.
			if (!p || !*p)
			{
				// We were looking for the end tag, but found nothing.
				// Fix for [ 1663758 ] Failure to report error on bad XML
				if (doc) { doc->error(XmlErrorCode::XML_ERROR_READING_END_TAG, p, data); }
				return nullptr;
			}

			// We should find the end tag now
			// note that:
			// </foo > and
			// </foo>
			// are both valid end tags.
			if (stringEqual(p, endTag.c_str(), false))
			{
				p += endTag.length();
				p = skipWhiteSpace(p);
				if (p && *p && *p == '>')
				{
					++p;
					return p;
				}
				if (doc) { doc->error(XmlErrorCode::XML_ERROR_READING_END_TAG, p, data); }
				return nullptr;
			}
			else
			{
				if (doc) { doc->error(XmlErrorCode::XML_ERROR_READING_END_TAG, p, data); }
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

			attrib->document(doc);
			pErr = p;
			p = attrib->parse(p, data);

			if (!p || !*p)
			{
				if (doc) doc->error(XmlErrorCode::XML_ERROR_PARSING_ELEMENT, pErr, data);
				delete attrib;
				return nullptr;
			}

			// Handle the strange case of double attributes:
			XmlAttribute* node = attributeSet.find(attrib->name());

			if (node)
			{
				if (doc) { doc->error(XmlErrorCode::XML_ERROR_PARSING_ELEMENT, pErr, data); }
				delete attrib;
				return nullptr;
			}

			attributeSet.add(attrib);
		}
	}

	return p;
}


const char* XmlElement::readValue(const char* p, void* data)
{
	XmlDocument* doc = document();

	// Read in text and elements in any order.
	const char* pWithWhiteSpace = p;
	p = skipWhiteSpace(p);

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

			if (XmlBase::whitespaceCondensed())
			{
				p = textNode->parse(p, data);
			}
			else
			{
				// Special case: we want to keep the white space
				// so that leading spaces aren't removed.
				p = textNode->parse(pWithWhiteSpace, data);
			}

			if (!textNode->blank())
			{
				linkEndChild(textNode);
			}
			else
			{
				delete textNode;
			}
		}
		else
		{
			// We hit a '<'
			// Have we hit a new element or an end tag? This could also be an
			// XmlText in the "CDATA" style.
			if (stringEqual(p, "</", false))
			{
				return p;
			}
			else
			{
				XmlNode* node = identify(p);
				if (node)
				{
					p = node->parse(p, data);
					linkEndChild(node);
				}
				else
				{
					return nullptr;
				}
			}
		}
		pWithWhiteSpace = p;
		p = skipWhiteSpace(p);
	}

	if (!p)
	{
		if (doc) doc->error(XmlErrorCode::XML_ERROR_READING_ELEMENT_VALUE, nullptr, nullptr);
	}
	return p;
}


void XmlUnknown::streamIn(std::istream& in, std::string& tag)
{
	while (in.good())
	{
		int c = in.get();
		if (c <= 0)
		{
			XmlDocument* doc = document();
			if (doc) { doc->error(XmlErrorCode::XML_ERROR_EMBEDDED_NULL, nullptr, nullptr); }
			return;
		}
		tag += static_cast<char>(c);

		if (c == '>')
		{
			return;
		}
	}
}


const char* XmlUnknown::parse(const char* p, void* data)
{
	XmlDocument* doc = document();
	p = skipWhiteSpace(p);

	if (data)
	{
		static_cast<XmlParsingData*>(data)->stamp(p);
		location = static_cast<XmlParsingData*>(data)->cursor();
	}

	if (!p || !*p || *p != '<')
	{
		if (doc) { doc->error(XmlErrorCode::XML_ERROR_PARSING_UNKNOWN, p, data); }
		return nullptr;
	}

	++p;
	_value = "";

	while (p && *p && *p != '>')
	{
		_value += *p;
		++p;
	}

	if (!p)
	{
		if (doc) { doc->error(XmlErrorCode::XML_ERROR_PARSING_UNKNOWN, nullptr, nullptr); }
	}

	if (p && *p == '>')
	{
		return p + 1;
	}

	return p;
}

void XmlComment::streamIn(std::istream& in, std::string& tag)
{
	while (in.good())
	{
		int c = in.get();
		if (c <= 0)
		{
			XmlDocument* doc = document();
			if (doc) { doc->error(XmlErrorCode::XML_ERROR_EMBEDDED_NULL, nullptr, nullptr); }
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


/**
 * Attribtue parsing starts: at the ! of the !--
 * 					 returns: next char past '>'
 */
const char* XmlComment::parse(const char* p, void* data)
{
	XmlDocument* doc = document();
	_value = "";

	p = skipWhiteSpace(p);

	if (data)
	{
		static_cast<XmlParsingData*>(data)->stamp(p);
		location = static_cast<XmlParsingData*>(data)->cursor();
	}
	const char* startTag = "<!--";
	const char* endTag = "-->";

	if (!stringEqual(p, startTag, false))
	{
		if (doc) { doc->error(XmlErrorCode::XML_ERROR_PARSING_COMMENT, p, data); }
		return nullptr;
	}

	p += strlen(startTag);

	_value = "";
	// Keep all the white space.
	while (p && *p && !stringEqual(p, endTag, false))
	{
		_value.append(p, 1);
		++p;
	}

	if (p && *p)
	{
		p += strlen(endTag);
	}

	return p;
}


const char* XmlAttribute::parse(const char* p, void* data)
{
	p = skipWhiteSpace(p);
	if (!p || !*p) return nullptr;

	if (data)
	{
		static_cast<XmlParsingData*>(data)->stamp(p);
		location = static_cast<XmlParsingData*>(data)->cursor();
	}
	// Read the name, the '=' and the value.
	const char* pErr = p;
	p = readName(p, _name);
	if (!p || !*p)
	{
		if (_document) _document->error(XmlErrorCode::XML_ERROR_READING_ATTRIBUTES, pErr, data);
		return nullptr;
	}
	p = skipWhiteSpace(p);
	if (!p || !*p || *p != '=')
	{
		if (_document) _document->error(XmlErrorCode::XML_ERROR_READING_ATTRIBUTES, p, data);
		return nullptr;
	}

	++p; // skip '='
	p = skipWhiteSpace(p);
	if (!p || !*p)
	{
		if (_document) _document->error(XmlErrorCode::XML_ERROR_READING_ATTRIBUTES, p, data);
		return nullptr;
	}

	const char* end;
	const char SINGLE_QUOTE = '\'';
	const char DOUBLE_QUOTE = '\"';

	if (*p == SINGLE_QUOTE)
	{
		++p;
		end = "\'"; // single quote in string
		p = readText(p, &_value, false, end, false);
	}
	else if (*p == DOUBLE_QUOTE)
	{
		++p;
		end = "\""; // double quote in string
		p = readText(p, &_value, false, end, false);
	}
	else
	{
		// All attribute values should be in single or double quotes. But this is such a common error
		// that the parser will try its best, even without them.
		_value = "";
		while ((p && *p) && !white_space(*p) && *p != '/' && *p != '>') // existance, whitespace, tag end
		{
			if (*p == SINGLE_QUOTE || *p == DOUBLE_QUOTE)
			{
				// [ 1451649 ] Attribute values with trailing quotes not handled correctly
				// We did not have an opening quote but seem to have a
				// closing one. Give up and throw an error.
				if (_document) { _document->error(XmlErrorCode::XML_ERROR_READING_ATTRIBUTES, p, data); }
				return nullptr;
			}

			_value += *p;
			++p;
		}
	}

	return p;
}


void XmlText::streamIn(std::istream& in, std::string& tag)
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
			XmlDocument* doc = document();
			if (doc) { doc->error(XmlErrorCode::XML_ERROR_EMBEDDED_NULL, nullptr, nullptr); }

			return;
		}

		tag += static_cast<char>(c);
		in.get(); // "commits" the peek made above

		if (cdata && c == '>' && tag.size() >= 3)
		{
			std::size_t len = tag.size();
			if (tag[len - 2] == ']' && tag[len - 3] == ']')
			{
				// terminator of cdata.
				return;
			}
		}
	}
}

const char* XmlText::parse(const char* p, void* data)
{
	_value = "";
	XmlDocument* doc = document();

	if (data)
	{
		static_cast<XmlParsingData*>(data)->stamp(p);
		location = static_cast<XmlParsingData*>(data)->cursor();
	}

	const char* const startTag = "<![CDATA[";
	const char* const endTag = "]]>";

	if (cdata || stringEqual(p, startTag, false))
	{
		cdata = true;

		if (!stringEqual(p, startTag, false))
		{
			if (doc) { doc->error(XmlErrorCode::XML_ERROR_PARSING_CDATA, p, data); }

			return nullptr;
		}
		p += strlen(startTag);

		// Keep all the white space, ignore the encoding, etc.
		while (p && *p && !stringEqual(p, endTag, false))
		{
			_value += *p;
			++p;
		}

		std::string dummy;
		p = readText(p, &dummy, false, endTag, false);
		return p;
	}
	else
	{
		bool ignoreWhite = true;

		const char* end = "<";
		p = readText(p, &_value, ignoreWhite, end, false);
		if (p && *p)
		{
			// don't truncate the '<'
			return p - 1;
		}

		return nullptr;
	}
}


} // namespace Xml
}
