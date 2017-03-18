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


// Used by the parsing routines.
enum XmlEncoding
{
	TIXML_ENCODING_UNKNOWN,
	TIXML_ENCODING_UTF8,
	TIXML_ENCODING_LEGACY
};

const XmlEncoding TIXML_DEFAULT_ENCODING = TIXML_ENCODING_UNKNOWN;


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
	XmlBase() : userData(nullptr) { fillErrorTable(); }
	virtual ~XmlBase() {}

	/**
	* All TinyXml classes can print themselves to a filestream or a string.
	*
	* This is a formatted print, and will insert tabs and newlines.
	*
	* (For an unformatted stream, use the << operator.)
	*/
	virtual void Print(std::string& buf, int depth) const = 0;

	/**
	* The world does not agree on whether white space should be kept or not. In order
	* to make everyone happy, these global, static functions are provided to set whether
	* or not TinyXml will condense all white space	into a single space or not. The
	* default is to condense. Note changing this value is not thread safe.
	*/
	static void SetCondenseWhiteSpace(bool condense) { condenseWhiteSpace = condense; }

	/// Return the current white space setting.
	static bool IsWhiteSpaceCondensed() { return condenseWhiteSpace; }

	int row() const;
	int column() const;

	void  SetUserData(void* user) { userData = user; }		///< Set a pointer to arbitrary user data.
	void* GetUserData() { return userData; }				///< Get a pointer to arbitrary user data.
	const void* GetUserData() const { return userData; }	///< Get a pointer to arbitrary user data.

															// Table that returns, for a given lead byte, the total number of bytes in the UTF-8 sequence.
	static const int utf8ByteTable[256];

	virtual const char* Parse(const char* p, TiXmlParsingData* data, XmlEncoding encoding) = 0;

	/**
	* Expands entities in a string. Note this should not contian the tag's '<', '>', etc,
	* or they will be transformed into entities!
	*/
	static void EncodeString(const std::string& str, std::string& out);

	enum
	{
		TIXML_NO_ERROR = 0,
		TIXML_ERROR,
		TIXML_ERROR_OPENING_FILE,
		TIXML_ERROR_PARSING_ELEMENT,
		TIXML_ERROR_FAILED_TO_READ_ELEMENT_NAME,
		TIXML_ERROR_READING_ELEMENT_VALUE,
		TIXML_ERROR_READING_ATTRIBUTES,
		TIXML_ERROR_PARSING_EMPTY,
		TIXML_ERROR_READING_END_TAG,
		TIXML_ERROR_PARSING_UNKNOWN,
		TIXML_ERROR_PARSING_COMMENT,
		TIXML_ERROR_PARSING_DECLARATION,
		TIXML_ERROR_DOCUMENT_EMPTY,
		TIXML_ERROR_EMBEDDED_NULL,
		TIXML_ERROR_PARSING_CDATA,
		TIXML_ERROR_DOCUMENT_TOP_ONLY,

		TIXML_ERROR_STRING_COUNT
	};

protected:
	static const char* SkipWhiteSpace(const char*, XmlEncoding encoding);

	inline static bool IsWhiteSpace(char c) { return (isspace((unsigned char)c) || c == '\n' || c == '\r'); }
	inline static bool IsWhiteSpace(int c) { if (c < 256) return IsWhiteSpace((char)c);	return false; }

	static bool	StreamWhiteSpace(std::istream& in, std::string& tag);
	static bool StreamTo(std::istream& in, int character, std::string& tag);

	/**
	* Reads an XML name into the string provided. Returns a pointer just past
	* the last character of the name, or 0 if the function has an error.
	*/
	static const char* ReadName(const char* p, std::string& name, XmlEncoding encoding);

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
	static const char* ReadText(const char* in, std::string* text, bool ignoreWhiteSpace, const char* endTag, bool ignoreCase, XmlEncoding encoding);

	// If an entity has been found, transform it into a character.
	static const char* GetEntity(const char* in, char* value, int* length, XmlEncoding encoding);

	// Get a character, while interpreting entities.
	// The length can be from 0 to 4 bytes.
	inline static const char* GetChar(const char* p, char* _value, int* length, XmlEncoding encoding);

	// Return true if the next characters in the stream are any of the endTag sequences.
	// Ignore case only works for english, and should only be relied on when comparing
	// to English words: StringEqual( p, "version", true ) is fine.
	static bool StringEqual(const char* p, const char* endTag, bool ignoreCase, XmlEncoding encoding);

	static std::vector<std::string> errorString;

	XmlCursor location;

	void* userData;		/**< Field containing a generic user pointer */

						// None of these methods are reliable for any language except English.
						// Good for approximation, not great for accuracy.
	static int IsAlpha(unsigned char anyByte, XmlEncoding encoding);
	static int IsAlphaNum(unsigned char anyByte, XmlEncoding encoding);
	inline static int ToLower(int v, XmlEncoding encoding)
	{
		if (encoding == TIXML_ENCODING_UTF8)
		{
			if (v < 128) return tolower(v);
			return v;
		}
		else
		{
			return tolower(v);
		}
	}
	static void ConvertUTF32ToUTF8(unsigned long input, char* output, int* length);

private:
	XmlBase(const XmlBase&); // Explicitly disallowed.
	void operator=(const XmlBase& base); // Explicitly disallowed.

private:
	void fillErrorTable();

	struct Entity
	{
		const char*     str;
		unsigned int	strLength;
		char		    chr;
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