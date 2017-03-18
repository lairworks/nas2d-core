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

// ==================================================================================
// = This is a heavily modified version of TinyXML based on v2.6.2.
// ==================================================================================
// = Notable Modifications:
// =	- Force use of STL containers.
// =	- Removed redundant calls that convert between basic types.
// =	- Removed file input/output and substituted buffer input/output
// ==================================================================================
#pragma once

#ifdef _MSC_VER
#pragma warning(push)
#pragma warning(disable : 4530)
#pragma warning(disable : 4786)
#endif

#include <string>
#include <vector>

// Deprecated library function hell. Compilers want to use the new safe versions. This
// probably doesn't fully address the problem, but it gets closer. There are too many
// compilers for me to fully test. If you get compilation troubles, undefine TIXML_SAFE
#define TIXML_SAFE

#ifdef TIXML_SAFE
	#if defined(_MSC_VER) && (_MSC_VER >= 1400 )
		// Microsoft visual studio, version 2005 and higher.
		#define TIXML_SNPRINTF _snprintf_s
		#define TIXML_SSCANF   sscanf_s
	#elif defined(_MSC_VER) && (_MSC_VER >= 1200 )
		// Microsoft visual studio, version 6 and higher.
		//#pragma message( "Using _sn* functions." )
		#define TIXML_SNPRINTF _snprintf
		#define TIXML_SSCANF   sscanf
	#elif defined(_MSC_VER)
	#error This version of Visual C++ is no longer support. Consider upgrading to a newer version.
	#elif defined(__GNUC__) && (__GNUC__ >= 3 )
		// GCC version 3 and higher.s
		//#warning( "Using sn* functions." )
		#define TIXML_SNPRINTF snprintf
		#define TIXML_SSCANF   sscanf
	#else
		#define TIXML_SNPRINTF snprintf
		#define TIXML_SSCANF   sscanf
	#endif
#endif	

#include "NAS2D/Xml/XmlBase.h"
#include "NAS2D/Xml/XmlNode.h"
#include "NAS2D/Xml/XmlVisitor.h"


namespace NAS2D {
namespace Xml {

class TiXmlDocument;
class TiXmlElement;
class TiXmlComment;
class TiXmlUnknown;
class TiXmlAttribute;
class TiXmlText;
class TiXmlDeclaration;
class TiXmlParsingData;

// Only used by Attribute::Query functions
enum
{
	TIXML_SUCCESS,
	TIXML_NO_ATTRIBUTE,
	TIXML_WRONG_TYPE
};





/**
 * An attribute is a name-value pair. Elements have an arbitrary number of attributes,
 * each with a unique name.
 * 
 * \note	The attributes are not TiXmlNodes, since they are not part of the TinyXML
 *			document object model. There are other suggested ways to look at this problem.
 */
class TiXmlAttribute : public TiXmlBase
{
	friend class TiXmlAttributeSet;

public:
	TiXmlAttribute() : TiXmlBase(), document(nullptr), prev(nullptr), next(nullptr) {}

	/// Construct an attribute with a name and value.
	TiXmlAttribute(const std::string& _name, std::string& _value) : TiXmlBase(), document(nullptr), name(_name), value(_value), prev(nullptr), next(nullptr) {}

	/**
	 * Name of the Attribute.
	 */
	const std::string&	Name()  const { return name; }

	/**
	 * Value of the Attribute.
	 */
	const std::string&	Value() const { return value; }

	int IntValue() const;					///< Return the value of this attribute, converted to an integer.
	double DoubleValue() const;				///< Return the value of this attribute, converted to a double.

	int QueryIntValue(int& _value) const;
	int QueryDoubleValue(double& _value) const;

	void SetName(const std::string& _name) { name = _name; }		///< Set the name of this attribute.
	void SetValue(const std::string& _value) { value = _value; }	///< Set the value.

	void SetIntValue(int _value);									///< Set the value from an integer.
	void SetDoubleValue(double _value);								///< Set the value from a double.

