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
	 * Parse the given null terminated block of XML data.
	 */
	virtual const char* parse(const char* p, TiXmlParsingData* data = nullptr);

	/**
	 * Get the root element -- the only top level element -- of the document. In well formed XML,
	 * there should only be one. TinyXml is tolerant of multiple elements at the document level.
	 */
	const XmlElement* rootElement() const { return firstChildElement(); }
	XmlElement* rootElement() { return firstChildElement(); }

	/**
	 * If an error occurs, Error will be set to true. Also,
	 * 	- The ErrorId() will contain the integer identifier of the error (not generally useful)
	 * 	- The ErrorDesc() method will return the name of the error. (very useful)
	 * 	- The ErrorRow() and ErrorCol() will return the location of the error (if known)
	 */
	bool error() const { return _error; }

	/**
	 * Gets the error descrition.
	 */
	const std::string& errorDesc() const { return _errorDesc; }

	/**
	 * Generally, you probably want the error string ( ErrorDesc() ). But if you
	 * prefer the ErrorId, this function will fetch it.
	 */
	int ErrorId()	const { return _errorId; }

	/**
	 * Returns the location (if known) of the error. The first column is column 1, and the first
	 * row is row 1. A value of 0 means the row and column wasn't applicable (memory errors, for
	 * example, have no row/column) or the parser lost the error. (An error in the error
	 * reporting, in that case.)
	 *
	 * @sa SetTabSize, Row, Column
	 */
	int ErrorRow() const { return _errorLocation.row + 1; }
	int ErrorCol() const { return _errorLocation.col + 1; }	///< The column where the error occured. See ErrorRow()

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
	void SetTabSize(int _tabsize) { _tabsize = _tabsize; }

	int TabSize() const { return _tabsize; }

	void clearError();

	virtual void write(std::string& buf, int depth = 0) const;

	virtual const XmlDocument* toDocument() const { return this; }
	virtual XmlDocument* toDocument() { return this; }

	virtual bool accept(XmlVisitor* content) const;

	// [internal use]
	void SetError(XmlErrorCode err, const char* errorLocation, TiXmlParsingData* prevData);

protected:
	virtual XmlNode* clone() const;
	virtual void streamIn(std::istream& in, std::string& tag);

private:
	void CopyTo(XmlDocument* target) const;

private:
	int				_errorId;
	int				_tabsize;

	bool			_error;

	std::string		_errorDesc;

	XmlCursor		_errorLocation;
};

} // namespace Xml
} // namespace NAS2D
