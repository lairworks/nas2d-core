// ==================================================================================
// = NAS2D
// = Copyright © 2008 - 2013 New Age Software
// ==================================================================================
// = NAS2D is distributed under the terms of the zlib license. You are free to copy,
// = modify and distribute the software under the terms of the zlib license.
// = 
// = Acknowledgement of your use of NAS2D is appriciated but is not required.
// ==================================================================================

#include "NAS2D/XmlAttributeParser.h"

#include <iostream>

using namespace std;

/**
 * Gets an int value from a named attribute.
 * 
 * \param	node		A pointer to a TiXmlNode.
 * \param	attribute	The name of the attribute to look for.
 * 
 * \return	The value contained in the attribute. If the attribute wasn't
 *			found, this will return 0.
 */
int XmlAttributeParser::intAttribute(TiXmlNode* node, const std::string& attribute)
{
	int returnValue = 0;

	int result = TIXML_SUCCESS;

	result = node->ToElement()->QueryIntAttribute(attribute.c_str(), &returnValue);
	if(result != TIXML_SUCCESS)
	{
		if(result == TIXML_NO_ATTRIBUTE)
		{
			cout << "Tag '<" << node->Value() << ">' is missing attribute '" << attribute << "' on row " << node->Row() << "." << endl;
		}
		else if(result == TIXML_WRONG_TYPE)
		{
			cout << "Attribute '" << attribute << "' in tag '<" << node->Value() << ">' contains invalid data on row " << node->Row() << "." << endl;
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
 * \param	node		A pointer to a TiXmlNode.
 * \param	attribute	The name of the attribute to look for.
 * 
 * \return	The value contained in the attribute. If the attribute wasn't
 *			found, this will return 0.0f.
 */
float XmlAttributeParser::floatAttribute(TiXmlNode* node, const std::string& attribute)
{
	float returnValue = 0.0f;

	int result = TIXML_SUCCESS;

	result = node->ToElement()->QueryFloatAttribute(attribute.c_str(), &returnValue);
	if(result != TIXML_SUCCESS)
	{
		if(result == TIXML_NO_ATTRIBUTE)
		{
			cout << "Tag '<" << node->Value() << ">' is missing attribute '" << attribute << "' on row " << node->Row() << "." << endl;
		}
		else if(result == TIXML_WRONG_TYPE)
		{
			cout << "Attribute '" << attribute << "' in tag '<" << node->Value() << ">' contains invalid data on row " << node->Row() << "." << endl;
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
 * \param	node		A pointer to a TiXmlNode.
 * \param	attribute	The name of the attribute to look for.
 * 
 * \return	The value contained in the attribute. If the attribute wasn't
 *			found, this will return 0.0.
 */
double XmlAttributeParser::doubleAttribute(TiXmlNode* node, const std::string& attribute)
{
	double returnValue = 0.0;

	int result = TIXML_SUCCESS;

	result = node->ToElement()->QueryDoubleAttribute(attribute.c_str(), &returnValue);
	if(result != TIXML_SUCCESS)
	{
		if(result == TIXML_NO_ATTRIBUTE)
		{
			cout << "Tag '<" << node->Value() << ">' is missing attribute '" << attribute << "' on row " << node->Row() << "." << endl;
		}
		else if(result == TIXML_WRONG_TYPE)
		{
			cout << "Attribute '" << attribute << "' in tag '<" << node->Value() << ">' contains invalid data on row " << node->Row() << "." << endl;
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
 * \param	node		A pointer to a TiXmlNode.
 * \param	attribute	The name of the attribute to look for.
 * 
 * \return	The value contained in the attribute. If the attribute wasn't
 *			found, this will return an empty string.
 */
string XmlAttributeParser::stringAttribute(TiXmlNode* node, const std::string& attribute)
{
	const char *returnValue = node->ToElement()->Attribute(attribute.c_str());
	if(returnValue == NULL)
	{
		cout << "Attribute '" << attribute << "' in tag '<" << node->Value() << ">' contains invalid data on row " << node->Row() << "." << endl;
		return "";
	}

	return returnValue;
}