	/// Get the next sibling attribute in the DOM. Returns nullptr at end.
	const TiXmlAttribute* Next() const;
	TiXmlAttribute* Next() { return const_cast<TiXmlAttribute*>((const_cast<const TiXmlAttribute*>(this))->Next()); }

	/// Get the previous sibling attribute in the DOM. Returns nullptr at beginning.
	const TiXmlAttribute* Previous() const;
	TiXmlAttribute* Previous() { return const_cast<TiXmlAttribute*>((const_cast<const TiXmlAttribute*>(this))->Previous()); }

	bool operator==(const TiXmlAttribute& rhs) const { return rhs.name == name; }
	bool operator<(const TiXmlAttribute& rhs)	 const { return name < rhs.name; }
	bool operator>(const TiXmlAttribute& rhs)  const { return name > rhs.name; }

	/**
	 * Attribute parsing starts: first letter of the name
	 * returns: the next char after the value end quote
	 */
	virtual const char* Parse(const char* p, TiXmlParsingData* data, TiXmlEncoding encoding);

	// Prints this Attribute to a FILE stream.
	virtual void Print(std::string& buf, int depth) const;

	// [internal use]
	// Set the document pointer so the attribute can report errors.
	void SetDocument(TiXmlDocument* doc) { document = doc; }

private:
	TiXmlAttribute(const TiXmlAttribute&); // Explicitly disallowed.
	void operator=(const TiXmlAttribute& base); // Explicitly disallowed.

	TiXmlDocument*	document;	// A pointer back to a document, for error reporting.
	std::string		name;
	std::string		value;
	TiXmlAttribute*	prev;
	TiXmlAttribute*	next;
};


/*	A class used to manage a group of attributes.
	It is only used internally, both by the ELEMENT and the DECLARATION.
	
	The set can be changed transparent to the Element and Declaration
	classes that use it, but NOT transparent to the Attribute
	which has to implement a next() and previous() method. Which makes
	it a bit problematic and prevents the use of STL.

	This version is implemented with circular lists because:
		- I like circular lists
		- it demonstrates some independence from the (typical) doubly linked list.
*/
class TiXmlAttributeSet
{
public:
	TiXmlAttributeSet();
	~TiXmlAttributeSet();

	void Add(TiXmlAttribute* attribute);
	void Remove(TiXmlAttribute* attribute);

	const TiXmlAttribute* First()	const { return (sentinel.next == &sentinel) ? 0 : sentinel.next; }
	TiXmlAttribute* First() { return (sentinel.next == &sentinel) ? 0 : sentinel.next; }
	const TiXmlAttribute* Last() const { return (sentinel.prev == &sentinel) ? 0 : sentinel.prev; }
	TiXmlAttribute* Last() { return (sentinel.prev == &sentinel) ? 0 : sentinel.prev; }

	TiXmlAttribute*	Find(const std::string& _name) const;
	TiXmlAttribute* FindOrCreate(const std::string& _name);


private:
	TiXmlAttributeSet(const TiXmlAttributeSet&); // Explicitly disallowed.
	void operator=(const TiXmlAttributeSet&); // Explicitly disallowed.

	TiXmlAttribute sentinel;	/**< Comment me. */
};


/** The element is a container class. It has a value, the element name,
	and can contain other elements, text, comments, and unknowns.
	Elements also contain an arbitrary number of attributes.
*/
class TiXmlElement : public TiXmlNode
{
public:
	/// Construct an element.
	TiXmlElement(const std::string& _value) : TiXmlNode(TiXmlNode::TINYXML_ELEMENT) { value = _value; }
	TiXmlElement(const TiXmlElement& copy) : TiXmlNode(TiXmlNode::TINYXML_ELEMENT) { copy.CopyTo(this); }

	TiXmlElement& operator=(const TiXmlElement& base);

	virtual ~TiXmlElement();

	/** Given an attribute name, Attribute() returns the value
		for the attribute of that name, or null if none exists.
	*/
	std::string Attribute(const std::string& name) const;

