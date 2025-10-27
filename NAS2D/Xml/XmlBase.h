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
#pragma once

#include <string>
#include <vector>

namespace NAS2D::Xml
{

class XmlAttribute;
class XmlComment;
class XmlDocument;
class XmlElement;
class XmlText;
class XmlUnknown;

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
	XmlBase() = default;
	XmlBase(const XmlBase&) = delete;
	void operator=(const XmlBase& base) = delete;
	virtual ~XmlBase();

	/**
	 * Writes the XML entity to a string buffer.
	 *
	 * \param	buf		Reference to a \c std::string to write the XML entity to.
	 * \param	indent	Indentation level.
	 */
	virtual void write(std::string& buf, int indent) const = 0;

	static void whitespaceCondensed(bool condense);
	static bool whitespaceCondensed();

	int row() const;
	int column() const;

	virtual const char* parse(const char* p, void* data) = 0;

	enum class XmlErrorCode : std::size_t
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

	struct ParseLocation
	{
		int row{0};
		int column{0};
	};

protected:

	static const char* skipWhiteSpace(const char*);

	inline static bool white_space(char c) { return (isspace(static_cast<unsigned char>(c)) || c == '\n' || c == '\r'); }
	inline static bool white_space(int c) { if (c < 256) return white_space(static_cast<char>(c)); return false; }

	static bool streamWhiteSpace(std::istream& in, std::string& tag);
	static bool streamTo(std::istream& in, int character, std::string& tag);

	static const char* readName(const char* p, std::string& name);
	static const char* readText(const char* in, std::string* text, bool ignoreWhiteSpace, const char* endTag, bool ignoreCase);
	static const char* getEntity(const char* in, char* value, std::size_t* length);

	inline static const char* getChar(const char* p, char* _value, std::size_t* length);

	static bool stringEqual(const char* p, const char* endTag, bool ignoreCase);

	static int isAlpha(unsigned char anyByte);
	static int isAlphaNum(unsigned char anyByte);

	inline static int toLower(int v) { return tolower(v); }

	ParseLocation location{};

private:
	static bool condenseWhiteSpace;
};

}
