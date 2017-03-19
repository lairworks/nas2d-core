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
#pragma once

#include <string>
#include <vector>

namespace NAS2D {
namespace Xml {

class XmlAttribute;
class XmlComment;
class XmlDocument;
class XmlElement;
class XmlText;
class XmlUnknown;
class TiXmlParsingData;

/**
 * Internal structure for tracking location of items in the XML file.
 */
struct XmlCursor
{
	XmlCursor() { clear(); }
	void clear() { row = col = -1; }

	int row; // 0 based.
	int col; // 0 based.
};


/**
 * XmlBase is a base class for every class in TinyXml. It does little except to establish
 * that TinyXml classes can be printed and provide some utility functions.
 * 
 * In XML, the document and elements can contain other elements and other types of nodes.
 * 
 * A Document can contain:
 * \li Element (container or leaf)
 * \li Comment (leaf)
 * \li Unknown (leaf)
 * \li Declaration(leaf)
 * 
 * An Element can contain:
 * \li Element (container or leaf)
 * \li Text (leaf)
 * \li Attributes (not on tree)
 * \li Comment (leaf)
 * \li Unknown (leaf)
 * 
 * A Decleration contains: Attributes (not on tree)
 */
class XmlBase
{
	friend class XmlNode;
	friend class XmlElement;
	friend class XmlDocument;

public:
	XmlBase() : _userData(nullptr) {}
	virtual ~XmlBase() {}

	/**
	 * Writes the XML entity to a string buffer.
	 * 
	 * \param	buf		Reference to a \c std::string to write the XML entity to.
	 * \param	indent	Indentation level.
	 */
	virtual void write(std::string& buf, int indent) const = 0;

	/**
	 * The world does not agree on whether white space should be kept or not. In order
	 * to make everyone happy, these global, static functions are provided to set whether
	 * or not TinyXml will condense all white space	into a single space or not. The
	 * default is to condense. Note changing this value is not thread safe.
	 */
	static void whitespaceCondensed(bool condense) { condenseWhiteSpace = condense; }

	/// Return the current white space setting.
	static bool whitespaceCondensed() { return condenseWhiteSpace; }

	int row() const;
	int column() const;

	void userData(void* user) { _userData = user; }		///< Set a pointer to arbitrary user data.
	void* userData() { return _userData; }				///< Get a pointer to arbitrary user data.
	const void* userData() const { return _userData; }	///< Get a pointer to arbitrary user data.

	virtual const char* parse(const char* p, TiXmlParsingData* data) = 0;

	enum XmlErrorCode
	{
		XML_NO_ERROR = 0,
		XML_ERROR,
		XML_ERROR_PARSING_ELEMENT,
		XML_ERROR_FAILED_TO_READ_ELEMENT_NAME,
		XML_ERROR_READING_ELEMENT_VALUE,
		XML_ERROR_READING_ATTRIBUTES,
		XML_ERROR_PARSING_EMPTY,
		XML_ERROR_READING_END_TAG,
		XML_ERROR_PARSING_UNKNOWN,
		XML_ERROR_PARSING_COMMENT,
		XML_ERROR_PARSING_DECLARATION,
		XML_ERROR_DOCUMENT_EMPTY,
		XML_ERROR_EMBEDDED_NULL,
		XML_ERROR_PARSING_CDATA,
		XML_ERROR_DOCUMENT_TOP_ONLY,

		XML_ERROR_STRING_COUNT
	};

protected:
	static const char* skipWhiteSpace(const char*);

	inline static bool white_space(char c) { return (isspace(static_cast<unsigned char>(c)) || c == '\n' || c == '\r'); }
	inline static bool white_space(int c) { if (c < 256) return white_space(static_cast<char>(c));	return false; }

	static bool	streamWhiteSpace(std::istream& in, std::string& tag);
	static bool streamTo(std::istream& in, int character, std::string& tag);

	/**
	 * Reads an XML name into the string provided. Returns a pointer just past
	 * the last character of the name, or 0 if the function has an error.
	 */
	static const char* readName(const char* p, std::string& name);

	/**
	 * Reads text. Returns a pointer past the given end tag. Wickedly complex options, but it
	 * keeps the (sensitive) code in one place.
	 *
	 * \param in				Where to start
	 * \param text				The string read
	 * \param ignoreWhiteSpace	Wheather to keep the white space
	 * \param endTag			What ends this text
	 * \param ignoreCase		Whether to ignore case in the end tag
	 * \param encoding			The current encoding.
	 */
	static const char* readText(const char* in, std::string* text, bool ignoreWhiteSpace, const char* endTag, bool ignoreCase);

	// If an entity has been found, transform it into a character.
	static const char* getEntity(const char* in, char* value, int* length);

	// Get a character, while interpreting entities.
	// The length can be from 0 to 4 bytes.
	inline static const char* getChar(const char* p, char* _value, int* length);

	// Return true if the next characters in the stream are any of the endTag sequences.
	// Ignore case only works for english, and should only be relied on when comparing
	// to English words: StringEqual( p, "version", true ) is fine.
	static bool stringEqual(const char* p, const char* endTag, bool ignoreCase);

	static int isAlpha(unsigned char anyByte);
	static int isAlphaNum(unsigned char anyByte);
	
	inline static int toLower(int v) { return tolower(v); }

protected:
	XmlCursor location;
	void* _userData;		/**< Field containing a generic user pointer */

private:
	XmlBase(const XmlBase&); // Explicitly disallowed.
	void operator=(const XmlBase& base); // Explicitly disallowed.

private:
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

	static Entity entity[NUM_ENTITY];
	static bool condenseWhiteSpace;
};




} // namespace Xml
} // namespace NAS2D