	/** Given an attribute name, Attribute() returns the value
		for the attribute of that name, or null if none exists.
		If the attribute exists and can be converted to an integer,
		the integer value will be put in the return 'i', if 'i'
		is non-null.
	*/
	std::string Attribute(const std::string& name, int& i) const;

	/** Given an attribute name, Attribute() returns the value
		for the attribute of that name, or null if none exists.
		If the attribute exists and can be converted to an double,
		the double value will be put in the return 'd', if 'd'
		is non-null.
	*/
	std::string Attribute(const std::string& name, double& d) const;

	/** QueryIntAttribute examines the attribute - it is an alternative to the
		Attribute() method with richer error checking.
		If the attribute is an integer, it is stored in 'value' and
		the call returns TIXML_SUCCESS. If it is not
		an integer, it returns TIXML_WRONG_TYPE. If the attribute
		does not exist, then TIXML_NO_ATTRIBUTE is returned.
	*/
	int QueryIntAttribute(const std::string& name, int& _value) const;

	/// QueryDoubleAttribute examines the attribute - see QueryIntAttribute().
	int QueryDoubleAttribute(const std::string& name, double& _value) const;

	/// QueryFloatAttribute examines the attribute - see QueryIntAttribute().
	int QueryFloatAttribute(const std::string& name, float& _value) const
	{
		double d = 0;
		int result = QueryDoubleAttribute(name, d);
		if (result == TIXML_SUCCESS)
			_value = static_cast<float>(d);

		return result;
	}

	/// QueryStringAttribute examines the attribute - see QueryIntAttribute().
	int QueryStringAttribute(const std::string& name, std::string& _value) const
	{
		std::string str = Attribute(name);
		if (!str.empty())
		{
			_value = str;
			return TIXML_SUCCESS;
		}
		return TIXML_NO_ATTRIBUTE;
	}


	/**
	 * Sets an attribute of name to a given value. The attribute
	 * will be created if it does not exist, or changed if it does.
	 */
	void SetAttribute(const std::string& name, const std::string& _value);

	/**
	 * Sets an attribute of name to a given value. The attribute
	 * will be created if it does not exist, or changed if it does.
	 */
	void SetAttribute(const std::string& name, int value);

	/**
	 * Sets an attribute of name to a given value. The attribute
	 * will be created if it does not exist, or changed if it does.
	 */
	void SetDoubleAttribute(const std::string& name, double value);

	/**
	 * Deletes an attribute with the given name.
	 */
	void RemoveAttribute(const std::string& name);


	const TiXmlAttribute* FirstAttribute() const { return attributeSet.First(); }		///< Access the first attribute in this element.
	TiXmlAttribute* FirstAttribute() { return attributeSet.First(); }
	const TiXmlAttribute* LastAttribute()	const { return attributeSet.Last(); }		///< Access the last attribute in this element.
	TiXmlAttribute* LastAttribute() { return attributeSet.Last(); }

	/** Convenience function for easy access to the text inside an element. Although easy
		and concise, GetText() is limited compared to getting the TiXmlText child
		and accessing it directly.

		If the first child of 'this' is a TiXmlText, the GetText()
		returns the character string of the Text node, else null is returned.

		This is a convenient method for getting the text of simple contained text:
		@verbatim
		<foo>This is text</foo>
		const char* str = fooElement->GetText();
		@endverbatim

		'str' will be a pointer to "This is text".

		Note that this function can be misleading. If the element foo was created from
		this XML:
		@verbatim
		<foo><b>This is text</b></foo>
		@endverbatim

		then the value of str would be null. The first child node isn't a text node, it is
		another element. From this XML:
		@verbatim
		<foo>This is <b>text</b></foo>
		@endverbatim
		GetText() will return "This is ".

		WARNING: GetText() accesses a child node - don't become confused with the
				 similarly named TiXmlHandle::Text() and TiXmlNode::ToText() which are
				 safe type casts on the referenced node.
	*/
	const std::string& GetText() const;

