// ==================================================================================
// = NAS2D
// = Copyright © 2008 - 2017 New Age Software
// ==================================================================================
// = NAS2D is distributed under the terms of the zlib license. You are free to copy,
// = modify and distribute the software under the terms of the zlib license.
// = 
// = Acknowledgement of your use of NAS2D is appriciated but is not required.
// ==================================================================================

#include "NAS2D/Xml/XmlAttributeParser.h"


#include <iostream>

using namespace std;
using namespace NAS2D;


/**
 * Gets an int value from a named attribute.
 * 
 * \param	node		A pointer to a XmlNode.
 * \param	attribute	The name of the attribute to look for.
 * 
 * \return	The value contained in the attribute. If the attribute wasn't
 *			found, this will return 0.
 */
int XmlAttributeParser::intAttribute(Xml::XmlNode* node, const std::string& attribute)
{
	int returnValue = 0;

	int result = Xml::XML_SUCCESS;

	result = node->ToElement()->QueryIntAttribute(attribute, returnValue);
	if(result != Xml::XML_SUCCESS)
	{
		if(result == Xml::XML_NO_ATTRIBUTE)
		{
			cout << "Tag '<" << node->Value() << ">' is missing attribute '" << attribute << "' on row " << node->row() << "." << endl;
		}
		else if(result == Xml::XML_WRONG_TYPE)
		{
			cout << "Attribute '" << attribute << "' in tag '<" << node->Value() << ">' contains invalid data on row " << node->row() << "." << endl;
		}
		else
		{
			cout << "Unspecified error." << endl;
		}
	}

	return returnValue;
}


/**
 * Gets a float value from a named attribute.
 * 
 * \param	node		A pointer to a XmlNode.
 * \param	attribute	The name of the attribute to look for.
 * 
 * \return	The value contained in the attribute. If the attribute wasn't
 *			found, this will return 0.0f.
 */
float XmlAttributeParser::floatAttribute(Xml::XmlNode* node, const std::string& attribute)
{
	float returnValue = 0.0f;

	int result = Xml::XML_SUCCESS;

	result = node->ToElement()->QueryFloatAttribute(attribute, returnValue);
	if(result != Xml::XML_SUCCESS)
	{
		if(result == Xml::XML_NO_ATTRIBUTE)
		{
			cout << "Tag '<" << node->Value() << ">' is missing attribute '" << attribute << "' on row " << node->row() << "." << endl;
		}
		else if(result == Xml::XML_WRONG_TYPE)
		{
			cout << "Attribute '" << attribute << "' in tag '<" << node->Value() << ">' contains invalid data on row " << node->row() << "." << endl;
		}
		else
		{
			cout << "Unspecified error." << endl;
		}
	}

	return returnValue;
}


/**
 * Gets a double value from a named attribute.
 * 
 * \param	node		A pointer to a XmlNode.
 * \param	attribute	The name of the attribute to look for.
 * 
 * \return	The value contained in the attribute. If the attribute wasn't
 *			found, this will return 0.0.
 */
double XmlAttributeParser::doubleAttribute(Xml::XmlNode* node, const std::string& attribute)
{
	double returnValue = 0.0;

	int result = Xml::XML_SUCCESS;

	result = node->ToElement()->QueryDoubleAttribute(attribute, returnValue);
	if(result != Xml::XML_SUCCESS)
	{
		if(result == Xml::XML_NO_ATTRIBUTE)
		{
			cout << "Tag '<" << node->Value() << ">' is missing attribute '" << attribute << "' on row " << node->row() << "." << endl;
		}
		else if(result == Xml::XML_WRONG_TYPE)
		{
			cout << "Attribute '" << attribute << "' in tag '<" << node->Value() << ">' contains invalid data on row " << node->row() << "." << endl;
		}
		else
		{
			cout << "Unspecified error." << endl;
		}
	}

	return returnValue;
}


/**
 * Gets a string value from a named attribute.
 * 
 * \param	node		A pointer to a XmlNode.
 * \param	attribute	The name of the attribute to look for.
 * 
 * \return	The value contained in the attribute. If the attribute wasn't
 *			found, this will return an empty string.
 */
string XmlAttributeParser::stringAttribute(Xml::XmlNode* node, const std::string& attribute)
{
	const std::string returnValue = node->ToElement()->Attribute(attribute);
	if(returnValue.empty())
	{
		cout << "Attribute '" << attribute << "' in tag '<" << node->Value() << ">' contains invalid data on row " << node->row() << "." << endl;
		return "";
	}

	return returnValue;
}
