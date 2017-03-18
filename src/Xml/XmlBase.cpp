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
#include "NAS2D/Xml/Xml.h"

using namespace NAS2D::Xml;

bool XmlBase::condenseWhiteSpace = true;

/**
* Return the position, in the original source file, of this node or attribute.
*
* Generally, the row and column value will be set when the XmlDocument::Load(),
* XmlDocument::LoadFile(), or any XmlNode::Parse() is called. It will NOT be set
* when the DOM was created from operator>>.
*
* The values reflect the initial load. Once the DOM is modified programmatically
* (by adding or changing nodes and attributes) the new values will NOT update to
* reflect changes in the document.
*
* There is a minor performance cost to computing the row and column. Computation
* can be disabled if XmlDocument::SetTabSize() is called with 0 as the value.
*
* @sa XmlDocument::SetTabSize()
*/
int XmlBase::row() const
{
	return location.row + 1;
}

int XmlBase::column() const { return location.col + 1; }	///< See Row()


void XmlBase::EncodeString(const std::string& str, std::string& outString)
{
	size_t i = 0;

	while (i < str.length())
	{
		unsigned char c = (unsigned char)str[i];

		if (c == '&'
			&& i < (str.length() - 2)
			&& str[i + 1] == '#'
			&& str[i + 2] == 'x')
		{
			// Hexadecimal character reference.
			// Pass through unchanged.
			// &#xA9;	-- copyright symbol, for example.
			//
			// The -1 is a bug fix from Rob Laveaux. It keeps
			// an overflow from happening if there is no ';'.
			// There are actually 2 ways to exit this loop -
			// while fails (error case) and break (semicolon found).
			// However, there is no mechanism (currently) for
			// this function to return an error.
			while (i < str.length() - 1)
			{
				outString.append(str[i], 1);
				++i;
				if (str[i] == ';')
					break;
			}
		}
		else if (c == '&')
		{
			outString.append(entity[0].str, entity[0].strLength);
			++i;
		}
		else if (c == '<')
		{
			outString.append(entity[1].str, entity[1].strLength);
			++i;
		}
		else if (c == '>')
		{
			outString.append(entity[2].str, entity[2].strLength);
			++i;
		}
		else if (c == '\"')
		{
			outString.append(entity[3].str, entity[3].strLength);
			++i;
		}
		else if (c == '\'')
		{
			outString.append(entity[4].str, entity[4].strLength);
			++i;
		}
		else if (c < 32)
		{
			// Easy pass at non-alpha/numeric/symbol
			// Below 32 is symbolic.
			char buf[32];

#if defined(TIXML_SNPRINTF)		
			TIXML_SNPRINTF(buf, sizeof(buf), "&#x%02X;", (unsigned)(c & 0xff));
#else
			sprintf(buf, "&#x%02X;", (unsigned)(c & 0xff));
#endif		

			//*ME:	warning C4267: convert 'size_t' to 'int'
			//*ME:	Int-Cast to make compiler happy ...
			outString.append(buf, (int)strlen(buf));
			++i;
		}
		else
		{
			outString += c;
			++i;
		}
	}
}


void XmlBase::fillErrorTable()
{
	XmlBase::errorString.push_back("No error");
	XmlBase::errorString.push_back("Unspecified Error");
	XmlBase::errorString.push_back("Error: Failed to open file");
	XmlBase::errorString.push_back("Error parsing Element.");
	XmlBase::errorString.push_back("Failed to read Element name.");
	XmlBase::errorString.push_back("Error reading Element value.");
	XmlBase::errorString.push_back("Error reading Attributes.");
	XmlBase::errorString.push_back("Error: Empty tag.");
	XmlBase::errorString.push_back("Error reading end tag.");
	XmlBase::errorString.push_back("Error parsing Unknown.");
	XmlBase::errorString.push_back("Error parsing Comment.");
	XmlBase::errorString.push_back("Error parsing Declaration.");
	XmlBase::errorString.push_back("Error: Document empty.");
	XmlBase::errorString.push_back("Error: Unexpected EOF found in input stream.");
	XmlBase::errorString.push_back("Error parsing CDATA.");
	XmlBase::errorString.push_back("Error adding XmlDocument to document: XmlDocument can only be at the root.");
}