	/// Creates a new Element and returns it - the returned element is a copy.
	virtual TiXmlNode* Clone() const;
	
	/// Print the Element to a buffer.
	virtual void Print(std::string& buf, int depth) const;

	/*	Attribtue parsing starts: next char past '<'
						 returns: next char past '>'
	*/
	virtual const char* Parse(const char* p, TiXmlParsingData* data, TiXmlEncoding encoding);

	virtual const TiXmlElement*     ToElement()     const { return this; } ///< Cast to a more defined type. Will return null not of the requested type.
	virtual TiXmlElement*           ToElement() { return this; } ///< Cast to a more defined type. Will return null not of the requested type.

	/** Walk the XML tree visiting this node and all of its children.
	*/
	virtual bool Accept(XmlVisitor* visitor) const;

protected:
	void CopyTo(TiXmlElement* target) const;
	void ClearThis();	// like clear, but initializes 'this' object as well

	// Used to be public [internal use]
	virtual void StreamIn(std::istream& in, std::string& tag);

	/*	[internal use]
		Reads the "value" of the element -- another element, or text.
		This should terminate with the current end tag.
	*/
	const char* ReadValue(const char* in, TiXmlParsingData* prevData, TiXmlEncoding encoding);

private:
	TiXmlAttributeSet attributeSet;
};


/**	An XML comment.
*/
class TiXmlComment : public TiXmlNode
{
public:
	TiXmlComment() : TiXmlNode(TiXmlNode::TINYXML_COMMENT) {}

	TiXmlComment(const std::string& _value) : TiXmlNode(TiXmlNode::TINYXML_COMMENT) { SetValue(_value); }
	TiXmlComment(const TiXmlComment& copy) { copy.CopyTo(this); }
	TiXmlComment& operator=(const TiXmlComment& base);

	virtual ~TiXmlComment() {}

	/// Returns a copy of this Comment.
	virtual TiXmlNode* Clone() const;

	virtual void Print(std::string& buf, int depth) const;

	/**
	 * Attribtue parsing starts: at the ! of the !--
	 * 					 returns: next char past '>'
	 */
	virtual const char* Parse(const char* p, TiXmlParsingData* data, TiXmlEncoding encoding);

	virtual const TiXmlComment* ToComment() const { return this; }	///< Cast to a more defined type. Will return null not of the requested type.
	virtual TiXmlComment* ToComment() { return this; }				///< Cast to a more defined type. Will return null not of the requested type.

	/**
	 * Walk the XML tree visiting this node and all of its children.
	 */
	virtual bool Accept(XmlVisitor* visitor) const { return visitor->Visit(*this); }

protected:
	void CopyTo(TiXmlComment* target) const { TiXmlNode::CopyTo(target); }
	virtual void StreamIn(std::istream& in, std::string& tag);

private:

};


/**
 * XML text.
 * A text node has two ways to output text: "normal" output and CDATA. It will default
 * to the mode it was parsed from the XML file.
 */
class TiXmlText : public TiXmlNode
{
	friend class TiXmlElement;
public:
	/**
	 * Constructor for text element. By default, it is treated as normal, encoded text.
	 * If you want it be output as a CDATA text	element, call \c CDATA(true).
	 */
	TiXmlText(const std::string& initValue) : TiXmlNode(TiXmlNode::TINYXML_TEXT), cdata(false) { SetValue(initValue); }
	virtual ~TiXmlText() {}

	TiXmlText(const TiXmlText& copy) : TiXmlNode(TiXmlNode::TINYXML_TEXT) { copy.CopyTo(this); }
	TiXmlText& operator=(const TiXmlText& base) { base.CopyTo(this); return *this; }

	virtual void Print(std::string& buf, int depth) const;

	/**
	 * Queries whether this represents text using a CDATA section.
	 */
	bool CDATA() const { return cdata; }

	/**
	 * Turns on or off a CDATA representation of text.
	 */
	void CDATA(bool _cdata) { cdata = _cdata; }

