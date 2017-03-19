#pragma once

#include "NAS2D/Xml/XmlNode.h"

namespace NAS2D {
namespace Xml {

/** Always the top level node. A document binds together all the
	XML pieces. It can be saved, loaded, and printed to the screen.
	The 'value' of a document node is the xml file name.
*/
class XmlDocument : public XmlNode
{
public:
	XmlDocument();
	XmlDocument(const std::string& documentName);

	XmlDocument(const XmlDocument& copy);
	XmlDocument& operator=(const XmlDocument& copy);

	virtual ~XmlDocument() {}

	/**
	 * Parse the given null terminated block of xml data. Passing in an encoding to this
	 * method (either TIXML_ENCODING_LEGACY or TIXML_ENCODING_UTF8 will force TinyXml
	 * to use that encoding, regardless of what TinyXml might otherwise try to detect.
	 */
	virtual const char* parse(const char* p, TiXmlParsingData* data = 0);

	/**
	 * Get the root element -- the only top level element -- of the document. In well formed XML,
	 * there should only be one. TinyXml is tolerant of multiple elements at the document level.
	 */
	const XmlElement* RootElement() const { return firstChildElement(); }
	XmlElement* RootElement() { return firstChildElement(); }

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
	 * XmlDocument doc;
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
	virtual void write(std::string& buf, int depth = 0) const;

	virtual const XmlDocument* toDocument() const { return this; }
	virtual XmlDocument* toDocument() { return this; }

	/**
	 * Walk the XML tree visiting this node and all of its children.
	 */
	virtual bool accept(XmlVisitor* content) const;

	// [internal use]
	void SetError(XmlErrorCode err, const char* errorLocation, TiXmlParsingData* prevData);

protected:
	virtual XmlNode* clone() const;
	virtual void streamIn(std::istream& in, std::string& tag);

private:
	void CopyTo(XmlDocument* target) const;

private:
	int				errorId;
	int				tabsize;

	bool			error;
	bool			useMicrosoftBOM;		// the UTF-8 BOM were found when read. Note this, and try to write.

	std::string		errorDesc;

	XmlCursor		errorLocation;
};

} // namespace Xml
} // namespace NAS2D