	virtual const char* Parse(const char* p, TiXmlParsingData* data, TiXmlEncoding encoding);

	virtual const TiXmlText* ToText() const { return this; }
	virtual TiXmlText* ToText() { return this; }

	/**
	 * Walk the XML tree visiting this node and all of its children.
	 */
	virtual bool Accept(XmlVisitor* visitor) const { return visitor->Visit(*this); }

protected:
	///  [internal use] Creates a new Element and returns it.
	virtual TiXmlNode* Clone() const;
	void CopyTo(TiXmlText* target) const;

	bool Blank() const;	// returns true if all white space and new lines
	virtual void StreamIn(std::istream& in, std::string& tag);

private:
	bool cdata;			// true if this should be input and output as a CDATA style text element
};


/**
 * In correct XML the declaration is the first entry in the file.
 * \code{.xml}
 * <?xml version="1.0" standalone="yes"?>
 * \endcode
 * 
 * TinyXml will happily read or write files without a declaration, however. There are three
 * possible attributes to the declaration: version, encoding, and standalone.
 * 
 * \note	In this version of the code, the attributes are handled as special cases, not
 *			generic attributes, simply because there can only be at most three and they are
 *			always the same.
 */
class TiXmlDeclaration : public TiXmlNode
{
public:
	TiXmlDeclaration() : TiXmlNode(TiXmlNode::TINYXML_DECLARATION) {}
	TiXmlDeclaration(const std::string& _version, const std::string& _encoding, const std::string& _standalone) : TiXmlNode(TiXmlNode::TINYXML_DECLARATION), version(_version), encoding(_encoding), standalone(_standalone) {}

	TiXmlDeclaration(const TiXmlDeclaration& copy) : TiXmlNode(TiXmlNode::TINYXML_DECLARATION) { copy.CopyTo(this); }
	TiXmlDeclaration& operator=(const TiXmlDeclaration& copy);

	virtual ~TiXmlDeclaration() {}

	/// Version. Will return an empty string if none was found.
	const std::string& Version() const { return version; }
	/// Encoding. Will return an empty string if none was found.
	const std::string& Encoding() const { return encoding; }
	/// Is this a standalone document?
	const std::string& Standalone() const { return standalone; }

	/// Creates a copy of this Declaration and returns it.
	virtual TiXmlNode* Clone() const;
	// Print this declaration to a FILE stream.
	virtual void Print(std::string& buf, int depth) const;

	virtual const char* Parse(const char* p, TiXmlParsingData* data, TiXmlEncoding encoding);

	virtual const TiXmlDeclaration* ToDeclaration() const { return this; }
	virtual TiXmlDeclaration* ToDeclaration() { return this; }

	/**
	 * Walk the XML tree visiting this node and all of its children.
	 */
	virtual bool Accept(XmlVisitor* visitor) const { return visitor->Visit(*this); }

protected:
	void CopyTo(TiXmlDeclaration* target) const;
	virtual void StreamIn(std::istream& in, std::string& tag);

private:
	std::string version;	/**< Comment Me. */
	std::string encoding;	/**< Comment Me. */
	std::string standalone;	/**< Comment Me. */
};


/** Any tag that tinyXml doesn't recognize is saved as an
	unknown. It is a tag of text, but should not be modified.
	It will be written back to the XML, unchanged, when the file
	is saved.

	DTD tags get thrown into TiXmlUnknowns.
*/
class TiXmlUnknown : public TiXmlNode
{
public:
	TiXmlUnknown() : TiXmlNode(TiXmlNode::TINYXML_UNKNOWN) {}
	virtual ~TiXmlUnknown() {}

	TiXmlUnknown(const TiXmlUnknown& copy) : TiXmlNode(TiXmlNode::TINYXML_UNKNOWN) { copy.CopyTo(this); }
	TiXmlUnknown& operator=(const TiXmlUnknown& copy) { copy.CopyTo(this); return *this; }

	/// Creates a copy of this Unknown and returns it.
	virtual TiXmlNode* Clone() const;
	
	/**
	 * Print this Unknown to a buffer.
	 */
	virtual void Print(std::string& buf, int depth) const;

	virtual const char* Parse(const char* p, TiXmlParsingData* data, TiXmlEncoding encoding);

	virtual const TiXmlUnknown* ToUnknown() const { return this; }	///< Cast to a more defined type. Will return null not of the requested type.
	virtual TiXmlUnknown* ToUnknown() { return this; }				///< Cast to a more defined type. Will return null not of the requested type.

	/**
	 * Walk the XML tree visiting this node and all of its children.
	 */
	virtual bool Accept(XmlVisitor* visitor) const { return visitor->Visit(*this); }

protected:
	void CopyTo(TiXmlUnknown* target) const { TiXmlNode::CopyTo(target); }

	virtual void StreamIn(std::istream& in, std::string& tag);

private:

};


/** Always the top level node. A document binds together all the
	XML pieces. It can be saved, loaded, and printed to the screen.
	The 'value' of a document node is the xml file name.
*/
class TiXmlDocument : public TiXmlNode
{
public:
	TiXmlDocument();
	TiXmlDocument(const std::string& documentName);

	TiXmlDocument(const TiXmlDocument& copy);
	TiXmlDocument& operator=(const TiXmlDocument& copy);

	virtual ~TiXmlDocument() {}

	/**
	 * Parse the given null terminated block of xml data. Passing in an encoding to this
	 * method (either TIXML_ENCODING_LEGACY or TIXML_ENCODING_UTF8 will force TinyXml
	 * to use that encoding, regardless of what TinyXml might otherwise try to detect.
	 */
	virtual const char* Parse(const char* p, TiXmlParsingData* data = 0, TiXmlEncoding encoding = TIXML_DEFAULT_ENCODING);

	/**
	 * Get the root element -- the only top level element -- of the document. In well formed XML,
	 * there should only be one. TinyXml is tolerant of multiple elements at the document level.
	 */
	const TiXmlElement* RootElement() const { return FirstChildElement(); }
	TiXmlElement* RootElement() { return FirstChildElement(); }

	/**
	 * If an error occurs, Error will be set to true. Also,
	 * 	- The ErrorId() will contain the integer identifier of the error (not generally useful)
	 * 	- The ErrorDesc() method will return the name of the error. (very useful)
	 * 	- The ErrorRow() and ErrorCol() will return the location of the error (if known)
	 */
	bool Error() const { return error; }

	/**
	 * Gets the error descrition.
	 */
	const std::string& ErrorDesc() const { return errorDesc; }

	/**
	 * Generally, you probably want the error string ( ErrorDesc() ). But if you
	 * prefer the ErrorId, this function will fetch it.
	 */
	int ErrorId()	const { return errorId; }

	/**
	 * Returns the location (if known) of the error. The first column is column 1, and the first
	 * row is row 1. A value of 0 means the row and column wasn't applicable (memory errors, for
	 * example, have no row/column) or the parser lost the error. (An error in the error
	 * reporting, in that case.)
	 *
	 * @sa SetTabSize, Row, Column
	 */
	int ErrorRow() const { return errorLocation.row + 1; }
	int ErrorCol() const { return errorLocation.col + 1; }	///< The column where the error occured. See ErrorRow()

	/**
	 * SetTabSize() allows the error reporting functions (ErrorRow() and ErrorCol()) to report
	 * the correct values for row and column. It does not change the output	or input in any way.
	 *
	 * By calling this method, with a tab size	greater than 0, the row and column of each node
	 * and attribute is stored when the file is loaded. Very useful for tracking the DOM back in
	 * to the source file.
	 *
	 * The tab size is required for calculating the location of nodes. If not set, the default
	 * of 4 is used. The tabsize is set per document. Setting the tabsize to 0 disables row/column
	 * tracking.
	 *
	 * The tab size needs to be enabled before the parse or load. Correct usage:
	 * @verbatim
	 * TiXmlDocument doc;
	 * doc.SetTabSize( 8 );
	 * doc.Load( "myfile.xml" );
	 * @endverbatim
	 *
	 * @sa Row, Column
	 */
	void SetTabSize(int _tabsize) { tabsize = _tabsize; }

	int TabSize() const { return tabsize; }

	/**
	 * If you have handled the error, it can be reset with this call. The error
	 * state is automatically cleared if you Parse a new XML block.
	 */
	void ClearError()
	{
		error = false;
		errorId = 0;
		errorDesc = "";
		errorLocation.row = errorLocation.col = 0;
	}

	/**
	 * Print the Document to a buffer.
	 */
	virtual void Print(std::string& buf, int depth = 0) const;

	virtual const TiXmlDocument* ToDocument() const { return this; }	///< Cast to a more defined type. Will return null not of the requested type.
	virtual TiXmlDocument* ToDocument() { return this; }				///< Cast to a more defined type. Will return null not of the requested type.

	/**
	 * Walk the XML tree visiting this node and all of its children.
	 */
	virtual bool Accept(XmlVisitor* content) const;

	// [internal use]
	void SetError(int err, const char* errorLocation, TiXmlParsingData* prevData, TiXmlEncoding encoding);

protected:
	virtual TiXmlNode* Clone() const;
	virtual void StreamIn(std::istream& in, std::string& tag);

private:
	void CopyTo(TiXmlDocument* target) const;

	bool			error;
	int				errorId;
	std::string		errorDesc;
	int				tabsize;
	XmlCursor		errorLocation;
	bool			useMicrosoftBOM;		// the UTF-8 BOM were found when read. Note this, and try to write.
};


/**
	A TiXmlHandle is a class that wraps a node pointer with null checks; this is
	an incredibly useful thing. Note that TiXmlHandle is not part of the TinyXml
	DOM structure. It is a separate utility class.

	Take an example:
	@verbatim
	<Document>
		<Element attributeA = "valueA">
			<Child attributeB = "value1" />
			<Child attributeB = "value2" />
		</Element>
	<Document>
	@endverbatim

	Assuming you want the value of "attributeB" in the 2nd "Child" element, it's very 
	easy to write a *lot* of code that looks like:

	@verbatim
	TiXmlElement* root = document.FirstChildElement( "Document" );
	if ( root )
	{
		TiXmlElement* element = root->FirstChildElement( "Element" );
		if ( element )
		{
			TiXmlElement* child = element->FirstChildElement( "Child" );
			if ( child )
			{
				TiXmlElement* child2 = child->NextSiblingElement( "Child" );
				if ( child2 )
				{
					// Finally do something useful.
	@endverbatim

	And that doesn't even cover "else" cases. TiXmlHandle addresses the verbosity
	of such code. A TiXmlHandle checks for null	pointers so it is perfectly safe 
	and correct to use:

	@verbatim
	TiXmlHandle docHandle( &document );
	TiXmlElement* child2 = docHandle.FirstChild( "Document" ).FirstChild( "Element" ).Child( "Child", 1 ).ToElement();
	if ( child2 )
	{
		// do something useful
	@endverbatim

	Which is MUCH more concise and useful.

	It is also safe to copy handles - internally they are nothing more than node pointers.
	@verbatim
	TiXmlHandle handleCopy = handle;
	@endverbatim

	What they should not be used for is iteration:

	@verbatim
	int i=0; 
	while ( true )
	{
		TiXmlElement* child = docHandle.FirstChild( "Document" ).FirstChild( "Element" ).Child( "Child", i ).ToElement();
		if ( !child )
			break;
		// do something
		++i;
	}
	@endverbatim

	It seems reasonable, but it is in fact two embedded while loops. The Child method is 
	a linear walk to find the element, so this code would iterate much more than it needs 
	to. Instead, prefer:

	@verbatim
	TiXmlElement* child = docHandle.FirstChild( "Document" ).FirstChild( "Element" ).FirstChild( "Child" ).ToElement();

	for( child; child; child=child->NextSiblingElement() )
	{
		// do something
	}
	@endverbatim
*/
class TiXmlHandle
{
public:
	TiXmlHandle(TiXmlNode* _node) { this->node = _node; }
	TiXmlHandle(const TiXmlHandle& ref) { this->node = ref.node; }
	TiXmlHandle operator=(const TiXmlHandle& ref) { if (&ref != this) this->node = ref.node; return *this; }

	TiXmlHandle FirstChild() const;
	TiXmlHandle FirstChild(const std::string& value) const;
	TiXmlHandle FirstChildElement() const;
	TiXmlHandle FirstChildElement(const std::string& value) const;

	/**
	 * Return a handle to the "index" child with the given name. The first child is 0, the second 1, etc.
	 */
	TiXmlHandle Child(const std::string& value, int index) const;

	/**
	 * Return a handle to the "index" child. The first child is 0, the second 1, etc.
	 */
	TiXmlHandle Child(int index) const;

	/**
	 * Return a handle to the "index" child element with the given name. The first child element is 0, the second 1, etc.
	 * 
	 * \note	Only XmlElements are indexed. Other types are not counted.
	 */
	TiXmlHandle ChildElement(const std::string& value, int index) const;

	/**
	 * Return a handle to the "index" child element. The first child element is 0, the second 1, etc.
	 * 
	 * \note	Only XmlElements are indexed. Other types are not counted.
	 */
	TiXmlHandle ChildElement(int index) const;

	/**
	 * Return the handle as a TiXmlNode. This may return null.
	 */
	TiXmlNode* ToNode() const { return node; }

	/**
	 * Return the handle as a TiXmlElement. This may return null.
	 */
	TiXmlElement* ToElement() const { return ((node && node->ToElement()) ? node->ToElement() : 0); }
	
	/**
	 * Return the handle as a TiXmlText. This may return null.
	 */
	TiXmlText* ToText() const { return ((node && node->ToText()) ? node->ToText() : 0); }
	
	/**
	 * Return the handle as a TiXmlUnknown. This may return null.
	 */
	TiXmlUnknown* ToUnknown() const { return ((node && node->ToUnknown()) ? node->ToUnknown() : 0); }

private:
	TiXmlNode* node;
};


/**
 * Writes an XML Document to a memory buffer.
 * 
 * Once you have a buffer containing the XML document, you can
 * save it to disk using the Filesystem class.
 * 
 * \code{.cpp}
 * XmlDocument doc;
 * XmlMemoryBuffer buff;
 * doc.Accept(&buff);
 * 
 * Utility<Filesystem>::get().write(File(buff.buffer(), mConfigPath));
 * \endcode
 */
class XmlMemoryBuffer : public XmlVisitor
{
public:
	XmlMemoryBuffer() : depth(0), _indent("\t"), _lineBreak("\n") {}

	virtual bool VisitEnter(const TiXmlDocument& doc) { return true; }
	virtual bool VisitExit(const TiXmlDocument& doc) { return true; }

	virtual bool VisitEnter(const TiXmlElement& element, const TiXmlAttribute* firstAttribute);
	virtual bool VisitExit(const TiXmlElement& element);

	virtual bool Visit(const TiXmlDeclaration& declaration);
	virtual bool Visit(const TiXmlText& text);
	virtual bool Visit(const TiXmlComment& comment);
	virtual bool Visit(const TiXmlUnknown& unknown);

	size_t size() { return _buffer.size(); }

	const std::string& buffer() { return _buffer; }

private:
	void indent() { for (int i = 0; i < depth; ++i) _buffer += _indent; }
	void line_break() { _buffer += _lineBreak; }

private:
	int depth;

	std::string _buffer;
	std::string _indent;
	std::string _lineBreak;
};

} // namespace Xml
} // namespace NAS2D

#ifdef _MSC_VER
#pragma warning(pop)
#endif
